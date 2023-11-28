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

	void InitializeLaser();

    Vector3f GetLeftEyeOrigin() const;
    Vector3f GetRightEyeOrigin() const;
    Vector3f GetTargetPosition() const;

    bool GetIsAttacking() const;

    int GetMonsterFace() const;

    float GetMonsterAlpha() const;
    float GetMonsterEyesAlpha() const;

    void SetEyeOrigins(const Player& player);

    void UpdateMonsterFace(float elapsedTime);
    int SetRandomMonsterFace();

    void PlayAttackSound();

private:
    Player& m_player;

	VertexBuffer m_laserVbo;

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
    bool m_isFirstAppearance = true;

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