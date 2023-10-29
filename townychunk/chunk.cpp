#include <iostream>
#include <climits>

#include "chunk.h"
#include "blockinfo.h"

Chunk::Chunk() : m_blocks(CHUNK_SIZE_X, CHUNK_SIZE_Y, CHUNK_SIZE_Z) {
	m_blocks.Reset(BTYPE_AIR);
	for (int x = 0; x < CHUNK_SIZE_X; ++x) {
		for (int z = 0; z < CHUNK_SIZE_Z; ++z) {
			for (int y = 0; y < 4; ++y) {
				SetBlock(x, y, z, BTYPE_HELL);
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
	}

	m_isDirty = false;
}

void Chunk::AddBlockToMesh(VertexBuffer::VertexData* vd, int& count, BlockType type, int x, int y, int z) {
	float u, v, w, h;

	// FRONT
	BlockInfo::GetBlockTextureCoords(type, BlockInfo::BlockFace::FRONT, u, v, w, h);
	vd[count++] = VertexBuffer::VertexData(x - 0.5f, y - 0.5f, z + 0.5f, 1.0f, 1.0f, 1.0f, u, v);
	vd[count++] = VertexBuffer::VertexData(x + 0.5f, y - 0.5f, z + 0.5f, 1.0f, 1.0f, 1.0f, u + w, v);
	vd[count++] = VertexBuffer::VertexData(x + 0.5f, y + 0.5f, z + 0.5f, 1.0f, 1.0f, 1.0f, u + w, v + h);
	vd[count++] = VertexBuffer::VertexData(x - 0.5f, y + 0.5f, z + 0.5f, 1.0f, 1.0f, 1.0f, u, v + h);

	// RIGHT
	BlockInfo::GetBlockTextureCoords(type, BlockInfo::BlockFace::RIGHT, u, v, w, h);
	vd[count++] = VertexBuffer::VertexData(x + 0.5f, y - 0.5f, z + 0.5f, 1.0f, 1.0f, 1.0f, u, v);
	vd[count++] = VertexBuffer::VertexData(x + 0.5f, y - 0.5f, z - 0.5f, 1.0f, 1.0f, 1.0f, u + w, v);
	vd[count++] = VertexBuffer::VertexData(x + 0.5f, y + 0.5f, z - 0.5f, 1.0f, 1.0f, 1.0f, u + w, v + h);
	vd[count++] = VertexBuffer::VertexData(x + 0.5f, y + 0.5f, z + 0.5f, 1.0f, 1.0f, 1.0f, u, v + h);

	// BACK
	BlockInfo::GetBlockTextureCoords(type, BlockInfo::BlockFace::BACK, u, v, w, h);
	vd[count++] = VertexBuffer::VertexData(x + 0.5f, y - 0.5f, z - 0.5f, 1.0f, 1.0f, 1.0f, u, v);
	vd[count++] = VertexBuffer::VertexData(x - 0.5f, y - 0.5f, z - 0.5f, 1.0f, 1.0f, 1.0f, u + w, v);
	vd[count++] = VertexBuffer::VertexData(x - 0.5f, y + 0.5f, z - 0.5f, 1.0f, 1.0f, 1.0f, u + w, v + h);
	vd[count++] = VertexBuffer::VertexData(x + 0.5f, y + 0.5f, z - 0.5f, 1.0f, 1.0f, 1.0f, u, v + h);

	// LEFT
	BlockInfo::GetBlockTextureCoords(type, BlockInfo::BlockFace::LEFT, u, v, w, h);
	vd[count++] = VertexBuffer::VertexData(x - 0.5f, y - 0.5f, z - 0.5f, 1.0f, 1.0f, 1.0f, u, v);
	vd[count++] = VertexBuffer::VertexData(x - 0.5f, y - 0.5f, z + 0.5f, 1.0f, 1.0f, 1.0f, u + w, v);
	vd[count++] = VertexBuffer::VertexData(x - 0.5f, y + 0.5f, z + 0.5f, 1.0f, 1.0f, 1.0f, u + w, v + h);
	vd[count++] = VertexBuffer::VertexData(x - 0.5f, y + 0.5f, z - 0.5f, 1.0f, 1.0f, 1.0f, u, v + h);

	// TOP
	BlockInfo::GetBlockTextureCoords(type, BlockInfo::BlockFace::TOP, u, v, w, h);
	vd[count++] = VertexBuffer::VertexData(x - 0.5f, y + 0.5f, z + 0.5f, 1.0f, 1.0f, 1.0f, u, v);
	vd[count++] = VertexBuffer::VertexData(x + 0.5f, y + 0.5f, z + 0.5f, 1.0f, 1.0f, 1.0f, u + w, v);
	vd[count++] = VertexBuffer::VertexData(x + 0.5f, y + 0.5f, z - 0.5f, 1.0f, 1.0f, 1.0f, u + w, v + h);
	vd[count++] = VertexBuffer::VertexData(x - 0.5f, y + 0.5f, z - 0.5f, 1.0f, 1.0f, 1.0f, u, v + h);

	// BOTTOM
	BlockInfo::GetBlockTextureCoords(type, BlockInfo::BlockFace::BOTTOM, u, v, w, h);
	vd[count++] = VertexBuffer::VertexData(x - 0.5f, y - 0.5f, z - 0.5f, 1.0f, 1.0f, 1.0f, u, v);
	vd[count++] = VertexBuffer::VertexData(x + 0.5f, y - 0.5f, z - 0.5f, 1.0f, 1.0f, 1.0f, u + w, v);
	vd[count++] = VertexBuffer::VertexData(x + 0.5f, y - 0.5f, z + 0.5f, 1.0f, 1.0f, 1.0f, u + w, v + h);
	vd[count++] = VertexBuffer::VertexData(x - 0.5f, y - 0.5f, z + 0.5f, 1.0f, 1.0f, 1.0f, u, v + h);
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

BlockType Chunk::GetBlock(int x, int y, int z) {
	return m_blocks.Get(x, y, z);
}
