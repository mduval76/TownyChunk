#ifndef MONSTER_H__
#define MONSTER_H__

#include "define.h"
#include "vector3.h"
#include "vertexbuffer.h"

class Monster {
public:
	Monster();
    ~Monster();

	void InitializeLaser();

    Vector3f GetLeftEyeOrigin() const;
    Vector3f GetRightEyeOrigin() const;
    Vector3f GetTargetPosition() const;

    bool GetIsAttacking() const;

    int GetMonsterFace() const;

    float GetMonsterAlpha() const;
    float GetMonsterEyesAlpha() const;

    void SetEyeOrigins();
	void SetLeftEyeOrigin(const Vector3f& origin);
	void SetRightEyeOrigin(const Vector3f& origin);
	void SetTarget(const Vector3f& target);

    void UpdateMonsterFace(float elapsedTime);
    int SetRandomMonsterFace();

private:
	VertexBuffer m_laserVbo;

    Vector3f m_playerPosition;
    Vector3f m_targetPosition;
    Vector3f m_leftEyePosition;
    Vector3f m_rightEyePosition;

	bool m_monsterFadeIn = false;
    bool m_monsterFadeOut = false;
    bool m_monsterEyesFadeIn = false;
    bool m_monsterEyesFadeOut = false;
    bool m_isAttacking = false;

    int m_monsterFace;

    float m_monsterAlpha;
    float m_monsterFadeTime;
    float m_monsterVisibleTime;
    float m_monsterInvisibleTime;

    float m_monsterEyesAlpha;
    float m_monsterEyesFadeTime;
    float m_monsterEyesVisibleTime;
};

#endif // MONSTER_H__