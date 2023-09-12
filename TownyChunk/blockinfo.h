#ifndef BLOCKINFO_H
#define BLOCKINFO_H

#include <iostream>
#include <string>

#include "define.h"

// Classe représentant toutes les informations d'un bloc dans le monde.
class BlockInfo {
public:
	BlockInfo(BlockType type = BTYPE_AIR, const std::string& name = "Air", const int durability = 0);
	~BlockInfo();

	BlockType GetType() const;
	void SetType(BlockType type);

	int GetDurability() const;
	void SetDurability(int durability);

	const std::string& GetName() const;
	void SetName(BlockType type);

	void Show() const;

private:
	BlockType m_type;
	std::string m_name;
	int m_durability;
};

#endif // BLOCKINFO_H