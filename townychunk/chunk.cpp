#include <iostream>
#include <climits>

#include "chunk.h"
#include "blockinfo.h"

Chunk::Chunk(int x, int z) : m_blocks(CHUNK_SIZE_X, CHUNK_SIZE_Y, CHUNK_SIZE_Z), m_chunkX(x), m_chunkZ(z) {
	m_blocks.Reset(BTYPE_AIR);
	for (int x = 0; x < CHUNK_SIZE_X; ++x) {
		for (int z = 0; z < CHUNK_SIZE_Z; ++z) {
			for (int y = 0; y < CHUNK_SIZE_Y; ++y) {

				if (y < 4) {
					SetBlock(x, y, z, BTYPE_HELL);
				}
				else if ((y == 4 || y == 5 || y == 6) && (x == 11) && (z == 1  || (z == 2  &&  y == 6)  ||  z == 3)   ||
						((y == 4 || y == 5 || y == 6) && (x == 10  ||  x == 11 ||  x == 12 ||  x == 13) && (z == 14)) ||
						((y == 4 || y == 5 || y == 6) && (x == 1)  && (z == 10 ||  z == 11 ||  z == 12  ||  z == 13)) ||
						((y == 4 && x == 7 && z == 7) || 
						 (y == 5 && x == 8 && z == 7) || 
						 (y == 6 && x == 9 && z == 7) || 
						 (y == 7 && x == 10 && z == 7))) {
					 SetBlock(x, y, z, BTYPE_MARBLE);
				}
			}
		}
	}
}

Chunk::~Chunk() {}

void Chunk::Update() {
	if (!m_isDirty) {
		return;
	}
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
					AddBlockToMesh(vd, count, bt, x, y, z);
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

	m_isDirty = false;
}

void Chunk::AddBlockToMesh(VertexBuffer::VertexData* vd, int& count, BlockType type, int x, int y, int z) {
	float u, v, w, h;
	float globalX = m_chunkX * CHUNK_SIZE_X + x;
	float globalZ = m_chunkZ * CHUNK_SIZE_Z + z;


	// FRONT
	if (GetBlock(x, y, z + 1) == BTYPE_AIR || z == CHUNK_SIZE_Z - 1) {
		BlockInfo::GetBlockTextureCoords(type, BlockInfo::BlockFace::FRONT, u, v, w, h);
		vd[count++] = VertexBuffer::VertexData(globalX - 0.5f, y - 0.5f, globalZ + 0.5f, 1.0f, 1.0f, 1.0f, u, v);
		vd[count++] = VertexBuffer::VertexData(globalX + 0.5f, y - 0.5f, globalZ + 0.5f, 1.0f, 1.0f, 1.0f, u + w, v);
		vd[count++] = VertexBuffer::VertexData(globalX + 0.5f, y + 0.5f, globalZ + 0.5f, 1.0f, 1.0f, 1.0f, u + w, v + h);
		vd[count++] = VertexBuffer::VertexData(globalX - 0.5f, y + 0.5f, globalZ + 0.5f, 1.0f, 1.0f, 1.0f, u, v + h);
	}

	// RIGHT
	if (GetBlock(x + 1, y, z) == BTYPE_AIR || x == CHUNK_SIZE_X - 1) {
		BlockInfo::GetBlockTextureCoords(type, BlockInfo::BlockFace::RIGHT, u, v, w, h);
		vd[count++] = VertexBuffer::VertexData(globalX + 0.5f, y - 0.5f, globalZ + 0.5f, 0.9f, 0.9f, 0.9f, u, v);
		vd[count++] = VertexBuffer::VertexData(globalX + 0.5f, y - 0.5f, globalZ - 0.5f, 0.9f, 0.9f, 0.9f, u + w, v);
		vd[count++] = VertexBuffer::VertexData(globalX + 0.5f, y + 0.5f, globalZ - 0.5f, 0.9f, 0.9f, 0.9f, u + w, v + h);
		vd[count++] = VertexBuffer::VertexData(globalX + 0.5f, y + 0.5f, globalZ + 0.5f, 0.9f, 0.9f, 0.9f, u, v + h);
	}

	// BACK
	if (GetBlock(x, y, z - 1) == BTYPE_AIR || z == 0) {
		BlockInfo::GetBlockTextureCoords(type, BlockInfo::BlockFace::BACK, u, v, w, h);
		vd[count++] = VertexBuffer::VertexData(globalX + 0.5f, y - 0.5f, globalZ - 0.5f, 1.0f, 1.0f, 1.0f, u, v);
		vd[count++] = VertexBuffer::VertexData(globalX - 0.5f, y - 0.5f, globalZ - 0.5f, 1.0f, 1.0f, 1.0f, u + w, v);
		vd[count++] = VertexBuffer::VertexData(globalX - 0.5f, y + 0.5f, globalZ - 0.5f, 1.0f, 1.0f, 1.0f, u + w, v + h);
		vd[count++] = VertexBuffer::VertexData(globalX + 0.5f, y + 0.5f, globalZ - 0.5f, 1.0f, 1.0f, 1.0f, u, v + h);
	}

	// LEFT
	if (GetBlock(x - 1, y, z) == BTYPE_AIR || x == 0) {
		BlockInfo::GetBlockTextureCoords(type, BlockInfo::BlockFace::LEFT, u, v, w, h);
		vd[count++] = VertexBuffer::VertexData(globalX - 0.5f, y - 0.5f, globalZ - 0.5f, 0.9f, 0.9f, 0.9f, u, v);
		vd[count++] = VertexBuffer::VertexData(globalX - 0.5f, y - 0.5f, globalZ + 0.5f, 0.9f, 0.9f, 0.9f, u + w, v);
		vd[count++] = VertexBuffer::VertexData(globalX - 0.5f, y + 0.5f, globalZ + 0.5f, 0.9f, 0.9f, 0.9f, u + w, v + h);
		vd[count++] = VertexBuffer::VertexData(globalX - 0.5f, y + 0.5f, globalZ - 0.5f, 0.9f, 0.9f, 0.9f, u, v + h);
	}

	// BOTTOM
	if (GetBlock(x, y - 1, z) == BTYPE_AIR || y == 0) {
		BlockInfo::GetBlockTextureCoords(type, BlockInfo::BlockFace::BOTTOM, u, v, w, h);
		vd[count++] = VertexBuffer::VertexData(globalX - 0.5f, y - 0.5f, globalZ - 0.5f, 0.8f, 0.8f, 0.8f, u, v);
		vd[count++] = VertexBuffer::VertexData(globalX + 0.5f, y - 0.5f, globalZ - 0.5f, 0.8f, 0.8f, 0.8f, u + w, v);
		vd[count++] = VertexBuffer::VertexData(globalX + 0.5f, y - 0.5f, globalZ + 0.5f, 0.8f, 0.8f, 0.8f, u + w, v + h);
		vd[count++] = VertexBuffer::VertexData(globalX - 0.5f, y - 0.5f, globalZ + 0.5f, 0.8f, 0.8f, 0.8f, u, v + h);
	}

	// TOP
	if (GetBlock(x, y + 1, z) == BTYPE_AIR) {
		BlockInfo::GetBlockTextureCoords(type, BlockInfo::BlockFace::TOP, u, v, w, h);
		vd[count++] = VertexBuffer::VertexData(globalX - 0.5f, y + 0.5f, globalZ + 0.5f, 0.8f, 0.8f, 0.8f, u, v);
		vd[count++] = VertexBuffer::VertexData(globalX + 0.5f, y + 0.5f, globalZ + 0.5f, 0.8f, 0.8f, 0.8f, u + w, v);
		vd[count++] = VertexBuffer::VertexData(globalX + 0.5f, y + 0.5f, globalZ - 0.5f, 0.8f, 0.8f, 0.8f, u + w, v + h);
		vd[count++] = VertexBuffer::VertexData(globalX - 0.5f, y + 0.5f, globalZ - 0.5f, 0.8f, 0.8f, 0.8f, u, v + h);
	}
}

void Chunk::Render() const {
	m_vbo.Render();
}

bool Chunk::IsDirty() const {
	return m_isDirty;
}

void Chunk::RemoveBlock(int x, int y, int z) {
	m_blocks.Set(x, y, z, BTYPE_AIR);
}

void Chunk::SetBlock(int x, int y, int z, BlockType type) {
	m_blocks.Set(x, y, z, type);
}

void Chunk::SetChunkCoords(int x, int z) {
	m_chunkX = x;
	m_chunkZ = z;
}

BlockType Chunk::GetBlock(int x, int y, int z) {
	return m_blocks.Get(x, y, z);
}
