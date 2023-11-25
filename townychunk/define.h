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
#define SPAWN_Y CHUNK_SIZE_Y
#define SPAWN_Z (WORLD_SIZE_Z * CHUNK_SIZE_Z) / 2

#define GRAVITY -25.0f
#define DECELERATION 0.05f
#define PI 3.1415926535897932384626433832795f

#define VIEW_DISTANCE   128
#define MAX_SELECT_DISTANCE 5.0f
#define PLAYER_HEIGHT   1.7f
#define PLAYER_MIDDLE   0.9f

#define WINDOW_WIDTH	800
#define WINDOW_HEIGHT	600
#define FULL_HEIGHT		1080
#define RESIZE_RATIO	WINDOW_HEIGHT / FULL_HEIGHT

typedef uint8_t BlockType;
enum BLOCK_TYPE { 
	BTYPE_AIR,
	BTYPE_DECORATION,
	BTYPE_DIRT,
	BTYPE_EMBOSSED_BROWN,
	BTYPE_EMBOSSED_WHITE,
	BTYPE_HELL,
	BTYPE_CIRCLE,
	BTYPE_MOSAIC,
	BTYPE_BLACK,  
	BTYPE_BROWN, 
	BTYPE_DARKBLUE, 
	BTYPE_DARKGREY, 
	BTYPE_DARKGREEN,
	BTYPE_LIGHTBLUE, 
	BTYPE_LIGHTGREEN, 
	BTYPE_LIGHTGREY,
	BTYPE_MAGENTA,
	BTYPE_ORANGE,
	BTYPE_PINK,
	BTYPE_PURPLE,
	BTYPE_RED,
	BTYPE_WHITE,
	BTYPE_YELLOW,
	BTYPE_LAST };

typedef uint8_t BlockFace;
enum BLOCK_FACE { FRONT, BACK, LEFT, RIGHT, TOP, BOTTOM, LAST };

#define TEXTURE_PATH    "../townychunk/media/textures/"
#define SHADER_PATH     "../townychunk/media/shaders/"

#endif // DEFINE_H__