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
	void RenderLaser();

	void UpdateMonsterFace(float elapsedTime);

    int GetMonsterFace() const;

    float GetMonsterAlpha() const;
    float GetMonsterEyesAlpha() const;

	void SetLeftEyeOrigin(const Vector3f& origin);
	void SetRightEyeOrigin(const Vector3f& origin);
	void SetTarget(const Vector3f& target);

private:
	VertexBuffer m_laserVbo;

	bool m_monsterFadeIn = false;
    bool m_monsterFadeOut = false;
    bool m_monsterEyesFadeIn = false;
    bool m_monsterEyesFadeOut = false;

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