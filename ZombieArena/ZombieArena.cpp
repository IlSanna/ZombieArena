#include <SFML/Graphics.hpp>
#include "ZombieArena.h"
#include <sstream>

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
	// About the game
	int score = 0;
	int hiScore = 0;
	// For the home/game over screen
	Sprite spriteGameOver;
	Texture textureGameOver = TextureHolder::GetTexture("src/Graphics/background.png");
	spriteGameOver.setTexture(textureGameOver);
	spriteGameOver.setPosition(0, 0);
	spriteGameOver.setScale(0.5, 0.5);
	// Create a view for the HUD
	View hudView(sf::FloatRect(0, 0, resolution.x, resolution.y));
	// Create a sprite for the ammo icon
	Sprite spriteAmmoIcon;
	Texture textureAmmoIcon = TextureHolder::GetTexture("src/Graphics/ammo_icon.png");
	spriteAmmoIcon.setTexture(textureAmmoIcon);
	spriteAmmoIcon.setPosition(20, resolution.y - 50);
	spriteAmmoIcon.setScale(0.7, 0.7);
	// Load the font
	Font font;
	font.loadFromFile("src/Fonts/Kust.ttf");
	// Paused
	Text pausedText;
	pausedText.setFont(font);
	pausedText.setCharacterSize(50);
	pausedText.setFillColor(Color::White);
	pausedText.setPosition(resolution.x/2, resolution.y / 2);
	pausedText.setString("Press Enter \n to continue");
	// Game Over
	Text gameOverText;
	gameOverText.setFont(font);
	gameOverText.setCharacterSize(50);
	gameOverText.setFillColor(Color::White);
	gameOverText.setPosition((resolution.x / 2)-250, resolution.y / 2);
	gameOverText.setString("Press Enter to play");
	// LEVELING up
	Text levelUpText;
	levelUpText.setFont(font);
	levelUpText.setCharacterSize(40);
	levelUpText.setFillColor(Color::White);
	levelUpText.setPosition(150, 200);
	std::stringstream levelUpStream;
	levelUpStream <<
		"1- Increased rate of fire" <<
		"\n2- Increased clip size(next reload)" <<
		"\n3- Increased max health" <<
		"\n4- Increased run speed" <<
		"\n5- More and better health pickups" <<
		"\n6- More and better ammo pickups";
	levelUpText.setString(levelUpStream.str());
	// Ammo
	Text ammoText;
	ammoText.setFont(font);
	ammoText.setCharacterSize(30);
	ammoText.setFillColor(Color::White);
	ammoText.setPosition(60, resolution.y - 50);
	// Score
	Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(30);
	scoreText.setFillColor(Color::White);
	scoreText.setPosition(20, 0);
	// Hi Score
	Text hiScoreText;
	hiScoreText.setFont(font);
	hiScoreText.setCharacterSize(30);
	hiScoreText.setFillColor(Color::White);
	hiScoreText.setPosition(resolution.x-150, 0);
	std::stringstream s;
	s << "Hi Score:" << hiScore;
	hiScoreText.setString(s.str());
	// Zombies remaining
	Text zombiesRemainingText;
	zombiesRemainingText.setFont(font);
	zombiesRemainingText.setCharacterSize(30);
	zombiesRemainingText.setFillColor(Color::White);
	zombiesRemainingText.setPosition(resolution.x -150, resolution.y -50);// 1500, 980
	zombiesRemainingText.setString("Zombies: 100");
	// Wave number
	int wave = 0;
	Text waveNumberText;
	waveNumberText.setFont(font);
	waveNumberText.setCharacterSize(30);
	waveNumberText.setFillColor(Color::White);
	waveNumberText.setPosition(resolution.x - 300, resolution.y - 50);//1250, 980
	waveNumberText.setString("Wave: 0");
	// Health bar
	RectangleShape healthBar;
	healthBar.setFillColor(Color::Red);
	healthBar.setScale(0.7, 0.7);
	healthBar.setPosition(resolution.x - 600, resolution.y - 50);

	// When did we last update the HUD?
	int framesSinceLastHUDUpdate = 0;
	// How often (in frames) should we update the HUD
	int fpsMeasurementFrameInterval = 1000;

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
			// Collision detection
			// Have any zombies been shot?
			for (int i = 0; i < 100; i++) {//100 is the number of bullets
				for (int j = 0; j < numZombies; j++) {
					if (bullets[i].isFlying() && zombies[j].isAlive()) {
						if (bullets[i].getPosition().intersects(zombies[j].getPosition())) {
							// Stop the bullet
							bullets[i].stop();
							// Register the hit and see if it was a kill
							if (zombies[j].hit()) {
								// Not just a hit but a kill too
								score += 10;
								if (score >= hiScore) {
									hiScore = score;
								}
								numZombiesAlive--;
								// When all the zombies are dead (again)
								if (numZombiesAlive == 0) {
									state = State::LEVELING_UP;
								}
							}
						}
					}
				}
			}// End zombie being shot
			// Have any zombies touched the player
			for (int i = 0; i < numZombies; i++) {
				if (player.getPosition().intersects
				(zombies[i].getPosition()) && zombies[i].isAlive()) {
					if (player.hit(gameTimeTotal)) {
						// More here later
					}
					if (player.getHealth() <= 0) {
						state = State::GAME_OVER;
					}
				}
			}// End player touched
			// Has the player touched health pickup
			if (player.getPosition().intersects(healthPickup.getPosition()) && healthPickup.isSpawned()) {
				player.increaseHealthLevel(healthPickup.gotIt());
			}
			// Has the player touched ammo pickup
			if (player.getPosition().intersects(ammoPickup.getPosition()) && ammoPickup.isSpawned()) {
				bulletsSpare += ammoPickup.gotIt();
			}
			// size up the health bar
			healthBar.setSize(Vector2f(player.getHealth() * 3, 50));
			// Increment the number of frames since the previous update
			framesSinceLastHUDUpdate++;
			// re-calculate every fpsMeasurementFrameInterval frames
			if (framesSinceLastHUDUpdate > fpsMeasurementFrameInterval) {
				// Update game HUD text
				std::stringstream ssAmmo;
				std::stringstream ssScore;
				std::stringstream ssHiScore;
				std::stringstream ssWave;
				std::stringstream ssZombiesAlive;
				// Update the ammo text
				ssAmmo << bulletsInClip << "/" << bulletsSpare;
				ammoText.setString(ssAmmo.str());
				// Update the score text
				ssScore << "Score:" << score;
				scoreText.setString(ssScore.str());
				// Update the high score text
				ssHiScore << "Hi Score:" << hiScore;
				hiScoreText.setString(ssHiScore.str());
				// Update the wave
				ssWave << "Wave:" << wave;
				waveNumberText.setString(ssWave.str());
				// Update the high score text
				ssZombiesAlive << "Zombies:" << numZombiesAlive;
				zombiesRemainingText.setString(ssZombiesAlive.str());
				framesSinceLastHUDUpdate = 0;
			}// End HUD update
		}//end updating the frame/scene
		//handle the drawing
		if (state == State::PLAYING) {
			window.clear();
			window.setView(mainView);
			window.draw(background, &textureBackground);
			for (int i = 0; i < numZombies; i++) { window.draw(zombies[i].getSprite());}
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
			// Switch to the HUD view
			window.setView(hudView);
			// Draw all the HUD elements
			window.draw(spriteAmmoIcon);
			window.draw(ammoText);
			window.draw(scoreText);
			window.draw(hiScoreText);
			window.draw(healthBar);
			window.draw(waveNumberText);
			window.draw(zombiesRemainingText);
		}
		if (state == State::LEVELING_UP) {
			window.draw(spriteGameOver);
			window.draw(levelUpText);
		}
		if (state == State::PAUSED) {
			window.draw(pausedText);
		}
		if (state == State::GAME_OVER) {
			window.draw(spriteGameOver);
			window.draw(gameOverText);
			window.draw(scoreText);
			window.draw(hiScoreText);
		}
		window.display();
		//end of the drawing
	}//end game loop

	delete[] zombies;
	return 0;
}