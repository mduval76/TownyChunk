#ifndef BLOCKINFO_H__
#define BLOCKINFO_H__

#include <map>
#include <string>
#include "define.h"

class BlockInfo
{
public:
    struct TextureCoordinates {
        float u, v, w, h;
    };

    BlockInfo(BlockType type, const std::string& name, int durability, float u, float v, float w, float h);
    ~BlockInfo();

    BlockType GetType() const;

    static void GetBlockTextureCoords(BlockType type, float& u, float& v, float& w, float& h);
    static void SetBlockTextureCoords(BlockType type, float u, float v, float w, float h);

    int GetDurability() const;
    void SetDurability(int durability);

    void Show() const;

private:
    BlockType m_type;
    std::string m_name;
    int m_durability;

    static std::map<BlockType, TextureCoordinates> textureCoords;
};

#endif // BLOCKINFO_H__