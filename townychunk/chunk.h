#ifndef CHUNK_H__
#define CHUNK_H__

#include "define.h"
#include "iworld.h"
#include "array2d.h"
#include "array3d.h"
#include "blockinfo.h"
#include "vertexbuffer.h"
#include "perlin.h"

class Chunk {
public:
	Chunk(IWorld* world, int x, int z);
	~Chunk();

	BlockType GetBlock(int x, int y, int z);
	void SetBlock(int x, int y, int z, BlockType type);
	void RemoveBlock(int x, int y, int z);

	int GetChunkXCoord() const;
	int GetChunkZCoord() const;

	bool IsBlockFaceVisible(int x, int y, int z, BlockFace face);
	bool IsDirty() const;

	void Update();
	void Render() const;
	void AddBlockToMesh(VertexBuffer::VertexData* vd, int& count, BlockType type, int x, int y, int z, BlockFace face);

private:
	Array3d<BlockType> m_blocks;
	Perlin perlin;
	VertexBuffer m_vbo = VertexBuffer();

	IWorld* m_world;
	
	int m_chunkXCoord = 0;
	int m_chunkZCoord = 0;

	bool m_isDirty = true;
};

#endif // CHUNK_H__
