#include "monster.h"

Monster::Monster() :
	m_monsterFace(1),
	m_monsterAlpha(0.0f),
	m_monsterFadeTime(2.5f),
	m_monsterVisibleTime(5.0f),
	m_monsterInvisibleTime(0.0f),
	m_monsterEyesAlpha(0.0f),
	m_monsterEyesFadeTime(2.5f),
	m_monsterEyesVisibleTime(5.0f) {
	srand(static_cast<unsigned int>(time(0)));
	//InitializeLaser();
}

Monster::~Monster() {
	//m_laserVbo.Delete();
}

void Monster::InitializeLaser() {
	VertexBuffer::VertexData laserVd[2];

	laserVd[0] = VertexBuffer::VertexData(0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	laserVd[1] = VertexBuffer::VertexData(1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);

	m_laserVbo.SetMeshData(laserVd, 2);
}

void Monster::RenderLaser() {}

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
			}
		}
		else if (m_monsterEyesVisibleTime > 0.0f) {
			m_monsterEyesVisibleTime -= elapsedTime;

			if (m_monsterEyesVisibleTime <= 0.0f) {
				m_monsterEyesFadeOut = true;
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
			int newFace;
			do {
				newFace = rand() % 4 + 1;
			} while (newFace == m_monsterFace);
			m_monsterFace = newFace;
			m_monsterFadeIn = true;
		}
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
