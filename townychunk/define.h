#ifndef DEFINE_H__
#define DEFINE_H__

#include <SFML/Window.hpp>
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

#define PI 3.1415926535897932384626433832795f

typedef uint8_t BlockType; // Using uint8_t instead of enum, the amount of memory used is reduced by 75%.
enum BLOCK_TYPE {BTYPE_AIR, BTYPE_DIRT, BTYPE_GRASS};

#define TEXTURE_PATH        "../townychunk/media/textures/"
#define VIEW_DISTANCE       128

#endif // DEFINE_H__
