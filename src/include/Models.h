#pragma once

#include <iostream>
#include <unordered_map>
#include <cmath>
#include <vector>

extern "C" {
	#include <raylib.h>
}

#include "Constants.h"

struct RenderMetaData {
	Texture2D* textureAtlas = nullptr;
};

// UI Models
struct ImageUI {
	Rectangle textureAtlasCoords;
	Rectangle screenCoords;

	void Draw(RenderMetaData metaData) {
		DrawTexturePro(*(metaData.textureAtlas), textureAtlasCoords, screenCoords, { 0, 0 }, 0.0, RAYWHITE);
	}
};

// Game Logic Models
class Player {
private:
	unsigned int score;
	ImageUI img;

public:
	Player();

	unsigned int GetScore() const;
	void SetScore(int newValue);

	Rectangle GetScreenCoords() const;

	void Draw(RenderMetaData metaData) const;
	void Move(Vector2 velocity);
};

class Pipe {
private:
	ImageUI img;
	float rotation;
	bool isUpperPipe;

public:
	Pipe(float rotation, bool isUpperPipe);

	Rectangle GetScreenCoords() const;
	void SetScreenCoords(Rectangle newValue);

	void Draw(RenderMetaData metaData) const;
	void Move(Vector2 velocity);
};

class Floor {
private:
	ImageUI img;

public:
	Floor();

	Rectangle GetScreenCoords() const;
	void SetScreenCoords(Rectangle newValue);

	void Draw(RenderMetaData metaData) const;
	void Move(Vector2 velocity);
};

// Scene Models

class Scene {
protected:
	std::string name;

public:
	Scene(const std::string& name);

	const std::string& GetName() const;

	virtual void Update() = 0;
	virtual void Render(RenderMetaData metaData) = 0;
};

class SceneManager {
private:
	std::unordered_map<std::string, Scene*> scenes;
	Scene* currentActiveScene = nullptr;
	std::unordered_map<std::string, std::string> internalCache;

public:
	SceneManager();

	void RenderActiveScene(RenderMetaData metaData) const;
	void SetActiveScene(const std::string& name);
	void RemoveAndRecreateScene(const std::string& name);

	bool CacheContainsKey(const std::string& key) const;
	const std::string& GetCacheEntryValue(const std::string& key) const;
	void SetCacheEntry(std::string key, std::string value);

	~SceneManager();
};

enum RenderingMode : unsigned char {
	Normal,
	ShowBoundingBoxes
};

class MenuScene : public Scene {
private:
	ImageUI titleText;
	ImageUI playBtn;
	SceneManager* sceneManager;

public:
	MenuScene(SceneManager* sceneManager);

	void Update();
	void Render(RenderMetaData metaData);
};

class GameScene : public Scene {
private:
	SceneManager* sceneManager;
	ImageUI getReadyText;
	Player player;
	RenderingMode currentRenderingMode = RenderingMode::Normal;
	bool hasStarted = false;
	Sound swooshingSound;
	Sound pointSound;
	Sound hitSound;

	std::vector<std::pair<Pipe*, Pipe*>> pipesPool;
	std::vector<Floor*> floors;

public:
	GameScene(SceneManager* sceneManager);

	void Update();
	void Render(RenderMetaData metaData);

	~GameScene();
};

class GameOverScene : public Scene {
private:
	SceneManager* sceneManager;
	ImageUI gameOverImg;
	ImageUI playBtn;

public:
	GameOverScene(SceneManager* sceneManager);

	void Update();
	void Render(RenderMetaData metaData);

	~GameOverScene();
};