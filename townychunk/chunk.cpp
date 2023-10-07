#include "chunk.h"

Chunk::Chunk() {
}

Chunk::~Chunk() {
}

void Chunk::RemoveBlock(int x, int y, int z) {
	m_blocks->Set(x, y, z, BTYPE_AIR);
}

void Chunk::SetBlock(int x, int y, int z, BlockType type) {
	m_blocks->Set(x, y, z, type);
}

BlockType Chunk::GetBlock(int x, int y, int z) {
	return m_blocks->Get(x, y, z);
}

