#include "blockinfo.h"

BlockInfo::BlockInfo(BlockType type, const std::string& name, int durability) : m_type(type), m_name(name), m_durability(durability) {
	std::cout << "ctor BlockInfo" << '\n' << std::endl;

}

BlockInfo::~BlockInfo() {
	std::cout << "dtor BlockInfo" << '\n' << std::endl;
}

BlockType BlockInfo::GetType() const {
	std::cout << "BlockInfo GetType called" << '\n';
	return m_type;
}

void BlockInfo::SetType(BlockType type) {
	std::cout << "BlockInfo SetType called" << '\n';
	m_type = type;
}

int BlockInfo::GetDurability() const {
	std::cout << "BlockInfo GetDurability called" << '\n';
	return m_durability;
}

void BlockInfo::SetDurability(int durability) {
	std::cout << "BlockInfo SetDurability called" << '\n';
	m_durability = durability;
}

const std::string& BlockInfo::GetName() const {
	std::cout << "BlockInfo GetName called" << '\n';
	return m_name;
}

void BlockInfo::SetName(BlockType type) {
	std::cout << "BlockInfo SetName called" << '\n';
	switch (type) {
	case BTYPE_DIRT:
		m_name = "Dirt";
		break;
	case BTYPE_GRASS:
		m_name = "Grass";
		break;
	default:
		m_name = "Air";
		break;
	}
}


void BlockInfo::Show() const {
	std::cout << "Type de bloc : " << m_type << std::endl;
	std::cout << "Nom du bloc : " << m_name << std::endl;
	std::cout << "Durabilite du bloc : " << m_durability << '\n' << std::endl;
}
