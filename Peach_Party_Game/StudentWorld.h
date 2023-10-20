#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Board.h"
#include "Actor.h"
#include <sstream>
#include <iomanip>
#include <string>
using namespace std;

const int Empty = 0;
const int Squares = 1;
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Actor;
class PlayerAvatar;
class Moveable;

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetPath);
    ~StudentWorld()
    {
        cleanUp();
        return ;
    }
	virtual int init();
	virtual int move();
	virtual void cleanUp();
	void displayGameText(int p1roll, int p1star, int p1coin, int p1vor, int bank, int p2roll, int p2star, int p2coin, int p2vor);
	bool checkSquare(int i, int j, int dir);
	Actor* getActor(int n) { return A[n]; }
	Actor* getSquareAt(int x, int y);
	Moveable* getMoveableCharacter(int n) { return M[n]; }
	int getSize() { return A.size(); }
	int getMoveableSize() { return M.size(); }
	int getNumOfPlayer() { return P.size(); }
	int MaxX() { return edgeX; }
	int MaxY() { return edgeY; }
	void push(int i, int j, Actor* m);
	void pushMoveableCharacter(int i, int j, Moveable* m);
	void addBank(int n) { bank += n; }
	int takeBank() { int tmp = bank; bank = 0; return tmp; }
	PlayerAvatar* getPlayer(int playerNum);
private:
	std::vector <Actor*> A;
	std::vector <PlayerAvatar* > P;
	std::vector <Moveable*> M;
	int MAP[30][30];
	int edgeX;
	int edgeY;
	int bank;
	ostringstream oss;
};

#endif
