#include "player.h"

Player::Player(const Vector3f& position, float rotX, float rotY) : m_position(position), m_rotX(rotX), m_rotY(rotY) {}

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

void Player::Move(Chunk* chunk, bool front, bool back, bool left, bool right, bool up, float elapsedTime) {
    float yrotrad = ((m_rotY / 180) * PI);
    float xrotrad = ((m_rotX / 180) * PI);
    float velocity = 0.25f;

    // Prevent faster movement when moving diagonally
    if ((front && left) || (front && right) || (back && left) || (back && right) ||
        (up && left) || (up && right) || (up && front) || (up && back)) {
        velocity /= 1.5;
    }

    // Jump mechanics
    if (up && m_position.y == 0) {
        m_isJumping = true;
        m_yVelocity = INITIAL_JUMP_FORCE;
    }

    // Apply gravity
    m_yVelocity += GRAVITY * elapsedTime;
    m_position.y += m_yVelocity * elapsedTime;

    // Ground limit
    if (m_position.y < 0) {
        m_position.y = 0;
        m_yVelocity = 0.0f;
        m_isJumping = false;
    }

    Vector3f targetPosition = m_position;

    // Movement mechanics
    if (front) {
        targetPosition.x += float(sin(yrotrad)) * velocity;
        targetPosition.z -= float(cos(yrotrad)) * velocity;
    }
    if (back) {
        targetPosition.x -= float(sin(yrotrad)) * velocity;
        targetPosition.z += float(cos(yrotrad)) * velocity;
    }
    if (left) {
        targetPosition.x -= float(cos(yrotrad)) * velocity;
        targetPosition.z -= float(sin(yrotrad)) * velocity;
    }
    if (right) {
        targetPosition.x += float(cos(yrotrad)) * velocity;
        targetPosition.z += float(sin(yrotrad)) * velocity;
    }

    targetPosition = AdjustEdgePosition(targetPosition);
    m_position = targetPosition;
}

void Player::ApplyTransformation(Transformation& transformation, bool includeRotation) {
	transformation.ApplyRotation(-m_rotX, 1.0f, 0, 0);
	transformation.ApplyRotation(-m_rotY, 0, 1.0f, 0);
	transformation.ApplyTranslation(-m_position);
}

Vector3f Player::AdjustEdgePosition(const Vector3f& targetPosition) const {
    float worldEdgeBlockX = CHUNK_SIZE_X * WORLD_SIZE_X;
    float worldEdgeBlockZ = CHUNK_SIZE_Z * WORLD_SIZE_Z;

    Vector3f adjustedPosition = targetPosition;

    if (targetPosition.x < 0.0f) {
		adjustedPosition.x = 0.0f;
	}
    else if (targetPosition.x > worldEdgeBlockX - 1.0f) {
		adjustedPosition.x = worldEdgeBlockX - 1.0f;
	}

    if (targetPosition.z < 0.0f) {
		adjustedPosition.z = 0.0f;
	}
    else if (targetPosition.z > worldEdgeBlockZ - 1.0f) {
		adjustedPosition.z = worldEdgeBlockZ - 1.0f;
	}

    if (targetPosition.y < 0.0f) adjustedPosition.y = 0.0f;

	return adjustedPosition;
}

bool Player::IsJumping() const {
	return m_isJumping;
}
