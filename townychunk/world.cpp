#include "world.h"

World::World() : m_chunks(WORLD_SIZE_X, WORLD_SIZE_Z) {
	for (int i = 0; i < WORLD_SIZE_X; ++i) {
		for (int j = 0; j < WORLD_SIZE_Z; ++j) {
			Chunk* chunk = new Chunk(this, i, j);
			m_chunks.Set(i, j, chunk);
		}
	}
}

World::~World() {
	for (int i = 0; i < WORLD_SIZE_X; ++i) {
		for (int j = 0; j < WORLD_SIZE_Z; ++j) {
			delete m_chunks.Get(i, j);
			m_chunks.Set(i, j, nullptr);
		}
	}
}

Chunk* World::GetChunk(int x, int z) const {
	return m_chunks.Get(x, z);
}

void World::CheckCollisions(Player& player, bool front, bool back, bool left, bool right, bool up, float elapsedTime) {
	player.Move(front, back, left, right, up, elapsedTime);
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
		for (int j = 0; j < WORLD_SIZE_X; j++) {
			Chunk* chunk = new Chunk(this, i, j);
			m_chunks.Set(i, j, chunk);
		}
	}
}
