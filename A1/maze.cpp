// Termm--Fall 2022

#include <algorithm>
#include <cstdio>
#include <iostream>

#include "A1.hpp"
#include "maze.hpp"

Maze::Maze( size_t D )
	: m_dim( D )
{
	m_values = new int[ D * D ];

	reset();
}

void Maze::reset()
{
	size_t sz = m_dim*m_dim;
	std::fill( m_values, m_values + sz, 0 );
}

Maze::~Maze()
{
	delete player;
	delete [] m_values;
}

size_t Maze::getDim() const
{
	return m_dim;
}

int Maze::getValue( int x, int y ) const
{
	return m_values[ y * m_dim + x ];
}

void Maze::setValue( int x, int y, int h )
{
	m_values[ y * m_dim + x ] = h;
}

int perm[24][4] = {
	0,1,2,3,
	0,1,3,2,
	0,2,1,3,
	0,2,3,1,
	0,3,1,2,
	0,3,2,1,

	1,0,2,3,
	1,0,3,2,
	1,2,0,3,
	1,2,3,0,
	1,3,0,2,
	1,3,2,0,

	2,1,0,3,
	2,1,3,0,
	2,0,1,3,
	2,0,3,1,
	2,3,1,0,
	2,3,0,1,

	3,1,2,0,
	3,1,0,2,
	3,2,1,0,
	3,2,0,1,
	3,0,1,2,
	3,0,2,1,
};

int Maze::numNeighbors(int r, int c) {
	return getValue(r-1,c) + getValue(r+1,c) + getValue(r,c-1) + getValue(r,c+1);
}

// Print ASCII version of maze for debugging
void Maze::printMaze() {
	int i,j;
	for (i=0; i<m_dim; i++) {
		for (j=0; j<m_dim; j++) { 
			if ( getValue(i,j)==1 ) {
				printf("X");
			} else if (getValue(i,j)==2) {
				printf("O");
			} else if (getValue(i,j)==3) {
				printf("E");
			} else {
				printf(" ");
			}
		}
		printf("\n");
	}
}

void Maze::drawMaze(GLint col_uni) {
	if (player != NULL) {
		player->draw(col_uni);
	}
	for (auto& i: cubes) {
		i.second.draw(col_uni);
	}
}

void Maze::genMazeCubes(ShaderProgram& m_shader) {
	int i,j;
	if (!cubes.empty()) {
		cubes.clear();
	}
	for (i=0; i<m_dim; i++) {
		for (j=0; j<m_dim; j++) { 
			if ( getValue(i,j)==1 ) {
				cubes.insert(std::make_pair(j*m_dim+i, Cube(m_shader, (float)i, 0.0f, (float)j)));
			} else if ( getValue(i,j)==2 ) {
				player = new Sphere(m_shader, (float)i, 0.0f, (float)j);
			} else {
				
			}
		}
		
	}
}

void Maze::playerCheatMove(int key) {
    int x = player_x;
	int y = player_y;

    if (key == GLFW_KEY_UP) {
		y -= 1;
	}
    if (key == GLFW_KEY_DOWN) {
		y += 1;
    }
    if (key == GLFW_KEY_LEFT) {
		x -= 1;
    }
    if (key == GLFW_KEY_RIGHT) {
		x += 1;
    }
	
	if (0<=x && x<m_dim && 0<=y && y<m_dim) {
            if (m_values[y * m_dim + x] == 1) {
                m_values[y * m_dim + x] = 0;
                std::map<int, Cube>::iterator it = cubes.find(y * m_dim + x);
                if (it != cubes.end()) {
                    cubes.erase(it);
                }
            }
            player_x = x;
            player_y = y;
            player->rePosition(x, y);
        }
	
}

void Maze::playerMove(int key) {
    int x = player_x;
	int y = player_y;

    if (key == GLFW_KEY_UP) {
		y -= 1;
	}
    if (key == GLFW_KEY_DOWN) {
		y += 1;
    }
    if (key == GLFW_KEY_LEFT) {
		x -= 1;
    }
    if (key == GLFW_KEY_RIGHT) {
		x += 1;
    }
	if (m_values[y*m_dim+x] != 1 && 0<=x && x<m_dim && 0<=y && y<m_dim) {
		player_x = x;
		player_y = y;
		player->rePosition(x, y);
	}	
}

void Maze::recDigMaze(int r, int c) {
	int* p;
	p = perm[random()%24];
	for (int i=0; i<4; i++) {
		switch (p[i]) {
		case 0:
			if ( r > 1 && getValue(r-1,c) && numNeighbors(r-1,c)==3 ) {
				setValue(r-1,c,0);
				recDigMaze(r-1,c);
			}
			break;
		case 1:
			if ( r < m_dim-2 && getValue(r+1,c) && numNeighbors(r+1,c)==3 ) {
				setValue(r+1,c,0);
				recDigMaze(r+1,c);
			}
			break;
		case 2:
			if ( c > 1 && getValue(r,c-1) && numNeighbors(r,c-1)==3 ) {
				setValue(r,c-1,0);
				recDigMaze(r,c-1);
			}
			break;
		case 3:
			if ( c < m_dim-2 && getValue(r,c+1) && numNeighbors(r,c+1)==3 ) {
				setValue(r,c+1,0);
				recDigMaze(r,c+1);
			}
			break;
		}
	} 
}

void Maze::digMaze()
{
	int i,j;
	// set all values to 1
	for (i=0;i<m_dim;i++) {
		for (j=0;j<m_dim;j++) {
			setValue(i,j,1);
		}
	}

	// clear out rings
	for (i=1; i<m_dim; i+=2) {
		for (j=i; j<m_dim-i; j++) {
			setValue(i,j,0);
			setValue(m_dim-i-1,m_dim-j-1,0);
			setValue(j,i,0);
			setValue(m_dim-j-1,m_dim-i-1,0);
		}
	}
	// Open and Block rings
	int s,pm;
	s=random()%(11-4+1)+4;
	pm = random()%2;
	setValue(s,2,0);
	if (pm)  { setValue(s+1,1,1); } else { setValue(s-1,1,1); }

	s=random()%(11-4+1)+4;
	setValue(s,13,0);
	if (pm)  { setValue(s-1,14,1); } else { setValue(s+1,14,1); }


	pm = random()%2;
	s=random()%(10-5+1)+5;
	setValue(4,s,0);
	if (pm) { setValue(3,s+1,1); } else { setValue(3,s-1,1); }

	s=random()%(10-5+1)+5;
	setValue(11,s,0);
	if (pm) { setValue(12,s-1,1); } else { setValue(12,s+1,1); }


	pm = random()%2;
	s=random()%(8-7+1)+7;
	setValue(s,6,0);
	if (pm)  { setValue(s+1,5,1); } else { setValue(s-1,5,1); }

	s=random()%(8-7+1)+7;
	setValue(s,9,0);
	if (pm) { setValue(s-1,10,1); } else { setValue(s+1,10,1); }
	
	// pick random start location
	s=random()%(m_dim-2)+1;
	setValue(0,s,0);
	setValue(1,s,2);
	player_x = 1;
	player_y = s;
	// find an end location
	do {
		s=rand()%(m_dim-2)+1;
		if ( getValue(m_dim-2,s)==0 ) {
			setValue(m_dim-1,s,0);
		}
	} while (getValue(m_dim-1,s)==1);

}
