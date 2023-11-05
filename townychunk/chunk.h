#ifndef CHUNK_H__
#define CHUNK_H__

#include "array2d.h"
#include "array3d.h"
#include "vertexbuffer.h"

struct ChunkWorldCoords {
	int x, z;
	ChunkWorldCoords(int chunkPosX = 0, int chunkPosZ = 0) : x(chunkPosX), z(chunkPosZ) {}
};

class Chunk {
public:
	Chunk();
	~Chunk();

	BlockType GetBlock(int x, int y, int z);

	bool IsDirty() const;

	void Update();
	void AddBlockToMesh(VertexBuffer::VertexData* vd, int& count, BlockType type, int x, int y, int z);
	void Render() const;
	void SetBlock(int x, int y, int z, BlockType type);
	void SetChunkCoords(int x, int z);
	void RemoveBlock(int x, int y, int z);

private:
	Array3d<BlockType> m_blocks;
	VertexBuffer m_vbo = VertexBuffer();
	
	ChunkWorldCoords m_chunkCoords;

	bool m_isDirty = true;
};

#endif // CHUNK_H__
