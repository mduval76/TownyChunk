#ifndef PLAYER_H__
#define PLAYER_H__

#include "transformation.h"
#include "vector3.h"

#include <array>

class Player {
public:
	Player(const Vector3f& position, float rotX = 0, float rotY = 0);

	Vector3f GetPosition() const;
	std::array<float, 2> GetRotation() const;
	void SetPosition(const Vector3f& position);

	void TurnLeftRight(float value);
	void TurnTopBottom(float value);
	void Move(bool front, bool back, bool left, bool right, float elapsedTime);

	void ApplyTransformation(Transformation& transformation, bool includeRotation = true);

private:
	Vector3f m_position = Vector3f(0.0f, 0.0f, 0.0f);
	std::array<float, 2> m_rotation = { 0.0f, 0.0f };

	float m_rotX;
	float m_rotY;
};

#endif // PLAYER_H__