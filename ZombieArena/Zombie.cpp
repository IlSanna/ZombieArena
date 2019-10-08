#include "Zombie.h"
#include "TextureHolder.h"
#include <cstdlib>
#include <ctime>

using namespace std;

void Zombie::spawn(float startX, float startY, int type, int seed) {
	switch (type) {
	case 0:
		m_Sprite = Sprite(TextureHolder::GetTexture("src/Graphics/bloater.png"));
		m_Speed = 40;
		m_Health = 5;
		break;
	case 1:
		m_Sprite = Sprite(TextureHolder::GetTexture("src/Graphics/chaser.png"));
		m_Speed = 70;
		m_Health = 1;
		break;
	case 2:
		m_Sprite = Sprite(TextureHolder::GetTexture("src/Graphics/crawler.png"));
		m_Speed = 20;
		m_Health = 3;
		break;
	}

	// Modify the speed to make the zombie unique
	// Every zombie is unique. Create a speed modifier
	srand((int)time(0) * seed);
	// Somewhere between 80 an 100
	float modifier = (rand() % MAX_VARRIANCE) + OFFSET;
	// Express this as a fraction of 1
	modifier /= 100; // Now equals between .7 and 1
	m_Speed *= modifier;

	m_Position.x = startX;
	m_Position.y = startY;
	m_Sprite.setOrigin(25, 25);
	m_Sprite.setScale(0.5, 0.5);
	m_Sprite.setPosition(m_Position);
}

bool Zombie::hit() {
	m_Health--;
	if (m_Health < 0) {//is dead?
		m_Alive = false;
		m_Sprite = Sprite(TextureHolder::GetTexture("src/Graphics/blood.png"));
		return true;
	}
	return false;
}

bool Zombie::isAlive() {
	return m_Alive;
}
FloatRect Zombie::getPosition() {
	return m_Sprite.getGlobalBounds();
}

Sprite Zombie::getSprite() {
	return m_Sprite;
}