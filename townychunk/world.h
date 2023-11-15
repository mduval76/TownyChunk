#ifndef WORLD_H__
#define WORLD_H__

#include "define.h"
#include "array2d.h"
#include "iworld.h"
#include "chunk.h"
#include "player.h"

class World : public IWorld {
public:
    World();
    ~World();

    Chunk* GetChunk(int x, int z) const override;
    void CheckCollisions(Player& player, bool front, bool back, bool left, bool right, bool up, float elapsedTime);
    void Update(); 
    void Render();

private:
    Array2d<Chunk*> m_chunks;
    BlockType m_btFeet, m_btMid, m_btHead;
};

#endif // WORLD_H__
