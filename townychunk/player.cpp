#include "player.h"

Player::Player(const Vector3f& position, float rotX, float rotY) : m_position(position), m_rotX(rotX), m_rotY(rotY) {}

Vector3f Player::GetPosition() const {
	return m_position;
}

std::array<float, 2> Player::GetRotation() const {
	return m_rotation;
}

void Player::SetPosition(const Vector3f& position) {
	m_position = position;
}

void Player::TurnLeftRight(float value) {
	m_rotY += value;
	if (m_rotY > 360 || m_rotY < -360)
		m_rotY = 0;

	m_rotation[0] = m_rotX;
	m_rotation[1] = m_rotY;
}

void Player::TurnTopBottom(float value) {
	m_rotX += value;
	if (m_rotX > 90)
		m_rotX = 90;
	if (m_rotX < -90)
		m_rotX = -90;

	m_rotation[0] = m_rotX;
	m_rotation[1] = m_rotY;
}

void Player::Move(bool front, bool back, bool left, bool right, bool up, float elapsedTime) {
	float yrotrad = (m_rotY / 180 * 3.141592654f);
	float speed = 0.25f;

	if ((front && left) || (front && right) || (back && left) || (back && right) ||
		(up && left) || (up && right) || (up && front) || (up && back)) {
		speed /= 1.5;
	}

	if (front) {
		m_position.x += float(sin(yrotrad)) * speed;
		m_position.z -= float(cos(yrotrad)) * speed;
	}
	if (back) {
		m_position.x -= float(sin(yrotrad)) * speed;
		m_position.z += float(cos(yrotrad)) * speed;
	}
	if (left) {
		m_position.x -= float(cos(yrotrad)) * speed;
		m_position.z -= float(sin(yrotrad)) * speed;
	}
	if (right) {
		m_position.x += float(cos(yrotrad)) * speed;
		m_position.z += float(sin(yrotrad)) * speed;
	}

	if (up) {
		float jumpSpeed = CalculateJumpSpeed(elapsedTime, true);
		m_position.y += jumpSpeed;
	}
	else if (m_position.y > 0) {
		float jumpSpeed = CalculateJumpSpeed(elapsedTime, false);
		m_position.y -= jumpSpeed;
	}

	if (m_position.y < 0)
		m_position.y = 0;
}

void Player::ApplyTransformation(Transformation& transformation, bool includeRotation) {
	transformation.ApplyRotation(-m_rotX, 1.0f, 0, 0);
	transformation.ApplyRotation(-m_rotY, 0, 1.0f, 0);
	transformation.ApplyTranslation(-m_position);
}

float Player::CalculateJumpSpeed(float elapsedTime, bool isJumping) {
    float decayFactor;
    float terminalSpeed;

    if (isJumping) {
        decayFactor = 0.8f;
        terminalSpeed = 0.1f;
    } else {
        decayFactor = 0.5f;
        terminalSpeed = 0.08f;
    }

    static float currentSpeed = isJumping ? 0.05f : 0.01f;

    currentSpeed *= decayFactor;

    if (currentSpeed < terminalSpeed)
        currentSpeed = terminalSpeed;

    return currentSpeed * elapsedTime;
}
