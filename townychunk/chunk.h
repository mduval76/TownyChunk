#ifndef CHUNK_H__
#define CHUNK_H__

#include "array3d.h"

class Chunk {
public:
	Chunk();
	~Chunk();

	BlockType GetBlock(int x, int y, int z);
	void SetBlock(int x, int y, int z, BlockType type);
	void RemoveBlock(int x, int y, int z);

private:
	Array3d<BlockType>* m_blocks;
};

#endif // CHUNK_H__
