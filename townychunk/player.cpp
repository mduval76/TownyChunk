#include "player.h"

Player::Player(const Vector3f& position, float rotX, float rotY)
	   : m_position(position),
		 m_direction(0.0f, 0.0f, 0.0f),
		 m_velocity(0.0f, 0.0f, 0.0f),
		 m_rotation{ rotX, rotY },      
		 m_rotX(rotX),
		 m_rotY(rotY),
		 m_jumpVelocity(8.5f),
		 m_isOnGround(false),
		 m_equippedItem(BTYPE_HELL) {}

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

BlockType Player::GetEquippedItem() const {
	return m_equippedItem;
}

void Player::SetPosition(const Vector3f& position) {
	m_position = position;
}

void Player::SetVelocity(const Vector3f& velocity) {
	m_velocity = velocity;
}

void Player::SetOnGround(bool grounded) {
	m_isOnGround = grounded;
}

void Player::SetEquippedItem(BlockType targetType) {
	if (targetType <= BTYPE_AIR) {
		m_equippedItem = static_cast<BlockType>(BTYPE_LAST - 1);
	}
	else if (targetType >= BTYPE_LAST) {
		m_equippedItem = static_cast<BlockType>(BTYPE_AIR + 1);
	}
	else {
		m_equippedItem = targetType;
	}
}

bool Player::GetIsOnGround() const {
	return m_isOnGround;
}

Vector3f Player::Move(bool front, bool back, bool left, bool right, bool up, float elapsedTime) {
	float yrotrad = (m_rotY / 180 * 3.141592654f);
	float speed = 0.2f;

	if ((front && left) || (front && right) || (back && left) || (back && right)) {
		speed /= 1.5f;
	}

	Vector3f delta(0.0f, 0.0f, 0.0f);

	if (front) {
		delta.x += float(sin(yrotrad)) * speed;
		delta.z -= float(cos(yrotrad)) * speed;
	}
	if (back) {
		delta.x -= float(sin(yrotrad)) * speed;
		delta.z += float(cos(yrotrad)) * speed;
	}
	if (left) {
		delta.x -= float(cos(yrotrad)) * speed;
		delta.z -= float(sin(yrotrad)) * speed;
	}
	if (right) {
		delta.x += float(cos(yrotrad)) * speed;
		delta.z += float(sin(yrotrad)) * speed;
	}

	if (up && m_isOnGround) {
		m_isOnGround = false;
		m_velocity.y = m_jumpVelocity;
	}

	if (!m_isOnGround) {
		m_velocity.y += GRAVITY * elapsedTime;
		delta.y += m_velocity.y * elapsedTime;

		if (m_position.y < -CHUNK_SIZE_Y) {
			m_position.x = SPAWN_X;
			m_position.y = CHUNK_SIZE_Y;
			m_position.z = SPAWN_Z;
			m_velocity.y = 0.0f;
			m_isOnGround = true;
		}
	}

	return delta;
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