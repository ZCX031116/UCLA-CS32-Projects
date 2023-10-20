class Arena;

#define MAXROWS 20              // max number of rows in the arena
#define MAXCOLS 25               // max number of columns in the arena
#define MAXRABBITS 100           // max number of rabbits allowed
#define INITIAL_RABBIT_HEALTH 2  // initial rabbit health
#define POISONED_IDLE_TIME 1     // poisoned rabbit idles this many turns
                                      //   between moves

#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3
#define NUMDIRS 4

#define EMPTY 0
#define HAS_POISON 1


// prototype declarations of utility functions
int randInt(int lowest, int highest);
bool decodeDirection(char ch, int& dir);
bool attemptMove(const Arena& a, int dir, int& r, int& c);
bool recommendMove(const Arena& a, int r, int c, int& bestDir);
int computeDanger(const Arena& a, int r, int c);
void clearScreen();


