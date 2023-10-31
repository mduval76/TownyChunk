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
	float yrotrad = ((m_rotY / 180) * 3.141592654f);
	float velocity = 0.25f;

	// Prevent faster movement when moving diagonally
	if ((front && left) || (front && right) || (back && left) || (back && right) ||
		(up && left) || (up && right) || (up && front) || (up && back)) {
		velocity /= 1.5;
	}

	// Jump mechanics
	if (up && !m_isJumping && m_position.y == 0) {
		m_isJumping = true;
		m_gravity = MAX_JUMP_HEIGHT / (JUMP_TIME * JUMP_TIME);
		m_jumpSpeed = sqrt(2 * MAX_JUMP_HEIGHT * m_gravity);

		m_jumpDirectionFront = front;
		m_jumpDirectionBack = back;
		m_jumpDirectionLeft = left;
		m_jumpDirectionRight = right;
	}
	if (m_isJumping) {
		front = m_jumpDirectionFront;
		back = m_jumpDirectionBack;
		left = m_jumpDirectionLeft;
		right = m_jumpDirectionRight;

		m_position.y += m_jumpSpeed * elapsedTime;
		m_jumpSpeed -= m_gravity * elapsedTime;

		if (m_position.y <= 0) {
			m_position.y = 0;
			m_isJumping = false;

			m_jumpDirectionFront = false;
			m_jumpDirectionBack = false;
			m_jumpDirectionLeft = false;
			m_jumpDirectionRight = false;
		}
	}

	// Movement mechanics
	if (front) {
		m_position.x += float(sin(yrotrad)) * velocity;
		m_position.z -= float(cos(yrotrad)) * velocity;
	}
	if (back) {
		m_position.x -= float(sin(yrotrad)) * velocity;
		m_position.z += float(cos(yrotrad)) * velocity;
	}
	if (left) {
		m_position.x -= float(cos(yrotrad)) * velocity;
		m_position.z -= float(sin(yrotrad)) * velocity;
	}
	if (right) {
		m_position.x += float(cos(yrotrad)) * velocity;
		m_position.z += float(sin(yrotrad)) * velocity;
	}
}

void Player::ApplyTransformation(Transformation& transformation, bool includeRotation) {
	transformation.ApplyRotation(-m_rotX, 1.0f, 0, 0);
	transformation.ApplyRotation(-m_rotY, 0, 1.0f, 0);
	transformation.ApplyTranslation(-m_position);
}

bool Player::IsJumping() const {
	return m_isJumping;
}
