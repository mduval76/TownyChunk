#ifndef BLOCKARRAY3D_H
#define BLOCKARRAY3D_H

#include <iostream>

#include "define.h"

class BlockArray3d {
public:
	BlockArray3d();
	BlockArray3d(int x, int y, int z);
	BlockArray3d(const BlockArray3d& arrayCopy);
	~BlockArray3d();

	void Reset();

protected:
	BlockType Get(int x, int y, int z) const;
	void Set(int x, int y, int z, BlockType type);

	int m_x = 0, m_y = 0, m_z = 0;
	BlockType* m_blocks = nullptr;
};

#endif // BLOCKARRAY3D_H