#ifndef IWORLD_H__
#define IWORLD_H__

class Chunk;

class IWorld {
public:
    virtual ~IWorld() = default;
    virtual Chunk* GetChunk(int x, int z) const = 0;
};

#endif // IWORLD_H__