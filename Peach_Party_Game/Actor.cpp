#include "Actor.h"
#include "StudentWorld.h"

bool Moveable::checkOverlap(Moveable* other)
{
	int x1 = other->getLeftBound(), x2 = other->getRightBound(), y1 = other->getLowBound(), y2 = other->getUpBound();
	int m_x1 = getLeftBound(), m_x2 = getRightBound(), m_y1 = getLowBound(), m_y2 = getUpBound();
	if ((m_x1 <= x1 && x1 <= m_x2) || (m_x1 <= x2 && x2 <= m_x2))
		if ((m_y1 <= y1 && y1 <= m_y2) || (m_y1 <= y2 && y2 <= m_y2))
			return true;
	return false;
}

void Moveable::updateBox()
{
	m_box.x1 = getX();
	m_box.y1 = getY();
	m_box.x2 = getX() + 15;
	m_box.y2 = getY() + 15;
}

bool Moveable::isWhereIFrom(int dir)
{
	if (dir == findOpDir())
		return true;
	return false;
}

int Moveable::getRoll()
{
	if (getTicks() % 8 == 0)
		setRoll(getTicks() / 8);
	else
		setRoll(getTicks() / 8 + 1);
	return roll;
}

int Moveable::findOpDir()
{
	int dir = getWalkDir();
	if (dir == right)
		return left;
	if (dir == left)
		return right;
	if (dir == up)
		return down;
	if (dir == down)
		return up;
	return right;
}

bool Moveable::checkNext(int dir)
{
	int newX = getX(), newY = getY();
	switch (dir)
	{
	case left:  newX -= 2; break;
	case right: newX += 2; break;
	case up:    newY += 2; break;
	case down:  newY -= 2; break;
	}
	if (newX > getWorld()->MaxX() || newX<0 || newY> getWorld()->MaxY() || newY < 0)
		return false;
	return getWorld()->checkSquare(newX, newY, dir);
}

void Moveable::checkFork(int possibleDir[], int& count)
{
	if (checkNext(right))
	{
		possibleDir[0] = 1;
		count++;
	}
	if (checkNext(up))
	{
		possibleDir[1] = 1;
		count++;
	}
	if (checkNext(left))
	{
		possibleDir[2] = 1;
		count++;
	}
	if (checkNext(down))
	{
		possibleDir[3] = 1;
		count++;
	}
	return;
}

void Moveable::teleport()
{
	int m;
	Actor* s;
	do
	{
		m = randInt(0, getWorld()->getSize() - 1);
		s = getWorld()->getActor(m);
		if (s->isSquare())
		{
			moveTo(s->getX(), s->getY());
			setTeleported(1);
			getWorld()->playSound(SOUND_PLAYER_TELEPORT);
			break;
		}
	}while (!s->isSquare());
	return;
}

void Moveable::setMovement(int dir)
{
	setWalkDir(dir);
	if (dir != left)
		setDirection(right);
	else
		setDirection(left);
}

void Moveable::turn()
{
	if (getWalkDir() == right || getWalkDir() == left)
	{
		if (checkNext(up))
			setMovement(up);
		else if (checkNext(down))
			setMovement(down);
	}
	else if (getWalkDir() == up || getWalkDir() == down)
	{
		if (checkNext(right))
			setMovement(right);
		else if (checkNext(left))
			setMovement(left);
	}
}

void Moveable::pickRandDir()
{
	while (1)
	{
		int n = randInt(1, 4);
		switch (n)
		{
		case 1:
			if (!checkNext(up))
				break;
			else
			{
				setMovement(up);
				return;
			}
		case 2:
			if (!checkNext(down))
				break;
			else
			{
				setMovement(down);
				return;
			}
		case 3:
			if (!checkNext(right))
				break;
			else
			{
				setMovement(right);
				return;
			}
		case 4:
			if (!checkNext(left))
				break;
			else
			{
				setMovement(left);
				return;
			}
		}
	}
}

void Enemy::impact()
{
	teleport();
	setMovement(right);
	setState(paused);
	setCounter(180);
}

void Enemy::walk()
{
	int possibleDir[4] = { 0 };
	int n, count = 0;
	if (getX() % 16 == 0 && getY() % 16 == 0)
	{
		checkFork(possibleDir, count);
		if (count > 2)
		{
			n = randInt(0, 3);
			while (possibleDir[n] == 0 || isWhereIFrom(90 * n))
				n = randInt(0, 3);
			setMovement(90 * n);
		}
		else if (count == 2 && !checkNext(getWalkDir()))
			turn();
	}
	moveAtAngle(getWalkDir(), 2);
	setTicks(getTicks() - 1);
	updateBox();
}

void Enemy::startMove(int n)
{
	int die_roll = randInt(1, n);
	setRoll(die_roll);
	setTicks(die_roll * 8);
	pickRandDir();
	setState(walking);
}

void Bowser::doSomething()
{
	PlayerAvatar* p;
	DroppingSquare* s;
	int n,p_x, p_y, p_state, p_stars, p_coins;
	if (getState() == paused)
	{
		for (int i = 1; i <= 2; i++)
		{
			p = getWorld()->getPlayer(i);
			p_x = p->getX();
			p_y = p->getY();
			p_state = p->getState();
			p_stars = p->getStar();
			p_coins = p->getCoin();
			if ((p_x == getX() && p_y == getY()) && p_state == waiting_to_roll)
			{
				n = randInt(1, 2);
				if (n == 1 && getActivate(i) == unactivated)
				{
					setActivate(i, activated);
					p->takeCoin(p_coins);
					p->takeStar(p_stars);
					getWorld()->playSound(SOUND_BOWSER_ACTIVATE);
				}
			}
			if ((p_x != getX() || p_y != getY()) && getActivate(i) == activated)
				setActivate(i, unactivated);
		}
		setCounter(getCounter() - 1);
		if (getCounter() == 0)
			startMove(10);
	}
	if (getState() == walking)
	{
		walk();
		if (getTicks() == 0)
		{
			setState(paused);
			setCounter(180);
			n = randInt(1, 4);
			if (n == 1)
			{
				s = new DroppingSquare(getX()/16, getY()/16, getWorld());
				getWorld()->getSquareAt(getX(), getY())->isdead();
				getWorld()->push(getX()/16, getY()/16, s);
				getWorld()->playSound(SOUND_DROPPING_SQUARE_CREATED);
			}
		}
	}
	return;
}

void Boo::doSomething()
{
	PlayerAvatar* p;
	PlayerAvatar* p2;
	int n, p_x, p_y, p_state, p_stars, p_coins;
	if (getState() == paused)
	{
		for (int i = 1; i <= 2; i++)
		{
			p = getWorld()->getPlayer(i);
			p_x = p->getX();
			p_y = p->getY();
			p_state = p->getState();
			p_stars = p->getStar();
			p_coins = p->getCoin();
			if (getX() == p_x && getY() == p_y && p_state == waiting_to_roll && getActivate(i) == unactivated)
			{
				setActivate(i, activated);
				if (i == 1)
					p2 = getWorld()->getPlayer(2);
				else if (i == 2)
					p2 = getWorld()->getPlayer(1);
				n = randInt(1, 2);
				if (n == 1)
					p->swapCoin(p2);
				else if (n == 2)
					p->swapStar(p2);
				getWorld()->playSound(SOUND_BOO_ACTIVATE);
			}
			if (getActivate(i) == activated && (p_x != getX() || p_y != getY()))
				setActivate(i, unactivated);
		}
		setCounter(getCounter() - 1);
		if (getCounter() == 0)
			startMove(3);
	}
	if (getState() == walking)
	{
		walk();
		if (getTicks() == 0)
		{
			setState(paused);
			setCounter(180);
		}
	}
	return;
}

void PlayerAvatar::swapCoin(PlayerAvatar* p)
{
	int tmp = m_coins;
	m_coins = p->getCoin();
	p->takeCoin(p->getCoin());
	p->addCoin(tmp);
	return;
}

void PlayerAvatar::swapStar(PlayerAvatar* p)
{
	int tmp = star;
	int tmp2 = p->getStar();
	setStar(tmp2);
	p->setStar(tmp);
	//cout<<"swapped" << endl;
	return;
}

void PlayerAvatar::takeStar(int n)
{
	//cout << "taked" << endl;
	star -= n;
	if (star < 0)
		star = 0;
}

void PlayerAvatar::takeCoin(int n)
{
	m_coins -= n;
	if (m_coins < 0)
		m_coins = 0;
}

bool PlayerAvatar::addStar()
{
	if (getCoin() < 20)
		return false;
	takeCoin(20);
	star++;
	return true;
}

void PlayerAvatar::swapPlayer(PlayerAvatar* p)
{
	int X = p->getX();
	int Y = p->getY();
	int tick = p->getTicks();
	int walk_dir = p->getWalkDir();
	int dir = p->getDirection();
	int state = p->getState();
	int roll = p->getRoll();
	p->moveTo(getX(), getY());
	moveTo(X, Y);
	p->setTicks(getTicks());
	setTicks(tick);
	p->setDirection(getDirection());
	setDirection(dir);
	p->setWalkDir(getWalkDir());
	setWalkDir(walk_dir);
	p->setState(getState());
	setState(state);
	p->setRoll(getRoll());
	setRoll(roll);
}

void PlayerAvatar::update()
{
	moveAtAngle(getWalkDir(), 2);
	setTicks(getTicks() - 1);
	addTicksMoved(1);
	updateBox();
	if (getTicksMoved() % 8 == 0)
		addSteps();
}

void PlayerAvatar::doSomething()
{
	int possibleDir[4] = { 0 };
	int n, count = 0,action;
	//cout <<"star before "<< star << endl;
	if (getState() == waiting_to_roll)
    {
        if (getTeleported() == 1)
        {
            pickRandDir();
            setTeleported(0);
        }
        action = getWorld()->getAction(getPlayerNum());
        if(action==ACTION_FIRE && getVor())
        {
			takeVor();
			m_vortex = new Vortexes(this, getX() / 16, getY() / 16, getWorld());
			m_vortex->setMovement(getWalkDir());
			getWorld()->push(getX() / 16, getY() / 16, m_vortex);
            getWorld()->playSound(SOUND_PLAYER_FIRE);
        }
        else if(action==ACTION_ROLL)
        {
			int die_roll = randInt(1, 10);
			setRoll(die_roll);
			setTicks(die_roll*8);
			setState(walking);	
        }
        else
            return ;
    }
	if (getState() == walking)
	{
		if (getX() % 16 == 0 && getY() % 16 == 0)
		{
			if (getWorld()->getSquareAt(getX(), getY())->isDirSquare())
				getWorld()->getSquareAt(getX(), getY())->Activate(getPlayerNum());
			else
			{
				checkFork(possibleDir, count);
				if (count > 2)
				{
					n = getWorld()->getAction(m_playerNum);
					if (n == ACTION_LEFT && possibleDir[2] == 1 && !isWhereIFrom(left))
						setMovement(left);
					else if (n == ACTION_UP && possibleDir[1] == 1 && !isWhereIFrom(up))
						setMovement(up);
					else if (n == ACTION_RIGHT && possibleDir[0] == 1 && !isWhereIFrom(right))
						setMovement(right);
					else if (n == ACTION_DOWN && possibleDir[3] == 1 && !isWhereIFrom(down))
						setMovement(down);
					else
						return;
				}
				else if (count == 2 && !checkNext(getWalkDir()))
					turn();
			}
		}
		else if(!checkNext(getWalkDir()))
			turn();
		
		update();
		
		if (getTicks() == 0)
		{
			setState(waiting_to_roll);
			setTicksMoved(0);
		}
		
	}
	//cout << "star after " << star << endl;
	return;
}

bool Vortexes::inBound()
{
	int x = getX();
	int y = getY();
	if (x<0 || y<0 || x>VIEW_WIDTH || y>VIEW_HEIGHT)
		return false;
	return true;
}

void Vortexes::doSomething()
{
	if(!ifalive())
		return;
	moveAtAngle(getWalkDir(), 2);
	updateBox();
	if (!inBound())
		isdead();
	for (int i = 0; i < getWorld()->getMoveableSize();i++)
	{
		Moveable* e = getWorld()->getMoveableCharacter(i);
		if (e->isEnemy()&&checkOverlap(e))
		{
			e->impact();
			isdead();
			getWorld()->playSound(SOUND_HIT_BY_VORTEX);
			return;
		}
	}
}

void DroppingSquare::op(PlayerAvatar* p)
{
	setState(p->getPlayerNum(), activated);
	int n = randInt(1, 2);
	switch (n)
	{
	case 1:
		p->takeCoin(10);
		break;
	case 2:
		p->takeStar(1);
		break;
	}
	getWorld()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
	return;
}

void BankSquare::op(PlayerAvatar* p)
{
	setState(p->getPlayerNum(), activated);
	if (p->getState() == waiting_to_roll)
	{
		p->addCoin(getWorld()->takeBank());
		getWorld()->playSound(SOUND_WITHDRAW_BANK);
	}
	else if (p->getState() == walking)
	{
		int n = p->getCoin();
		if (n < 5)
		{
			p->takeCoin(n);
			getWorld()->addBank(n);
		}
		else
		{
			p->takeCoin(5);
			getWorld()->addBank(5);
		}
		getWorld()->playSound(SOUND_DEPOSIT_BANK);
	}
	return;
}

void StarSquare::op(PlayerAvatar* p)
{
	setState(p->getPlayerNum(), activated);
	if (p->addStar())
		getWorld()->playSound(SOUND_GIVE_STAR);
	else
		return;
}

void DirSquare::op(PlayerAvatar* p)
{
	setState(p->getPlayerNum(), activated);
	p->setWalkDir(m_dir);
	if (m_dir == left)
		p->setDirection(left);
	else
		p->setDirection(right);
	return;
}

void EventSquare::op(PlayerAvatar* p)
{
	setState(p->getPlayerNum(), activated);
	int n = randInt(1,3);
	PlayerAvatar* p2;
	switch (n)
	{
	case 1:
		p->teleport();
		break;
	case 2:
		p2 = getWorld()->getPlayer(1);
		if (p == p2)
			p2 = getWorld()->getPlayer(2);
		p->swapPlayer(p2);
		setState(p2->getPlayerNum(), activated);
		getWorld()->playSound(SOUND_PLAYER_TELEPORT);
		break;
	case 3:
		p->addVor();
		getWorld()->playSound(SOUND_GIVE_VORTEX);
		break;
	}
	return;
}

void RedCoinSquare::op(PlayerAvatar* p)
{
	setState(p->getPlayerNum(), activated);
	p->takeCoin(3);
	getWorld()->playSound(SOUND_TAKE_COIN);
	return;
}

void BlueCoinSquare::op(PlayerAvatar* p)
{
    setState(p->getPlayerNum(), activated);
    p->addCoin(3);
	getWorld()->playSound(SOUND_GIVE_COIN);
    return ;
}

void UnconditionalSquare::Activate(int playerNum)
{
	PlayerAvatar* p = getWorld()->getPlayer(playerNum);
	if (p == nullptr)
		return;
	int p_x = p->getX();
	int p_y = p->getY();
	if (p_x == getX() && p_y == getY() && getState(playerNum) == unactivated)
		op(p);
	else if (getState(playerNum) == activated && (p_x != getX() || p_y != getY()))
		setState(playerNum, unactivated);
}

void Square::Activate(int playerNum)
{
    PlayerAvatar* p=getWorld()->getPlayer(playerNum);
    if(p==nullptr)
        return ;
    int p_x=p->getX();
    int p_y=p->getY();
    if(p_x == getX() && p_y == getY() && p->getState()==waiting_to_roll && getState(playerNum)==unactivated)
	    op(p);
	else if(getState(playerNum)==activated && (p_x != getX() || p_y != getY()))
		setState(playerNum, unactivated);
}

void Square::doSomething()
{
	if (ifalive() == dead)
		return;
    Activate(1);
    Activate(2);
	return;
}
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
