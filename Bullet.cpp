#include "Bullet.h"

Bullet::Bullet()
{
	m_BulletShape.setSize(sf::Vector2f(4, 4));
	m_BulletShape.setFillColor(Color::Yellow);
}

void Bullet::shoot(Vector2f start, Vector2f target)
{
	m_InFlight = true;
	m_Position = start;

	float gradient = (start.x - target.x) / (start.y / target.y);

	if (gradient < 0)
	{
		gradient *= -1;
	}

	float ratioXY = m_BulletSpeed / (1 + gradient);

	m_BulletDistanceY = ratioXY;
	m_BulletDistanceX = ratioXY * gradient;

	if (target.x < start.x)
	{
		m_BulletDistanceX *= -1;
	}

	if (target.y < start.y)
	{
		m_BulletDistanceY *= -1;
	}

	float range = 1000;
	m_MinX = start.x - range;
	m_MinY = start.y - range;
	m_MaxX = start.x + range;
	m_MaxY = start.y + range;

	m_BulletShape.setPosition(m_Position);
}

void Bullet::stop()
{
	m_InFlight = false;
}
bool Bullet::isInFlight()
{
	return m_InFlight;
}
FloatRect Bullet::getPosition()
{
	return m_BulletShape.getGlobalBounds();
}
RectangleShape Bullet::getShape()
{
	return m_BulletShape;
}

void Bullet::update(float elapsedTime)
{
	m_Position.x += m_BulletDistanceX * elapsedTime;
	m_Position.y += m_BulletDistanceY * elapsedTime;

	m_BulletShape.setPosition(m_Position);

	if (m_Position.x < m_MinX || m_Position.x > m_MaxX ||
		m_Position.y < m_MinY || m_Position.y > m_MaxY)
	{
		m_InFlight = false;
	}
}
