#include "StudentWorld.h"
#include "GameConstants.h"

#include <string>
#include<algorithm>
using namespace std;

const int Left = 180;
const int Right = 0;
const int up = 90;
const int down = 270;

class Actor;
class PlayerAvatar;
class CoinSquare;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

PlayerAvatar* StudentWorld::getPlayer(int playerNum)
{
    for (unsigned int i = 0; i < P.size(); i++)
    {
        if (P[i]->getPlayerNum() == playerNum)
            return P[i];
    }
    return nullptr;
}

Actor* StudentWorld::getSquareAt(int x, int y)
{
    for(unsigned int i=0;i<A.size();i++)
        if(A[i]->getX()==x && A[i]->getY()==y && A[i]->isSquare())
            return A[i];
    return nullptr;
}

void StudentWorld::pushMoveableCharacter(int i, int j, Moveable* m)
{
    M.push_back(m);
    edgeX = max(edgeX, m->getX());
    edgeY = max(edgeY, m->getY());
}

void StudentWorld::push(int i, int j, Actor* m)
{
    A.push_back(m);
    MAP[i][j] = Squares;
    edgeX = max(edgeX, m->getX());
    edgeY = max(edgeY, m->getY());
}

StudentWorld::StudentWorld(string assetPath) : GameWorld(assetPath)
{
    bank=edgeX = edgeY = 0;
    for (int i = 0; i < BOARD_WIDTH; i++)
        for (int j = 0; j < BOARD_HEIGHT; j++)
            MAP[i][j] = Empty;
}

int StudentWorld::init()
{
    int n = 0;
    Board bd;
    string board_file = assetPath() + "board0"+ char(getBoardNumber()+48) +".txt";
    Board::LoadResult result = bd.loadBoard(board_file);
    if (result == Board::load_fail_file_not_found)
        return GWSTATUS_BOARD_ERROR;
    else if (result == Board::load_fail_bad_format)
        return GWSTATUS_BOARD_ERROR;
    else if (result == Board::load_success)
    {
        for (int i = 0; i < BOARD_WIDTH; i++)
        {
            for (int j = 0; j < BOARD_HEIGHT; j++)
            {
                Board::GridEntry ge = bd.getContentsOf(i, j);
                PlayerAvatar* p;
                PlayerAvatar* y;
                Square* s;
                Enemy* e;
                switch (ge)
                {
                case Board::empty:
                    break;
                case Board::boo:
                    n++;
                    e = new Boo(n,i, j, this);
                    s = new BlueCoinSquare(i, j, this);
                    pushMoveableCharacter(i, j, e);
                    push(i, j, e);
                    push(i, j, s);
                    break;
                case Board::bowser:
                    e = new Bowser(i,j,this);
                    s = new BlueCoinSquare(i, j, this);
                    pushMoveableCharacter(i, j, e);
                    push(i, j, e);
                    push(i, j, s);
                    break;
                case Board::player:
                    p = new PlayerAvatar(1,IID_PEACH, i, j, this);
                    y = new PlayerAvatar(2,IID_YOSHI, i, j, this);
                    s = new BlueCoinSquare(i, j, this);
                    push(i, j, s);
                    P.push_back(p);
                    P.push_back(y);
                    break;
                case Board::blue_coin_square:
                    s = new BlueCoinSquare(i, j, this);
                    push(i, j, s);
                    break;
                case Board::red_coin_square:
                    s = new RedCoinSquare(i, j, this);
                    push(i, j, s);
                    break; 
                case Board::up_dir_square:
                    s = new DirSquare(i, j, this,up);
                    push(i, j, s);
                    break;
                case Board::down_dir_square:
                    s = new DirSquare(i, j, this,down);
                    push(i, j, s);
                    break;
                case Board::left_dir_square:
                    s = new DirSquare(i, j, this,Left);
                    push(i, j, s);
                    break;
                case Board::right_dir_square:
                    s = new DirSquare(i, j, this,Right);
                    push(i, j, s);
                    break;
                case Board::event_square:
                    s = new EventSquare(i, j, this);
                    push(i, j, s);
                    break;
                case Board::bank_square:
                    s = new BankSquare(i, j, this);
                    push(i, j, s);
                    break;
                case Board::star_square:
                    s = new StarSquare(i, j, this);
                    push(i, j, s);
                    break;
                }
            }
        }
    }
    //cout << A.size() << " " << P.size() << endl;
	startCountdownTimer(99);  // this placeholder causes timeout after 5 seconds
    return GWSTATUS_CONTINUE_GAME;
}

bool StudentWorld::checkSquare(int i, int j, int dir) {
    int x = i, y = j;
    if (x > 15*16 || y > 15*16 || x < 0 || y < 0)
        return false;
    if(dir==Right||dir==up)
    {
        if (x % 16 != 0)
        {
            x /= 16;
            x += 1;
        }
        else
            x /= 16;
        if (y % 16 != 0)
        {
            y /= 16;
            y += 1;
        }
        else
            y /= 16;
    }
    else
    {
        x /= 16;
        y /= 16;
    }
    return MAP[x][y] == Squares;
}

void StudentWorld::displayGameText(int p1roll, int p1star, int p1coin, int p1vor, int bank, int p2roll, int p2star, int p2coin, int p2vor)
{
    ostringstream oss;
    oss << "P1 Roll: " << p1roll << " Stars: " << p1star << " $$: " << p1coin;
    if (p1vor > 0)
        oss << " VOR";
    oss<<" | Time: "<< timeRemaining()<<" | Bank: "<<bank<<" | P2 Roll: "<<p2roll<< " Stars: " << p2star <<" $$: " << p2coin;
    if (p2vor > 0)
        oss << " VOR ";
    string s = oss.str();
    setGameStatText(s);
}

int StudentWorld::move()
{
    for (unsigned int i = 0; i < P.size(); i++)
        if (P[i]->ifalive() == living)
            P[i]->doSomething();

    for (unsigned int i = 0; i < A.size(); i++)
        if (A[i]->ifalive() == living)
            A[i]->doSomething();
    vector <Actor*>::iterator it=A.begin();
    while(it!=A.end())
    {
        if ((*it)->ifalive() != living)
        {
            delete (*it);
            it = A.erase(it);
        }
        else
            it++;
    }
    displayGameText(P[0]->getRoll(), P[0]->getStar(), P[0]->getCoin(), P[0]->getVor(),bank, P[1]->getRoll(),P[1]->getStar(), P[1]->getCoin(), P[1]->getVor());// update the coins/stars stats text at screen top
    int n = randInt(0, 1);
    if (timeRemaining() <= 0)
    {
        playSound(SOUND_GAME_FINISHED);
        if (P[0]->getStar() > P[1]->getStar())
        {
            setFinalScore(P[0]->getStar(), P[0]->getCoin());
            return GWSTATUS_PEACH_WON;
        }
        else if (P[1]->getStar() > P[0]->getStar())
        {
            setFinalScore(P[1]->getStar(), P[1]->getCoin());
            return GWSTATUS_YOSHI_WON;
        }
        else
        {
            if (P[0]->getCoin() > P[1]->getCoin())
            {
                setFinalScore(P[0]->getStar(), P[0]->getCoin());
                return GWSTATUS_PEACH_WON;
            }
            else if (P[1]->getCoin() > P[0]->getCoin())
            {
                setFinalScore(P[1]->getStar(), P[1]->getCoin());
                return GWSTATUS_YOSHI_WON;
            }
            else
            {
                if (n == 0)
                {
                    setFinalScore(P[0]->getStar(), P[0]->getCoin());
                    return GWSTATUS_PEACH_WON;
                }
                if (n == 1)
                {
                    setFinalScore(P[1]->getStar(), P[1]->getCoin());
                    return GWSTATUS_YOSHI_WON;
                }
            }
        }
    }
    if (timeRemaining() <= 0)
        return GWSTATUS_NOT_IMPLEMENTED;
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    while (!A.empty())
    {
        delete A.back();
        A.pop_back();
    }
    while (!P.empty())
    {
        delete P.back();
        P.pop_back();
    }
    while (!M.empty())
    {
        delete M.back();
        M.pop_back();
    }
}
