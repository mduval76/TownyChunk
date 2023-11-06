#include "world.h"

World::World() : m_chunks(WORLD_SIZE_X, WORLD_SIZE_Z) {
    for (int i = 0; i < WORLD_SIZE_X; ++i) {
        for (int j = 0; j < WORLD_SIZE_Z; ++j) {
            Chunk* chunk = new Chunk(i, j);
            chunk->SetChunkCoords(i, j);
            m_chunks.Set(i, j, chunk);
        }
    }
}

World::~World() {
    for (int i = 0; i < WORLD_SIZE_X; ++i) {
        for (int j = 0; j < WORLD_SIZE_Z; ++j) {
            Chunk* chunkPointer =  m_chunks.Get(i, j);
            if (chunkPointer != nullptr) {
                delete chunkPointer;
                m_chunks.Set(i, j, nullptr);
            }
        }
    }
}

Chunk* World::GetChunk(int x, int z) {
    if (x < 0 || z < 0 || x >= WORLD_SIZE_X || z >= WORLD_SIZE_Z) {
        return nullptr;
    }
    return m_chunks.Get(x, z);
}

void World::Update() {
    for (int i = 0; i < WORLD_SIZE_X; ++i) {
        for (int j = 0; j < WORLD_SIZE_Z; ++j) {
            m_chunks.Get(i, j)->Update();
        }
    }
}

void World::Render() {
    for (int i = 0; i < WORLD_SIZE_X; i++) {
        for (int j = 0; j < WORLD_SIZE_Z; j++) {
            m_chunks.Get(i, j)->Render();
        }
    }
}