#ifndef ENGINE_H__
#define ENGINE_H__

#include "define.h"
#include "array2d.h"
#include "blockinfo.h"
#include "chunk.h"
#include "openglcontext.h"
#include "player.h"
#include "shader.h"
#include "texture.h"
#include "textureatlas.h"
#include "vector3.h"
#include "world.h"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>

class Engine : public OpenglContext
{
public:
    Engine();
    virtual ~Engine();
    virtual void Init();
    virtual void DeInit();
    virtual void LoadResource();
    virtual void UnloadResource();
    virtual void Render(float elapsedTime);
    virtual void KeyPressEvent(unsigned char key);
    virtual void KeyReleaseEvent(unsigned char key);
    virtual void MouseMoveEvent(int x, int y);
    virtual void MousePressEvent(const MOUSE_BUTTON& button, int x, int y);
    virtual void MouseReleaseEvent(const MOUSE_BUTTON &button, int x, int y);

private:
    bool LoadTexture(Texture& texture, const std::string& filename, bool stopOnError = true);
    std::string DirectionToString(const Vector3f& direction) const;

    unsigned int GetFps(float elapsedTime) const;

    void GetBlockAtCursor();
    static bool EqualWithEpsilon(const float& val1, const float& val2, float epsilon = 0.0001f);
    static bool InRangeWithEpsilon(const float& val, const float& minVal, const float& maxVal, float epsilon = 0.0001f);

    void AddBlendFunction(bool isOrtho);
    void RemoveBlendFunction(bool isOrtho);

    void DrawArm();
    void DrawCrosshair();
    void DrawBlock(float elapsedTime);
    void DrawHud(float elapsedTime);
    void DrawSkybox();

    void UpdateMonsterFace(float elapsedTime);

    void PrintText(unsigned int x, unsigned int y, const std::string& t);

private:
    World* m_world;

    BlockInfo* m_blockInfo[BTYPE_LAST];

    Player m_player;

    sf::Music m_music;

    Shader m_shader01;

    Texture m_textureCrosshair;
    Texture m_textureDark;
    Texture m_textureFont;
    Texture m_textureMonster;
    Texture m_textureMonsterEyes;
    Texture m_textureArm;

    TextureAtlas m_textureAtlas = TextureAtlas(BTYPE_LAST);

    Vector3f m_currentBlock;
    Vector3f m_currentFaceNormal;

    bool m_monsterFadeIn = false;
    bool m_monsterFadeOut = false;
    int m_monsterFace;
    float m_monsterAlpha;
    float m_monsterFadeTime;
    float m_monsterVisibleTime;
    float m_monsterInvisibleTime;
    float m_lastMonsterFaceChange;

    bool m_wireframe = false;
    bool m_isOrtho = false;

    bool m_keyA = false;
    bool m_keyC = true;
    bool m_keyD = false;
    bool m_keyI = false;
    bool m_keyR = false;
    bool m_keyS = false;
    bool m_keySpace = false;
    bool m_keyW = false;

    bool m_mouseTest;
};

#endif // ENGINE_H__
