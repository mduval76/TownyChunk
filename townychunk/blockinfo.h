#ifndef BLOCKINFO_H__
#define BLOCKINFO_H__

#include <map>
#include <string>
#include "define.h"

class BlockInfo {
public:
    struct TextureCoordinates {
        float u, v, w, h;
    };

    enum BlockFace {
        FRONT,
        BACK,
        LEFT,
        RIGHT,
        TOP,
        BOTTOM,
        LAST
    };

    BlockInfo(BlockType type, const std::string& name, int durability);
    ~BlockInfo();

    BlockType GetType() const;

    int GetDurability() const;
    void SetDurability(int durability);

    static void GetBlockTextureCoords(BlockType type, BlockFace face, float& u, float& v, float& w, float& h);
    static void SetBlockTextureCoords(BlockType type, BlockFace face, float u, float v, float w, float h);

    void Show() const;

private:
    BlockType m_type;
    std::string m_name;
    int m_durability;

    static std::map<BlockType, std::map<BlockFace, TextureCoordinates>> textureCoords;
};

#endif // BLOCKINFO_H__