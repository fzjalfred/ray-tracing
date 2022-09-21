// Termm--Fall 2022

#pragma once
#include "Cube.hpp"
#include "Sphere.hpp"
#include <vector>
#include <map>
class Maze
{
public:
	Maze( size_t dim );
	~Maze();

	void reset();

	size_t getDim() const;

	int getValue( int x, int y ) const;

	void setValue( int x, int y, int h );

	void digMaze();
	void printMaze(); // for debugging
	void drawMaze(GLint col_uni);
	void genMazeCubes(ShaderProgram& m_shader);

	void playerMove(int key);
	void playerCheatMove(int key);

	std::map<int, Cube> cubes;
	Sphere* player = NULL;
	
private:
	size_t m_dim;
	int *m_values;

	int player_x = 0;
	int player_y = 0;
	
	void recDigMaze(int r, int c);
	int numNeighbors(int r, int c);
};
