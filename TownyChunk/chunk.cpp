#include "chunk.h"

Chunk::Chunk() : BlockArray3d(CHUNK_SIZE_X, CHUNK_SIZE_Y, CHUNK_SIZE_Z){
	std::cout << "ctor Chunk" << '\n' << std::endl;
}

Chunk::~Chunk() {
	std::cout << "dtor Chunk" << '\n' << std::endl;
}

void Chunk::RemoveBlock(int x, int y, int z) {
	std::cout << "Removing block at (" << x << ", " << y << ", " << z << ")" << '\n' << std::endl;
	BlockArray3d::Set(x, y, z, BTYPE_AIR);
}

BlockType Chunk::GetBlock(int x, int y, int z) const {
	return BlockArray3d::Get(x, y, z);
}

void Chunk::SetBlock(int x, int y, int z, BlockType type) {
	std::cout << "Setting block at (" << x << ", " << y << ", " << z << ") to type " << type << '\n';
	BlockArray3d::Set(x, y, z, type);
}
