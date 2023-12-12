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
	struct BoundingBox {
		Vector3f min;
		Vector3f max;
	
		BoundingBox(const Vector3f& min, const Vector3f& max) 
			: min(Vector3f(FLT_MAX, FLT_MAX, FLT_MAX)), max(Vector3f(-FLT_MAX, -FLT_MAX, -FLT_MAX)) {}
	};

	Player(const Vector3f& position, float rotX = 0, float rotY = 0);

	std::array<float, 2> GetRotation() const;
	Vector3f GetPosition() const;
	Vector3f GetDirection();
	Vector3f GetVelocity() const;

	bool GetIsOnGround() const;
	void SetPosition(const Vector3f& position);
	void SetVelocity(const Vector3f& velocity);
	void SetOnGround(bool grounded);

	Vector3f Move(bool front, bool back, bool left, bool right, bool up, float elapsedTime);
	void ApplyTransformation(Transformation& transformation, bool includeRotation = true);
	void TurnLeftRight(float value);
	void TurnTopBottom(float value);

	Vector3f GetPositionAtIndex(int index) const;
	void RecordPositionHistory(const Vector3f& position);
	void ResetPositionHistory();

	BlockType GetEquippedItem() const;
	void SetEquippedItem(BlockType targetType);
	float GetPlayerHealth() const;
	void UpdateHealth(float damage);


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