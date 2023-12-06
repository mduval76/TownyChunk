#ifndef MONSTER_H__
#define MONSTER_H__

#include "define.h"
#include "vector3.h"
#include "vertexbuffer.h"

class Player;

class Monster {
public:
	Monster(Player& player);
    ~Monster();

	void UpdateLaserBeams();
    const VertexBuffer& GetLeftEyeLaserVBO() const;
    const VertexBuffer& GetRightEyeLaserVBO() const;

    Vector3f GetLeftEyeOrigin() const;
    Vector3f GetRightEyeOrigin() const;
    Vector3f GetTargetPosition() const;

    bool GetIsAttacking() const;
    bool GetIsCausingDamage() const;

    int GetMonsterFace() const;

    float GetMonsterAlpha() const;
    float GetMonsterEyesAlpha() const;

    void SetEyeOrigins(const Player& player);

    void TriggerMonsterAttackCycle(float elapsedTime);
    int SetRandomMonsterFace();

    void PlayAttackSound();
    void StopAttackSound();
    bool CheckLaserHit(const Player& player, const Vector3f& target);

private:
    Player& m_player;

	VertexBuffer m_leftLaserBeamVbo;
    VertexBuffer m_rightLaserBeamVbo;

    Vector3f m_targetPosition;
    Vector3f m_leftEyePosition;
    Vector3f m_rightEyePosition;

    sf::SoundBuffer m_laserBuffer;
    sf::Sound m_laser1; 
    sf::Sound m_laser2; 
    sf::Sound m_laser3; 
    sf::Sound m_laser4; 
    sf::Sound m_laser5;

	bool m_monsterFadeIn = false;
    bool m_monsterFadeOut = false;
    bool m_monsterEyesFadeIn = false;
    bool m_monsterEyesFadeOut = false;
    bool m_isAttacking = false;
    bool m_isCausingDamage = false;
    bool m_isFirstAppearance = true;
    bool m_isRecordingPlayerPositions = false;

    int m_monsterFace;

    float m_monsterAlpha;
    float m_monsterFadeTime;
    float m_monsterVisibleTime;
    float m_monsterVisible;
    float m_monsterInvisibleTime;

    float m_monsterEyesAlpha;
    float m_monsterEyesFadeTime;
    float m_monsterEyesVisibleTime;

    int m_attackCount;
    int m_lastRecordedTime = 0;
    int m_UsedRecordingCount = 0;
};

#endif // MONSTER_H__