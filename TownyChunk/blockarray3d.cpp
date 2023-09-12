#include "blockarray3d.h"

BlockArray3d::BlockArray3d() { }

BlockArray3d::BlockArray3d(int x, int y, int z) : m_x(x), m_y(y), m_z(z) {
    std::cout << "ctor BlockArray3d" << '\n' << std::endl;
    m_blocks = new BlockType[m_x * m_y * m_z];
    Reset();
}

BlockArray3d::BlockArray3d(const BlockArray3d& arrayCopy) : m_x(arrayCopy.m_x), m_y(arrayCopy.m_y), m_z(arrayCopy.m_z) {
    std::cout << "copy ctor BlockArray3d" << '\n' << std::endl;
    m_blocks = new BlockType[m_x * m_y * m_z];
    for (int i = 0; i < m_x * m_y * m_z; ++i) {
		m_blocks[i] = arrayCopy.m_blocks[i];
	}
}

BlockArray3d::~BlockArray3d() {
    std::cout << "dtor BlockArray3d" << '\n' << std::endl;
    delete[] m_blocks;
}

BlockType BlockArray3d::Get(int x, int y, int z) const {
    return m_blocks[x + (z * m_x) + (y * m_z * m_x)];
}

void BlockArray3d::Set(int x, int y, int z, BlockType type) {
    m_blocks[x + (z * m_x) + (y * m_z * m_x)] = type;
}

void BlockArray3d::Reset() {
    for (int i = 0; i < m_x * m_y * m_z; ++i) {
		m_blocks[i] = BTYPE_AIR;
	}
}
