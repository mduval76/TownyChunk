#ifndef PLAYER_H__
#define PLAYER_H__

#include "transformation.h"
#include "vector3.h"

#include <array>
#include <cmath>
#include <iostream>

class Player {
public:
	Player(const Vector3f& position, float rotX = 0, float rotY = 0);

	std::array<float, 2> GetRotation() const;
	Vector3f GetDirection();
	Vector3f GetPosition() const;
	Vector3f GetVelocity() const;
	BlockType GetEquippedItem() const;
	bool GetIsOnGround() const;
	void SetPosition(const Vector3f& position);
	void SetVelocity(const Vector3f& velocity);
	void SetOnGround(bool grounded);
	void SetEquippedItem(BlockType targetType);

	Vector3f Move(bool front, bool back, bool left, bool right, bool up, float elapsedTime);

	void TurnLeftRight(float value);
	void TurnTopBottom(float value);

	void ApplyTransformation(Transformation& transformation, bool includeRotation = true);

private:
	BlockType m_equippedItem;

	Vector3f m_position;
	Vector3f m_direction;
	Vector3f m_velocity;
	std::array<float, 2> m_rotation;
	
	float m_rotX;
	float m_rotY;
	float m_jumpVelocity;

	bool m_isOnGround;
};

#endif // PLAYER_H__