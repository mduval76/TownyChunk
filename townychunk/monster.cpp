#include "monster.h"

Monster::Monster() :
	m_playerPosition(Vector3f(0.0f, 0.0f, 0.0f)),
	m_targetPosition(Vector3f(64.0f, 0.0f, 64.0f)),
	m_leftEyePosition(Vector3f(0.0f, 0.0f, 0.0f)),
	m_rightEyePosition(Vector3f(0.0f, 0.0f, 0.0f)),
	m_monsterFace(1),
	m_monsterAlpha(0.0f),
	m_monsterFadeTime(2.5f),
	m_monsterVisibleTime(0.0f),
	m_monsterInvisibleTime(1.0f), // TODO: Make initial value longer after testing
	m_monsterEyesAlpha(0.0f),
	m_monsterEyesFadeTime(2.5f),
	m_monsterEyesVisibleTime(0.0f) {
	srand(static_cast<unsigned int>(time(0)));
	//InitializeLaser();
}

Monster::~Monster() {
	//delete m_laserVbo;
}

void Monster::InitializeLaser() {
	VertexBuffer::VertexData laserVd[2];

	laserVd[0] = VertexBuffer::VertexData(0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	laserVd[1] = VertexBuffer::VertexData(1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);

	m_laserVbo.SetMeshData(laserVd, 2);
}

Vector3f Monster::GetLeftEyeOrigin() const {
	return m_leftEyePosition;
}

Vector3f Monster::GetRightEyeOrigin() const {
	return m_rightEyePosition;
}

Vector3f Monster::GetTargetPosition() const {
	return m_targetPosition;
}

bool Monster::GetIsAttacking() const {
	return m_isAttacking;
}

void Monster::UpdateMonsterFace(float elapsedTime) {
	if (m_monsterFadeIn) {
		m_monsterAlpha += elapsedTime / m_monsterFadeTime;

		if (m_monsterAlpha >= 1.0f) {
			m_monsterAlpha = 1.0f;
			m_monsterFadeIn = false;
			m_monsterEyesFadeIn = true;
			m_monsterVisibleTime = 10.0f;
		}
	}
	else if (m_monsterVisibleTime > 0.0f) {
		m_monsterVisibleTime -= elapsedTime;

		if (m_monsterEyesFadeIn) {
			m_monsterEyesAlpha += elapsedTime / m_monsterEyesFadeTime;

			if (m_monsterEyesAlpha >= 1.0f) {
				m_monsterEyesAlpha = 1.0f;
				m_monsterEyesFadeIn = false;
				m_monsterEyesVisibleTime = 5.0f; 
				m_isAttacking = true;
				//std::cout << "Monster started attacking on face " << m_monsterFace << "!" << std::endl;
			}
		}
		else if (m_monsterEyesVisibleTime > 0.0f) {
			m_monsterEyesVisibleTime -= elapsedTime;

			if (m_monsterEyesVisibleTime <= 0.0f) {
				m_monsterEyesFadeOut = true;
				m_isAttacking = false;
				std::cout << "Monster stopped attacking!" << std::endl;
			}
		}
		else if (m_monsterEyesFadeOut) {
			m_monsterEyesAlpha -= elapsedTime / m_monsterEyesFadeTime;

			if (m_monsterEyesAlpha <= 0.0f) {
				m_monsterEyesAlpha = 0.0f;
				m_monsterEyesFadeOut = false;
			}
		}

		if (m_monsterVisibleTime <= 0.0f) {
			m_monsterFadeOut = true;
		}
	}
	else if (m_monsterFadeOut) {
		m_monsterAlpha -= elapsedTime / m_monsterFadeTime;

		if (m_monsterAlpha <= 0.0f) {
			m_monsterAlpha = 0.0f;
			m_monsterFadeOut = false;
			m_monsterInvisibleTime = 5 + rand() % 16;
		}
	}
	else if (m_monsterInvisibleTime > 0.0f) {
		m_monsterInvisibleTime -= elapsedTime;
		if (m_monsterInvisibleTime <= 0.0f) {
			SetRandomMonsterFace();
			m_monsterFadeIn = true;
		}
	}
}

// TODO: Fix randomness
int Monster::SetRandomMonsterFace() {
	int newFace;
	do {
		newFace = rand() % 4 + 1;
	} while (newFace == m_monsterFace);
	m_monsterFace = newFace;

	return m_monsterFace;
}

void Monster::SetEyeOrigins() {
	switch (m_monsterFace) {
	case FRONT: // Z-
		m_leftEyePosition.x = m_playerPosition.x + (-VIEW_DISTANCE + NORMALIZED_LEFT_EYE_X * VIEW_DISTANCE * 2);
		m_leftEyePosition.y = m_playerPosition.y + (VIEW_DISTANCE - NORMALIZED_LEFT_EYE_Y * VIEW_DISTANCE * 2);
		m_leftEyePosition.z = m_playerPosition.z - VIEW_DISTANCE;

		m_rightEyePosition.x = m_playerPosition.x + (-VIEW_DISTANCE + NORMALIZED_RIGHT_EYE_X * VIEW_DISTANCE * 2);
		m_rightEyePosition.y = m_playerPosition.y + (VIEW_DISTANCE - NORMALIZED_RIGHT_EYE_Y * VIEW_DISTANCE * 2);
		m_rightEyePosition.z = m_playerPosition.z - VIEW_DISTANCE;
		break;
	case BACK: // Z+
		m_leftEyePosition.x = m_playerPosition.x + (-VIEW_DISTANCE + NORMALIZED_LEFT_EYE_X * VIEW_DISTANCE * 2);
		m_leftEyePosition.y = m_playerPosition.y + (VIEW_DISTANCE - NORMALIZED_LEFT_EYE_Y * VIEW_DISTANCE * 2);
		m_leftEyePosition.z = m_playerPosition.z + VIEW_DISTANCE;

		m_rightEyePosition.x = m_playerPosition.x + (-VIEW_DISTANCE + NORMALIZED_RIGHT_EYE_X * VIEW_DISTANCE * 2);
		m_rightEyePosition.y = m_playerPosition.y + (VIEW_DISTANCE - NORMALIZED_RIGHT_EYE_Y * VIEW_DISTANCE * 2);
		m_rightEyePosition.z = m_playerPosition.z + VIEW_DISTANCE;
		break;
	case LEFT: // X+
		m_leftEyePosition.x = m_playerPosition.x + VIEW_DISTANCE;
		m_leftEyePosition.y = m_playerPosition.y + (VIEW_DISTANCE - NORMALIZED_LEFT_EYE_Y * VIEW_DISTANCE * 2);
		m_leftEyePosition.z = m_playerPosition.z + (-VIEW_DISTANCE + NORMALIZED_LEFT_EYE_X * VIEW_DISTANCE * 2);

		m_rightEyePosition.x = m_playerPosition.x + VIEW_DISTANCE;
		m_rightEyePosition.y = m_playerPosition.y + (VIEW_DISTANCE - NORMALIZED_RIGHT_EYE_Y * VIEW_DISTANCE * 2);
		m_rightEyePosition.z = m_playerPosition.z + (-VIEW_DISTANCE + NORMALIZED_LEFT_EYE_X * VIEW_DISTANCE * 2);
		break;
	case RIGHT: // X-
		m_leftEyePosition.x = m_playerPosition.x - VIEW_DISTANCE;
		m_leftEyePosition.y = m_playerPosition.y + (VIEW_DISTANCE - NORMALIZED_LEFT_EYE_Y * VIEW_DISTANCE * 2);
		m_leftEyePosition.z = m_playerPosition.z + (-VIEW_DISTANCE + NORMALIZED_LEFT_EYE_X * VIEW_DISTANCE * 2);

		m_rightEyePosition.x = m_playerPosition.x - VIEW_DISTANCE;
		m_rightEyePosition.y = m_playerPosition.y + (VIEW_DISTANCE - NORMALIZED_RIGHT_EYE_X * VIEW_DISTANCE * 2);
		m_rightEyePosition.z = m_playerPosition.z + (-VIEW_DISTANCE + NORMALIZED_RIGHT_EYE_Y * VIEW_DISTANCE * 2);
		break;
	default:
		break;
	}
}

int Monster::GetMonsterFace() const {
	return m_monsterFace;
}

float Monster::GetMonsterAlpha() const {
	return m_monsterAlpha;
}

float Monster::GetMonsterEyesAlpha() const {
	return m_monsterEyesAlpha;
}

void Monster::SetLeftEyeOrigin(const Vector3f& origin) {}

void Monster::SetRightEyeOrigin(const Vector3f& origin) {}

void Monster::SetTarget(const Vector3f& target) {}
