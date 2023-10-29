#ifndef ENGINE_H__
#define ENGINE_H__

#include "blockinfo.h"
#include "chunk.h"
#include "define.h"
#include "openglcontext.h"
#include "player.h"
#include "shader.h"
#include "texture.h"
#include "textureatlas.h"

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

    void DrawSkybox();
    void DrawFloor();
    void DrawBlock();

private:
    BlockInfo* m_blockInfo[BTYPE_LAST];

    Chunk m_chunk;

    Player m_player = Player(Vector3f(0.0f, 0.0f, 0.0f));

    Shader m_shader01;

    Texture m_textureMonster;
    Texture m_textureDark;

    TextureAtlas m_textureAtlas = TextureAtlas(BTYPE_LAST);

    bool m_wireframe = false;

    bool m_keyW = false;
    bool m_keyA = false;
    bool m_keyS = false;
    bool m_keyD = false;
};

#endif // ENGINE_H__
