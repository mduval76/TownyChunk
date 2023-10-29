#include "engine.h"
#include <algorithm>
#include <cmath>
#include <iostream>

Engine::Engine() {}

Engine::~Engine() {}

void Engine::Init() {
	GLenum glewErr = glewInit();

	if (glewErr != GLEW_OK) {
		std::cerr << " ERREUR GLEW : " << glewGetErrorString(glewErr) << std::endl;
		abort();
	}

	std::cout << "Loading and compiling shaders..." << std::endl;
	if (!m_shader01.Load(SHADER_PATH "shader01.vert", SHADER_PATH "shader01.frag", true)) {
		std::cout << "Failed to load shader" << std::endl;
		exit(1);
	}

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (float)Width() / (float)Height(), 0.0001f, 1000.0f);
	glEnable(GL_DEPTH_TEST);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_CULL_FACE);

	// Light
	GLfloat light0Pos[4] = { 0.0f, CHUNK_SIZE_Y, 0.0f, 1.0f };
	GLfloat light0Amb[4] = { 0.9f, 0.9f, 0.9f, 1.0f };
	GLfloat light0Diff[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat light0Spec[4] = { 0.2f, 0.2f, 0.2f, 1.0f };

	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light0Pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0Amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0Diff);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0Spec);

	CenterMouse();
	HideCursor();
}

void Engine::DeInit() {}

void Engine::LoadResource() {
	LoadTexture(m_textureMonster, TEXTURE_PATH "monster.jpg");
	LoadTexture(m_textureDark, TEXTURE_PATH "darkness.jpg");

	TextureAtlas::TextureIndex texIdxDirt = m_textureAtlas.AddTexture(TEXTURE_PATH "dirt.jpg");
	TextureAtlas::TextureIndex texIdxFace = m_textureAtlas.AddTexture(TEXTURE_PATH "face.jpg");
	TextureAtlas::TextureIndex texIdxHellX = m_textureAtlas.AddTexture(TEXTURE_PATH "hell_x.png");
	TextureAtlas::TextureIndex texIdxHellY = m_textureAtlas.AddTexture(TEXTURE_PATH "hell_y.png");
	TextureAtlas::TextureIndex texIdxHellZ = m_textureAtlas.AddTexture(TEXTURE_PATH "hell_z.png");
	TextureAtlas::TextureIndex texIdxMarble = m_textureAtlas.AddTexture(TEXTURE_PATH "marble.jpg");
	TextureAtlas::TextureIndex texIdxStone = m_textureAtlas.AddTexture(TEXTURE_PATH "stone.jpg");

	if (!m_textureAtlas.Generate(256, false)) {
		std::cerr << "Unable to generate texture atlas..." << std::endl;
		abort();
	}

	std::map<BlockType, TextureAtlas::TextureIndex> btIndices;
	btIndices[BTYPE_DIRT] = texIdxDirt;
	btIndices[BTYPE_FACE] = texIdxFace;
	btIndices[BTYPE_MARBLE] = texIdxMarble;
	btIndices[BTYPE_STONE] = texIdxStone;

	for (auto& pair : btIndices) {
		float u, v, w, h;
		m_textureAtlas.TextureIndexToCoord(pair.second, u, v, w, h);

		for (int face = BlockInfo::FRONT; face < BlockInfo::LAST; ++face) {
			BlockInfo::SetBlockTextureCoords(pair.first, static_cast<BlockInfo::BlockFace>(face), u, v, w, h);
		}
	}

	float u, v, w, h;
	m_textureAtlas.TextureIndexToCoord(texIdxHellZ, u, v, w, h);
	BlockInfo::SetBlockTextureCoords(BTYPE_HELL, BlockInfo::FRONT, u, v, w, h); 
	m_textureAtlas.TextureIndexToCoord(texIdxHellZ, u, v, w, h);
	BlockInfo::SetBlockTextureCoords(BTYPE_HELL, BlockInfo::BACK, u, v, w, h);  
	m_textureAtlas.TextureIndexToCoord(texIdxHellX, u, v, w, h);
	BlockInfo::SetBlockTextureCoords(BTYPE_HELL, BlockInfo::LEFT, u, v, w, h);  
	m_textureAtlas.TextureIndexToCoord(texIdxHellX, u, v, w, h);
	BlockInfo::SetBlockTextureCoords(BTYPE_HELL, BlockInfo::RIGHT, u, v, w, h);
	m_textureAtlas.TextureIndexToCoord(texIdxHellX, u, v, w, h);
	BlockInfo::SetBlockTextureCoords(BTYPE_HELL, BlockInfo::TOP, u, v, w, h);
	m_textureAtlas.TextureIndexToCoord(texIdxHellX, u, v, w, h);
	BlockInfo::SetBlockTextureCoords(BTYPE_HELL, BlockInfo::BOTTOM, u, v, w, h);
}

void Engine::UnloadResource() {}

void Engine::Render(float elapsedTime) {
	static float gameTime = elapsedTime; // Valeur conserv�e entre les appels car static
	gameTime += elapsedTime;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Efface le tampon de couleur et de profondeur

	// Transformations initiales
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
 
	// Camera (Player)
	Transformation t;
	m_player.Move(m_keyW, m_keyS, m_keyA, m_keyD, gameTime);
	std::array<float, 2> rot = m_player.GetRotation();
	m_player.ApplyTransformation(t);
	t.ApplyTranslation(0.0f, 0.0f, -25.0f);
	t.Use();

	// Plancher
	DrawFloor();

	// Chunk
	m_textureAtlas.Bind();
	if (m_chunk.IsDirty())
		m_chunk.Update();

	m_shader01.Use();
	m_chunk.Render();
	Shader::Disable();

	// Skybox
	t.SetIdentity();
	t.ApplyRotation(-rot[0], 1.0f, 0, 0);
	t.ApplyRotation(-rot[1], 0, 1.0f, 0);
	t.Use();

	DrawSkybox();
}

void Engine::KeyPressEvent(unsigned char key) {
	switch (key) {
	case 0: // ( GAUCHE ) A
		m_keyA = true;
		break;
	case 3: // ( ARRI�RE ) D
		m_keyD = true;
		break;
	case 18: // ( DROITE ) S
		m_keyS = true;
		break;
	case 22: // ( AVANT ) W
		m_keyW = true;
		break;
	case 36: // ESC
		Stop();
		break;
	case 94: // F10
		SetFullscreen(!IsFullscreen());
		break;
	default:
		std::cout << "Unhandled key: " << (int)key << std::endl;
	}
}

void Engine::KeyReleaseEvent(unsigned char key) {
	switch (key) {
	case 0: // ( GAUCHE ) A
		m_keyA = false;
		break;
	case 3: // ( ARRI�RE ) D
		m_keyD = false;
		break;
	case 18: // ( DROITE ) S
		m_keyS = false;
		break;
	case 22: // ( AVANT ) W
		m_keyW = false;
		break;
	case 24: // Y
		m_wireframe = !m_wireframe;
		if (m_wireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	}
}

void Engine::MouseMoveEvent(int x, int y) {
	// Centrer la souris seulement si elle n'est pas d�j� centr�e
	// Il est n�cessaire de faire la v�rification pour �viter de tomber
	// dans une boucle infinie o� l'appel � CenterMouse g�n�re un
	// MouseMoveEvent, qui rapelle CenterMouse qui rapelle un autre
	// MouseMoveEvent, etc
	if (x == (Width() / 2) && y == (Height() / 2))
		return;

	MakeRelativeToCenter(x, y);
	m_player.TurnLeftRight((float)x);
	m_player.TurnTopBottom((float)y);
	CenterMouse();
}

void Engine::MousePressEvent(const MOUSE_BUTTON& button, int x, int y) {}

void Engine::MouseReleaseEvent(const MOUSE_BUTTON& button, int x, int y) {}

bool Engine::LoadTexture(Texture& texture, const std::string& filename, bool stopOnError) {
	texture.Load(filename);
	if (!texture.IsValid()) {
		std::cerr << "Unable to load texture (" << filename << ")" << std::endl;
		if (stopOnError)
			Stop();

		return false;
	}

	return true;
}

void Engine::DrawSkybox() {
	m_textureMonster.Bind();
	glBegin(GL_QUADS); // FRONT 
		glNormal3f(0, 0, 1);
		glTexCoord2f(0, 0); glVertex3f(-VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2);
		glTexCoord2f(1, 0); glVertex3f(VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2);
		glTexCoord2f(1, 1); glVertex3f(VIEW_DISTANCE * 2, VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2);
		glTexCoord2f(0, 1); glVertex3f(-VIEW_DISTANCE * 2, VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2);
	glEnd();

	m_textureDark.Bind();
		glBegin(GL_QUADS); // RIGHT
		glTexCoord2f(0, 0); glVertex3f(VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2);
		glTexCoord2f(1, 0); glVertex3f(VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2, VIEW_DISTANCE * 2);
		glTexCoord2f(1, 1); glVertex3f(VIEW_DISTANCE * 2, VIEW_DISTANCE * 2, VIEW_DISTANCE * 2);
		glTexCoord2f(0, 1); glVertex3f(VIEW_DISTANCE * 2, VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2);
	glEnd();

	glBegin(GL_QUADS); // BACK
		glNormal3f(0, 0, 1);
		glTexCoord2f(0, 0); glVertex3f(VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2, VIEW_DISTANCE * 2);
		glTexCoord2f(0, 1); glVertex3f(-VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2, VIEW_DISTANCE * 2);
		glTexCoord2f(1, 1); glVertex3f(-VIEW_DISTANCE * 2, VIEW_DISTANCE * 2, VIEW_DISTANCE * 2);
		glTexCoord2f(1, 0); glVertex3f(VIEW_DISTANCE * 2, VIEW_DISTANCE * 2, VIEW_DISTANCE * 2);
	glEnd();

	glBegin(GL_QUADS); // LEFT
		glTexCoord2f(0, 0); glVertex3f(-VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2, VIEW_DISTANCE * 2);
		glTexCoord2f(1, 0); glVertex3f(-VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2);
		glTexCoord2f(1, 1); glVertex3f(-VIEW_DISTANCE * 2, VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2);
		glTexCoord2f(0, 1); glVertex3f(-VIEW_DISTANCE * 2, VIEW_DISTANCE * 2, VIEW_DISTANCE * 2);
	glEnd();

	glBegin(GL_QUADS); // TOP
		glTexCoord2f(0, 0); glVertex3f(-VIEW_DISTANCE * 2, VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2);
		glTexCoord2f(0, 1); glVertex3f(-VIEW_DISTANCE * 2, VIEW_DISTANCE * 2, VIEW_DISTANCE * 2);
		glTexCoord2f(1, 1); glVertex3f(VIEW_DISTANCE * 2, VIEW_DISTANCE * 2, VIEW_DISTANCE * 2);
		glTexCoord2f(1, 0); glVertex3f(VIEW_DISTANCE * 2, VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2);
	glEnd();

	glBegin(GL_QUADS); // BOTTOM
		glTexCoord2f(0, 0); glVertex3f(-VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2, VIEW_DISTANCE * 2);
		glTexCoord2f(1, 0); glVertex3f(-VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2);
		glTexCoord2f(1, 1); glVertex3f(VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2);
		glTexCoord2f(0, 1); glVertex3f(VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2, VIEW_DISTANCE * 2);
	glEnd();
}

void Engine::DrawFloor() {
	//m_textureFloor.Bind();
	//float nbRep = 25.0f;
	//glBegin(GL_QUADS);
	//	glNormal3f(0, 1, 0);
	//	glTexCoord2f(0, 0);			glVertex3f(-100.f, -2.0f, 100.f);
	//	glTexCoord2f(nbRep, 0);		glVertex3f(100.f, -2.0f, 100.f);
	//	glTexCoord2f(nbRep, nbRep); glVertex3f(100.f, -2.0f, -100.f);
	//	glTexCoord2f(0, nbRep);		glVertex3f(-100.f, -2.0f, -100.f);
	//glEnd();
}

void Engine::DrawBlock() {
	//m_textureFaceZ.Bind();
	//glBegin(GL_QUADS);			// FRONT
	//	glNormal3f(0, 0, 1);	// Normal Z+
	//	glTexCoord2f(0, 0);	glVertex3f(-0.5, -0.5, 0.5);
	//	glTexCoord2f(1, 0);	glVertex3f(0.5, -0.5, 0.5);
	//	glTexCoord2f(1, 1);	glVertex3f(0.5, 0.5, 0.5);
	//	glTexCoord2f(0, 1);	glVertex3f(-0.5, 0.5, 0.5);
	//glEnd();

	//glBegin(GL_QUADS);			// BACK
	//	glNormal3f(0, 0, -1);	// Normal Z-
	//	glTexCoord2f(0, 0); glVertex3f(0.5, -0.5, -0.5);
	//	glTexCoord2f(1, 0); glVertex3f(-0.5, -0.5, -0.5);
	//	glTexCoord2f(1, 1); glVertex3f(-0.5, 0.5, -0.5);
	//	glTexCoord2f(0, 1); glVertex3f(0.5, 0.5, -0.5);
	//glEnd();

	//m_textureFaceX.Bind();
	//glBegin(GL_QUADS);			// RIGHT
	//	glNormal3f(1, 0, 0);	// Normal X+
	//	glTexCoord2f(0, 0);	glVertex3f(0.5, -0.5, 0.5);
	//	glTexCoord2f(1, 0);	glVertex3f(0.5, -0.5, -0.5);
	//	glTexCoord2f(1, 1);	glVertex3f(0.5, 0.5, -0.5);
	//	glTexCoord2f(0, 1);	glVertex3f(0.5, 0.5, 0.5);
	//glEnd();

	//glBegin(GL_QUADS);			// LEFT
	//	glNormal3f(-1, 0, 0);	// Normal X-
	//	glTexCoord2f(0, 0);	glVertex3f(-0.5, -0.5, -0.5);
	//	glTexCoord2f(1, 0);	glVertex3f(-0.5, -0.5, 0.5);
	//	glTexCoord2f(1, 1);	glVertex3f(-0.5, 0.5, 0.5);
	//	glTexCoord2f(0, 1);	glVertex3f(-0.5, 0.5, -0.5);
	//glEnd();

	//m_textureFaceY.Bind();
	//glBegin(GL_QUADS);			// TOP	
	//	glNormal3f(0, 1, 0);	// Normal Y+
	//	glTexCoord2f(0, 0);	glVertex3f(-0.5, 0.5, 0.5);
	//	glTexCoord2f(1, 0);	glVertex3f(0.5, 0.5, 0.5);
	//	glTexCoord2f(1, 1);	glVertex3f(0.5, 0.5, -0.5);
	//	glTexCoord2f(0, 1);	glVertex3f(-0.5, 0.5, -0.5);
	//glEnd();

	//glBegin(GL_QUADS);			// BOTTOM
	//	glNormal3f(0, -1, 0);	// Normal Y-
	//	glTexCoord2f(0, 0);	glVertex3f(-0.5, -0.5, -0.5);
	//	glTexCoord2f(1, 0);	glVertex3f(0.5, -0.5, -0.5);
	//	glTexCoord2f(1, 1);	glVertex3f(0.5, -0.5, 0.5);
	//	glTexCoord2f(0, 1);	glVertex3f(-0.5, -0.5, 0.5);
	//glEnd();
}
