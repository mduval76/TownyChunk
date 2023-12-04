#ifndef ENGINE_H__
#define ENGINE_H__

#include "define.h"
#include "array2d.h"
#include "blockinfo.h"
#include "chunk.h"
#include "openglcontext.h"
#include "player.h"
#include "monster.h"
#include "shader.h"
#include "texture.h"
#include "textureatlas.h"
#include "vector3.h"
#include "world.h"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>

class Engine : public OpenglContext {
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
    virtual void MouseReleaseEvent(const MOUSE_BUTTON& button, int x, int y);

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
    void DrawHealthBar(float currentHealth);
    void DrawHud(float elapsedTime);
    void DrawFaceWithMonster(int face);
    void DrawSkybox();
    void DrawEndScreen(float elapsedTime);
    void DrawStartScreen(float elapsedTime);

    void RenderLaserBeams(float elapsedTime);

    void PrintText(unsigned int x, unsigned int y, const std::string& t);

private:
    World* m_world;

    BlockInfo* m_blockInfo[BTYPE_LAST];

    Player m_player;
    Monster m_monster;

    sf::Music m_music;

    GameState m_gameState;

    Shader m_shader01;
    Shader m_laserShader;

    Texture m_textureArm;
    Texture m_textureCrosshair;
    Texture m_textureDark;
    Texture m_textureFont;
    Texture m_textureHealthBar;
    Texture m_textureMonster;
    Texture m_textureMonsterEyes;
    Texture m_texturePlayButton;
    Texture m_textureEndScreen;
    Texture m_textureStartScreen;

    TextureAtlas m_textureAtlas = TextureAtlas(BTYPE_LAST);

    Vector3f m_currentBlock;
    Vector3f m_currentFaceNormal;

    float m_playButtonAnimationTime;
    float m_buttonWidth;
    float m_buttonHeight;
    float m_centerX;
    float m_centerY;

    bool m_wireframe = false;
    bool m_isOrtho = false;

    bool m_keyA = false;
    bool m_keyC = true;
    bool m_keyD = false;
    bool m_keyH = true;
    bool m_keyI = false;
    bool m_keyR = true;
    bool m_keyS = false;
    bool m_keySpace = false;
    bool m_keyW = false;
};

#endif // ENGINE_H__