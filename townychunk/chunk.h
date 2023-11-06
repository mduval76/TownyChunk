#ifndef CHUNK_H__
#define CHUNK_H__

#include "array2d.h"
#include "array3d.h"
#include "vertexbuffer.h"

class Chunk {
public:
	Chunk(int x, int z);
	~Chunk();

	BlockType GetBlock(int x, int y, int z);
	void SetBlock(int x, int y, int z, BlockType type);
	void RemoveBlock(int x, int y, int z);

	int GetChunkXCoord() const;
	int GetChunkZCoord() const;

	bool IsDirty() const;

	void Render() const;
	void Update();
	void AddBlockToMesh(VertexBuffer::VertexData* vd, int& count, BlockType type, int x, int y, int z);

private:
	Array3d<BlockType> m_blocks;
	VertexBuffer m_vbo = VertexBuffer();

	int m_chunkX = 0;
	int m_chunkZ = 0;

	bool m_isDirty = true;
};

#endif // CHUNK_H__
