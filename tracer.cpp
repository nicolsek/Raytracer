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

/* Includes for SFML */
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <iostream>

class Player {
	//const int FOV = 60; //The player's field of view. TODO: 
	const int Height = 32; //Height of player in units.

public:
	double x = 13, y = 0; //Coordinate position of where the player is. Magic starting values.
	double dirX = -1, dirY = 0; //Direction vector of player.
} player;

class Camera { //Projection plane
	const int Width = 320; //Dimensions of the projection plane.
	const int Height = 200;

public:
	double x, y; //Position of the camera itself.
	double planeX = 0, planeY = .66; //Vector of the projection plane.
} cam;

class Ray {
public:
	double x, y;
	double dirX, dirY;

	int mapX, mapY; //Coordinates on the actual map array.

	double sideDistX, sideDistY; //Length of ray from current position to next x or y side.
	double deltaDistX, deltaDistY; //Length of ray from one x or y side to next x or y side;

	double perpWallDist; //Will be used to calculate the length of the ray.

	short stepX, stepY; //What direction to step in, either 0 or 1.

	bool hit; //Has this hit a wall?
	bool side; //Type of the hit? NS - EW?
} ray;

double oldTime, newTime;

const int mapWidth = 6;
const int mapHeight = 6;

const int windowWidth = 800;
const int windowHeight = 600;

//All the zero's in the grid are empty space, so basically you see a very big room, 
//with a wall around it (the values 1), 
//a small room inside it (the values 2), 
//a few pilars (the values 3), 
//and a corridor with a room (the values 4).

int map[mapWidth][mapHeight] = 
{
  {1,1,1,1,1,1},
  {1,0,0,0,0,1},
  {1,0,0,0,0,1},
  {1,0,0,0,0,1},
  {1,0,0,0,0,1},
  {1,1,1,1,1,1},
};

// debug ... Debug information about the stuff.
void debug() {
	std::cout << "Player-x: " << player.x << std::endl;
	std::cout << "Player-x Dir: " << player.dirX << std::endl;
}

int main() {
	//Create the window!
	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Ray Casting!");

	//Set window active for rendering
	window.setActive();

	sf::Clock deltaClock;

	//Loop for event handling
	while (window.isOpen()) {

		//debug();

		//Create event type event.
		sf::Event event;
		while(window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		//For every vertical stripe.
		for (int x = 0; x < windowWidth; x++) {
			/* Calculate ray position and direction */
			cam.x = 2 * x / double(windowWidth) - 1; //Current camera coordinate from the pixel coordinate.
			
			//Set ray position to current player position.
			ray.x = player.x;
			ray.y = player.y;

			ray.dirX = player.dirX + cam.planeX * cam.x;
			ray.dirY = player.dirY + cam.planeY * cam.x;

			/* Where the ray actually is map wise. */
			ray.mapX = int(ray.x);
			ray.mapY = int(ray.y);

			ray.deltaDistX = sqrt(1 + pow(ray.dirY, 2) / pow(ray.dirX, 2));
			ray.deltaDistY = sqrt(1 + pow(ray.dirX, 2) / pow(ray.dirY, 2));

			if (ray.dirX < 0) {
				ray.stepX = -1;
				ray.sideDistX = (ray.x - ray.mapX) * ray.deltaDistX;
			} else {
				ray.stepX = 1;
				ray.sideDistX = (ray.mapX + 1 - ray.x) * ray.deltaDistX;
			}

			if (ray.dirY < 0) {
				ray.stepY = -1;
				ray.sideDistY = (ray.y - ray.mapY) * ray.deltaDistY;
			} else {
				ray.stepY = 1;
				ray.sideDistY = (ray.mapY + 1 - ray.y) * ray.deltaDistY;
			}

			/* Digital Differential Analyzer (DDA) Alg! */
			while(!ray.hit) {
				if (ray.sideDistX < ray.sideDistY) {
					ray.sideDistX += ray.deltaDistX;
					ray.mapX += ray.stepX;
					ray.side = 0;
				} else {
					ray.sideDistY += ray.deltaDistY;
					ray.mapY += ray.stepY;
					ray.side = 1;
				}

				//Has the ray hit a wall?
				if (map[ray.mapX][ray.mapY] > 0) {
					ray.hit = true;
				}
			}

			//Calculate the ray distance projected on to the cameras direction. 
			if (!ray.side) {
				ray.perpWallDist = (ray.mapX - ray.x + (1 - ray.stepX) / 2) / ray.dirX;
			} else {
				ray.perpWallDist = (ray.mapY - ray.y + (1 - ray.stepY) / 2) / ray.dirY;
			}

			/* Calculate the drawing height for the walls */
			int lineHeight = (int)(windowHeight / ray.perpWallDist);

			//Calculate lowest and highest pixel to fill in collumn.
			int drawStart = -lineHeight / 2 + windowHeight / 2;

			//Cap to 0.
			if (drawStart < 0) {
				drawStart = 0;
			}

			int drawEnd = lineHeight / 2 + windowHeight / 2;

			//Cap top height - 1.
			if (drawEnd >= windowHeight) {
				drawEnd = windowHeight - 1;
			}

			sf::Color color(0, 0, 0);

			/* Color the walls depending on type. */
			switch(map[ray.mapX][ray.mapY]) {
				case 1:
					color = sf::Color::White;
					break;
				case 2:
					color = sf::Color::Green;
					break;
				case 3:
					color = sf::Color::Blue;
					break;
				case 4:
					color = sf::Color::White;
					break;
				case 0:
					color = sf::Color::White;
					break;
			}

			if (ray.side) {
				color.r /= 2;
				color.g /= 2;
				color.b /= 2;
			}

			/* Actually Rendering */
			sf::VertexArray lines(sf::LineStrip, 2);
			lines[0].position = sf::Vector2f(x, drawStart);
			lines[0].color = color;

			lines[1].position = sf::Vector2f(x, drawEnd);
			lines[1].color = color;

			window.draw(lines);
		}

		window.display();
		window.clear();

		sf::Time dt = deltaClock.restart();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
			player.dirX += 10 * dt.asSeconds() / 100;
		}
	}

	return 0;
}