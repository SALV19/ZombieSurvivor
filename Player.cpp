#include "Player.h"
#include "TextureHolder.h"
#include <cmath>

Player::Player()
{
	m_Speed = START_SPEED;
	m_Health = START_HEALTH;
	m_MaxHealth = START_HEALTH;

	/*m_Texture.loadFromFile("graphics/player.png");*/
	m_Sprite = Sprite(TextureHolder::GetTexture(
		"graphics/player.png"
	));

	m_Sprite.setOrigin(25, 25);
}

void Player::spawn(IntRect arena, Vector2f resolution, int tileSize)
{
	m_Position.x = arena.width / 2;
	m_Position.y = arena.height / 2;

	m_Arena.left = arena.left;
	m_Arena.width = arena.width;
	m_Arena.top = arena.top;
	m_Arena.height = arena.height;
	
	m_TileSize = tileSize;

	m_Resolution.x = resolution.x;
	m_Resolution.y = resolution.y;
}

void Player::resetPlayerStats()
{
	m_Speed = START_SPEED;
	m_Health = START_HEALTH;
	m_MaxHealth = START_HEALTH;
}
bool Player::hit(Time timeHit)
{
	if (timeHit.asMilliseconds()
		- m_LastHit.asMilliseconds() > 200)
	{
		m_LastHit = timeHit;
		m_Health -= 10;
		return true;
	}
	else return false;
}
Time Player::getLastHitTime()
{
	return m_LastHit;
}

FloatRect Player::getPosition()
{
	return m_Sprite.getGlobalBounds();
}
Vector2f Player::getCenter()
{
	return m_Position;
}

float Player::getRotation()
{
	return m_Sprite.getRotation();
}

Sprite Player::getSprite()
{
	return m_Sprite;
}

void Player::moveLeft()
{
	m_LeftPressed = true;
}
void Player::moveRight()
{
	m_RightPressed = true;
}
void Player::moveUp()
{
	m_UpPressed = true;
}
void Player::moveDown()
{
	m_DownPressed = true;
}

void Player::stopLeft()
{
	m_LeftPressed = false;
}
void Player::stopRight()
{
	m_RightPressed = false;
}
void Player::stopUp()
{
	m_UpPressed = false;
}
void Player::stopDown()
{
	m_DownPressed = false;
}

void Player::update(float elapsedTime, Vector2i mousePosition)
{
	// Update position
	if (m_UpPressed)
		m_Position.y -= m_Speed * elapsedTime;
	if (m_DownPressed)
		m_Position.y += m_Speed * elapsedTime;
	if (m_LeftPressed)
		m_Position.x -= m_Speed * elapsedTime;
	if (m_RightPressed)
		m_Position.x += m_Speed * elapsedTime;

	m_Sprite.setPosition(m_Position);

	// Keep player in the arena
	if (m_Position.x > m_Arena.width - m_TileSize)
		m_Position.x = m_Arena.width - m_TileSize;
	if (m_Position.x < m_Arena.left + m_TileSize)
		m_Position.x = m_Arena.left + m_TileSize;
	if (m_Position.y > m_Arena.height - m_TileSize)
		m_Position.y = m_Arena.height - m_TileSize;
	if (m_Position.y < m_Arena.top + m_TileSize)
		m_Position.y = m_Arena.top + m_TileSize;

	float angle = (atan2(mousePosition.y - m_Resolution.y / 2, mousePosition.x  - m_Resolution.x / 2) * 180) / 3.141; // Revisar
	

	m_Sprite.setRotation(angle);
}

void Player::upgradeSpeed()
{
	m_Speed += (START_SPEED * .2);
}
void Player::upgradeHealth()
{
	m_MaxHealth += (START_HEALTH * .2);
}

void Player::increaseHealthLevel(int amount)
{
	m_Health += amount;

	if (m_Health > m_MaxHealth)
	{
		m_Health = m_MaxHealth;
	}
}
int Player::getHealth()
{
	return m_Health;
}