#include "blockinfo.h"
#include <iostream>

std::map<BlockType, std::map<BlockInfo::BlockFace, BlockInfo::TextureCoordinates>> BlockInfo::textureCoords = {};

BlockInfo::BlockInfo(BlockType type, const std::string& name, int durability)
    : m_type(type), m_name(name), m_durability(durability) {}

BlockInfo::~BlockInfo() {}

BlockType BlockInfo::GetType() const {
    return m_type;
}

void BlockInfo::GetBlockTextureCoords(BlockType type, BlockFace face, float& u, float& v, float& w, float& h) {
    if (textureCoords.find(type) != textureCoords.end() && textureCoords[type].find(face) != textureCoords[type].end()) {
        u = textureCoords[type][face].u;
        v = textureCoords[type][face].v;
        w = textureCoords[type][face].w;
        h = textureCoords[type][face].h;
    }
}

void BlockInfo::SetBlockTextureCoords(BlockType type, BlockFace face, float u, float v, float w, float h) {
    textureCoords[type][face] = { u, v, w, h };
}

int BlockInfo::GetDurability() const {
    return m_durability;
}

void BlockInfo::SetDurability(int durability) {
    m_durability = durability;
}

void BlockInfo::Show() const {
    // Display the block properties and texture coordinates for each face
    std::cout << "Type: " << m_type << std::endl;
    std::cout << "Nom: " << m_name << std::endl;
    std::cout << "Durabilite: " << m_durability << std::endl;

    for (int face = 0; face < BlockFace::LAST; ++face) {
        float u, v, w, h;
        GetBlockTextureCoords(m_type, static_cast<BlockFace>(face), u, v, w, h);
        std::cout << "Face " << face << " Coordonnees: " << u << ", " << v << ", " << w << ", " << h << std::endl;
    }
}
