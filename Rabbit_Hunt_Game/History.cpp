#include <iostream>
#include "History.h"

using namespace std;

History::History(int nRows, int nCols)
{
    m_rows = nRows;
    m_cols = nCols;
    for (int r = 1; r <= m_rows; r++)
        for (int c = 1; c <= m_cols; c++)
            m_grid[r - 1][c - 1] = 0;
}
bool History::record(int r, int c)
{
    if(!isPosInBounds(r, c))
        return false; 
    m_grid[r - 1][c - 1]++;
    return true;
}
void History::display() const
{
    clearScreen();
    for (int r = 1; r <= m_rows; r++)
    {
        for (int c = 1; c <= m_cols; c++)
        {
            cout << convert(m_grid[r - 1][c - 1]);
        }
        cout << endl;
    }
    cout << endl;
}
bool History::isPosInBounds(int r, int c) const
{
    return (r >= 1 && r <= m_rows && c >= 1 && c <= m_cols);
}
char History::convert(int n) const
{
    if (n == 0)
        return '.';
    else if (n >= 1 && n <= 25)
    {
        return char(n + 64);
    }
    else if (n >= 26)
        return 'Z';
}