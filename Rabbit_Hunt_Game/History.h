#ifndef HISTORY_H
#define HISTORY_H

#include <iostream>
#include <string>
#include "globals.h"

class Arena;

class History
{
	public:
		History(int nRows, int nCols);
		bool record(int r, int c);
		void display() const;
	private:
		int m_grid[MAXROWS][MAXCOLS];
		int m_rows;
		int m_cols;
		bool isPosInBounds(int r, int c) const;
		char convert(int n) const;
};

#endif
