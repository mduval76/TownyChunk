#ifndef CHUNK_H
#define CHUNK_H

#include <iostream>
#include "blockarray3d.h"
#include "define.h"

class Chunk : public BlockArray3d {
public:
	Chunk();
	~Chunk();

	BlockType GetBlock(int x, int y, int z) const;
	void SetBlock(int x, int y, int z, BlockType type);
	void RemoveBlock(int x, int y, int z);

private:
};

#endif // CHUNK_H