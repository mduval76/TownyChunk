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

void World::SetDirtyChunk(Chunk* chunk, int blockX, int blockY, int blockZ) {
	int localX = blockX % CHUNK_SIZE_X;
	int localZ = blockZ % CHUNK_SIZE_Z;

	int chunkX = chunk->GetChunkXCoord();
	int chunkZ = chunk->GetChunkZCoord();

	if (localX == 0 && chunkX > 0) {
		Chunk* adjacentChunk = GetChunk(chunkX - 1, chunkZ);
		if (adjacentChunk && blockX > 0) adjacentChunk->SetIsDirty();
	}
	if (localX == CHUNK_SIZE_X - 1) {
		Chunk* adjacentChunk = GetChunk(chunkX + 1, chunkZ);
		if (adjacentChunk && blockX < WORLD_SIZE_X - 1) adjacentChunk->SetIsDirty();
	}

	if (localZ == 0 && chunkZ > 0) {
		Chunk* adjacentChunk = GetChunk(chunkX, chunkZ - 1);
		if (adjacentChunk && blockZ > 0) adjacentChunk->SetIsDirty();
	}
	if (localZ == CHUNK_SIZE_Z - 1) {
		Chunk* adjacentChunk = GetChunk(chunkX, chunkZ + 1);
		if (adjacentChunk && blockZ < WORLD_SIZE_Z - 1) adjacentChunk->SetIsDirty();
	}
}

void World::CheckCollisions(Player& player, Vector3f& delta, bool front, bool back, bool left, bool right, bool up, float elapsedTime) {
	Vector3f pos = player.GetPosition();
	Vector3f vel = player.GetVelocity();
	
	// X collisions
	if (IsBlocked(pos.x + delta.x, pos.y, pos.z) ||
		IsBlocked(pos.x + delta.x, pos.y - PLAYER_MIDDLE, pos.z) ||
		IsBlocked(pos.x + delta.x, pos.y - PLAYER_HEIGHT, pos.z)) {
		delta.x = 0;
	}

	// Y collisions
	if (IsBlocked(pos.x, pos.y + delta.y, pos.z) ||
		IsBlocked(pos.x, pos.y - PLAYER_MIDDLE + delta.y, pos.z) ||
		IsBlocked(pos.x, pos.y - PLAYER_HEIGHT + delta.y, pos.z)) {
		delta.y = 0;
		vel.y = 0.0f;
		player.SetOnGround(true);
		player.SetVelocity(vel);
	}
	else if (!IsBlocked(pos.x, pos.y - PLAYER_HEIGHT, pos.z)) {
		player.SetOnGround(false);
	}

	// Z collisions
	if (IsBlocked(pos.x, pos.y, pos.z + delta.z) ||
		IsBlocked(pos.x, pos.y - PLAYER_MIDDLE, pos.z + delta.z) ||
		IsBlocked(pos.x, pos.y - PLAYER_HEIGHT, pos.z + delta.z)) {
		delta.z = 0;
	}

	player.SetVelocity(vel);
	pos += delta;
	player.SetPosition(pos);
}

bool World::IsBlocked(float x, float y, float z) {
	BlockType blockType = BlockAt(x, y, z, BTYPE_AIR);
	return blockType != BTYPE_AIR;
}

Chunk* World::ChunkAt(int x, int y, int z) const {
	int cx = x >= 0 ? x / CHUNK_SIZE_X : (x + 1) / CHUNK_SIZE_X - 1;
	int cz = z >= 0 ? z / CHUNK_SIZE_Z : (z + 1) / CHUNK_SIZE_Z - 1;

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
