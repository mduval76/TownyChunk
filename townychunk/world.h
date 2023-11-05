#ifndef WORLD_H__
#define WORLD_H__

#include "define.h"
#include "array2d.h"
#include "blockinfo.h"
#include "chunk.h"

class World {
public:
    World();
    ~World();

    Chunk* GetChunk(int x, int z);
    void Update(); 
    void Render();

private:
    Array2d<Chunk*> m_chunks;
    BlockInfo* m_blockInfo[BTYPE_LAST];
};

#endif // WORLD_H__
