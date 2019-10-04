#include "Player.h"

Player::Player() {
	resetPlayerStats();
	/*m_Speed = START_SPEED;
	m_MaxHealth = START_HEALTH;
	m_Health = START_HEALTH;*/

	m_Texture.loadFromFile("src/Graphics/player.png");
	m_Sprite.setTexture(m_Texture);
	m_Sprite.setOrigin(25, 25);//for a smoother rotation, set the origin to the center
	m_Sprite.setScale(0.5, 0.5);
}
void Player::spawn(IntRect arena, Vector2f resolution, int tileSize) {
	m_Position.x = arena.width / 2;
	m_Position.y = arena.height / 2;

	m_Arena = arena;//vedere se funge
	m_TileSize = tileSize;
	m_Resolution = resolution;//vedere se funge
}
void Player::resetPlayerStats() {
	m_Speed = START_SPEED;
	m_Health = START_HEALTH;
	m_MaxHealth = START_HEALTH;
}
bool Player::hit(Time hitTime) {
	//if the time between the last time player was hit is greater than 200
	//health will only be deducted from the player up to five times per second.200*5=1s
	if (hitTime.asMilliseconds() - m_LastHit.asMilliseconds() > 200) {
		m_LastHit = hitTime;//update lasthit
		m_Health -= 10;//damage the player
		return true;
	}
	else {
		return false;
	}
}

void Player::update(float elapsedTime, Vector2i mousePosition) {
	//move
	if (m_UpPressed) {
		m_Position.y -= elapsedTime * m_Speed;
	}
	if (m_DownPressed) {
		m_Position.y += elapsedTime * m_Speed;
	}
	if (m_LeftPressed) {
		m_Position.x -= elapsedTime * m_Speed;
	}
	if (m_RightPressed) {
		m_Position.x += elapsedTime * m_Speed;
	}
	m_Sprite.setPosition(m_Position);
	//create collisions with arena walls, total arena width/height-playertilesize
	if (m_Position.x > m_Arena.width - m_TileSize) {//right
		m_Position.x = m_Arena.width - m_TileSize;
	}
	if (m_Position.x < m_Arena.left + m_TileSize) {//left
		m_Position.x = m_Arena.left + m_TileSize;
	}
	if (m_Position.y > m_Arena.height - m_TileSize) {//bottom
		m_Position.y = m_Arena.height - m_TileSize;
	}
	if (m_Position.y < m_Arena.top + m_TileSize) {//top
		m_Position.y = m_Arena.top + m_TileSize;
	}
	//calculate angle player is facing
	/* ange = arc tangent of y/x and then it convert it from 
	   radiants to degree 180/pi
	   y = mouseY - centerY
	   x = mouseX - centerX
	*/
	float m_angle = (atan2(mousePosition.y - m_Resolution.y / 2,
		mousePosition.x - m_Resolution.x / 2) * 180) /3.141;
	m_Sprite.setRotation(m_angle);
}

void Player::upgradeSpeed() {
	m_Speed += 0.2f * START_SPEED;//20% increase
}

void Player::upgradeHealth() {
	m_Health += 0.2f * START_HEALTH;//20% increase
}

void Player::increaseHealthLevel(int amount) {
	m_MaxHealth += amount;
	// But not beyond the maximum
	if (m_Health > m_MaxHealth) {
		m_Health = m_MaxHealth;
	}
}

Time Player::getLastHitTime() {
	return m_LastHit;
}
FloatRect Player::getPosition() {
	return m_Sprite.getGlobalBounds();
}
Vector2f Player::getCenter() {
	return m_Position;
}
int Player::getHealth() {
	return m_Health;
}
float Player::getRotation() {
	return m_Sprite.getRotation();
}
Sprite Player::getSprite() {
	return m_Sprite;
}

void Player::moveLeft() {
	m_LeftPressed = true;
}
void Player::moveRight() {
	m_RightPressed = true;
}
void Player::moveUp() {
	m_UpPressed = true;
}
void Player::moveDown() {
	m_DownPressed = true;
}
void Player::stopLeft() {
	m_LeftPressed = false;
}
void Player::stopRight() {
	m_RightPressed = false;
}
void Player::stopUp() {
	m_UpPressed = false;
}
void Player::stopDown() {
	m_DownPressed = false;
}