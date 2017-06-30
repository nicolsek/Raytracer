///////////////////////////
//Raycasting Requirements//
///////////////////////////

// 1. Walls are always perpendicular to the floor. (90 deg)
// 2. Walls are made of cubes that have the same size.
// 3. The floor is always flat.

// Example --> http://permadi.com/tutorial/raycast/images/figure6.jpg

// Coordinate System --> http://permadi.com/tutorial/raycast/images/figure7.gif, (x increases horizontally right, y increases vertically downwards)

// 1. Player/viewer’s height, player’s field of view (FOV), and player’s position. --> Fov 60 deg, height 32 units (half of wall height), position just exists as unit coordinates.
// 2. Projection plane’s dimension.
// 3. Relationship between player and projection plane.

class Player {
	const int FOV = 60; //The player's field of view.
	const int Height = 32; //Height of player in units.

public:
	double x = 22, y = 12; //Coordinate position of where the player is. Magic starting values.
	double dirX = -1, dirY = 0; //Direction vector of player.

	double oldTime, newTime;
};

class Camera { //Projection plane
	const int Width = 320; //Dimensions of the projection plane.
	const int Height = 200;

public:
	double planeX = 0, planeY = .66; //Vector of the projection plane.
};

const int mapWidth = 24;
const int mapHeight = 24;

//All the zero's in the grid are empty space, so basically you see a very big room, 
//with a wall around it (the values 1), 
//a small room inside it (the values 2), 
//a few pilars (the values 3), 
//and a corridor with a room (the values 4).

int map[mapWidth][mapHeight] = 
{
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

/* Includes for SFML */
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>

int main() {
	//Create the window!
	sf::RenderWindow window(sf::VideoMode(800, 600), "Ray Casting!");

	while (window.isOpen()) {
		//Create event type event.
		sf::Event event;
		while(window.pollEvent(event)) {
			switch (event.type) {
				case (sf::Event::Closed):
					window.close();
					break;
			}
		}
	}

	return 0;
}