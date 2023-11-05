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

    unsigned int GetFps(float elapsedTime) const;

    void DrawHud(float elapsedTime);
    void DrawSkybox();

    void PrintText(unsigned int x, unsigned int y, const std::string& t);

private:
    World* m_world;
    BlockInfo* m_blockInfo[BTYPE_LAST];

    Player m_player = Player(Vector3f(0.0f, 0.0f, 0.0f));

    sf::Music m_music;

    Shader m_shader01;

    Texture m_textureCrosshair;
    Texture m_textureDark;
    Texture m_textureFont;
    Texture m_textureMonster;

    TextureAtlas m_textureAtlas = TextureAtlas(BTYPE_LAST);

    bool m_wireframe = false;

    bool m_keyA = false;
    bool m_keyD = false;
    bool m_keyI = false;
    bool m_keyS = false;
    bool m_keySpace = false;
    bool m_keyW = false;
};

#endif // ENGINE_H__
