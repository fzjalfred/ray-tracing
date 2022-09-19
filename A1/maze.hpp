// Termm--Fall 2022

#pragma once
#include "Cube.hpp"
#include <vector>
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

	std::vector<Cube> cubes;
	
private:
	size_t m_dim;
	int *m_values;
	
	void recDigMaze(int r, int c);
	int numNeighbors(int r, int c);
};
