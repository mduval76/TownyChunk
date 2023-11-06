#include "engine.h"


Engine::Engine() : m_world() {}

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
	glEnable(GL_MULTISAMPLE);

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
	LoadTexture(m_textureFont, TEXTURE_PATH "font.bmp");
	LoadTexture(m_textureCrosshair, TEXTURE_PATH "cross.bmp");

	TextureAtlas::TextureIndex texIdxDirt = m_textureAtlas.AddTexture(TEXTURE_PATH "dirt.jpg");
	TextureAtlas::TextureIndex texIdxFace = m_textureAtlas.AddTexture(TEXTURE_PATH "face.jpg");
	TextureAtlas::TextureIndex texIdxHellX = m_textureAtlas.AddTexture(TEXTURE_PATH "hell_x.png");
	TextureAtlas::TextureIndex texIdxHellY = m_textureAtlas.AddTexture(TEXTURE_PATH "hell_y.png");
	TextureAtlas::TextureIndex texIdxHellZ = m_textureAtlas.AddTexture(TEXTURE_PATH "hell_z.png");
	TextureAtlas::TextureIndex texIdxMarble = m_textureAtlas.AddTexture(TEXTURE_PATH "marble.png");
	TextureAtlas::TextureIndex texIdxStone = m_textureAtlas.AddTexture(TEXTURE_PATH "stone.jpg");

	if (!m_textureAtlas.Generate(256, false)) {
		std::cerr << "Unable to generate texture atlas..." << std::endl;
		abort();
	}

	GLfloat maxAnisotropy;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
	glBindTexture(GL_TEXTURE_2D, texIdxDirt);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	std::map<BlockType, TextureAtlas::TextureIndex> btIndices;
	btIndices[BTYPE_DIRT] = texIdxDirt;
	btIndices[BTYPE_FACE] = texIdxFace;
	btIndices[BTYPE_MARBLE] = texIdxMarble;
	btIndices[BTYPE_STONE] = texIdxStone;

	// Single texture for all faces
	for (auto& pair : btIndices) {
		float u, v, w, h;
		m_textureAtlas.TextureIndexToCoord(pair.second, u, v, w, h);

		for (int face = BlockInfo::FRONT; face < BlockInfo::LAST; ++face) {
			BlockInfo::SetBlockTextureCoords(pair.first, static_cast<BlockInfo::BlockFace>(face), u, v, w, h);
		}
	}

	// Different texture for each face
	float u, v, w, h;
	m_textureAtlas.TextureIndexToCoord(texIdxHellZ, u, v, w, h);
	BlockInfo::SetBlockTextureCoords(BTYPE_HELL, BlockInfo::FRONT, u, v, w, h); 
	m_textureAtlas.TextureIndexToCoord(texIdxHellZ, u, v, w, h);
	BlockInfo::SetBlockTextureCoords(BTYPE_HELL, BlockInfo::BACK, u, v, w, h);  
	m_textureAtlas.TextureIndexToCoord(texIdxHellX, u, v, w, h);
	BlockInfo::SetBlockTextureCoords(BTYPE_HELL, BlockInfo::LEFT, u, v, w, h);  
	m_textureAtlas.TextureIndexToCoord(texIdxHellX, u, v, w, h);
	BlockInfo::SetBlockTextureCoords(BTYPE_HELL, BlockInfo::RIGHT, u, v, w, h);
	m_textureAtlas.TextureIndexToCoord(texIdxHellY, u, v, w, h);
	BlockInfo::SetBlockTextureCoords(BTYPE_HELL, BlockInfo::TOP, u, v, w, h);
	m_textureAtlas.TextureIndexToCoord(texIdxHellY, u, v, w, h);
	BlockInfo::SetBlockTextureCoords(BTYPE_HELL, BlockInfo::BOTTOM, u, v, w, h);

	m_world = new World();

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
	static float gameTime = elapsedTime; // Valeur conservée entre les appels car static
	gameTime += elapsedTime;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Efface le tampon de couleur et de profondeur

	// Transformations initiales
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
 
	// Camera (Player)
	Transformation t;
	int chunkX = static_cast<int>(std::floor(m_player.GetPosition().x / CHUNK_SIZE_X));
	int chunkZ = static_cast<int>(std::floor(m_player.GetPosition().z / CHUNK_SIZE_Z));
	Chunk* currentChunk = m_world->GetChunk(chunkX, chunkZ);
	if (currentChunk != nullptr) {
		m_player.Move(currentChunk, m_keyW, m_keyS, m_keyA, m_keyD, m_keySpace, elapsedTime);
	}

	std::array<float, 2> rot = m_player.GetRotation();
	m_player.ApplyTransformation(t);
	t.ApplyTranslation(0.0f, 0.0f, 0.0f);
	t.Use();

	// Chunk
	m_textureAtlas.Bind();
	m_shader01.Use();
	for (int i = 0; i < WORLD_SIZE_X; i++) {
		for (int j = 0; j < WORLD_SIZE_Z; j++) {
			Chunk* chunk = m_world->GetChunk(i, j);
			if (chunk == nullptr) {
				continue;
			}
			if (chunk->IsDirty()) {
				chunk->Update();
			}
			chunk->Render();
		}
	}
	Shader::Disable();

	// Skybox
	t.SetIdentity();
	t.ApplyRotation(-rot[0], 1.0f, 0, 0);
	t.ApplyRotation(-rot[1], 0, 1.0f, 0);
	t.Use();

	DrawSkybox();

	if (m_wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	if (m_keyI) {
		DrawHud(elapsedTime);
	}

	if (m_wireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
}

void Engine::KeyPressEvent(unsigned char key) {
	switch (key) {
	case 0: // ( LEFT ) A
		m_keyA = true;
		break;
	case 3: // ( BACK ) D
		m_keyD = true;
		break;
	case 8: // ( INFO ) I
		m_keyI = !m_keyI;
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
	// Centrer la souris seulement si elle n'est pas déjà centrée
	// Il est nécessaire de faire la vérification pour éviter de tomber
	// dans une boucle infinie où l'appel à CenterMouse génère un
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

std::string Engine::DirectionToString(const Vector3f& direction) const {
	const float x = direction.x;
	const float y = direction.y;
	const float z = direction.z;

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

	for (unsigned int i = 0; i < t.length(); ++i)
	{
		char c = t[i];
		int col = (c - 32) % numCols;
		int row = (c - 32) / numCols;

		float left = col * charWidth / atlasWidth;
		float right = left + charWidth / atlasWidth;
		float top = 1.0f - (row * charHeight) / atlasHeight;
		float bottom = top - charHeight / atlasHeight;

		glBegin(GL_QUADS);
		glTexCoord2f(left, bottom);
		glVertex2f(0, 0);
		glTexCoord2f(right, bottom);
		glVertex2f(charWidth - 5, 0);
		glTexCoord2f(right, top);
		glVertex2f(charWidth - 5, charHeight - 5);
		glTexCoord2f(left, top);
		glVertex2f(0, charHeight - 5);
		glEnd();

		glTranslated(charWidth, 0, 0);
	}
}

void Engine::DrawHud(float elapsedTime) {
	// Setter le blend function , tout ce qui sera noir sera transparent
	glDisable(GL_LIGHTING);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, Width(), 0, Height(), -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	// Bind de la texture pour le font
	m_textureFont.Bind();
	std::ostringstream ss;
	Vector3f currentDirection = m_player.GetDirection();
	ss << " DIRECTION : " << DirectionToString(currentDirection);
	PrintText(10, Height() - 30, ss.str());
	ss.str("");
	ss << " FPS : " << GetFps(elapsedTime);
	PrintText(10, Height() - 60, ss.str());
	ss.str("");

	Vector3f pos = m_player.GetPosition();
	ss << " CHUNK: ( X " << (int)(pos.x / CHUNK_SIZE_X) << " | Z " << (int)(pos.z / CHUNK_SIZE_Z) << " )";
	PrintText(10, 80, ss.str());
	ss.str("");

	ss << " BLOCK: ( X " << (int)(pos.x) % CHUNK_SIZE_X << " | Y " << (int)(pos.y) % CHUNK_SIZE_Y << " | Z " << (int)(pos.z) % CHUNK_SIZE_Z << " )";
	PrintText(10, 50, ss.str());
	ss.str("");

	ss << " GLOBAL: ( X " << std::fixed << std::setprecision(2) << pos.x << " | Y " << pos.y << " | Z " << pos.z << " )";
	PrintText(10, 20, ss.str());
	ss.str("");

	// Affichage du crosshair
	m_textureCrosshair.Bind();
	static const int crossSize = 32;
	glLoadIdentity();
	glTranslated(Width() / 2 - crossSize / 2, Height() / 2 - crossSize / 2, 0);
	glBegin(GL_QUADS);
	
	glTexCoord2f(0, 0);
	glVertex2i(0, 0);
	glTexCoord2f(1, 0);
	glVertex2i(crossSize, 0);
	glTexCoord2f(1, 1);
	glVertex2i(crossSize, crossSize);
	glTexCoord2f(0, 1);
	glVertex2i(0, crossSize);
	glEnd();
	glEnable(GL_LIGHTING);
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}
