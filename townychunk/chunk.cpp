#include <iostream>
#include <climits>

#include "chunk.h"

Chunk::Chunk() : m_blocks(CHUNK_SIZE_X, CHUNK_SIZE_Y, CHUNK_SIZE_Z) {
	m_blocks.Reset(BTYPE_AIR);
	for (int x = 0; x < CHUNK_SIZE_X; ++x) {
		for (int z = 0; z < CHUNK_SIZE_Z; ++z) {
			for (int y = 0; y < 4; ++y) {
					SetBlock(x, y, z, BTYPE_STONE);
			}
		}
	}
}

Chunk::~Chunk() {
}


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
	// FRONT
	vd[count++] = VertexBuffer::VertexData(x - 0.5f, y - 0.5f, z + 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f);
	vd[count++] = VertexBuffer::VertexData(x + 0.5f, y - 0.5f, z + 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f);
	vd[count++] = VertexBuffer::VertexData(x + 0.5f, y + 0.5f, z + 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
	vd[count++] = VertexBuffer::VertexData(x - 0.5f, y + 0.5f, z + 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f);

	// RIGHT
	vd[count++] = VertexBuffer::VertexData(x + 0.5f, y - 0.5f, z + 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f);
	vd[count++] = VertexBuffer::VertexData(x + 0.5f, y - 0.5f, z - 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f);
	vd[count++] = VertexBuffer::VertexData(x + 0.5f, y + 0.5f, z - 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
	vd[count++] = VertexBuffer::VertexData(x + 0.5f, y + 0.5f, z + 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f);

	// BACK
	vd[count++] = VertexBuffer::VertexData(x + 0.5f, y - 0.5f, z - 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f);
	vd[count++] = VertexBuffer::VertexData(x - 0.5f, y - 0.5f, z - 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f);
	vd[count++] = VertexBuffer::VertexData(x - 0.5f, y + 0.5f, z - 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
	vd[count++] = VertexBuffer::VertexData(x + 0.5f, y + 0.5f, z - 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f);

	// LEFT
	vd[count++] = VertexBuffer::VertexData(x - 0.5f, y - 0.5f, z - 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f);
	vd[count++] = VertexBuffer::VertexData(x - 0.5f, y - 0.5f, z + 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f);
	vd[count++] = VertexBuffer::VertexData(x - 0.5f, y + 0.5f, z + 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
	vd[count++] = VertexBuffer::VertexData(x - 0.5f, y + 0.5f, z - 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f);

	// TOP
	vd[count++] = VertexBuffer::VertexData(x - 0.5f, y + 0.5f, z + 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f);
	vd[count++] = VertexBuffer::VertexData(x + 0.5f, y + 0.5f, z + 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f);
	vd[count++] = VertexBuffer::VertexData(x + 0.5f, y + 0.5f, z - 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
	vd[count++] = VertexBuffer::VertexData(x - 0.5f, y + 0.5f, z - 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f);

	// BOTTOM
	vd[count++] = VertexBuffer::VertexData(x - 0.5f, y - 0.5f, z - 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f);
	vd[count++] = VertexBuffer::VertexData(x + 0.5f, y - 0.5f, z - 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f);
	vd[count++] = VertexBuffer::VertexData(x + 0.5f, y - 0.5f, z + 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
	vd[count++] = VertexBuffer::VertexData(x - 0.5f, y - 0.5f, z + 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f);
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
