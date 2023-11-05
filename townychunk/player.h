#ifndef PLAYER_H__
#define PLAYER_H__

#include "chunk.h"
#include "transformation.h"
#include "vector3.h"

#include <array>

class Player {
public:
	Player(const Vector3f& position, float rotX = 0, float rotY = 0);

	Vector3f GetPosition() const;
	Vector3f GetDirection();
	std::array<float, 2> GetRotation() const;
	void SetPosition(const Vector3f& position);

	void TurnLeftRight(float value);
	void TurnTopBottom(float value);
	void Move(Chunk* chunk, bool front, bool back, bool left, bool right, bool up, float elapsedTime);

	void ApplyTransformation(Transformation& transformation, bool includeRotation = true);
	bool IsJumping() const;

private:
	Vector3f m_position = Vector3f(0.0f, 0.0f, 0.0f);
	Vector3f m_direction = Vector3f(0.0f, 0.0f, 0.0f);
	std::array<float, 2> m_rotation = { 0.0f, 0.0f };

	float m_rotX;
	float m_rotY;
	float m_yVelocity = 0.0f;
	float m_offset = -10.0f;
	float m_jumpSpeed = 0.0f;

	bool m_isJumping = false;
	bool m_jumpDirectionFront = false;
	bool m_jumpDirectionBack = false;
	bool m_jumpDirectionLeft = false;
	bool m_jumpDirectionRight = false;
};

#endif // PLAYER_H__