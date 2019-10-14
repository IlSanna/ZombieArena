#include <SFML/Graphics.hpp>
#include "ZombieArena.h"

using namespace sf;

int main() {
	//Init
	TextureHolder holder;
	
	enum class State {//game states
		PAUSED, GAME_OVER, PLAYING, LEVELING_UP
	};
	State state = State::GAME_OVER;//starting from game over state
	Vector2f resolution;
	//get screen resolution
	resolution.x = VideoMode::getDesktopMode().width/2;//potrei dividere per 2
	resolution.y = VideoMode::getDesktopMode().height/2;
	RenderWindow window(VideoMode(resolution.x, resolution.y),"Zombie Arena", Style::None);
	View mainView(FloatRect(0,0,resolution.x,resolution.y));

	Clock clock;//main clock
	Time gameTimeTotal;//How long has the PLAYING state been active

	Vector2f mouseWorldPosition;// Where is the mouse in relation to world coordinates
	Vector2i mouseScreenPosition;// Where is the mouse in relation to screen coordinates

	Player player;
	IntRect arena;
	VertexArray background;
	Texture textureBackground;
	textureBackground.loadFromFile("src/Graphics/background_sheet.png");

	int numZombies, numZombiesAlive;
	Zombie* zombies = new Zombie[5];//must be initialized the first time

	Bullet bullets[100];
	int currentBullet = 0;
	int bulletsSpare = 24;
	int bulletsInClip = 6;
	int clipSize = 6;
	float fireRate = 1;
	
	Time lastPressed;// When was the fire button last pressed?

	// Hide the mouse pointer and replace it with crosshair
	window.setMouseCursorVisible(false);
	Sprite spriteCrosshair;
	Texture textureCrosshair = TextureHolder::GetTexture("src/Graphics/crosshair.png");
	spriteCrosshair.setTexture(textureCrosshair);
	spriteCrosshair.setOrigin(25, 25);
	spriteCrosshair.setScale(0.5, 0.5);

	Pickup healthPickup(1);
	Pickup ammoPickup(2);

	while (window.isOpen()) {
		//input handlings
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::KeyPressed) {
				//if i press return while playing
				if (event.key.code == Keyboard::Return && state == State::PLAYING) {
					state = State::PAUSED;
				}
				//if i press return while playing
				if (event.key.code == Keyboard::Return && state == State::PAUSED) {
					state = State::PLAYING;
					clock.restart();//restart to avoid frame jumps
				}//new game
				else if (event.key.code == Keyboard::Return && state == State::GAME_OVER) {
					state = State::LEVELING_UP;
				}
				if (state == State::PLAYING) {
					// Reloading
					if (event.key.code == Keyboard::R) {
						if (bulletsSpare >= clipSize) {
							// Plenty of bullets. Reload.
							bulletsInClip = clipSize;
							bulletsSpare -= clipSize;
						}
						else if (bulletsSpare > 0) {
							// Only few bullets left
							bulletsInClip = bulletsSpare;
							bulletsSpare = 0;
						}
						else {
							// More here soon?!
						}
					}
				}
			}
		}//end polling
		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			window.close();
		}
		//handle player inputs
		if (state == State::PLAYING) {
			//movement
			Keyboard::isKeyPressed(Keyboard::W) ? player.moveUp() : player.stopUp();
			Keyboard::isKeyPressed(Keyboard::S) ? player.moveDown() : player.stopDown();
			Keyboard::isKeyPressed(Keyboard::A) ? player.moveLeft() : player.stopLeft();
			Keyboard::isKeyPressed(Keyboard::D) ? player.moveRight() : player.stopRight();

			if (Mouse::isButtonPressed(Mouse::Left)) {
				//1000 because this is the number of milliseconds in a second.
				if ( (gameTimeTotal.asMilliseconds() - lastPressed.asMilliseconds() > 1000 / fireRate)
					 && bulletsInClip > 0 ) {//can shoot
					bullets[currentBullet].shoot(player.getCenter().x, player.getCenter().y,
						mouseWorldPosition.x, mouseWorldPosition.y);
					currentBullet++;
					if (currentBullet > 99) {
						currentBullet = 0;
					}
					lastPressed = gameTimeTotal;
					bulletsInClip--;
				}
			}//end firing
		}//end wasd while playing
		// Handle the LEVELING up state
		if (state == State::LEVELING_UP) {
			// Handle the player LEVELING up
			if (event.key.code == Keyboard::Num1) { state = State::PLAYING; }
			if (event.key.code == Keyboard::Num2) { state = State::PLAYING; }
			if (event.key.code == Keyboard::Num3) { state = State::PLAYING; }
			if (event.key.code == Keyboard::Num4) { state = State::PLAYING; }
			if (event.key.code == Keyboard::Num5) { state = State::PLAYING; }
			if (event.key.code == Keyboard::Num6) { state = State::PLAYING; }
			
			if (state == State::PLAYING) {
				// Prepare the level
				// We will modify the next two lines later
				arena.width = 500;
				arena.height = 500;
				arena.left = 0;
				arena.top = 0;
				int tileSize = createBackground(background,arena);
				//int tileSize = 50;
				// Spawn the player in the middle of the arena
				player.spawn(arena, resolution, tileSize);

				// Configure the pickups
				healthPickup.setArena(arena);
				ammoPickup.setArena(arena);

				//init zombies
				numZombies = 10;
				delete[] zombies;
				zombies = createHorde(numZombies, arena);
				numZombiesAlive = numZombies;

				clock.restart();// Reset the clock so there isn't a frame jump
			}
		}//end leveling up
		//handle frame update
		if (state == State::PLAYING) {
			Time deltaTime = clock.restart();//update delta time
			gameTimeTotal += deltaTime;//update game time
			float deltaTimeAsSeconds = deltaTime.asSeconds();
			/* Mouse::getPosition() use of a static function 
			   If we define a function in a class with the static keyword, 
			   we are able to call that function using the class name and 
			   without an instance of the class
			 */
			mouseScreenPosition = Mouse::getPosition();//where is the mouse
			//convert mouse screen pos to word pos
			mouseWorldPosition = window.mapPixelToCoords(Mouse::getPosition()/2, mainView);
			spriteCrosshair.setPosition(mouseWorldPosition);
			player.update(deltaTimeAsSeconds, Mouse::getPosition()/2);
			Vector2f playerPos(player.getCenter());//take player new pos
			mainView.setCenter(player.getCenter());//the view/camera follow the player

			for (int i = 0; i < numZombies; i++) {
				if (zombies[i].isAlive()) { zombies[i].update(deltaTimeAsSeconds, playerPos); }
			}
			for (int i = 0; i < 100; i++) {
				if (bullets[i].isFlying()) { bullets[i].update(deltaTimeAsSeconds); }
			}
			// Update the pickups
			healthPickup.update(deltaTimeAsSeconds);
			ammoPickup.update(deltaTimeAsSeconds);
		}//end updating the frame/scene
		//handle the drawing
		if (state == State::PLAYING) {
			window.clear();
			window.setView(mainView);
			window.draw(background, &textureBackground);
			//for (int i = 0; i < numZombies; i++) { window.draw(zombies[i].getSprite());}
			//draw bullets
			for (int i = 0; i < 100; i++) { 
				if (bullets[i].isFlying()) { 
					window.draw(bullets[i].getShape());
				} 
			}
			if (ammoPickup.isSpawned()) { window.draw(ammoPickup.getSprite()); }
			if (healthPickup.isSpawned()) { window.draw(healthPickup.getSprite()); }
			window.draw(player.getSprite());
			window.draw(spriteCrosshair);
		}
		if (state == State::LEVELING_UP) {
		}
		if (state == State::PAUSED) {
		}
		if (state == State::GAME_OVER) {
		}
		window.display();
		//end of the drawing
	}//end game loop

	delete[] zombies;
	return 0;
}