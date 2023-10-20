#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

const int waiting_to_roll = 0;
const int walking = 1;
const int dead = 2;
const int living = 3;
const int unactivated=4;
const int activated=5;
const int paused = 6;
class StudentWorld;
class PlayerAvatar;

class Actor : public GraphObject
{
public:
	Actor(int imageID, int startX, int startY, StudentWorld* m, int dir, int depth, double size) : 
    m_world(m),m_imageID(imageID), m_alive(living), GraphObject(imageID, startX*16, startY*16,dir, depth, size) {}
	virtual void doSomething() = 0;
	virtual bool isEnemy() { return false; }
	virtual bool isSquare() { return false; }
	virtual bool isDirSquare() { return false; }
	virtual void Activate(int playerNum) { return; }
	void isdead() { m_alive = dead; }
	int ifalive(){ return m_alive;}
	StudentWorld* getWorld() { return m_world;}
private:
	int m_alive, m_imageID;
	StudentWorld* m_world;
};

class Moveable : public Actor
{
public:
	Moveable(int imageID, int startX, int startY, StudentWorld* m, int dir, int depth, double size) : 
	m_teleported(0), roll(0), ticks_to_move(0), walk_dir(right), Actor(imageID, startX, startY, m, dir, depth, size) 
	{
		m_box.x1 = startX * 16;
		m_box.y1 = startY * 16;
		m_box.x2 = m_box.x1 + 15;
		m_box.y2 = m_box.y1 + 15;
	}
	virtual void impact() { return; };
	virtual bool checkNext(int dir);
	bool checkOverlap(Moveable* other);
	bool isWhereIFrom(int n);
	void checkFork(int a[], int& count);
	void teleport();
	void turn();
	void pickRandDir();
	void setTeleported(int n) { m_teleported = n; }
	void setWalkDir(int dir) { walk_dir = dir; }
	void setTicks(int n) { ticks_to_move = n; }
	void setRoll(int n) { roll = n; }
	void setMovement(int dir);
	void updateBox();
	int getUpBound() { return m_box.y2; }
	int getLowBound() { return m_box.y1; }
	int getRightBound() { return m_box.x2; }
	int getLeftBound() { return m_box.x1; }
	int getWalkDir() { return walk_dir; }
	int getTicks() { return ticks_to_move; }
	int getRoll();
	int getTeleported() { return m_teleported; }
	int findOpDir();
private:
	int walk_dir, ticks_to_move, roll, m_teleported;
	struct box {
		int x1, x2, y1, y2;
	}m_box;
};

class Enemy : public Moveable
{
public:
	Enemy(int imageID, int startX, int startY, StudentWorld* m, int dir, int depth, double size, int state, int counter) :
	pause_counter(counter), m_state(state), Moveable(imageID, startX, startY, m, dir, depth, size) 
	{
		m_activate[0] = unactivated;
		m_activate[1] = unactivated;
	}
	virtual bool isEnemy() { return true; }
	virtual void impact();
	void startMove(int n);
	void walk();
	int getState() { return m_state; }
	int getCounter() { return pause_counter; }
	void setState(int n) { m_state = n; }
	void setCounter(int n) { pause_counter = n; }
	void setActivate(int playerNum, int activate) { m_activate[playerNum - 1] = activate; }
	int getActivate(int playerNum) { return m_activate[playerNum - 1]; }
private:
	int m_state, pause_counter, m_activate[2];
};

class Bowser : public Enemy
{
public: 
	Bowser(int startX, int startY, StudentWorld* m, int dir=right, int depth=0, double size=1.0, int state= paused, int counter=180) :
		Enemy(IID_BOWSER, startX, startY, m, dir, depth, size, state, counter) {}
	virtual void doSomething();
private:

};

class Boo : public Enemy
{
public:
	Boo(int num, int startX, int startY, StudentWorld* m, int dir = right, int depth = 0, double size = 1.0, int state= paused, int counter = 180) :
	Num(num),Enemy(IID_BOO, startX, startY, m, dir, depth, size, state, counter) {}
	virtual void doSomething();
private:
	int Num;
};

class Vortexes : public Moveable
{
public:
	Vortexes(PlayerAvatar* p, int x, int y, StudentWorld* m) : m_player(p), Moveable(IID_VORTEX, x, y, m, right, 0, 1){};
	bool inBound();
	virtual void doSomething();
private:
	PlayerAvatar* m_player;
};

class PlayerAvatar : public Moveable
{
public:
	PlayerAvatar(int playerNum,int imageID, int startX, int startY, StudentWorld* m, int dir = right, int depth = 0, double size = 1.0) :
	m_ticksMoved(0), m_steps(0), m_coins(0), star(0), m_vortexNum(0), m_state(waiting_to_roll), m_vortex(nullptr),
	m_playerNum(playerNum), Moveable(imageID, startX, startY, m, dir, depth, size) {}
	virtual void doSomething();
	void update();
	bool addStar();
	void addVor() { m_vortexNum = 1; }
    void addCoin(int n) {m_coins+=n;}
	void addSteps() { m_steps++; }
	void addTicksMoved(int n) { m_ticksMoved += n; }
	void takeCoin(int n);
	void takeStar(int n);
	void takeVor() { m_vortexNum = 0; }
	void swapPlayer(PlayerAvatar* p);
	void setStar(int n) { star = n; }
	void setState(int n) { m_state = n; }
	void setTicksMoved(int n) { m_ticksMoved = n; }
	void swapCoin(PlayerAvatar* p);
	void swapStar(PlayerAvatar* p);
	int getStar() { return star; }
	int getCoin() { return m_coins; }
	int getVor() { return m_vortexNum; }
    int getPlayerNum() {return m_playerNum;}
    int getState() {return m_state;}
	int getSteps() { return m_steps; }
	int getTicksMoved() { return m_ticksMoved; }
	Vortexes* getVortex(){ return m_vortex; }
private:
	int m_coins, star, m_vortexNum, m_state, m_playerNum, m_steps, m_ticksMoved;
	Vortexes* m_vortex;
	//std::vector <Vortexes*> m_vortex;

};

class Square : public Actor
{
public:
    Square(int imageID, int startX, int startY, StudentWorld* m, int dir, int depth , double size ) : Actor(imageID, startX, startY, m, dir, depth, size)
    {
		m_state[0]=unactivated;
        m_state[1]=unactivated;
    }
    void setState(int playerNum, int state) {m_state[playerNum-1]=state;}
    int getState(int playerNum) {return m_state[playerNum-1];}
	virtual bool isSquare() { return true; }
	virtual bool isDirSquare() { return false; }
	virtual void Activate(int playerNum);
	virtual void doSomething();
    virtual void op(PlayerAvatar* p)=0;
   // bool ifPlayerMoved(PlayerAvatar* p);
private:
   int m_state[2];
};

class UnconditionalSquare : public Square
{
public:
	UnconditionalSquare(int imageID, int startX, int startY, StudentWorld* m, int dir, int depth = 1, double size = 1.0) :
		Square(imageID, startX, startY, m, dir, depth, size) {}
	virtual void Activate(int playerNum);
	virtual bool isSquare() { return true; }
	virtual bool isDirSquare() { return false; }
private:

};

class StarSquare : public UnconditionalSquare
{
public:
	StarSquare(int startX, int startY, StudentWorld* m, int dir = right, int depth = 1, double size = 1.0) :
		UnconditionalSquare(IID_STAR_SQUARE, startX, startY, m, dir, depth, size) {}
	virtual void op(PlayerAvatar* p);
	virtual bool isSquare() { return true; }
	virtual bool isDirSquare() { return false; }
};

class BankSquare : public UnconditionalSquare
{
public:
	BankSquare(int startX, int startY, StudentWorld* m, int dir=right, int depth = 1, double size = 1.0) :
	UnconditionalSquare(IID_BANK_SQUARE, startX, startY, m, dir, depth, size) {}
	virtual void op(PlayerAvatar* p) ;
	virtual bool isSquare() { return true; }
	virtual bool isDirSquare() { return false; }
private:

};

class DirSquare : public UnconditionalSquare
{
public:
	DirSquare(int startX, int startY, StudentWorld* m, int dir, int depth = 1, double size = 1.0) :
	m_dir(dir), UnconditionalSquare(IID_DIR_SQUARE, startX, startY, m, dir, depth, size) {}
	virtual void op(PlayerAvatar* p);
	virtual bool isSquare() { return true; }
	virtual bool isDirSquare() { return true; }
private:
	int m_dir;
};

class DroppingSquare : public Square
{
public:
	DroppingSquare(int startX, int startY, StudentWorld* m, int dir = right, int depth = 1, double size = 1.0) :
		Square(IID_DROPPING_SQUARE, startX, startY, m, dir, depth, size) {}
	virtual void op(PlayerAvatar* p);
	virtual bool isSquare() { return true; }
	virtual bool isDirSquare() { return false; }
private:

};

class EventSquare : public Square
{
public:
	EventSquare(int startX, int startY, StudentWorld* m, int dir = right, int depth = 1, double size = 1.0) :
		Square(IID_EVENT_SQUARE, startX, startY, m, dir, depth, size) {}
	virtual void op(PlayerAvatar* p) ;
	virtual bool isSquare() { return true; }
	virtual bool isDirSquare() { return false; }
private:

};

class CoinSquare : public Square
{
public:
	CoinSquare(int imageID, int startX, int startY, StudentWorld* m, int dir = right, int depth = 1, double size = 1.0) : 
	Square(imageID, startX, startY, m, dir, depth, size) {}
    virtual void op(PlayerAvatar* p)=0;
	virtual bool isSquare() { return true; }
	virtual bool isDirSquare() { return false; }
private:

};

class BlueCoinSquare : public CoinSquare
{
public:
	BlueCoinSquare(int startX, int startY, StudentWorld* m, int dir = right, int depth = 1, double size = 1.0) :
		CoinSquare(IID_BLUE_COIN_SQUARE, startX, startY, m, dir, depth, size) {}
	virtual void op(PlayerAvatar* p);
	virtual bool isDirSquare() { return false; }
	virtual bool isSquare() { return true; }
private:

};

class RedCoinSquare : public CoinSquare
{
public:
	RedCoinSquare(int startX, int startY, StudentWorld* m, int dir = right, int depth = 1, double size = 1.0) :
		CoinSquare(IID_RED_COIN_SQUARE, startX, startY, m, dir, depth, size) {}
	virtual void op(PlayerAvatar* p);
	virtual bool isSquare() { return true; }
	virtual bool isDirSquare() { return false; }
private:

};

/*
class ActivatingObject : public GameObject {
public:
  ActivatingObject(StudentWorld *sw, int imageID, int startX, int startY, int dir, double size, int depth);
  void do_something();
};

class ActivateOnPlayer : public ActivatingObject {
public:
  ActivateOnPlayer(StudentWorld *sw, int imageID, int startX, int startY,
                   int dir, double size, int depth, bool activate_when_go_lands);
};

class Vortex : public ActivatingObject {
public:
  Vortex(StudentWorld *sw, int imageID, int startX, int startY, DIR dir);
  bool is_a_square() const;
  bool can_be_hit_by_vortex() const;
  std::vector<GameObject*> do_i_activate();
  void do_something();
};*/

#endif // ACTOR_H_
