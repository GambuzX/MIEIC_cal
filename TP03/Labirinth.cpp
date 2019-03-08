/*
 * labirinth.cpp
 */

#include "Labirinth.h"

#include <iostream>
using namespace std;



Labirinth::Labirinth(int values[10][10])
{
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 10; j++)
			labirinth[i][j] = values[i][j];
	initializeVisited();
}


void Labirinth::initializeVisited()
{
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 10; j++)
			visited[i][j] = false;
}




void  Labirinth::printLabirinth()
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
			cout << labirinth[i][j] << " ";

		cout << endl;
	}
}


bool Labirinth::findGoal(int x, int y)
{
	// Check array boundaries
	if (x < 0 || y < 0 || x > 9 || y > 9) return false;

	// Check if already visited or a wall
	if (visited[x][y] || labirinth[x][y] == 0) return false;

	// Found element
	if (labirinth[x][y] == 2) return true;

	// Did not find element
	visited[x][y] = true;

	// Search in all directions
	bool top = findGoal(x, y-1);
	bool bottom = findGoal(x, y+1);
	bool left = findGoal(x-1,y);
	bool right = findGoal(x+1,y);

	return top || bottom || left || right;
}


