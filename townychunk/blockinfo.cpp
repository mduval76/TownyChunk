#include "blockinfo.h"
#include <iostream>

// Initialize the static member here
std::map<BlockType, BlockInfo::TextureCoordinates> BlockInfo::textureCoords = {};

BlockInfo::BlockInfo(BlockType type, const std::string& name, int durability, float u, float v, float w, float h)
    : m_type(type), m_name(name), m_durability(durability)
{
    // Using the static method to set the texture coordinates for this BlockType
    SetBlockTextureCoords(type, u, v, w, h);
}

BlockInfo::~BlockInfo() {}

BlockType BlockInfo::GetType() const {
    return m_type;
}

void BlockInfo::GetBlockTextureCoords(BlockType type, float& u, float& v, float& w, float& h) {
    if (textureCoords.find(type) != textureCoords.end()) {
        u = textureCoords[type].u;
        v = textureCoords[type].v;
        w = textureCoords[type].w;
        h = textureCoords[type].h;
    }
}

void BlockInfo::SetBlockTextureCoords(BlockType type, float u, float v, float w, float h) {
    textureCoords[type] = { u, v, w, h };
}

int BlockInfo::GetDurability() const {
    return m_durability;
}

void BlockInfo::SetDurability(int durability) {
    m_durability = durability;
}

void BlockInfo::Show() const {
    float u, v, w, h;
    GetBlockTextureCoords(m_type, u, v, w, h); // using the static method to retrieve the coords

    std::cout << "Type: " << m_type << std::endl;
    std::cout << "Nom: " << m_name << std::endl;
    std::cout << "Durabilite: " << m_durability << std::endl;
    std::cout << "Coordonnees: " << u << ", " << v << ", " << w << ", " << h << std::endl;
}
