#ifndef WORLD_H__
#define WORLD_H__

#include "define.h"
#include "array2d.h"
#include "iworld.h"
#include "chunk.h"
#include "player.h"
#include <iomanip>

class World : public IWorld {
public:
    World();
    ~World();

    Chunk* GetChunk(int x, int z) const override;
    void SetDirtyChunk(Chunk* chunk, int blockX, int blockY, int blockZ);
    void CheckCollisions(Player& player, Vector3f& delta, bool front, bool back, bool left, bool right, bool up, float elapsedTime);
    bool IsBlocked(float x, float y, float z);
    Chunk* ChunkAt(int x, int y, int z) const;
    Chunk* ChunkAt(const Vector3<int>& pos) const;
    BlockType BlockAt(int x, int y, int z, BlockType defaultBlockType) const;
    void Update(); 
    void Render();

private:
    Array2d<Chunk*> m_chunks;
    BlockType m_btFeet, m_btMid, m_btHead;
};

#endif // WORLD_H__
