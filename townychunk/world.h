#ifndef WORLD_H__
#define WORLD_H__

#include "define.h"
#include "array2d.h"
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
};

#endif // WORLD_H__
