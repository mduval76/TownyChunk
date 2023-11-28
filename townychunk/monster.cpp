#include "monster.h"
#include "player.h"


Monster::Monster(Player& player) : 
	m_player(player),
	m_playerPosition(Vector3f(0.0f, 0.0f, 0.0f)),
	m_targetPosition(Vector3f(0.0f, 0.0f, 0.0f)),
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
	srand(static_cast<unsigned int>(time(NULL))); // Seed the random number generator
	// InitializeLaser(); // Uncomment if laser initialization is needed here
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
	if (m_isFirstAppearance) {
		m_monsterInvisibleTime = 5 + rand() % 16;
		m_isFirstAppearance = false; // Reset the flag after the first update cycle
		// You might want to perform any initial setup needed for the first appearance here
		return; // Skip the rest of the method on the first call
	}
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
				std::cout << "Monster started attacking on face " << m_monsterFace << "!" << std::endl;
				std::cout << "Left eye position was : (" << m_leftEyePosition.x << ", " << m_leftEyePosition.y << ", " << m_leftEyePosition.z << ") when monster attacked on face(" << m_monsterFace << ")" << std::endl;
				std::cout << "Right eye position was : (" << m_rightEyePosition.x << ", " << m_rightEyePosition.y << ", " << m_rightEyePosition.z << ") when monster attacked on face(" << m_monsterFace << ")" << std::endl;
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
			m_monsterInvisibleTime = 5 + rand() % 5;
		}
	}
	else if (m_monsterInvisibleTime > 0.0f) {
		m_monsterInvisibleTime -= elapsedTime;
		if (m_monsterInvisibleTime <= 0.0f) {
			SetRandomMonsterFace();
			SetEyeOrigins(m_player);
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

void Monster::SetEyeOrigins(const Player& player) {
	float playerX = player.GetPosition().x;
	float playerY = player.GetPosition().y;
	float playerZ = player.GetPosition().z;

	switch (m_monsterFace) {
	case 1: // Z- (Front)
		m_leftEyePosition.x = playerX + (NORMALIZED_LEFT_EYE_X - 0.5f) * VIEW_DISTANCE * 2;
		m_leftEyePosition.y = playerY - (NORMALIZED_LEFT_EYE_Y - 0.5f) * VIEW_DISTANCE * 2;  // Inverted Y
		m_leftEyePosition.z = playerZ - VIEW_DISTANCE;

		m_rightEyePosition.x = playerX + (NORMALIZED_RIGHT_EYE_X - 0.5f) * VIEW_DISTANCE * 2;
		m_rightEyePosition.y = playerY - (NORMALIZED_RIGHT_EYE_Y - 0.5f) * VIEW_DISTANCE * 2;  // Inverted Y
		m_rightEyePosition.z = playerZ - VIEW_DISTANCE;
		break;
	case 2: // Z+ (Back)
		m_leftEyePosition.x = playerX + (NORMALIZED_LEFT_EYE_X - 0.5f) * VIEW_DISTANCE * 2;
		m_leftEyePosition.y = playerY - (NORMALIZED_LEFT_EYE_Y - 0.5f) * VIEW_DISTANCE * 2;  // Inverted Y
		m_leftEyePosition.z = playerZ + VIEW_DISTANCE;

		m_rightEyePosition.x = playerX + (NORMALIZED_RIGHT_EYE_X - 0.5f) * VIEW_DISTANCE * 2;
		m_rightEyePosition.y = playerY - (NORMALIZED_RIGHT_EYE_Y - 0.5f) * VIEW_DISTANCE * 2;  // Inverted Y
		m_rightEyePosition.z = playerZ + VIEW_DISTANCE;
		break;
	case 3: // X- (Left)
		m_leftEyePosition.x = playerX - VIEW_DISTANCE;
		m_leftEyePosition.y = playerY - (NORMALIZED_LEFT_EYE_Y - 0.5f) * VIEW_DISTANCE * 2;  // Inverted Y
		m_leftEyePosition.z = playerZ + (NORMALIZED_LEFT_EYE_X - 0.5f) * VIEW_DISTANCE * 2;

		m_rightEyePosition.x = playerX - VIEW_DISTANCE;
		m_rightEyePosition.y = playerY - (NORMALIZED_RIGHT_EYE_Y - 0.5f) * VIEW_DISTANCE * 2;  // Inverted Y
		m_rightEyePosition.z = playerZ + (NORMALIZED_RIGHT_EYE_X - 0.5f) * VIEW_DISTANCE * 2;
		break;
	case 4: // X+ (Right)
		m_leftEyePosition.x = playerX + VIEW_DISTANCE;
		m_leftEyePosition.y = playerY - (NORMALIZED_LEFT_EYE_Y - 0.5f) * VIEW_DISTANCE * 2;  // Inverted Y
		m_leftEyePosition.z = playerZ + (NORMALIZED_LEFT_EYE_X - 0.5f) * VIEW_DISTANCE * 2;

		m_rightEyePosition.x = playerX + VIEW_DISTANCE;
		m_rightEyePosition.y = playerY - (NORMALIZED_RIGHT_EYE_Y - 0.5f) * VIEW_DISTANCE * 2;  // Inverted Y
		m_rightEyePosition.z = playerZ + (NORMALIZED_RIGHT_EYE_X - 0.5f) * VIEW_DISTANCE * 2;
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

void Monster::SetTargetPosition(const Vector3f& target) {
	m_targetPosition = m_playerPosition;
}
