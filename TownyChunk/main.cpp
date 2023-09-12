#include <iostream>
#include <random>

#include "chunk.h"
#include "blockarray3d.h"
#include "blockinfo.h"
#include "define.h"

std::random_device rd;
std::mt19937 gen(rd());

std::uniform_int_distribution<> dist1(0, 15);   // Range [0, 15]
std::uniform_int_distribution<> dist2(0, 127);  // Range [0, 127]


void GetSame5BlocksResults(Chunk& chunk) {
	for (int i = 9; i < 14; i++) {
		std::cout << chunk.GetBlock(15, 127, i);
		if (i < 13) {
			std::cout << '-';
		}
	}
	std::cout << '\n' << std::endl;
}

void Test_1(Chunk& chunk) {
	int random1 = dist1(gen);
	int random2 = dist2(gen);
	int random3 = dist1(gen);
	BlockType blockType = chunk.GetBlock(random1, random2, random3);

	std::cout << "Target Block @ (" << random1 << ", " << random2 << ", " << random3 << ")" << '\n';
	std::cout << "EXPECTED RESULT : TYPE 0 (AIR)" << '\n';
	std::cout << "ACTUAL RESULT : TYPE " << blockType << '\n' << std::endl;
}

void Test_2(Chunk& chunk) {
	std::cout << "Target Block starting @ (15, 127, 9)" << '\n';
	std::cout << "EXPECTED RESULT : TYPE 0-2-1-2-0 (AIR-GRASS-DIRT-GRASS-AIR)" << '\n';
	std::cout << "ACTUAL RESULT : TYPE ";
	GetSame5BlocksResults(chunk);
}

void Test_3(Chunk& chunk) {
	std::cout << "Target Block starting @ (15, 127, 9)" << '\n';
	std::cout << "EXPECTED RESULT : TYPE 0-2-0-2-0 (AIR-GRASS-AIR-GRASS-AIR)" << '\n';
	std::cout << "ACTUAL RESULT : TYPE ";
	GetSame5BlocksResults(chunk);
}

void Test_4(Chunk& chunk) {
	for (int i = 0; i < 3; i++) {
		int random1 = dist1(gen);
		int random2 = dist2(gen);
		int random3 = dist1(gen);
		switch (i) {
		case 0: {
			chunk.SetBlock(random1, random2, random3, BTYPE_DIRT);
			BlockType blockType_1 = chunk.GetBlock(random1, random2, random3);
			std::cout << "Target Block_1 @ (" << random1 << ", " << random2 << ", " << random3 << ")" << '\n';
			std::cout << "EXPECTED RESULT : TYPE 1 (DIRT)" << '\n';
			std::cout << "ACTUAL RESULT : TYPE " << blockType_1 << '\n' << std::endl;
			break;
			}
		case 1: {
			chunk.SetBlock(random1, random2, random3, BTYPE_GRASS);
			BlockType blockType_2 = chunk.GetBlock(random1, random2, random3);
			std::cout << "Target Block_2 @ (" << random1 << ", " << random2 << ", " << random3 << ")" << '\n';
			std::cout << "EXPECTED RESULT : TYPE 2 (GRASS)" << '\n';
			std::cout << "ACTUAL RESULT : TYPE " << blockType_2 << '\n' << std::endl;
			break;
		}
		case 2: {
			chunk.SetBlock(random1, random2, random3, BTYPE_AIR);
			BlockType blockType_3 = chunk.GetBlock(random1, random2, random3);
			std::cout << "Target Block_3 @ (" << random1 << ", " << random2 << ", " << random3 << ")" << '\n';
			std::cout << "EXPECTED RESULT : TYPE 0 (AIR)" << '\n';
			std::cout << "ACTUAL RESULT : TYPE " << blockType_3 << '\n' << std::endl;
			break;
			}
		default:
			chunk.SetBlock(random1, random2, random3, BTYPE_AIR);
			break;
		}
	}
}

void Test_5(BlockInfo& blockInfo) {
	std::cout << "ACTUAL RESULTS: " << '\n';
	blockInfo.Show();
}

void Test_6(BlockInfo& blockInfo){
	blockInfo.SetType(BTYPE_DIRT);
	blockInfo.SetName(BTYPE_GRASS); // Name change even if type is not BTYPE_DIRT
	blockInfo.SetDurability(99);

	std::cout << "EXPECTED RESULT : 1 - \"Grass\" - 99" << '\n';
	std::cout << "ACTUAL RESULT TYPE : " << blockInfo.GetType() << '\n';
	std::cout << "ACTUAL RESULT NAME : " << blockInfo.GetName() << '\n';
	std::cout << "ACTUAL RESULT DURABILITY : " << blockInfo.GetDurability() << '\n';
	std::cout << std::endl;
}

int main() {
	// ---------------------------------------------------------------------------------------------
	std::cout << "TESTS FOR CHUNK/BLOCKARRAY3D : " << '\n' << std::endl;
	Chunk chunk01 = Chunk();

	std::cout << "TEST_1 : Return a random block type after initialization." << '\n';
	Test_1(chunk01);

	std::cout << "TEST_2 : Set 3 consecutive blocks to types: GRASS(2), DIRT(1), GRASS(2)" << '\n';
	chunk01.SetBlock(15, 127, 10, BTYPE_GRASS);
	chunk01.SetBlock(15, 127, 11, BTYPE_DIRT);
	chunk01.SetBlock(15, 127, 12, BTYPE_GRASS);
	Test_2(chunk01);

	std::cout << "TEST_3 : Remove the DIRT(1) block from Test_2" << '\n';
	chunk01.RemoveBlock(15, 127, 11);
	Test_3(chunk01);

	std::cout << "TEST_4 : Set 3 blocks to random locations. Use show() to display right information for these blocks." << '\n';
	Test_4(chunk01);

	// ---------------------------------------------------------------------------------------------
	std::cout << "TESTS FOR BLOCKINFO : " << '\n' << std::endl;
	std::cout << "TEST_5 : Set a BlockInfo object to BTYPE_DIRT and another to BTYPE_GRASS and display their information." << '\n' << std::endl;

	BlockInfo blockInfo1 = BlockInfo(BTYPE_DIRT, "Dirt", 10);
	std::cout << "EXPECTED RESULT : 1 - \"Dirt\" - 10" << '\n';
	Test_5(blockInfo1);

	BlockInfo blockInfo2 = BlockInfo(BTYPE_GRASS, "Grass", 10);
	std::cout << "EXPECTED RESULT : 2 - \"Grass\" - 10" << '\n';
	Test_5(blockInfo2);

	BlockInfo blockInfo3 = BlockInfo(BTYPE_AIR, "Air", 0);
	std::cout << "TEST_6 : Set a BlockInfo objects' to AIR. Change it's properties and display the new values without Show()" << '\n';
	Test_6(blockInfo3);
}
