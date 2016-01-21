#include "InhabitedGrid.hpp"
#include "Unit.hpp"
#include <cmath>

int pythagoreanDistance(Coordinate a, Coordinate b){
	return pow(pow(a.first-b.first, 2) + pow(a.second-b.second, 2), 0.5);
}

InhabitedGrid::InhabitedGrid(int w, int h, int dw, int dh):
		cellsX(dw),
		cellsY(dh),
		cellWidth(w),
		cellHeight(h)
		{}

Coordinate InhabitedGrid::getCellCoords(Coordinate p){
	return std::pair<int, int>(p.first/cellWidth, p.second/cellWidth);
}

void InhabitedGrid::move(Unit* unit, Coordinate c){
	std::pair<int, int> oldpos, newpos;
	oldpos = getCellCoords(unit->xy);
	newpos = getCellCoords(c);
	unit->xy = c;
	if (oldpos==newpos){
		return;
	}
	else{
		grid[oldpos].erase(unit->id);
		grid[oldpos].erase(unit->id);
	}

}
