#include "monster.h"
#include "player.h"


Monster::Monster(Player& player) : 
	m_player(player),
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
	srand(static_cast<unsigned int>(time(NULL)));
	// InitializeLaser();
}

Monster::~Monster() {
	//delete m_laserVbo;
}

void Monster::UpdateLaserBeams() {
	float width = 0.25f;

	Vector3f leftDirection = m_targetPosition - m_leftEyePosition;
	Vector3f rightDirection = m_targetPosition - m_rightEyePosition;

	leftDirection.Normalize();
	rightDirection.Normalize();

	Vector3f up = (fabs(leftDirection.z) < 0.99f) ? Vector3f(0.0f, 0.0f, 1.0f) : Vector3f(0.0f, 1.0f, 0.0f);

	Vector3f leftPerp1 = leftDirection.Cross(up);
	Vector3f leftPerp2 = leftDirection.Cross(leftPerp1);

	Vector3f rightPerp1 = rightDirection.Cross(up);
	Vector3f rightPerp2 = rightDirection.Cross(rightPerp1);

	leftPerp1.Normalize();
	leftPerp2.Normalize();

	rightPerp1.Normalize();
	rightPerp2.Normalize();

	leftPerp1 *= width;
	leftPerp2 *= width;
	rightPerp1 *= width;
	rightPerp2 *= width;

	//Left eye
	VertexBuffer::VertexData leftLaserVd[8];

	leftLaserVd[0] = VertexBuffer::VertexData(m_leftEyePosition.x + leftPerp1.x, m_leftEyePosition.y + leftPerp1.y, m_leftEyePosition.z + leftPerp1.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	leftLaserVd[1] = VertexBuffer::VertexData(m_targetPosition.x + leftPerp1.x, m_targetPosition.y + leftPerp1.y, m_targetPosition.z + leftPerp1.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	leftLaserVd[2] = VertexBuffer::VertexData(m_targetPosition.x - leftPerp1.x, m_targetPosition.y - leftPerp1.y, m_targetPosition.z - leftPerp1.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	leftLaserVd[3] = VertexBuffer::VertexData(m_leftEyePosition.x - leftPerp1.x, m_leftEyePosition.y - leftPerp1.y, m_leftEyePosition.z - leftPerp1.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

	leftLaserVd[4] = VertexBuffer::VertexData(m_leftEyePosition.x + leftPerp2.x, m_leftEyePosition.y + leftPerp2.y, m_leftEyePosition.z + leftPerp2.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	leftLaserVd[5] = VertexBuffer::VertexData(m_targetPosition.x + leftPerp2.x, m_targetPosition.y + leftPerp2.y, m_targetPosition.z + leftPerp2.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	leftLaserVd[6] = VertexBuffer::VertexData(m_targetPosition.x - leftPerp2.x, m_targetPosition.y - leftPerp2.y, m_targetPosition.z - leftPerp2.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	leftLaserVd[7] = VertexBuffer::VertexData(m_leftEyePosition.x - leftPerp2.x, m_leftEyePosition.y - leftPerp2.y, m_leftEyePosition.z - leftPerp2.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

	// Right Eye
	VertexBuffer::VertexData rightLaserVd[8];

	rightLaserVd[0] = VertexBuffer::VertexData(m_rightEyePosition.x + rightPerp1.x, m_rightEyePosition.y + rightPerp1.y, m_rightEyePosition.z + rightPerp1.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	rightLaserVd[1] = VertexBuffer::VertexData(m_targetPosition.x + rightPerp1.x, m_targetPosition.y + rightPerp1.y, m_targetPosition.z + rightPerp1.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	rightLaserVd[2] = VertexBuffer::VertexData(m_targetPosition.x - rightPerp1.x, m_targetPosition.y - rightPerp1.y, m_targetPosition.z - rightPerp1.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	rightLaserVd[3] = VertexBuffer::VertexData(m_rightEyePosition.x - rightPerp1.x, m_rightEyePosition.y - rightPerp1.y, m_rightEyePosition.z - rightPerp1.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	
	rightLaserVd[4] = VertexBuffer::VertexData(m_rightEyePosition.x + rightPerp2.x, m_rightEyePosition.y + rightPerp2.y, m_rightEyePosition.z + rightPerp2.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	rightLaserVd[5] = VertexBuffer::VertexData(m_targetPosition.x + rightPerp2.x, m_targetPosition.y + rightPerp2.y, m_targetPosition.z + rightPerp2.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	rightLaserVd[6] = VertexBuffer::VertexData(m_targetPosition.x - rightPerp2.x, m_targetPosition.y - rightPerp2.y, m_targetPosition.z - rightPerp2.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	rightLaserVd[7] = VertexBuffer::VertexData(m_rightEyePosition.x - rightPerp2.x, m_rightEyePosition.y - rightPerp2.y, m_rightEyePosition.z - rightPerp2.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

	m_leftLaserBeamVbo.SetMeshData(leftLaserVd, 8);
	m_rightLaserBeamVbo.SetMeshData(rightLaserVd, 8);
}

const VertexBuffer& Monster::GetLeftEyeLaserVBO() const {
	return m_leftLaserBeamVbo;
}

const VertexBuffer& Monster::GetRightEyeLaserVBO() const {
	return m_rightLaserBeamVbo;
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

bool Monster::GetIsCausingDamage() const {
	return m_isCausingDamage;
}

void Monster::TriggerMonsterAttackCycle(float elapsedTime) {
	static float accumulatedTime = 0.0f;

	if (m_isFirstAppearance) {
		m_monsterInvisibleTime = 5 + rand() % 16;
		m_isFirstAppearance = false;
		accumulatedTime = 0.0f;
		return;
	}

	accumulatedTime += elapsedTime;

	if (m_monsterFadeIn) {
		m_monsterAlpha += elapsedTime / m_monsterFadeTime;

		if (m_monsterAlpha >= 1.0f) {
			m_monsterAlpha = 1.0f;
			m_monsterFadeIn = false;
			m_monsterEyesFadeIn = true;
			m_isRecordingPlayerPositions = true;
			m_monsterVisibleTime = 10.0f;
		}
	}
	else if (m_monsterVisibleTime > 0.0f) {
		m_monsterVisibleTime -= elapsedTime;

		if (m_monsterEyesFadeIn) {
			m_monsterEyesAlpha += elapsedTime / m_monsterEyesFadeTime;

			if (m_isRecordingPlayerPositions) {
				m_player.RecordPositionHistory(m_player.GetPosition());
			}

			if (m_monsterEyesAlpha >= 1.0f) {
				m_monsterEyesAlpha = 1.0f;
				m_monsterEyesFadeIn = false;
				m_monsterEyesVisibleTime = 5.0f;
				m_isAttacking = true;
				PlayAttackSound();
			}
		}
		else if (m_monsterEyesVisibleTime > 0.0f) {
			m_monsterEyesVisibleTime -= elapsedTime;

			if (m_isAttacking) {
				m_targetPosition = m_player.GetPositionAtIndex(m_attackCount);
				m_targetPosition.y -= 1.7f;
				UpdateLaserBeams();

				if (m_isRecordingPlayerPositions && m_monsterEyesVisibleTime >= 2.5f) {
					m_player.RecordPositionHistory(m_player.GetPosition());
				}
				else {
					m_isRecordingPlayerPositions = false;
				}

				m_attackCount++;

				if (CheckLaserHit(m_player, m_targetPosition)) {
					m_isCausingDamage = true;
				}
				else {
					m_isCausingDamage = false;
				}

				SetEyeOrigins(m_player);

				if (accumulatedTime > 10.0f) {
					m_isAttacking = false;
				}
			}

			if (m_monsterEyesVisibleTime <= 0.0f) {
				m_monsterEyesFadeOut = true;
				m_isAttacking = false;
				m_isCausingDamage = false;
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
			accumulatedTime = 0.0f;
			m_player.ResetPositionHistory();
			m_attackCount = 0;
		}
	}
}

int Monster::SetRandomMonsterFace() {
	int newFace;
	do {
		newFace = rand() % 4 + 1;
	} while (newFace == m_monsterFace);
	m_monsterFace = newFace;

	return m_monsterFace;
}

void Monster::PlayAttackSound() {
	int sound = rand() % 5 + 1;

	switch (sound) {
		case 1:
			if (!m_laserBuffer.loadFromFile("../townychunk/media/audio/laser1.ogg")) {
				return;
			}
			m_laser1.setBuffer(m_laserBuffer);
			m_laser1.setVolume(50.0f);
			m_laser1.play();
			break;
		case 2:
			if (!m_laserBuffer.loadFromFile("../townychunk/media/audio/laser2.ogg")) {
				return;
			}
			m_laser2.setBuffer(m_laserBuffer);
			m_laser2.setVolume(50.0f);
			m_laser2.play();
			break;
		case 3:
			if (!m_laserBuffer.loadFromFile("../townychunk/media/audio/laser3.ogg")) {
				return;
			}
			m_laser3.setBuffer(m_laserBuffer);
			m_laser3.setVolume(50.0f);
			m_laser3.play();
			break;
		case 4:
			if (!m_laserBuffer.loadFromFile("../townychunk/media/audio/laser4.ogg")) {
				return;
			}
			m_laser4.setBuffer(m_laserBuffer);
			m_laser4.setVolume(50.0f);
			m_laser4.play();
			break;
		case 5:
			if (!m_laserBuffer.loadFromFile("../townychunk/media/audio/laser5.ogg")) {
				return;
			}
			m_laser5.setBuffer(m_laserBuffer);
			m_laser5.setVolume(50.0f);
			m_laser5.play();
			break;
	}
}

bool Monster::CheckLaserHit(const Player& player, const Vector3f& target) {
	int playerX = static_cast<int>(std::floor(player.GetPosition().x));
	int playerY = static_cast<int>(std::floor(player.GetPosition().y));
	int playerZ = static_cast<int>(std::floor(player.GetPosition().z));

	int targetX = static_cast<int>(std::floor(m_targetPosition.x));
	int targetY = static_cast<int>(std::floor(m_targetPosition.y));
	int targetZ = static_cast<int>(std::floor(m_targetPosition.z));

	if (playerX == targetX && playerZ == targetZ) {
		return true;
	}

	return false;
}

void Monster::SetEyeOrigins(const Player& player) {
	float playerX = player.GetPosition().x;
	float playerY = player.GetPosition().y;
	float playerZ = player.GetPosition().z;

	float offsetZPlus = 5.0f;

	switch (m_monsterFace) {
	case 1: // Z- (Front)
		m_leftEyePosition.x = playerX + (NORMALIZED_LEFT_EYE_X - 0.5f) * VIEW_DISTANCE * 2;
		m_leftEyePosition.y = playerY - (NORMALIZED_LEFT_EYE_Y - 0.5f) * VIEW_DISTANCE * 2; 
		m_leftEyePosition.z = playerZ - VIEW_DISTANCE;

		m_rightEyePosition.x = playerX + (NORMALIZED_RIGHT_EYE_X - 0.5f) * VIEW_DISTANCE * 2;
		m_rightEyePosition.y = playerY - (NORMALIZED_RIGHT_EYE_Y - 0.5f) * VIEW_DISTANCE * 2; 
		m_rightEyePosition.z = playerZ - VIEW_DISTANCE;
		break;
	case 2: // Z+ (Back)
		m_leftEyePosition.x = playerX + (NORMALIZED_LEFT_EYE_X - 0.5f) * VIEW_DISTANCE * 2 - offsetZPlus;
		m_leftEyePosition.y = playerY - (NORMALIZED_LEFT_EYE_Y - 0.5f) * VIEW_DISTANCE * 2;  
		m_leftEyePosition.z = playerZ + VIEW_DISTANCE;

		m_rightEyePosition.x = playerX + (NORMALIZED_RIGHT_EYE_X - 0.5f) * VIEW_DISTANCE * 2 - offsetZPlus;
		m_rightEyePosition.y = playerY - (NORMALIZED_RIGHT_EYE_Y - 0.5f) * VIEW_DISTANCE * 2; 
		m_rightEyePosition.z = playerZ + VIEW_DISTANCE;
		break;
	case 3: // X- (Left)
		m_leftEyePosition.x = playerX - VIEW_DISTANCE;
		m_leftEyePosition.y = playerY - (NORMALIZED_LEFT_EYE_Y - 0.5f) * VIEW_DISTANCE * 2;
		m_leftEyePosition.z = playerZ + (NORMALIZED_LEFT_EYE_X - 0.5f) * VIEW_DISTANCE * 2 - offsetZPlus;

		m_rightEyePosition.x = playerX - VIEW_DISTANCE;
		m_rightEyePosition.y = playerY - (NORMALIZED_RIGHT_EYE_Y - 0.5f) * VIEW_DISTANCE * 2; 
		m_rightEyePosition.z = playerZ + (NORMALIZED_RIGHT_EYE_X - 0.5f) * VIEW_DISTANCE * 2 - offsetZPlus;

		break;
	case 4: // X+ (Right)
		m_leftEyePosition.x = playerX + VIEW_DISTANCE;
		m_leftEyePosition.y = playerY - (NORMALIZED_LEFT_EYE_Y - 0.5f) * VIEW_DISTANCE * 2;
		m_leftEyePosition.z = playerZ + (NORMALIZED_LEFT_EYE_X - 0.5f) * VIEW_DISTANCE * 2;

		m_rightEyePosition.x = playerX + VIEW_DISTANCE;
		m_rightEyePosition.y = playerY - (NORMALIZED_RIGHT_EYE_Y - 0.5f) * VIEW_DISTANCE * 2;
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
