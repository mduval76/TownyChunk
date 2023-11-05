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

#define GRAVITY -9.81f
#define PI 3.1415926535897932384626433832795f

#define INITIAL_JUMP_FORCE 5.0f

typedef uint8_t BlockType;
enum BLOCK_TYPE { BTYPE_AIR, BTYPE_DIRT, BTYPE_FACE, BTYPE_HELL, BTYPE_MARBLE, BTYPE_STONE, BTYPE_LAST };

#define TEXTURE_PATH    "../townychunk/media/textures/"
#define SHADER_PATH     "../townychunk/media/shaders/"
#define VIEW_DISTANCE   128

#define WORLD_SIZE_X   5
#define WORLD_SIZE_Z   5

#endif // DEFINE_H__