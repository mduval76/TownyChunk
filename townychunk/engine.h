#ifndef ENGINE_H__
#define ENGINE_H__

#include "define.h"
#include "openglcontext.h"
#include "player.h"
#include "texture.h"
#include "shader.h"
#include "chunk.h"

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
    virtual void MousePressEvent(const MOUSE_BUTTON &button, int x, int y);
    virtual void MouseReleaseEvent(const MOUSE_BUTTON &button, int x, int y);

private:
    bool LoadTexture(Texture& texture, const std::string& filename, bool stopOnError = true);

    void DrawSkybox();
    void DrawFloor();
    void DrawBlock();

private:
    Player m_player = Player(Vector3f(0.0f, 0.0f, 0.0f));

    Chunk m_testChunk;

    Shader m_shader01;

    Texture m_textureFloor;
    Texture m_textureFaceX;
    Texture m_textureFaceY;
    Texture m_textureFaceZ;
    Texture m_textureMonster;
    Texture m_textureDark;

    bool m_wireframe = false;

    bool m_keyW = false;
    bool m_keyA = false;
    bool m_keyS = false;
    bool m_keyD = false;
};

#endif // ENGINE_H__
