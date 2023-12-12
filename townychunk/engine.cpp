#include "engine.h"

Engine::Engine() :
	m_world(nullptr),
	m_gameState(START_SCREEN),
	m_player(Vector3f(SPAWN_X, CHUNK_SIZE_Y, SPAWN_Z)),
	m_currentBlock(Vector3f(0.0f, 0.0f, 0.0f)),
	m_monster(m_player),
	m_playButtonAnimationTime(0.0f){}

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
		std::cout << "Failed to load shader01 shader" << std::endl;
		exit(1);
	}

	if (!m_laserShader.Load(SHADER_PATH "laser.vert", SHADER_PATH "laser.frag", true)) {
		std::cout << "Failed to load laser shader" << std::endl;
		exit(1);
	}

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (float)Width() / (float)Height(), 0.001f, 1000.0f);
	glEnable(GL_DEPTH_TEST);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_CULL_FACE);

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
}

void Engine::DeInit() {}

void Engine::LoadResource() {
	LoadTexture(m_textureArm, TEXTURE_PATH "arm.png");
	LoadTexture(m_textureMonster, TEXTURE_PATH "monster.png");
	LoadTexture(m_textureMonsterEyes, TEXTURE_PATH "monster_eyes.png");
	LoadTexture(m_texturePlayButton, TEXTURE_PATH "play_button.png");
	LoadTexture(m_textureDark, TEXTURE_PATH "darkness.jpg");
	LoadTexture(m_textureFont, TEXTURE_PATH "font.bmp");
	LoadTexture(m_textureHealthBar, TEXTURE_PATH "health_bar.png");
	LoadTexture(m_textureCrosshair, TEXTURE_PATH "cross.bmp");
	LoadTexture(m_textureEndScreen, TEXTURE_PATH "end_screen.png");
	LoadTexture(m_textureStartScreen, TEXTURE_PATH "start_screen.png");

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
}

void Engine::UnloadResource() {}

void Engine::Render(float elapsedTime) {
	if (elapsedTime > 1.0f / 60.0f) {
		elapsedTime = 1.0f / 60.0f;
	}

	static float gameTime = elapsedTime;
	gameTime += elapsedTime;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Transformations initiales
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if (m_gameState == START_SCREEN) {
		AddBlendFunction(true);
		DrawStartScreen(elapsedTime);
		RemoveBlendFunction(true);
		return;
	}
	
	if (m_gameState == END_SCREEN) {
		AddBlendFunction(true);
		DrawEndScreen(elapsedTime);
		RemoveBlendFunction(true);
		return;
	}

	HideCursor();

	// Camera (Player)
	Vector3f pos = m_player.GetPosition();
	Vector3f delta = m_player.Move(m_keyW, m_keyS, m_keyA, m_keyD, m_keySpace, elapsedTime);

	m_world->CheckCollisions(m_player, delta, m_keyW, m_keyS, m_keyA, m_keyD, m_keySpace, elapsedTime);
	
	Transformation t;

	m_player.ApplyTransformation(t);
	t.ApplyTranslation(0.5f, 0.5f, 0.5f);
	t.Use();

	// Chunk
	m_textureAtlas.Bind();
	m_shader01.Use();
	for (int i = 0; i < WORLD_SIZE_X; i++) {
		for (int j = 0; j < WORLD_SIZE_Z; j++) {
			Chunk* chunk = m_world->GetChunk(i, j);
			chunk->IsDirty();
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
	m_monster.TriggerMonsterAttackCycle(elapsedTime);
	if (m_monster.GetIsCausingDamage()) {
		m_player.UpdateHealth(0.1f);

		if (m_player.GetPlayerHealth() <= 0.0f) {
			m_gameState = END_SCREEN;
			m_monster.StopAttackSound();
		}
	}

	DrawSkybox();
	RemoveBlendFunction(false);

	// Wireframe
	if (m_wireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (m_wireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	// HUD
	AddBlendFunction(true);
	if (m_keyH) { DrawHealthBar(m_player.GetPlayerHealth()); }
	if (m_keyR) { DrawArm(); }
	if (m_keyI) { DrawHud(elapsedTime); }
	if (m_keyC) { DrawCrosshair(); }
	RemoveBlendFunction(true);

	// Equipped block
	if (m_keyR) { DrawBlock(elapsedTime); }
	
	t.Pop();
	t.Use();

	if (m_monster.GetIsAttacking()) {
		m_laserShader.Use();
		RenderLaserBeams(elapsedTime);
		Shader::Disable();
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
		gluPerspective(45.0f, (float)Width() / (float)Height(), 0.001f, 1000.0f);
	}
	else {
		glOrtho(0, Width(), 0, Height(), -1, 1);
	}

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
}

void Engine::RemoveBlendFunction(bool isOrtho) {
	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);

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
		if (face == m_monster.GetMonsterFace()) {
			glDepthMask(GL_FALSE);
			m_textureMonster.Bind();
			glColor4f(1.0, 1.0, 1.0, m_monster.GetMonsterAlpha());
			DrawFaceWithMonster(face);

			m_textureMonsterEyes.Bind();
			glColor4f(1.0, 1.0, 1.0, m_monster.GetMonsterEyesAlpha());
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

void Engine::DrawEndScreen(float elapsedTime) {
	ShowCursor();
	m_textureEndScreen.Bind();
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex2i(0, 0);
	glTexCoord2f(1, 0); glVertex2i(Width(), 0);
	glTexCoord2f(1, 1); glVertex2i(Width(), Height());
	glTexCoord2f(0, 1); glVertex2i(0, Height());
	glEnd();
}

void Engine::DrawStartScreen(float elapsedTime) {
	m_textureStartScreen.Bind();
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex2i(0, 0);
		glTexCoord2f(1, 0); glVertex2i(Width(), 0);
		glTexCoord2f(1, 1); glVertex2i(Width(), Height());
		glTexCoord2f(0, 1); glVertex2i(0, Height());
	glEnd();

	m_playButtonAnimationTime += elapsedTime;

	float amplitude = 0.05f; 
	float frequency = 6.0f; 
	float scale = 0.4f + amplitude * sin(frequency * m_playButtonAnimationTime);

	m_buttonWidth = Width() * 0.4f;
	m_buttonHeight = Height() * 0.4f;

	float scaledButtonWidth = m_buttonWidth * scale;
	float scaledButtonHeight = m_buttonHeight * scale;

	m_centerX = Width() * 0.5f;
	m_centerY = Height() * 0.3f;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_texturePlayButton.Bind();
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex2i(m_centerX - scaledButtonWidth / 2, m_centerY - scaledButtonHeight / 2);
	glTexCoord2f(1, 0); glVertex2i(m_centerX + scaledButtonWidth / 2, m_centerY - scaledButtonHeight / 2);
	glTexCoord2f(1, 1); glVertex2i(m_centerX + scaledButtonWidth / 2, m_centerY + scaledButtonHeight / 2);
	glTexCoord2f(0, 1); glVertex2i(m_centerX - scaledButtonWidth / 2, m_centerY + scaledButtonHeight / 2);
	glEnd();
	glDisable(GL_BLEND);
}

void Engine::RenderLaserBeams(float elapsedTime) {
	glEnable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	static float laserTime = 0.0f;
	laserTime += elapsedTime;

	GLint timeLocation = m_laserShader.GetUniformLocation("time");
	m_laserShader.UpdateFloatUniform(timeLocation, laserTime);

	GLint screenSizeLocation = m_laserShader.GetUniformLocation("screenSize");
	m_laserShader.UpdateVec2Uniform(screenSizeLocation, Width(), Height());

	GLint colorLocation = m_laserShader.GetUniformLocation("color");
	float alphaValue = (sin(laserTime) / 4.0f) + 0.75f;
	float redValue = (sin(laserTime * 25.0) / 2.0f) + 0.5f;
	float greenValue = (sin(laserTime * 25.0 + PI) / 2.0f) + 0.5f;
	float blueValue = (sin(laserTime * 25.0 + (PI / 2.0f)) / 2.0f) + 0.5f;
	m_laserShader.UpdateVec4Uniform(colorLocation, redValue, greenValue, blueValue, alphaValue);

	const VertexBuffer& leftEyeLaserVbo = m_monster.GetLeftEyeLaserVBO();
	if (leftEyeLaserVbo.IsValid()) {
		leftEyeLaserVbo.Render();
	}

	const VertexBuffer& rightEyeLaserVbo = m_monster.GetRightEyeLaserVBO();
	if (rightEyeLaserVbo.IsValid()) {
		rightEyeLaserVbo.Render();
	}
	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
}

void Engine::DrawFaceWithMonster(int face) {
	glBegin(GL_QUADS);
	switch (face) {
	case 1: // Z-
		glNormal3f(0, 0, 1);
		glTexCoord2f(0, 0); glVertex3f(-VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2);
		glTexCoord2f(1, 0); glVertex3f(VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2);
		glTexCoord2f(1, 1); glVertex3f(VIEW_DISTANCE * 2, VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2);
		glTexCoord2f(0, 1); glVertex3f(-VIEW_DISTANCE * 2, VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2);
		break;
	case 2: // Z+
		glNormal3f(0, 0, -1);
		glTexCoord2f(0, 0); glVertex3f(VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2, VIEW_DISTANCE * 2);
		glTexCoord2f(1, 0); glVertex3f(-VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2, VIEW_DISTANCE * 2);
		glTexCoord2f(1, 1); glVertex3f(-VIEW_DISTANCE * 2, VIEW_DISTANCE * 2, VIEW_DISTANCE * 2);
		glTexCoord2f(0, 1); glVertex3f(VIEW_DISTANCE * 2, VIEW_DISTANCE * 2, VIEW_DISTANCE * 2);
		break;
	case 3: // X-
		glNormal3f(1, 0, 0);
		glTexCoord2f(0, 0); glVertex3f(-VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2, VIEW_DISTANCE * 2);
		glTexCoord2f(1, 0); glVertex3f(-VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2);
		glTexCoord2f(1, 1); glVertex3f(-VIEW_DISTANCE * 2, VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2);
		glTexCoord2f(0, 1); glVertex3f(-VIEW_DISTANCE * 2, VIEW_DISTANCE * 2, VIEW_DISTANCE * 2);
		break;
	case 4: // X+
		glNormal3f(-1, 0, 0);
		glTexCoord2f(0, 0); glVertex3f(VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2);
		glTexCoord2f(1, 0); glVertex3f(VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2, VIEW_DISTANCE * 2);
		glTexCoord2f(1, 1); glVertex3f(VIEW_DISTANCE * 2, VIEW_DISTANCE * 2, VIEW_DISTANCE * 2);
		glTexCoord2f(0, 1); glVertex3f(VIEW_DISTANCE * 2, VIEW_DISTANCE * 2, -VIEW_DISTANCE * 2);
		break;
	}
	glEnd(); 
}

void Engine::DrawArm() {
	glLoadIdentity();

	GLint originalBlendSrc, originalBlendDst;
	glGetIntegerv(GL_BLEND_SRC_ALPHA, &originalBlendSrc);
	glGetIntegerv(GL_BLEND_DST_ALPHA, &originalBlendDst);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	float armLeft = Width() * 0.625f;
	float armRight = Width() * 0.9f;
	float armTop = Width() * 0.275f;
	float armBottom = 0.0f;

	m_textureArm.Bind();
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex2i(armLeft, armBottom);
	glTexCoord2f(1, 0); glVertex2i(armRight, armBottom);
	glTexCoord2f(1, 1); glVertex2i(armRight, armTop);
	glTexCoord2f(0, 1); glVertex2i(armLeft, armTop);
	glEnd();
	glBlendFunc(originalBlendSrc, originalBlendDst);
}

void Engine::DrawBlock(float elapsedTime) {
	static float angleX = 0.0f;
	static float angleY = 0.0f;
	angleX += (elapsedTime * 100);
	angleY += (elapsedTime * 100);

	Transformation t;
	t.Push();
	t.ApplyTranslation(2.5f, -0.65, -6.75f);
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
	glNormal3f(0, 0, 1);	// Normal Z+
	glTexCoord2f(u, v); glVertex3f(0.5, -0.5, -0.5);
	glTexCoord2f(u + w, v); glVertex3f(-0.5, -0.5, -0.5);
	glTexCoord2f(u + w, v + h); glVertex3f(-0.5, 0.5, -0.5);
	glTexCoord2f(u, v + h); glVertex3f(0.5, 0.5, -0.5);
	glEnd();

	BlockInfo::GetBlockTextureCoords(equippedItem, BACK, u, v, w, h);
	glBegin(GL_QUADS);		// BACK
	glNormal3f(0, 0, -1);	// Normal Z-
	glTexCoord2f(u, v);	glVertex3f(-0.5, -0.5, 0.5);
	glTexCoord2f(u + w, v);	glVertex3f(0.5, -0.5, 0.5);
	glTexCoord2f(u + w, v + h);	glVertex3f(0.5, 0.5, 0.5);
	glTexCoord2f(u, v + h);	glVertex3f(-0.5, 0.5, 0.5);
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
	glNormal3f(-1, 0, 0);	// Normal X+
	glTexCoord2f(u, v);	glVertex3f(0.5, -0.5, 0.5);
	glTexCoord2f(u + w, v);	glVertex3f(0.5, -0.5, -0.5);
	glTexCoord2f(u + w, v + h);	glVertex3f(0.5, 0.5, -0.5);
	glTexCoord2f(u, v + h);	glVertex3f(0.5, 0.5, 0.5);
	glEnd();

	BlockInfo::GetBlockTextureCoords(equippedItem, RIGHT, u, v, w, h);
	glBegin(GL_QUADS);		// RIGHT
	glNormal3f(1, 0, 0);	// Normal X-
	glTexCoord2f(u, v);	glVertex3f(-0.5, -0.5, -0.5);
	glTexCoord2f(u + w, v);	glVertex3f(-0.5, -0.5, 0.5);
	glTexCoord2f(u + w, v + h);	glVertex3f(-0.5, 0.5, 0.5);
	glTexCoord2f(u, v + h);	glVertex3f(-0.5, 0.5, -0.5);
	glEnd();

	glDepthRange(0.0, 1.0);

	t.Pop();

	glBindTexture(GL_TEXTURE_2D, 0); // DON'T REMOVE THIS FUCKER!!!
}

void Engine::DrawHealthBar(float currentHealth) {
	glLoadIdentity();

	GLint originalBlendSrc, originalBlendDst;
	glGetIntegerv(GL_BLEND_SRC_ALPHA, &originalBlendSrc);
	glGetIntegerv(GL_BLEND_DST_ALPHA, &originalBlendDst);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	float healthBarLeft = Width() * 0.025f;
	float healthBarRight = Width() * 0.35f;

	float healthBarTop = Height() * 0.95f;
	float healthBarBottom = Height() * 0.8f;

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	m_textureHealthBar.Bind();
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex2i(healthBarLeft, healthBarBottom);
	glTexCoord2f(1, 0); glVertex2i(healthBarRight, healthBarBottom);
	glTexCoord2f(1, 1); glVertex2i(healthBarRight, healthBarTop);
	glTexCoord2f(0, 1); glVertex2i(healthBarLeft, healthBarTop);
	glEnd();

	float dynamicHealthBarLeft = Width() * 0.08f;
	float dynamicHealthBarRight = Width() * 0.295f;

	float totalHealthBarWidth = dynamicHealthBarRight - dynamicHealthBarLeft;
	float dynamicHealthBarWidth = (currentHealth / 100.0f) * totalHealthBarWidth;

	float dynamicHealthBarTop = Height() * 0.89f;
	float dynamicHealthBarBottom = Height() * 0.875f;

	if (dynamicHealthBarWidth > totalHealthBarWidth / 2) {
		glColor4f(0.0f, 1.0f, 0.0f, 0.5f);
	}
	else if (dynamicHealthBarWidth > totalHealthBarWidth / 4) {
		glColor4f(1.0f, 1.0f, 0.0f, 0.5f);
	}
	else {
		glColor4f(1.0f, 0.0f, 0.0f, 0.5f);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex2i(dynamicHealthBarLeft, dynamicHealthBarBottom);
	glTexCoord2f(1, 0); glVertex2i(dynamicHealthBarLeft + dynamicHealthBarWidth, dynamicHealthBarBottom);
	glTexCoord2f(1, 1); glVertex2i(dynamicHealthBarLeft + dynamicHealthBarWidth, dynamicHealthBarTop);
	glTexCoord2f(0, 1); glVertex2i(dynamicHealthBarLeft, dynamicHealthBarTop);
	glEnd();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBlendFunc(originalBlendSrc, originalBlendDst);
}

void Engine::DrawHud(float elapsedTime) {
	m_textureFont.Bind();
	std::ostringstream ss;

	float w = Width();
	float h = Height();

	ss << " CURSOR : ( X " << m_currentBlock.x << " | Y " << m_currentBlock.y << " | Z " << m_currentBlock.z << " )";
	PrintText(w * 0.01f, h * 0.75f, ss.str());
	ss.str("");
	Vector3f currentDirection = m_player.GetDirection();
	ss << " DIRECTION : " << DirectionToString(currentDirection);
	PrintText(w * 0.01f, h * 0.7f, ss.str());
	ss.str("");
	ss << " FPS : " << GetFps(elapsedTime);
	PrintText(w * 0.01f, h * 0.65f, ss.str());
	ss.str("");

	Vector3f pos = m_player.GetPosition();
	ss << (pos.x > 0 ? " CHUNK: ( X " : " CHUNK: ( X-") << (int)(pos.x / CHUNK_SIZE_X) <<
		(pos.z > 0 ? " | Z " : " | Z-") << (int)(pos.z / CHUNK_SIZE_Z) << " )";
	PrintText(w * 0.01f, h * 0.15f, ss.str());
	ss.str("");

	ss << (pos.x > 0 ? " BLOCK: ( X " : " BLOCK: ( X-") <<
		abs((int)(pos.x) % CHUNK_SIZE_X) << (pos.y > 0 ? " | Y " : " | Y-") <<
		abs((int)(pos.y) % CHUNK_SIZE_Y) << (pos.z > 0 ? " | Z " : " | Z-") <<
		abs((int)(pos.z) % CHUNK_SIZE_Z) << " )";
	PrintText(w * 0.01f, h * 0.1f, ss.str());
	ss.str("");

	ss << (pos.x > 0 ? " GLOBAL: ( X " : " GLOBAL: ( X-") << std::fixed << std::setprecision(2) <<
		abs(pos.x) << (pos.y > 0 ? " | Y " : " | Y-") <<
		abs(pos.y) << (pos.z > 0 ? " | Z " : " | Z-") <<
		abs(pos.z) << " )";
	PrintText(w * 0.01f, h * 0.05f, ss.str());
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

		glTranslated(charWidth - 10, 0, 0);
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
	case 7: // ( INFO ) I
		m_keyH = !m_keyH;
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
	MakeRelativeToCenter(x, y);
	m_player.TurnLeftRight((float)x);
	m_player.TurnTopBottom((float)y);

	if (m_gameState == START_SCREEN) {
		return;
	}
	
	if (m_gameState == END_SCREEN) {
		return;
	}

	CenterMouse();
}

void Engine::MousePressEvent(const MOUSE_BUTTON& button, int x, int y) {
	switch (button) {
	case MOUSE_BUTTON_WHEEL_UP:
		if (m_player.GetEquippedItem() < BTYPE_LAST - 1) {
			m_player.SetEquippedItem(static_cast<BlockType>(m_player.GetEquippedItem() + 1));
		}
		else {
			m_player.SetEquippedItem(static_cast<BlockType>(BTYPE_AIR + 1));
		}
		return; 
	case MOUSE_BUTTON_WHEEL_DOWN:
		if (m_player.GetEquippedItem() > BTYPE_AIR + 1) {
			m_player.SetEquippedItem(static_cast<BlockType>(m_player.GetEquippedItem() - 1));
		}
		else {
			m_player.SetEquippedItem(static_cast<BlockType>(BTYPE_LAST - 1));
		}
		return;
	}

	if (m_currentBlock.x < 0) {
		return;
	}

	Chunk* currentChunk;
	Chunk* targetChunk;

	int targetChunkX;
	int targetChunkZ;

	int targetX;
	int targetY;
	int targetZ;
	int playerX;
	int playerY;
	int playerZ;

	BlockType addBt;
	BlockType removeBt;
	BlockType equippedItem;

	float buttonLeft = m_centerX - m_buttonWidth / 2;
	float buttonRight = m_centerX + m_buttonWidth / 2;
	float buttonTop = m_centerY + m_buttonHeight / 4;
	float buttonBottom = m_centerY - m_buttonHeight / 4;
	float invertedY = Height() - y;

	switch (button) {
	case MOUSE_BUTTON_LEFT:
		// FIX CLICKABLE PLAY BUTTON SURFACE
		if (m_gameState == START_SCREEN && (x > buttonLeft && x < buttonRight && invertedY < buttonTop && invertedY > buttonBottom)) {
			m_gameState = PLAY;
			HideCursor();
			CenterMouse();
			m_music.play();
			m_music.setLoop(true);
			return;
		}

		currentChunk = m_world->ChunkAt(static_cast<int>(m_currentBlock.x), static_cast<int>(m_currentBlock.y), static_cast<int>(m_currentBlock.z));
		removeBt = currentChunk->GetBlock(static_cast<int>(m_currentBlock.x) % CHUNK_SIZE_X,
			static_cast<int>(m_currentBlock.y) % CHUNK_SIZE_Y,
			static_cast<int>(m_currentBlock.z) % CHUNK_SIZE_Z);

		if (removeBt != BTYPE_AIR) {
			currentChunk->RemoveBlock(static_cast<int>(m_currentBlock.x) % CHUNK_SIZE_X,
				static_cast<int>(m_currentBlock.y) % CHUNK_SIZE_Y,
				static_cast<int>(m_currentBlock.z) % CHUNK_SIZE_Z);

			m_world->SetDirtyChunk(currentChunk, m_currentBlock.x, m_currentBlock.y, m_currentBlock.z);
		}
		break;
	case MOUSE_BUTTON_RIGHT:
		targetX = static_cast<int>(m_currentBlock.x) + m_currentFaceNormal.x;
		targetY = static_cast<int>(m_currentBlock.y) + m_currentFaceNormal.y;
		targetZ = static_cast<int>(m_currentBlock.z) + m_currentFaceNormal.z;
		playerX = static_cast<int>(m_player.GetPosition().x);
		playerY = static_cast<int>(m_player.GetPosition().y);
		playerZ = static_cast<int>(m_player.GetPosition().z);

		targetChunkX = targetX / CHUNK_SIZE_X;
		targetChunkZ = targetZ / CHUNK_SIZE_Z;

		if (targetChunkX < 0 || targetChunkX > WORLD_SIZE_X - 1 ||
			targetChunkZ < 0 || targetChunkZ > WORLD_SIZE_Z - 1) {
			break;
		}

		targetChunk = m_world->ChunkAt(targetX, targetY, targetZ);
		addBt = m_world->BlockAt(targetX, targetY, targetZ, BTYPE_AIR);
		if (addBt != BTYPE_AIR) {
			return;
		}
		else if ((targetX == playerX && targetY == playerY && targetZ == playerZ) ||
			(targetX == playerX && targetY == playerY - 1 && targetZ == playerZ)) { // TODO: Apply y detection
			return;
		}
		else {
			equippedItem = m_player.GetEquippedItem();
			targetChunk->SetBlock(targetX % CHUNK_SIZE_X, targetY % CHUNK_SIZE_Y, targetZ % CHUNK_SIZE_Z, equippedItem);
			m_world->SetDirtyChunk(targetChunk, targetX, targetY, targetZ);
		}
		break;
	default:
		break;
	}
}

void Engine::MouseReleaseEvent(const MOUSE_BUTTON& button, int x, int y) {}

void Engine::GetBlockAtCursor() {
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

	int px = (int)(posX);
	int py = (int)(posY);
	int pz = (int)(posZ);

	bool found = false;

	if ((m_player.GetPosition() - Vector3f((float)posX, (float)posY, (float)posZ)).Length() < MAX_SELECT_DISTANCE) {
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

						if (InRangeWithEpsilon((float)posX, (float)x, (float)x + 1.f, 0.075f) && InRangeWithEpsilon((float)posY, (float)y, (float)y + 1.f, 0.075f) && InRangeWithEpsilon((float)posZ, (float)z, (float)z + 1.f, 0.075f)) {
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
		m_currentFaceNormal.Zero();

		const float epsilon = 0.075f;

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
	return (fabs(val2 - val1) < epsilon);
}

bool Engine::InRangeWithEpsilon(const float& val, const float& minVal, const float& maxVal, float epsilon) {
	return (val >= minVal - epsilon && val <= maxVal + epsilon);
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
