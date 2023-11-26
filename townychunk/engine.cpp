#include "engine.h"

Engine::Engine() :
	m_world(nullptr),
	m_player(Vector3f(SPAWN_X, CHUNK_SIZE_Y, SPAWN_Z)),
	m_currentBlock(Vector3f(0.0f, 0.0f, 0.0f)),
	m_monsterFace(1),
	m_monsterAlpha(0.0f),
	m_monsterFadeTime(2.5f),
	m_monsterVisibleTime(5.0f),
	m_monsterInvisibleTime(0.0f),
	m_monsterEyesAlpha(0.0f),
	m_monsterEyesFadeTime(2.5f),
	m_monsterEyesVisibleTime(5.0f) {}

Engine::~Engine() {
	delete m_world;
}

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

	m_world = new World();
	srand(static_cast<unsigned int>(time(0)));

	CenterMouse();
	HideCursor();
}

void Engine::DeInit() {}

void Engine::LoadResource() {
	LoadTexture(m_textureArm, TEXTURE_PATH "arm.png");
	LoadTexture(m_textureMonster, TEXTURE_PATH "monster.png");
	LoadTexture(m_textureMonsterEyes, TEXTURE_PATH "monster_eyes.png");
	LoadTexture(m_textureDark, TEXTURE_PATH "darkness.jpg");
	LoadTexture(m_textureFont, TEXTURE_PATH "font.bmp");
	LoadTexture(m_textureCrosshair, TEXTURE_PATH "cross.bmp");

	TextureAtlas::TextureIndex texIdxEmbossedBrown = m_textureAtlas.AddTexture(TEXTURE_PATH "embossed_brown.png");
	TextureAtlas::TextureIndex texIdxEmbossedWhite = m_textureAtlas.AddTexture(TEXTURE_PATH "embossed_white.png");
	TextureAtlas::TextureIndex texIdxCircle = m_textureAtlas.AddTexture(TEXTURE_PATH "circle.png");
	TextureAtlas::TextureIndex texIdxDecoration = m_textureAtlas.AddTexture(TEXTURE_PATH "decoration.png");
	TextureAtlas::TextureIndex texIdxDirt = m_textureAtlas.AddTexture(TEXTURE_PATH "dirt.png");
	TextureAtlas::TextureIndex texIdxMosaic = m_textureAtlas.AddTexture(TEXTURE_PATH "mosaic.png");
	TextureAtlas::TextureIndex texIdxHellX = m_textureAtlas.AddTexture(TEXTURE_PATH "hell_x.png");
	TextureAtlas::TextureIndex texIdxHellY = m_textureAtlas.AddTexture(TEXTURE_PATH "hell_y.png");
	TextureAtlas::TextureIndex texIdxHellZ = m_textureAtlas.AddTexture(TEXTURE_PATH "hell_z.png");
	TextureAtlas::TextureIndex texIdxBlack = m_textureAtlas.AddTexture(TEXTURE_PATH "black.png");
	TextureAtlas::TextureIndex texIdxBrown = m_textureAtlas.AddTexture(TEXTURE_PATH "brown.png");
	TextureAtlas::TextureIndex texIdxDarkBlue = m_textureAtlas.AddTexture(TEXTURE_PATH "dark_blue.png");
	TextureAtlas::TextureIndex texIdxDarkGreen = m_textureAtlas.AddTexture(TEXTURE_PATH "dark_green.png");
	TextureAtlas::TextureIndex texIdxDarkGrey = m_textureAtlas.AddTexture(TEXTURE_PATH "dark_grey.png");
	TextureAtlas::TextureIndex texIdxLightBlue = m_textureAtlas.AddTexture(TEXTURE_PATH "light_blue.png");
	TextureAtlas::TextureIndex texIdxLightGreen = m_textureAtlas.AddTexture(TEXTURE_PATH "light_green.png");
	TextureAtlas::TextureIndex texIdxLightGrey = m_textureAtlas.AddTexture(TEXTURE_PATH "light_grey.png");
	TextureAtlas::TextureIndex texIdxMagenta = m_textureAtlas.AddTexture(TEXTURE_PATH "magenta.png");
	TextureAtlas::TextureIndex texIdxOrange = m_textureAtlas.AddTexture(TEXTURE_PATH "orange.png");
	TextureAtlas::TextureIndex texIdxPink = m_textureAtlas.AddTexture(TEXTURE_PATH "pink.png");
	TextureAtlas::TextureIndex texIdxPurple = m_textureAtlas.AddTexture(TEXTURE_PATH "purple.png");
	TextureAtlas::TextureIndex texIdxRed = m_textureAtlas.AddTexture(TEXTURE_PATH "red.png");
	TextureAtlas::TextureIndex texIdxWhite = m_textureAtlas.AddTexture(TEXTURE_PATH "white.png");
	TextureAtlas::TextureIndex texIdxYellow = m_textureAtlas.AddTexture(TEXTURE_PATH "yellow.png");

	if (!m_textureAtlas.Generate(256, false)) {
		std::cerr << "Unable to generate texture atlas..." << std::endl;
		abort();
	}

	std::map<BlockType, TextureAtlas::TextureIndex> btIndices;
	btIndices[BTYPE_EMBOSSED_BROWN] = texIdxEmbossedBrown;
	btIndices[BTYPE_EMBOSSED_WHITE] = texIdxEmbossedWhite;
	btIndices[BTYPE_CIRCLE] = texIdxCircle;
	btIndices[BTYPE_DECORATION] = texIdxDecoration;
	btIndices[BTYPE_DIRT] = texIdxDirt;
	btIndices[BTYPE_MOSAIC] = texIdxMosaic;
	btIndices[BTYPE_BLACK] = texIdxBlack;
	btIndices[BTYPE_BROWN] = texIdxBrown;
	btIndices[BTYPE_DARKBLUE] = texIdxDarkBlue;
	btIndices[BTYPE_DARKGREEN] = texIdxDarkGreen;
	btIndices[BTYPE_DARKGREY] = texIdxDarkGrey;
	btIndices[BTYPE_LIGHTBLUE] = texIdxLightBlue;
	btIndices[BTYPE_LIGHTGREEN] = texIdxLightGreen;
	btIndices[BTYPE_LIGHTGREY] = texIdxLightGrey;
	btIndices[BTYPE_MAGENTA] = texIdxMagenta;
	btIndices[BTYPE_ORANGE] = texIdxOrange;
	btIndices[BTYPE_PINK] = texIdxPink;
	btIndices[BTYPE_PURPLE] = texIdxPurple;
	btIndices[BTYPE_RED] = texIdxRed;
	btIndices[BTYPE_WHITE] = texIdxWhite;
	btIndices[BTYPE_YELLOW] = texIdxYellow;

	// Single texture for all faces
	for (auto& pair : btIndices) {
		float u, v, w, h;
		m_textureAtlas.TextureIndexToCoord(pair.second, u, v, w, h);

		for (int face = FRONT; face < LAST; ++face) {
			BlockInfo::SetBlockTextureCoords(pair.first, static_cast<BlockFace>(face), u, v, w, h);
		}
	}

	// Different texture for each face
	float u, v, w, h;
	m_textureAtlas.TextureIndexToCoord(texIdxHellZ, u, v, w, h);
	BlockInfo::SetBlockTextureCoords(BTYPE_HELL, FRONT, u, v, w, h); 
	m_textureAtlas.TextureIndexToCoord(texIdxHellZ, u, v, w, h);
	BlockInfo::SetBlockTextureCoords(BTYPE_HELL, BACK, u, v, w, h);  
	m_textureAtlas.TextureIndexToCoord(texIdxHellX, u, v, w, h);
	BlockInfo::SetBlockTextureCoords(BTYPE_HELL, LEFT, u, v, w, h);  
	m_textureAtlas.TextureIndexToCoord(texIdxHellX, u, v, w, h);
	BlockInfo::SetBlockTextureCoords(BTYPE_HELL, RIGHT, u, v, w, h);
	m_textureAtlas.TextureIndexToCoord(texIdxHellY, u, v, w, h);
	BlockInfo::SetBlockTextureCoords(BTYPE_HELL, TOP, u, v, w, h);
	m_textureAtlas.TextureIndexToCoord(texIdxHellY, u, v, w, h);
	BlockInfo::SetBlockTextureCoords(BTYPE_HELL, BOTTOM, u, v, w, h);

	m_music.setVolume(50.0f);
	if (!m_music.openFromFile("../townychunk/media/audio/music.ogg")) {
		std::cerr << "Unable to load music" << std::endl;
		abort();
	}

	m_music.play();
	m_music.setLoop(true);
}

void Engine::UnloadResource() {}

void Engine::Render(float elapsedTime) {
	if (elapsedTime > 1.0f / 60.0f) {
		elapsedTime = 1.0f / 60.0f;
	}

	static float gameTime = elapsedTime; // Valeur conserv�e entre les appels car static
	gameTime += elapsedTime;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Efface le tampon de couleur et de profondeur

	// Transformations initiales
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Camera (Player)
	Vector3f pos = m_player.GetPosition();
	Vector3f delta = m_player.Move(m_keyW, m_keyS, m_keyA, m_keyD, m_keySpace, elapsedTime);
	m_world->CheckCollisions(m_player, delta, m_keyW, m_keyS, m_keyA, m_keyD, m_keySpace, elapsedTime);
	
	Transformation t;
	std::array<float, 2> rot = m_player.GetRotation();
	m_player.ApplyTransformation(t);
	t.ApplyTranslation(0.5f, 0.5f, 0.5f);
	t.Use();

	// Chunk
	m_textureAtlas.Bind();
	m_shader01.Use();
	for (int i = 0; i < WORLD_SIZE_X; i++) {
		for (int j = 0; j < WORLD_SIZE_Z; j++) {
			Chunk* chunk = m_world->GetChunk(i, j);
			bool dirty = chunk->IsDirty();
			if (chunk->IsDirty()) {
				chunk->Update();
			}
			chunk->Render();
		}
	}
	Shader::Disable();

	// Skybox
	t.Push();
	t.ApplyTranslation(m_player.GetPosition().x, m_player.GetPosition().y, m_player.GetPosition().z);
	t.Use();

	AddBlendFunction(false);

	UpdateMonsterFace(elapsedTime);
	DrawSkybox();

	RemoveBlendFunction(false);

	if (m_wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	AddBlendFunction(true);

	if (m_keyR) {
		DrawArm();
	}

	if (m_keyI) {
		DrawHud(elapsedTime);
	}

	if (m_keyC) {
		DrawCrosshair();
	}

	RemoveBlendFunction(true);

	if (m_keyR) {
		DrawBlock(elapsedTime);
	}

	t.Pop();
	t.Use();

	if (m_wireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	GetBlockAtCursor();
}

void Engine::AddBlendFunction(bool isOrtho) {
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);

	if (!isOrtho) {
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else {
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glDisable(GL_DEPTH_TEST);
	}

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	if (!isOrtho) {
		gluPerspective(45.0f, (float)Width() / (float)Height(), 0.0001f, 1000.0f);
	}
	else {
		glOrtho(0, Width(), 0, Height(), -1, 1);
	}

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
}

void Engine::RemoveBlendFunction(bool isOrtho) {
	glEnable(GL_LIGHTING);
	glDisable(GL_BLEND);

	if (isOrtho) {
		glEnable(GL_DEPTH_TEST);
	}

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void Engine::DrawSkybox() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (int face = 1; face <= 4; ++face) {
		if (face == m_monsterFace) {
			glDepthMask(GL_FALSE);
			m_textureMonster.Bind();
			glColor4f(1.0, 1.0, 1.0, m_monsterAlpha);
			DrawFaceWithMonster(face);

			m_textureMonsterEyes.Bind();
			glColor4f(1.0, 1.0, 1.0, m_monsterEyesAlpha);
			DrawFaceWithMonster(face);
			glDepthMask(GL_TRUE);
		}
		else {
			m_textureDark.Bind();
		}
	}

	m_textureDark.Bind();
	glBegin(GL_QUADS); // TOP
	glTexCoord2f(0, 0); glVertex3f(-VIEW_DISTANCE * 2, VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2);
	glTexCoord2f(1, 0); glVertex3f(VIEW_DISTANCE * 2, VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2);
	glTexCoord2f(1, 1); glVertex3f(VIEW_DISTANCE * 2, VIEW_DISTANCE * 2, VIEW_DISTANCE * 2);
	glTexCoord2f(0, 1); glVertex3f(-VIEW_DISTANCE * 2, VIEW_DISTANCE * 2, VIEW_DISTANCE * 2);
	glEnd();

	glBegin(GL_QUADS); // BOTTOM
	glTexCoord2f(0, 0); glVertex3f(-VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2, VIEW_DISTANCE * 2);
	glTexCoord2f(1, 0); glVertex3f(-VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2);
	glTexCoord2f(1, 1); glVertex3f(VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2);
	glTexCoord2f(0, 1); glVertex3f(VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2, VIEW_DISTANCE * 2);
	glEnd();

	glDisable(GL_BLEND);
}

void Engine::DrawFaceWithMonster(int face) {
	glBegin(GL_QUADS);
	switch (face) {
	case 1: // FRONT
		glNormal3f(0, 0, 1);
		glTexCoord2f(0, 0); glVertex3f(-VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2);
		glTexCoord2f(1, 0); glVertex3f(VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2);
		glTexCoord2f(1, 1); glVertex3f(VIEW_DISTANCE * 2, VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2);
		glTexCoord2f(0, 1); glVertex3f(-VIEW_DISTANCE * 2, VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2);
		break;
	case 2: // RIGHT
		glNormal3f(-1, 0, 0);
		glTexCoord2f(0, 0); glVertex3f(VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2);
		glTexCoord2f(1, 0); glVertex3f(VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2, VIEW_DISTANCE * 2);
		glTexCoord2f(1, 1); glVertex3f(VIEW_DISTANCE * 2, VIEW_DISTANCE * 2, VIEW_DISTANCE * 2);
		glTexCoord2f(0, 1); glVertex3f(VIEW_DISTANCE * 2, VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2);
		break;
	case 3: // BACK
		glNormal3f(0, 0, -1);
		glTexCoord2f(0, 0); glVertex3f(VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2, VIEW_DISTANCE * 2);
		glTexCoord2f(1, 0); glVertex3f(-VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2, VIEW_DISTANCE * 2);
		glTexCoord2f(1, 1); glVertex3f(-VIEW_DISTANCE * 2, VIEW_DISTANCE * 2, VIEW_DISTANCE * 2);
		glTexCoord2f(0, 1); glVertex3f(VIEW_DISTANCE * 2, VIEW_DISTANCE * 2, VIEW_DISTANCE * 2);
		break;
	case 4: // LEFT
		glNormal3f(1, 0, 0);
		glTexCoord2f(0, 0); glVertex3f(-VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2, VIEW_DISTANCE * 2);
		glTexCoord2f(1, 0); glVertex3f(-VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2);
		glTexCoord2f(1, 1); glVertex3f(-VIEW_DISTANCE * 2, VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2);
		glTexCoord2f(0, 1); glVertex3f(-VIEW_DISTANCE * 2, VIEW_DISTANCE * 2, VIEW_DISTANCE * 2);
		break;
	}
	glEnd();
}

void Engine::UpdateMonsterFace(float elapsedTime) {
	if (m_monsterFadeIn) {
		m_monsterAlpha += elapsedTime / m_monsterFadeTime;

		if (m_monsterAlpha >= 1.0f) {
			m_monsterAlpha = 1.0f;
			m_monsterFadeIn = false;
			m_monsterEyesFadeIn = true;
			m_monsterVisibleTime = 10.0f;
		}
	}
	else if (m_monsterVisibleTime > 0.0f) {
		m_monsterVisibleTime -= elapsedTime;

		if (m_monsterEyesFadeIn) {
			m_monsterEyesAlpha += elapsedTime / m_monsterEyesFadeTime;

			if (m_monsterEyesAlpha >= 1.0f) {
				m_monsterEyesAlpha = 1.0f;
				m_monsterEyesFadeIn = false;
				m_monsterEyesVisibleTime = 5.0f;
			}
		}
		else if (m_monsterEyesVisibleTime > 0.0f) {
			m_monsterEyesVisibleTime -= elapsedTime;

			if (m_monsterEyesVisibleTime <= 0.0f) {
				m_monsterEyesFadeOut = true;
			}
		}
		else if (m_monsterEyesFadeOut) {
			m_monsterEyesAlpha -= elapsedTime / m_monsterEyesFadeTime;

			if (m_monsterEyesAlpha <= 0.0f) {
				m_monsterEyesAlpha = 0.0f;
				m_monsterEyesFadeOut = false;
			}
		}

		if (m_monsterVisibleTime <= 0.0f) {
			m_monsterFadeOut = true;
		}
	}
	else if (m_monsterFadeOut) {
		m_monsterAlpha -= elapsedTime / m_monsterFadeTime;

		if (m_monsterAlpha <= 0.0f) {
			m_monsterAlpha = 0.0f;
			m_monsterFadeOut = false;
			m_monsterInvisibleTime = 5 + rand() % 16;
		}
	}
	else if (m_monsterInvisibleTime > 0.0f) {
		m_monsterInvisibleTime -= elapsedTime;
		if (m_monsterInvisibleTime <= 0.0f) {
			int newFace;
			do {
				newFace = rand() % 4 + 1;
			} while (newFace == m_monsterFace);
			m_monsterFace = newFace;
			m_monsterFadeIn = true;
		}
	}
}


void Engine::DrawArm() {
	float armWidth = Width() * 0.33f;
	float armHeight = Height() * 0.5f;
	float armPosX = Width() * 0.95f;
	float armPosY = 0.0f;

	glLoadIdentity();
	glTranslated(armPosX * RESIZE_RATIO, armPosY, 0);

	m_textureArm.Bind();
	GLint originalBlendSrc, originalBlendDst;
	glGetIntegerv(GL_BLEND_SRC_ALPHA, &originalBlendSrc);
	glGetIntegerv(GL_BLEND_DST_ALPHA, &originalBlendDst);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBegin(GL_QUADS);
	glTexCoord2f(1, 0); glVertex2i(0, 0);
	glTexCoord2f(0, 0); glVertex2i(armWidth, 0);
	glTexCoord2f(0, 1); glVertex2i(armWidth, armHeight);
	glTexCoord2f(1, 1); glVertex2i(0, armHeight);
	glEnd();

	glBlendFunc(originalBlendSrc, originalBlendDst);
}

void Engine::DrawBlock(float elapsedTime) {
	Transformation t;
	t.Push();
	static float angleX = 0.0f;
	static float angleY = 0.0f;
	angleX += (elapsedTime * 100);
	angleY += (elapsedTime * 100);
	t.ApplyTranslation(1.135f, -0.25, -5.5f);
	t.ApplyRotation(angleX, 1.0f, 0.0f, 0.0f);
	t.ApplyRotation(angleY, 0.0f, 1.0f, 0.0f);
	t.Use();

	m_textureAtlas.Bind();

	glDepthRange(0.0, 0.1);
	GLfloat cubeLightPos[4] = { 0.0f, 0.0f, 0.0f, 10.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, cubeLightPos);

	BlockType equippedItem = m_player.GetEquippedItem();

	float u, v, w, h;
	BlockInfo::GetBlockTextureCoords(equippedItem, FRONT, u, v, w, h);
	glBegin(GL_QUADS);		// FRONT
	glNormal3f(0, 0, -1);	// Normal Z-
	glTexCoord2f(u, v);	glVertex3f(-0.5, -0.5, 0.5);
	glTexCoord2f(u + w, v);	glVertex3f(0.5, -0.5, 0.5);
	glTexCoord2f(u + w, v + h);	glVertex3f(0.5, 0.5, 0.5);
	glTexCoord2f(u, v + h);	glVertex3f(-0.5, 0.5, 0.5);
	glEnd();

	BlockInfo::GetBlockTextureCoords(equippedItem, BACK, u, v, w, h);
	glBegin(GL_QUADS);		// BACK
	glNormal3f(0, 0, 1);	// Normal Z+
	glTexCoord2f(u, v); glVertex3f(0.5, -0.5, -0.5);
	glTexCoord2f(u + w, v); glVertex3f(-0.5, -0.5, -0.5);
	glTexCoord2f(u + w, v + h); glVertex3f(-0.5, 0.5, -0.5);
	glTexCoord2f(u, v + h); glVertex3f(0.5, 0.5, -0.5);
	glEnd();

	BlockInfo::GetBlockTextureCoords(equippedItem, TOP, u, v, w, h);
	glBegin(GL_QUADS);		// TOP	
	glNormal3f(0, 1, 0);	// Normal Y+
	glTexCoord2f(u, v);	glVertex3f(-0.5, -0.5, -0.5);
	glTexCoord2f(u + w, v);	glVertex3f(0.5, -0.5, -0.5);
	glTexCoord2f(u + w, v + h);	glVertex3f(0.5, -0.5, 0.5);
	glTexCoord2f(u, v + h);	glVertex3f(-0.5, -0.5, 0.5);
	glEnd();

	BlockInfo::GetBlockTextureCoords(equippedItem, BOTTOM, u, v, w, h);
	glBegin(GL_QUADS);		// BOTTOM
	glNormal3f(0, -1, 0);	// Normal Y-
	glTexCoord2f(u, v);	glVertex3f(-0.5, 0.5, -0.5);
	glTexCoord2f(u + w, v);	glVertex3f(-0.5, 0.5, 0.5);
	glTexCoord2f(u + w, v + h);	glVertex3f(0.5, 0.5, 0.5);
	glTexCoord2f(u, v + h);	glVertex3f(0.5, 0.5, -0.5);
	glEnd();

	BlockInfo::GetBlockTextureCoords(equippedItem, LEFT, u, v, w, h);
	glBegin(GL_QUADS);		// LEFT
	glNormal3f(1, 0, 0);	// Normal X-
	glTexCoord2f(u, v);	glVertex3f(-0.5, -0.5, -0.5);
	glTexCoord2f(u + w, v);	glVertex3f(-0.5, -0.5, 0.5);
	glTexCoord2f(u + w, v + h);	glVertex3f(-0.5, 0.5, 0.5);
	glTexCoord2f(u, v + h);	glVertex3f(-0.5, 0.5, -0.5);
	glEnd();

	BlockInfo::GetBlockTextureCoords(equippedItem, RIGHT, u, v, w, h);
	glBegin(GL_QUADS);		// RIGHT
	glNormal3f(-1, 0, 0);	// Normal X+
	glTexCoord2f(u, v);	glVertex3f(0.5, -0.5, 0.5);
	glTexCoord2f(u + w, v);	glVertex3f(0.5, -0.5, -0.5);
	glTexCoord2f(u + w, v + h);	glVertex3f(0.5, 0.5, -0.5);
	glTexCoord2f(u, v + h);	glVertex3f(0.5, 0.5, 0.5);
	glEnd();

	glDepthRange(0.0, 1.0);

	t.Pop();
	t.Use();
}

void Engine::DrawHud(float elapsedTime) {
	m_textureFont.Bind();
	std::ostringstream ss;

	ss << " CURSOR : ( X " << m_currentBlock.x << " | Y " << m_currentBlock.y << " | Z " << m_currentBlock.z << " )";
	PrintText(10, Height() - 30, ss.str());
	ss.str("");
	Vector3f currentDirection = m_player.GetDirection();
	ss << " DIRECTION : " << DirectionToString(currentDirection);
	PrintText(10, Height() - 60, ss.str());
	ss.str("");
	ss << " FPS : " << GetFps(elapsedTime);
	PrintText(10, Height() - 90, ss.str());
	ss.str("");

	Vector3f pos = m_player.GetPosition();
	ss << (pos.x > 0 ? " CHUNK: ( X " : " CHUNK: ( X-") << (int)(pos.x / CHUNK_SIZE_X) <<
		(pos.z > 0 ? " | Z " : " | Z-") << (int)(pos.z / CHUNK_SIZE_Z) << " )";
	PrintText(10, 80, ss.str());
	ss.str("");

	ss << (pos.x > 0 ? " BLOCK: ( X " : " BLOCK: ( X-") <<
		abs((int)(pos.x) % CHUNK_SIZE_X) << (pos.y > 0 ? " | Y " : " | Y-") <<
		abs((int)(pos.y) % CHUNK_SIZE_Y) << (pos.z > 0 ? " | Z " : " | Z-") <<
		abs((int)(pos.z) % CHUNK_SIZE_Z) << " )";
	PrintText(10, 50, ss.str());
	ss.str("");

	ss << (pos.x > 0 ? " GLOBAL: ( X " : " GLOBAL: ( X-") << std::fixed << std::setprecision(2) <<
		abs(pos.x) << (pos.y > 0 ? " | Y " : " | Y-") <<
		abs(pos.y) << (pos.z > 0 ? " | Z " : " | Z-") <<
		abs(pos.z) << " )";
	PrintText(10, 20, ss.str());
	ss.str("");
}

void Engine::DrawCrosshair() {
	if (m_keyC) {
		m_textureCrosshair.Bind();
		static const int crossSize = 32;

		glLoadIdentity();
		glTranslated(Width() / 2 - crossSize / 2, Height() / 2 - crossSize / 2, 0);

		glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex2i(0, 0);
		glTexCoord2f(1, 0); glVertex2i(crossSize, 0);
		glTexCoord2f(1, 1); glVertex2i(crossSize, crossSize);
		glTexCoord2f(0, 1); glVertex2i(0, crossSize);
		glEnd();
	}
}

void Engine::PrintText(unsigned int x, unsigned int y, const std::string& t) {
	glLoadIdentity();
	glTranslated(x, y, 0);

	const float atlasWidth = 400.0f;
	const float atlasHeight = 400.0f;
	const int numCols = 16;
	const int numRows = 16;
	const float charWidth = atlasWidth / numCols;
	const float charHeight = atlasHeight / numRows;

	for (unsigned int i = 0; i < t.length(); ++i) {
		char c = t[i];
		int col = (c - 32) % numCols;
		int row = (c - 32) / numCols;

		float left = col * charWidth / atlasWidth;
		float right = left + charWidth / atlasWidth;
		float top = 1.0f - (row * charHeight) / atlasHeight;
		float bottom = top - charHeight / atlasHeight;

		glBegin(GL_QUADS);
		glTexCoord2f(left, bottom);  glVertex2f(0, 0);
		glTexCoord2f(right, bottom); glVertex2f(charWidth - 5, 0);
		glTexCoord2f(right, top);	 glVertex2f(charWidth - 5, charHeight - 5);
		glTexCoord2f(left, top);	 glVertex2f(0, charHeight - 5);
		glEnd();

		glTranslated(charWidth * RESIZE_RATIO, 0, 0);
	}
}

void Engine::KeyPressEvent(unsigned char key) {
	switch (key) {
	case 0: // ( LEFT ) A
		m_keyA = true;
		break;
	case 2: // ( CROSSHAIR ) C
		m_keyC = !m_keyC;
		break;
	case 3: // ( BACK ) D
		m_keyD = true;
		break;
	case 8: // ( INFO ) I
		m_keyI = !m_keyI;
		break;
	case 17: // ( ARM ) R
		m_keyR = !m_keyR;
		break;
	case 18: // ( RIGHT ) S
		m_keyS = true;
		break;
	case 22: // ( FRONT ) W
		m_keyW = true;
		break;
	case 36: // ESC
		Stop();
		break;
	case 57: // ( JUMP ) SPACE
		m_keySpace = true;
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
	case 0: // ( LEFT ) A
		m_keyA = false;
		break;
	case 3: // ( BACK ) D
		m_keyD = false;
		break;
	case 18: // ( RIGHT ) S
		m_keyS = false;
		break;
	case 22: // ( FRONT ) W
		m_keyW = false;
		break;
	case 24: // Y
		m_wireframe = !m_wireframe;
		if (m_wireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case 57: // ( JUMP ) SPACE
		m_keySpace = false;
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

void Engine::MousePressEvent(const MOUSE_BUTTON& button, int x, int y) {
	switch (button) {
	case MOUSE_BUTTON_WHEEL_UP:
		if (m_player.GetEquippedItem() < BTYPE_LAST - 1) {
			m_player.SetEquippedItem(static_cast<BlockType>(m_player.GetEquippedItem() + 1));
		}
		return; 
	case MOUSE_BUTTON_WHEEL_DOWN:
		if (m_player.GetEquippedItem() > BTYPE_AIR + 1) {
			m_player.SetEquippedItem(static_cast<BlockType>(m_player.GetEquippedItem() - 1));
		}
		return;
	}

	if (m_currentBlock.x < 0) {
		std::cout << "X was < 0" << std::endl;
		return;
	}

	Chunk* currentChunk = m_world->ChunkAt(static_cast<int>(m_currentBlock.x), static_cast<int>(m_currentBlock.y), static_cast<int>(m_currentBlock.z));

	int targetX = (static_cast<int>(m_currentBlock.x) % CHUNK_SIZE_X) + m_currentFaceNormal.x;
	int targetY = (static_cast<int>(m_currentBlock.y) % CHUNK_SIZE_Y) + m_currentFaceNormal.y;
	int targetZ = (static_cast<int>(m_currentBlock.z) % CHUNK_SIZE_Z) + m_currentFaceNormal.z;

	BlockType addBt = currentChunk->GetBlock(targetX, targetY, targetZ);
	BlockType removeBt = currentChunk->GetBlock(static_cast<int>(m_currentBlock.x) % CHUNK_SIZE_X,
												static_cast<int>(m_currentBlock.y) % CHUNK_SIZE_Y, 
												static_cast<int>(m_currentBlock.z) % CHUNK_SIZE_Z);

	int playerBlockX = static_cast<int>(m_player.GetPosition().x) % CHUNK_SIZE_X;
	int playerBlockZ = static_cast<int>(m_player.GetPosition().z) % CHUNK_SIZE_Z;

	BlockType equippedItem = m_player.GetEquippedItem();

	switch (button) {
		case MOUSE_BUTTON_LEFT:
			if (removeBt != BTYPE_AIR) {
				currentChunk->RemoveBlock(static_cast<int>(m_currentBlock.x) % CHUNK_SIZE_X,
										  static_cast<int>(m_currentBlock.y) % CHUNK_SIZE_Y,
										  static_cast<int>(m_currentBlock.z) % CHUNK_SIZE_Z);

				m_world->SetDirtyChunk(currentChunk, m_currentBlock.x, m_currentBlock.y, m_currentBlock.z);
			}

			std::cout << "Tried removing block at (" << m_currentBlock.x << ", " << m_currentBlock.y << ", " << m_currentBlock.z << ")" << std::endl;
			std::cout << "Block at removing position was : " << static_cast<int>(removeBt) << std::endl;
			break;
		case MOUSE_BUTTON_RIGHT:
			if (addBt != BTYPE_AIR || (targetX == playerBlockX && targetZ == playerBlockZ)) {
				std::cout << "Target block and Player block are on the same X or Z axis" << std::endl;
				std::cout << "Target block positions were : (" << targetX << ", " << targetY << ", " << targetZ << ")" << std::endl;
				std::cout << "Player position was : (" << m_player.GetPosition().x << ", " << m_player.GetPosition().y << ", " << m_player.GetPosition().z << ")" << std::endl;
				std::cout << "Player block positions were : (" << playerBlockX << ", " << playerBlockZ << ")" << std::endl;
				return;
			}
			currentChunk->SetBlock(targetX, targetY, targetZ, equippedItem);
			std::cout << "Tried adding block at (" << targetX << ", " << targetY << ", " << targetZ << ")" << std::endl;
			std::cout << "Block at adding position was : " << static_cast<int>(addBt) << std::endl;
			break;
		default:
			break;
	}
}

void Engine::MouseReleaseEvent(const MOUSE_BUTTON& button, int x, int y) {}

void Engine::GetBlockAtCursor() {
	bool grounded = m_player.GetIsOnGround();
	int x = Width() / 2;
	int y = Height() / 2;

	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	GLdouble posX, posY, posZ;

	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);

	winX = (float)x;
	winY = (float)viewport[3] - (float)y;
	glReadPixels(x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

	gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

	posX += .5f;
	posY += .5f;
	posZ += .5f;

	// Le cast vers int marche juste pour les valeurs entiere, utiliser une fonction de la libc si besoin
	// de valeurs negatives
	int px = (int)(posX);
	int py = (int)(posY);
	int pz = (int)(posZ);

	bool found = false;

	if ((m_player.GetPosition() - Vector3f((float)posX, (float)posY, (float)posZ)).Length() < MAX_SELECT_DISTANCE) {
		// Apres avoir determine la position du bloc en utilisant la partie entiere du hit
		// point retourne par opengl, on doit verifier de chaque cote du bloc trouve pour trouver
		// le vrai bloc. Le vrai bloc peut etre different a cause d'erreurs de precision de nos
		// nombres flottants (si z = 14.999 par exemple, et qu'il n'y a pas de blocs a la position
		// 14 (apres arrondi vers l'entier) on doit trouver et retourner le bloc en position 15 s'il existe
		// A cause des erreurs de precisions, ils arrive que le cote d'un bloc qui doit pourtant etre a la
		// position 15 par exemple nous retourne plutot la position 15.0001
		for (int x = px - 1; !found && x <= px + 1; ++x) {
			for (int y = py - 1; !found && x >= 0 && y <= py + 1; ++y) {
				for (int z = pz - 1; !found && y >= 0 && z <= pz + 1; ++z) {
					if (z >= 0) {
						BlockType bt = m_world->BlockAt((float)x, (float)y, (float)z, BTYPE_AIR);
						if (bt == BTYPE_AIR)
							continue;

						m_currentBlock.x = x;
						m_currentBlock.y = y;
						m_currentBlock.z = z;

						if (InRangeWithEpsilon((float)posX, (float)x, (float)x + 1.f, 0.05f) && InRangeWithEpsilon((float)posY, (float)y, (float)y + 1.f, 0.05f) && InRangeWithEpsilon((float)posZ, (float)z, (float)z + 1.f, 0.05f)) {
							found = true;
						}
					}
				}
			}
		}
	}

	if (!found) {
		m_currentBlock.x = -1;
	}
	else {
		// Find on which face of the bloc we got a hit
		m_currentFaceNormal.Zero();

		const float epsilon = 0.05f;

		// Front et back:
		if (EqualWithEpsilon((float)posZ, (float)m_currentBlock.z, epsilon))
			m_currentFaceNormal.z = -1;
		else if (EqualWithEpsilon((float)posZ, (float)m_currentBlock.z + 1.f, epsilon))
			m_currentFaceNormal.z = 1;
		else if (EqualWithEpsilon((float)posX, (float)m_currentBlock.x, epsilon))
			m_currentFaceNormal.x = -1;
		else if (EqualWithEpsilon((float)posX, (float)m_currentBlock.x + 1.f, epsilon))
			m_currentFaceNormal.x = 1;
		else if (EqualWithEpsilon((float)posY, (float)m_currentBlock.y, epsilon))
			m_currentFaceNormal.y = -1;
		else if (EqualWithEpsilon((float)posY, (float)m_currentBlock.y + 1.f, epsilon))
			m_currentFaceNormal.y = 1;
	}
}

bool Engine::EqualWithEpsilon(const float& val1, const float& val2, float epsilon) {
	bool result = (fabs(val2 - val1) < epsilon);

	//std::cout << "EqualWithEpsilon result : " << result <<  " | fabs(val2 - val1) = " << fabs(val2 - val1) << std::endl;
	return result;
}

bool Engine::InRangeWithEpsilon(const float& val, const float& minVal, const float& maxVal, float epsilon) {
	bool result = (val >= minVal - epsilon && val <= maxVal + epsilon);

	//std::cout << "InRangeWithEpsilon result : " << result << " | val = " << val << " | minVal = " << minVal << " | maxVal = " << maxVal << std::endl;
	return result;
}

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

std::string Engine::DirectionToString(const Vector3f& direction) const {
	std::string currentDirection;

	if (std::abs(direction.x) > std::abs(direction.y) && std::abs(direction.x) > std::abs(direction.z)) {
		currentDirection = (direction.x > 0) ? "X+" : "X-";
	}
	else if (std::abs(direction.y) > std::abs(direction.x) && std::abs(direction.y) > std::abs(direction.z)) {
		currentDirection = (direction.y < 0) ? "Y+" : "Y-";
	}
	else {
		currentDirection = (direction.z > 0) ? "Z+" : "Z-";
	}

	return currentDirection;
}

unsigned int Engine::GetFps(float elapsedTime) const {
	return 1 / elapsedTime;
}
