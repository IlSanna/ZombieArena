#pragma once
#include <SFML\Graphics.hpp>

using namespace sf;

class Player {
private:
	const float START_SPEED = 200.0f;
	const float START_HEALTH = 100.0f;

	Sprite m_Sprite;
	Texture m_Texture;
	Vector2f m_Resolution;
	IntRect m_Arena;
	int m_TileSize;
	//moving directions
	bool m_UpPressed;
	bool m_DownPressed;
	bool m_LeftPressed;
	bool m_RightPressed;

	int m_Health;//current
	int m_MaxHealth;
	Time m_LastHit;
	float m_Speed;//current
public:
	Player();
	void spawn(IntRect arena,Vector2f resolution,int tileSize);
	void resetPlayerStats();
	bool hit(Time timeHit);
	Time getLastHitTime();
	FloatRect getPosition();
	Vector2f getCenter();
	int getHealth();
	//which angle is the player facing
	float getRotation();
	Sprite getSprite();

	void moveUp(); void moveDown(); void moveLeft(); void moveRight();
	void stopLeft();void stopRight();void stopUp();void stopDown();	// Stop the player moving in a specific direction

	void update(float elapsedTime, Vector2i mousePosition);

	void upgradeSpeed();
	void upgradeHealth();
	void increaseHealthLevel(int amount);
};