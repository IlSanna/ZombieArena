#include <SFML/Graphics.hpp>
#include "Player.h"

using namespace sf;

int main() {

	enum class State {//game states
		PAUSED, GAME_OVER, PLAYING, LEVELING_UP
	};
	State state = State::GAME_OVER;//starting from game over state
	Vector2f resolution;
	//get screen resolution
	resolution.x = VideoMode::getDesktopMode().width;//potrei dividere per 2
	resolution.y = VideoMode::getDesktopMode().height;
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
		}
	}

	return 0;
}