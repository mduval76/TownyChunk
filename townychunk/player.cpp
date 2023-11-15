#include "player.h"

Player::Player(const Vector3f& position, float rotX, float rotY)
	   : m_position(position),
		 m_direction(0.0f, 0.0f, 0.0f),
		 m_velocity(0.0f, 0.0f, 0.0f), 
		 m_rotation{ rotX, rotY },     
		 m_rotX(rotX),
		 m_rotY(rotY),
		 m_isOnGround(false) {}

std::array<float, 2> Player::GetRotation() const {
	return m_rotation;
}

Vector3f Player::GetPosition() const {
	return m_position;
}

Vector3f Player::GetVelocity() const {
	return m_velocity;
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

void Player::SetVelocity(const Vector3f& velocity) {
	m_velocity = velocity;
}

void Player::SetOnGround(bool isOnGround) {
	m_isOnGround = isOnGround;
}

Vector3f Player::SimulateMove(bool front, bool back, bool left, bool right, bool up, float elapsedTime) {
	Vector3f originalPosition = m_position;
	Vector3f originalVelocity = m_velocity;

	Vector3f currentDirection = GetDirection();
	float speed = 10.0f;

	if (front) {
		m_velocity.x += currentDirection.x * speed;
		m_velocity.z += currentDirection.z * speed;
	}
	if (back) {
		m_velocity.x -= currentDirection.x * speed;
		m_velocity.z -= currentDirection.z * speed;
	}

	Vector3f rightDirection = Vector3f(currentDirection.z, 0.0f, -currentDirection.x);

	if (left) {
		m_velocity.x += rightDirection.x * speed;
		m_velocity.z += rightDirection.z * speed;
	}
	if (right) {
		m_velocity.x -= rightDirection.x * speed;
		m_velocity.z -= rightDirection.z * speed;
	}

	float gravityFactor = 15.0f;
	float jumpFactor = 25.0f;

	if (up && m_isOnGround) {
		m_isOnGround = false;
		m_velocity.y = sqrt(2 * MAX_JUMP_HEIGHT * GRAVITY) * jumpFactor;
	}
	else {
		m_velocity.y -= GRAVITY * elapsedTime * gravityFactor;
	}

	m_position += m_velocity * elapsedTime;

	if (m_position.y < PLAYER_HEIGHT) {
		m_position.y = PLAYER_HEIGHT;
		m_velocity.y = 0.0f;
		m_isOnGround = true;
	}

	if (m_isOnGround) {
		m_velocity.x *= 0.9f;
		m_velocity.z *= 0.9f;
	}

	Vector3f deltaPosition = m_position - originalPosition;

	m_position = originalPosition;
	m_velocity = originalVelocity;
	
	return deltaPosition;
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

void Player::ApplyTransformation(Transformation& transformation, bool includeRotation) {
	transformation.ApplyRotation(-m_rotX, 1.0f, 0, 0);
	transformation.ApplyRotation(-m_rotY, 0, 1.0f, 0);
	transformation.ApplyTranslation(-m_position);
}