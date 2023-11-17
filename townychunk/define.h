#ifndef DEFINE_H__
#define DEFINE_H__

#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#ifdef _WIN32
#include <windows.h>
#include <GL/glew.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#else
#include <GL/glew.h>
#endif

#define CHUNK_SIZE_X 16
#define CHUNK_SIZE_Y 128
#define CHUNK_SIZE_Z 16

#define WORLD_SIZE_X   VIEW_DISTANCE / CHUNK_SIZE_X
#define WORLD_SIZE_Z   VIEW_DISTANCE / CHUNK_SIZE_Z

#define SPAWN_X (WORLD_SIZE_X * CHUNK_SIZE_X) / 2
#define SPAWN_Y CHUNK_SIZE_Y / 8
#define SPAWN_Z (WORLD_SIZE_Z * CHUNK_SIZE_Z) / 2

#define GRAVITY -9.81f
#define PI 3.1415926535897932384626433832795f

#define VIEW_DISTANCE   128
#define PLAYER_HEIGHT   1.7f
#define PLAYER_MIDDLE   0.9f

typedef uint8_t BlockType;
enum BLOCK_TYPE { BTYPE_AIR, BTYPE_DIRT, BTYPE_FACE, BTYPE_HELL, BTYPE_MARBLE, BTYPE_STONE, BTYPE_LAST };

typedef uint8_t BlockFace;
enum BLOCK_FACE { FRONT, BACK, LEFT, RIGHT, TOP, BOTTOM, LAST };

#define TEXTURE_PATH    "../townychunk/media/textures/"
#define SHADER_PATH     "../townychunk/media/shaders/"

#endif // DEFINE_H__