#include "player.h"

Player::Player(const Vector3f& position, float rotX, float rotY) : m_position(position), m_rotX(rotX), m_rotY(rotY) {}

std::array<float, 2> Player::GetRotation() const {
	return m_rotation;
}

Vector3f Player::GetPosition() const {
	return m_position;
}

Vector3f Player::GetDirection() {
	float yrotrad = (m_rotY / 180.0f) * static_cast<float>(PI);
	float xrotrad = (m_rotX / 180.0f) * static_cast<float>(PI);

	m_direction.x = sin(yrotrad) * cos(xrotrad);
	m_direction.y = sin(xrotrad);
	m_direction.z = -cos(yrotrad) * cos(xrotrad);
	m_direction.Normalize();

	return m_direction;
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

void Player::UpdateJump(bool up, float elapsedTime) {
	if (up && !m_isJumping && m_position.y == 0) {
		m_isJumping = true;
		m_jumpSpeed = sqrt(-2 * MAX_JUMP_HEIGHT * GRAVITY);
	}

	m_position.y += m_jumpSpeed * elapsedTime;
	m_jumpSpeed += GRAVITY * elapsedTime;

	if (m_position.y < 0) {
		m_position.y = 0;
		m_isJumping = false;
		m_jumpSpeed = 0;
	}
}

void Player::Move(bool front, bool back, bool left, bool right, bool up, float elapsedTime) {
	UpdateJump(up, elapsedTime);
	UpdatePosition(front, back, left, right, elapsedTime);
}

void Player::UpdatePosition(bool front, bool back, bool left, bool right, float elapsedTime) {
	float yrotrad = ((m_rotY / 180) * 3.141592654f);
	float velocity = 0.25f;

	if ((front && left) || (front && right) || (back && left) || (back && right)) {
		velocity /= 1.414213562f;
	}

	if (m_isJumping) {
		velocity *= AIR_CONTROL;
	}

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
