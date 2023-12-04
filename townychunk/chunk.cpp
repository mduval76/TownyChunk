#include <iostream>
#include <climits>

#include "chunk.h"

Chunk::Chunk(IWorld* world, int x, int z) : m_blocks(CHUNK_SIZE_X, CHUNK_SIZE_Y, CHUNK_SIZE_Z), m_world(world), m_chunkXCoord(x), m_chunkZCoord(z), perlin(5, 4, 1, 3444443) {
	m_blocks.Reset(BTYPE_AIR);

	float posX = x * CHUNK_SIZE_X;
	float posZ = z * CHUNK_SIZE_Z;

	for (int x = 0; x < CHUNK_SIZE_X; ++x) {
		for (int z = 0; z < CHUNK_SIZE_Z; ++z) {
			float val = perlin.Get((float)(posX + x) / 2000.0f, (float)(posZ + z) / 2000.0f);
			val *= CHUNK_SIZE_Y;

			for (int y = 0; y < val; ++y) {
				if (val < 2) {
					SetBlock(x, y, z, BTYPE_CIRCLE);
				}
				else if (val >= 2 && val < 15) {
					SetBlock(x, y, z, BTYPE_EMBOSSED_BROWN);
				}
				else if (val >= 15) {
					SetBlock(x, y, z, BTYPE_EMBOSSED_WHITE);
				}
			}
		
		}
	}
}

Chunk::~Chunk() {}

void Chunk::Update() {
	if (m_isDirty) {
		int maxVertexCount = (CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z) * (6 * 4);
		VertexBuffer::VertexData* vd = new VertexBuffer::VertexData[maxVertexCount];
		int count = 0;

		for (int x = 0; x < CHUNK_SIZE_X; ++x) {
			for (int z = 0; z < CHUNK_SIZE_Z; ++z) {
				for (int y = 0; y < CHUNK_SIZE_Y; ++y) {
					if (count > USHRT_MAX) {
						break;
					}

					BlockType bt = GetBlock(x, y, z);

					if (bt != BTYPE_AIR) {
						if (IsBlockFaceVisible(x, y, z, FRONT)) {
							AddBlockToMesh(vd, count, bt, x, y, z, FRONT);
						}
						if (IsBlockFaceVisible(x, y, z, BACK)) {
							AddBlockToMesh(vd, count, bt, x, y, z, BACK);
						}
						if (IsBlockFaceVisible(x, y, z, LEFT)) {
							AddBlockToMesh(vd, count, bt, x, y, z, LEFT);
						}
						if (IsBlockFaceVisible(x, y, z, RIGHT)) {
							AddBlockToMesh(vd, count, bt, x, y, z, RIGHT);
						}
						if (IsBlockFaceVisible(x, y, z, TOP)) {
							AddBlockToMesh(vd, count, bt, x, y, z, TOP);
						}
						if (IsBlockFaceVisible(x, y, z, BOTTOM)) {
							AddBlockToMesh(vd, count, bt, x, y, z, BOTTOM);
						}
					}
				}
			}
		}

		if (count > USHRT_MAX) {
			count = USHRT_MAX;
			std::cout << "[Chunk::Update] Chunk data truncated, too many vertices to have a 16bit index" << std::endl;
		}

		m_vbo.SetMeshData(vd, count);
		delete[] vd;
	}

	m_isDirty = false;
}

void Chunk::AddBlockToMesh(VertexBuffer::VertexData* vd, int& count, BlockType type, int x, int y, int z, BlockFace face) {
	float u, v, w, h;
	float globalX = m_chunkXCoord * CHUNK_SIZE_X + x;
	float globalZ = m_chunkZCoord * CHUNK_SIZE_Z + z;

	// FRONT
	switch (face)
	{
		case FRONT:
			BlockInfo::GetBlockTextureCoords(type, face, u, v, w, h);
			vd[count++] = VertexBuffer::VertexData(globalX - 0.5f, y - 0.5f, globalZ + 0.5f, 0.9f, 0.9f, 0.9f, u, v);
			vd[count++] = VertexBuffer::VertexData(globalX + 0.5f, y - 0.5f, globalZ + 0.5f, 0.9f, 0.9f, 0.9f, u + w, v);
			vd[count++] = VertexBuffer::VertexData(globalX + 0.5f, y + 0.5f, globalZ + 0.5f, 0.9f, 0.9f, 0.9f, u + w, v + h);
			vd[count++] = VertexBuffer::VertexData(globalX - 0.5f, y + 0.5f, globalZ + 0.5f, 0.9f, 0.9f, 0.9f, u, v + h);
			break;
		case RIGHT:
			BlockInfo::GetBlockTextureCoords(type, face, u, v, w, h);
			vd[count++] = VertexBuffer::VertexData(globalX + 0.5f, y - 0.5f, globalZ + 0.5f, 0.8f, 0.8f, 0.8f, u, v);
			vd[count++] = VertexBuffer::VertexData(globalX + 0.5f, y - 0.5f, globalZ - 0.5f, 0.8f, 0.8f, 0.8f, u + w, v);
			vd[count++] = VertexBuffer::VertexData(globalX + 0.5f, y + 0.5f, globalZ - 0.5f, 0.8f, 0.8f, 0.8f, u + w, v + h);
			vd[count++] = VertexBuffer::VertexData(globalX + 0.5f, y + 0.5f, globalZ + 0.5f, 0.8f, 0.8f, 0.8f, u, v + h);
			break;
		case BACK:
			BlockInfo::GetBlockTextureCoords(type, face, u, v, w, h);
			vd[count++] = VertexBuffer::VertexData(globalX + 0.5f, y - 0.5f, globalZ - 0.5f, 0.9f, 0.9f, 0.9f, u, v);
			vd[count++] = VertexBuffer::VertexData(globalX - 0.5f, y - 0.5f, globalZ - 0.5f, 0.9f, 0.9f, 0.9f, u + w, v);
			vd[count++] = VertexBuffer::VertexData(globalX - 0.5f, y + 0.5f, globalZ - 0.5f, 0.9f, 0.9f, 0.9f, u + w, v + h);
			vd[count++] = VertexBuffer::VertexData(globalX + 0.5f, y + 0.5f, globalZ - 0.5f, 0.9f, 0.9f, 0.9f, u, v + h);
			break;
		case LEFT:
			BlockInfo::GetBlockTextureCoords(type, face, u, v, w, h);
			vd[count++] = VertexBuffer::VertexData(globalX - 0.5f, y - 0.5f, globalZ - 0.5f, 0.8f, 0.8f, 0.8f, u, v);
			vd[count++] = VertexBuffer::VertexData(globalX - 0.5f, y - 0.5f, globalZ + 0.5f, 0.8f, 0.8f, 0.8f, u + w, v);
			vd[count++] = VertexBuffer::VertexData(globalX - 0.5f, y + 0.5f, globalZ + 0.5f, 0.8f, 0.8f, 0.8f, u + w, v + h);
			vd[count++] = VertexBuffer::VertexData(globalX - 0.5f, y + 0.5f, globalZ - 0.5f, 0.8f, 0.8f, 0.8f, u, v + h);
			break;
		case BOTTOM:
			BlockInfo::GetBlockTextureCoords(type, face, u, v, w, h);
			vd[count++] = VertexBuffer::VertexData(globalX - 0.5f, y - 0.5f, globalZ - 0.5f, 1.0f, 1.0f, 1.0f, u, v);
			vd[count++] = VertexBuffer::VertexData(globalX + 0.5f, y - 0.5f, globalZ - 0.5f, 1.0f, 1.0f, 1.0f, u + w, v);
			vd[count++] = VertexBuffer::VertexData(globalX + 0.5f, y - 0.5f, globalZ + 0.5f, 1.0f, 1.0f, 1.0f, u + w, v + h);
			vd[count++] = VertexBuffer::VertexData(globalX - 0.5f, y - 0.5f, globalZ + 0.5f, 1.0f, 1.0f, 1.0f, u, v + h);
			break;
		case TOP:
			BlockInfo::GetBlockTextureCoords(type, face, u, v, w, h);
			vd[count++] = VertexBuffer::VertexData(globalX - 0.5f, y + 0.5f, globalZ + 0.5f, 1.0f, 1.0f, 1.0f, u, v);
			vd[count++] = VertexBuffer::VertexData(globalX + 0.5f, y + 0.5f, globalZ + 0.5f, 1.0f, 1.0f, 1.0f, u + w, v);
			vd[count++] = VertexBuffer::VertexData(globalX + 0.5f, y + 0.5f, globalZ - 0.5f, 1.0f, 1.0f, 1.0f, u + w, v + h);
			vd[count++] = VertexBuffer::VertexData(globalX - 0.5f, y + 0.5f, globalZ - 0.5f, 1.0f, 1.0f, 1.0f, u, v + h);
			break;
		default:
			break;
	}
}

void Chunk::Render() const {
	m_vbo.Render();
}

bool Chunk::IsDirty() const {
	return m_isDirty;
}

void Chunk::SetIsDirty() {
	m_isDirty = true;
}

void Chunk::RemoveBlock(int x, int y, int z) {
	m_blocks.Set(x, y, z, BTYPE_AIR);
	m_isDirty = true;
}

int Chunk::GetChunkXCoord() const {
	return m_chunkXCoord;
}

int Chunk::GetChunkZCoord() const {
	return m_chunkZCoord;
}

bool Chunk::IsBlockFaceVisible(int x, int y, int z, BlockFace face) {
	if ((face == FRONT && z == CHUNK_SIZE_Z - 1 && m_chunkZCoord == WORLD_SIZE_Z - 1) ||
		(face == BACK && z == 0 && m_chunkZCoord == 0) ||
		(face == LEFT && x == 0 && m_chunkXCoord == 0) ||
		(face == RIGHT && x == CHUNK_SIZE_X - 1 && m_chunkXCoord == WORLD_SIZE_X - 1) ||
		(face == TOP && y == CHUNK_SIZE_Y - 1) ||
		(face == BOTTOM && y == 0)) {
		return true;
	}

	if	((face == FRONT && z == CHUNK_SIZE_Z - 1) ||
		(face == BACK && z == 0) ||
		(face == LEFT && x == 0) ||
		(face == RIGHT && x == CHUNK_SIZE_X - 1)) {

		int adjacentChunkX = m_chunkXCoord + ((face == LEFT) ? -1 : (face == RIGHT) ? 1 : 0);
		int adjacentChunkZ = m_chunkZCoord + ((face == BACK) ? -1 : (face == FRONT) ? 1 : 0);

		Chunk* adjacentChunk = m_world->GetChunk(adjacentChunkX, adjacentChunkZ);

		if (!adjacentChunk) {
			return true;
		}

		int adjacentBlockX = (face == LEFT) ? CHUNK_SIZE_X - 1 : (face == RIGHT) ? 0 : x;
		int adjacentBlockZ = (face == BACK) ? CHUNK_SIZE_Z - 1 : (face == FRONT) ? 0 : z;

		BlockType adjacentBlock = adjacentChunk->GetBlock(adjacentBlockX, y, adjacentBlockZ);

		return adjacentBlock == BTYPE_AIR;
	}

	int dx = 0, dy = 0, dz = 0;
	switch (face) {
		case FRONT: dz = 1; break;
		case BACK: dz = -1; break;
		case LEFT: dx = -1; break;
		case RIGHT: dx = 1; break;
		case TOP: dy = 1; break;
		case BOTTOM: dy = -1; break;
	}

	BlockType neighbor = GetBlock(x + dx, y + dy, z + dz);
	return neighbor == BTYPE_AIR;
}

void Chunk::SetBlock(int x, int y, int z, BlockType type) {
	m_blocks.Set(x, y, z, type);
	m_isDirty = true;
}

BlockType Chunk::GetBlock(int x, int y, int z) {
	return m_blocks.Get(x, y, z);
}
