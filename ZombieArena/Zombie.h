#pragma once
#include <SFML\Graphics.hpp>

using namespace sf;

class Zombie {
private:
	const float BLOATER_SPEED = 40;
	const float CHASER_SPEED = 80;
	const float CRAWLER_SPEED = 20;

	const float BLOATER_HEALTH = 5;
	const float CHASER_HEALTH = 1;
	const float CRAWLER_HEALTH = 3;

	/*we will vary the speed of every zombie by a small percentage
	from these values. This stops zombies of the same type from bunching 
	up together as they pursue the player.*/
	const int MAX_VARRIANCE = 30;
	const int OFFSET = 101 - MAX_VARRIANCE;

	Vector2f m_Position;
	Sprite m_Sprite;
	float m_Speed;
	float m_Health;
	bool m_Alive;
public:
	// Handle when a bullet hits a zombie
	bool hit();
	bool isAlive();
	void spawn(float startX, float startY, int type, int seed);
	FloatRect getPosition();
	Sprite getSprite();
	void update(float elapsedTime, Vector2f playerLocation);
};