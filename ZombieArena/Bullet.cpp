#include "Bullet.h"

Bullet::Bullet() {
	m_BulletShape.setSize(Vector2f(3, 3));
}
void Bullet::shoot(float startX, float startY, float targetX, float targetY) {
	m_Flying = true;
	m_Position = Vector2f(startX, startY);

	// Calculate the gradient of the flight path
	float gradient = (startX - targetX) / (startY - targetY);//forse sbagliato
	// Any gradient less than 1 needs to be negative
	if (gradient < 0) { gradient *= -1; }//to keep it always positive
	// Calculate the ratio between x and y
	float ratioXY = m_speed / (1 + gradient);
	// Set the "speed" horizontally and vertically
	m_BulletDistanceY = ratioXY;
	m_BulletDistanceX = ratioXY * gradient;
	// Point the bullet in the right direction
	targetX < startX ? m_BulletDistanceX *= -1 : m_BulletDistanceX;
	targetY < startY ? m_BulletDistanceY *= -1 : m_BulletDistanceY;
	
	// Set a max range of 1000 pixels
	float range = 1000;
	m_MinX = startX - range;
	m_MaxX = startX + range;
	m_MinY = startY - range;
	m_MaxY = startY + range;
	// Position the bullet ready to be drawn
	m_BulletShape.setPosition(m_Position);
}

void Bullet::update(float elapsedTime) {
	m_Position.x += m_BulletDistanceX * elapsedTime;
	m_Position.y += m_BulletDistanceY * elapsedTime;
	// Move the bullet
	m_BulletShape.setPosition(m_Position);
	// Has the bullet gone out of range?
	if (m_Position.x < m_MinX || m_Position.x > m_MaxX ||
		m_Position.y < m_MinY || m_Position.y > m_MaxY) {
		m_Flying = false;
	}
}
void Bullet::stop() {
	m_Flying = false;
}
FloatRect Bullet::getPosition() {
	return m_BulletShape.getGlobalBounds();
}
RectangleShape Bullet::getShape() {
	return m_BulletShape;
}
bool Bullet::isFlying() {
	return m_Flying;
}