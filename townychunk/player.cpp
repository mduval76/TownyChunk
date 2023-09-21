#include "player.h"

Player::Player() {
}

Player::Player(const Vector3f& position, float rotX, float rotY) : m_position(position), m_rotX(rotX), m_rotY(rotY) {
}

Vector3f Player::GetPosition() const {
	return m_position;
}

void Player::SetPosition(const Vector3f& position) {
	m_position = position;
}

void Player::TurnLeftRight(float value) {
	m_rotY += value;
	if (m_rotY > 360 || m_rotY < -360)
		m_rotY = 0;
}

void Player::TurnTopBottom(float value) {
	m_rotX += value;
	if (m_rotX > 90)
		m_rotX = 90;
	if (m_rotX < -90)
		m_rotX = -90;
}

void Player::Move(bool front, bool back, bool left, bool right, float elapsedTime) {
	Vector3f delta(0, 0, 0);

	//if (front == true) {
	//	float xrotrad, yrotrad;
	//	yrotrad = (m_rotY / 180 * 3.141592654f);
	//	xrotrad = (m_rotX / 180 * 3.141592654f);
	//	delta.x += float(sin(yrotrad)) * elapsedTime;
	//	delta.z -= float(cos(yrotrad)) * elapsedTime;
	//}
	//if (back == true) {
	//	float xrotrad, yrotrad;
	//	yrotrad = (m_rotY / 180.0f * 3.141592654f);
	//	xrotrad = (m_rotX / 180.0f * 3.141592654f);
	//	delta.x -= float(sin(yrotrad)) * elapsedTime;
	//	delta.z += float(cos(yrotrad)) * elapsedTime;
	//}
	//if (left == true) {
	//	float yrotrad;
	//	yrotrad = (m_rotY / 180.0f * 3.141592654f);
	//	delta.x += float(cos(yrotrad)) * elapsedTime;
	//	delta.z += float(sin(yrotrad)) * elapsedTime;
	//}
	//if (right == true) {
	//	float yrotrad;
	//	yrotrad = (m_rotY / 180.0f * 3.141592654f);
	//	delta.x -= float(cos(yrotrad)) * elapsedTime;
	//	delta.z -= float(sin(yrotrad)) * elapsedTime;
	//}
}

void Player::ApplyTransformation(Transformation& transformation) const {
	transformation.ApplyRotation(-m_rotX, 1.0f, 0, 0);
	transformation.ApplyRotation(-m_rotY, 0, 1.0f, 0);
	transformation.ApplyTranslation(-m_position);

}
