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
	Vector3f pos = player.GetPosition();
	Vector3f delta = player.SimulateMove(front, back, left, right, up, elapsedTime);
	Vector3f newPosition = player.GetPosition() + delta;

	// Collision detection for X-axis
	if (IsBlocked(pos.x + delta.x, pos.y, pos.z) ||
		IsBlocked(pos.x + delta.x, pos.y - 0.9f, pos.z) ||
		IsBlocked(pos.x + delta.x, pos.y - 1.7f, pos.z)) {
		delta.x = 0;
	}

	// Collision detection for Y-axis
	if (IsBlocked(pos.x, pos.y + delta.y, pos.z) ||
		IsBlocked(pos.x, pos.y + delta.y - 0.9f, pos.z) ||
		IsBlocked(pos.x, pos.y + delta.y - 1.7f, pos.z)) {
		delta.y = 0;
		player.SetOnGround(true);
	}

	// Collision detection for Z-axis
	if (IsBlocked(pos.x, pos.y, pos.z + delta.z) ||
		IsBlocked(pos.x, pos.y - 0.9f, pos.z + delta.z) ||
		IsBlocked(pos.x, pos.y - 1.7f, pos.z + delta.z)) {
		delta.z = 0;
	}

	// Update player position
	pos += delta;
	player.SetPosition(pos);
}

bool World::IsBlocked(float x, float y, float z) {
	BlockType blockType = BlockAt(x, y, z, BTYPE_AIR);
	return blockType != BTYPE_AIR;
}

Chunk* World::ChunkAt(int x, int y, int z) const {
	int cx = static_cast<int>(std::floor(x)) / CHUNK_SIZE_X;
	int cz = static_cast<int>(std::floor(z)) / CHUNK_SIZE_Z;

	if (cx < 0 || cx >= WORLD_SIZE_X || cz < 0 || cz >= WORLD_SIZE_Z) {
		return nullptr;
	}

	return m_chunks.Get(cx, cz);
}

Chunk* World::ChunkAt(const Vector3<int>& pos) const {
	return ChunkAt(pos.x, pos.y, pos.z);
}

BlockType World::BlockAt(int x, int y, int z, BlockType bt) const {
	Chunk* c = ChunkAt(x, y, z);

	if (!c || y < 0 || y >= CHUNK_SIZE_Y) {
		return bt;
	}

	int bx = static_cast<int>(std::floor(x)) % CHUNK_SIZE_X;
	int by = static_cast<int>(std::floor(y)) % CHUNK_SIZE_Y;
	int bz = static_cast<int>(std::floor(z)) % CHUNK_SIZE_Z;

	// Correct for negative modulos
	bx = (bx + CHUNK_SIZE_X) % CHUNK_SIZE_X;
	by = (by + CHUNK_SIZE_Y) % CHUNK_SIZE_Y;
	bz = (bz + CHUNK_SIZE_Z) % CHUNK_SIZE_Z;

	return c->GetBlock(bx, by, bz);
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
