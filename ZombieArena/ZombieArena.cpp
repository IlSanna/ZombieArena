#include <SFML/Graphics.hpp>
#include "Player.h"

using namespace sf;

int main() {
	//Init
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

				}
			}
		}//end polling
		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			window.close();
		}
		//handle player inputs
		if (state == State::PLAYING) {
			Keyboard::isKeyPressed(Keyboard::W) ? player.moveUp() : player.stopUp();
			Keyboard::isKeyPressed(Keyboard::S) ? player.moveDown() : player.stopDown();
			Keyboard::isKeyPressed(Keyboard::A) ? player.moveLeft() : player.stopLeft();
			Keyboard::isKeyPressed(Keyboard::D) ? player.moveRight() : player.stopRight();
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
				int tileSize = 50;
				// Spawn the player in the middle of the arena
				player.spawn(arena, resolution, tileSize);
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
			mouseWorldPosition = window.mapPixelToCoords(Mouse::getPosition(), mainView);
			player.update(deltaTimeAsSeconds, Mouse::getPosition()/2);
			Vector2f playerPos(player.getCenter());//take player new pos
			mainView.setCenter(player.getCenter());//the view/camera follow the player
		}//end updating the frame/scene
		//handle the drawing
		if (state == State::PLAYING) {
			window.clear();
			window.setView(mainView);
			window.draw(player.getSprite());
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

	return 0;
}