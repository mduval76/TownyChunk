#ifndef PLAYER_H__
#define PLAYER_H__

#include "transformation.h"
#include "vector3.h"

#include <array>
#include <cmath>
#include <deque>
#include <iostream>
#include <utility>

class Player {
public:
	Player(const Vector3f& position, float rotX = 0, float rotY = 0);

	std::array<float, 2> GetRotation() const;
	Vector3f GetDirection();
	Vector3f GetPosition() const;
	Vector3f GetVelocity() const;
	BlockType GetEquippedItem() const;
	bool GetIsOnGround() const;
	float GetPlayerHealth() const;

	void SetPosition(const Vector3f& position);
	void SetVelocity(const Vector3f& velocity);
	void SetOnGround(bool grounded);
	void SetEquippedItem(BlockType targetType);

	Vector3f Move(bool front, bool back, bool left, bool right, bool up, float elapsedTime);
	Vector3f GetPositionAtIndex(int index) const;
	void RecordPositionHistory(const Vector3f& position);
	void ResetPositionHistory();

	void TurnLeftRight(float value);
	void TurnTopBottom(float value);

	void UpdateHealth(float damage);

	void ApplyTransformation(Transformation& transformation, bool includeRotation = true);

private:
	BlockType m_equippedItem;

	Vector3f m_position;
	Vector3f m_direction;
	Vector3f m_velocity;

	std::vector<Vector3f> m_positionHistory;
	std::array<float, 2> m_rotation;
	
	float m_rotX;
	float m_rotY;
	float m_jumpVelocity;
	float m_hp;

	bool m_isOnGround;
	bool m_isRecordingPositions;
};

#endif // PLAYER_H__