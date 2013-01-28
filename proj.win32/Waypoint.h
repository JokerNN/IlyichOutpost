#pragma once

#include <stdlib.h>
#include <vector>

#include "FileNames.h"

using namespace std;

enum CellState
{
	STATE_CELL_FREE = 0,
	STATE_CELL_BUSY,
	STATE_CELL_BUILD
};

struct Cell
{
	CellState type;
	int x; // center of cell
	int y; // center of cell
};

class Waypoint
{
	vector<Cell *> way;

public:
	Waypoint(void);
	~Waypoint(void);

	void AddPoint(Cell *);
	void RemovePoint(const Cell *);

	Cell* GetPoint(const int index);
};

