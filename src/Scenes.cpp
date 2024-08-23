#include "include\Models.h"

Scene::Scene(const std::string& name) {
	this->name = name;
}

const std::string& Scene::GetName() const {
	return name;
}

SceneManager::SceneManager() {
	scenes.insert({ "MenuScene", new MenuScene(this) });
	scenes.insert({ "GameScene", new GameScene(this) });
	scenes.insert({ "GameOverScene", new GameOverScene(this) });

	SetActiveScene("MenuScene");
}

void SceneManager::RenderActiveScene(RenderMetaData metaData) const {
	if (currentActiveScene) {
		currentActiveScene->Update();
		currentActiveScene->Render(metaData);
	}
}

void SceneManager::SetActiveScene(const std::string& name) {
	auto sceneIt = scenes.find(name);

	if (sceneIt != scenes.end()) {
		currentActiveScene = scenes[name];
	}
}

void SceneManager::RemoveAndRecreateScene(const std::string& name) {
	Scene* scene = scenes[name];
	if (scene) {
		delete scene;
		if (name == "GameScene") {
			scenes[name] = new GameScene(this);
		}
		else if (name == "GameOverScene") {
			scenes[name] = new GameOverScene(this);
		}
		else {
			scenes[name] = new MenuScene(this);
		}
		SetActiveScene(name);
	}
}

bool SceneManager::CacheContainsKey(const std::string& key) const {
	return internalCache.find(key) != internalCache.end();
}

const std::string& SceneManager::GetCacheEntryValue(const std::string& key) const {
	return internalCache.at(key);
}

void SceneManager::SetCacheEntry(std::string key, std::string value) {
	if (CacheContainsKey(key)) {
		internalCache.find(key)->second = value;
	}
	else {
		internalCache.insert({ key, value });
	}
}

SceneManager::~SceneManager() {
	for (auto scene : scenes) {
		if (scene.second) {
			delete scene.second;
			scene.second = nullptr;
		}
	}
	scenes.clear();
}

// Scenes
MenuScene::MenuScene(SceneManager* sceneManager) : Scene("MenuScene") {
	this->sceneManager = sceneManager;
	float height = GetScreenHeight();
	float width = GetScreenWidth();

	titleText.textureAtlasCoords = { 700, 180, 182, 48 };
	titleText.screenCoords = { (width - 364.0f) / 2, 100, 364.0f, 96.0f };

	playBtn.textureAtlasCoords = { 700, 230, 125, 70 };
	playBtn.screenCoords = { (width - 140) / 2, (height - 70) / 2, 140, 96.0f };
}

bool IsClicked(Vector2 mousePos, Rectangle clickableItemCoords) {
	return (mousePos.x >= clickableItemCoords.x && mousePos.x <= (clickableItemCoords.x + clickableItemCoords.width) && mousePos.y >= clickableItemCoords.y && mousePos.y <= (clickableItemCoords.y + clickableItemCoords.height));
}

void MenuScene::Update() {
 	Vector2 mousePos = GetMousePosition();

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		if (IsClicked(mousePos, playBtn.screenCoords)) {
			sceneManager->SetActiveScene("GameScene");
		}
	}
}

void MenuScene::Render(RenderMetaData metaData) {
	float height = GetScreenHeight();
	float width = GetScreenWidth();
	int noOfRenderingTimesHor = round(width / 512.0f);
	for (int i = 0; i < noOfRenderingTimesHor; ++i) {
		DrawTexturePro(*(metaData.textureAtlas), { 0, 0, 288, 512 }, { i * 512.0f, 0, 512, height }, { 0, 0 }, 0.0, RAYWHITE);
	}

	// Rendering the title
	titleText.Draw(metaData);

	// Rendering the play button
	playBtn.Draw(metaData);
}

GameScene::GameScene(SceneManager* sceneManager) : Scene("GameScene") {
	this->sceneManager = sceneManager;
	float height = GetScreenHeight();
	float width = GetScreenWidth();

	int noOfFloorsToBePlaced = round(width / FLOOR_WIDTH) + 10;

	for (int i = 0; i < noOfFloorsToBePlaced; ++i) {
		Floor* floor = new Floor();
		Rectangle floorCoords = floor->GetScreenCoords();
		floor->SetScreenCoords({ i * floorCoords.width, height - floorCoords.height, floorCoords.width, floorCoords.height });
		floors.push_back(floor);
	}

	getReadyText.textureAtlasCoords = { 586.0f, 112.0f, 196.0f, 62.0f };
	getReadyText.screenCoords = { (width - getReadyText.textureAtlasCoords.width) / 2, (height - getReadyText.textureAtlasCoords.height) / 2, getReadyText.textureAtlasCoords.width, getReadyText.textureAtlasCoords.height };

	// adding 2 here as backup pipes to be able to remove pipes with x less than 0
	// without the user noticing
	int noOfPipesOnTheScreen = round(width / (FLOOR_WIDTH)) + 2;

	for (int i = 0; i < noOfPipesOnTheScreen; ++i) {
		// lower pipe
		Pipe* pipe1 = new Pipe(0.0f, false);
		Rectangle screenCoordsPipe1 = pipe1->GetScreenCoords();
		float randomYValue = GetRandomValue(height - (FLOOR_HEIGHT + screenCoordsPipe1.height), height - screenCoordsPipe1.height);
		pipe1->SetScreenCoords(
			{
				PIPE_STARTING_OFFSET_X + (screenCoordsPipe1.width + OFFSET_BETWEEN_PIPE_X) * i,
				randomYValue,
				screenCoordsPipe1.width,
				screenCoordsPipe1.height
			}
		);

		// upper pipe
		Pipe* pipe2 = new Pipe(0.0f, true);
		Rectangle screenCoordsPipe2 = pipe2->GetScreenCoords();
		pipe2->SetScreenCoords(
			{ 
				PIPE_STARTING_OFFSET_X + (screenCoordsPipe2.width + OFFSET_BETWEEN_PIPE_X) * i, 
				randomYValue - OFFSET_BETWEEN_PIPE_Y - screenCoordsPipe2.height,
				screenCoordsPipe2.width,
				screenCoordsPipe2.height
			}
		);
		pipesPool.push_back(std::make_pair(pipe1, pipe2));
	}
}

bool IsCollidingWithPipe(Rectangle coords1, Rectangle coords2) {
	return CheckCollisionRecs(coords1, coords2);
}

void GameScene::Update() {
 	Vector2 mousePos = GetMousePosition();

	if (IsKeyPressed(KEY_B)) {
		if (currentRenderingMode == RenderingMode::Normal) {
			currentRenderingMode = RenderingMode::ShowBoundingBoxes;
		}
		else {
			currentRenderingMode = RenderingMode::Normal;
		}
	}

	if (hasStarted) {
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			Vector2 velocity = { 0.0f, -30.0f };
			player.Move(velocity);
		}
		else {
			Vector2 velocity = { 0.0f, 1.5f };
			player.Move(velocity);
		}
	}
	else {
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			hasStarted = true;
		}
	}

	for (auto pair : pipesPool) {
		if (IsCollidingWithPipe(player.GetScreenCoords(), pair.first->GetScreenCoords()) || 
			IsCollidingWithPipe(player.GetScreenCoords(), pair.second->GetScreenCoords())) {
			std::string playerScore(TextFormat("%i", player.GetScore()));
			sceneManager->SetCacheEntry("score", playerScore);
			sceneManager->SetActiveScene("GameOverScene");
		}
	}
}

void GameScene::Render(RenderMetaData metaData) {
	float height = GetScreenHeight();
	float width = GetScreenWidth();

	// drawing the background
	int noOfRenderingTimesHor = round(width / 512.0f);
	for (int i = 0; i < noOfRenderingTimesHor; ++i) {
		DrawTexturePro(*(metaData.textureAtlas), { 300.0f, 0.0f, 275.0f, 512.0f }, { i * 512.0f, 0.0f, 512.0f, height }, { 0.0f, 0.0f }, 0.0f, RAYWHITE);
	}

	for (auto pair : pipesPool) {
		pair.first->Draw(metaData);
		if (hasStarted) {
			pair.first->Move({ -2.0f, 0.0f });
		}

		if (currentRenderingMode == RenderingMode::ShowBoundingBoxes) {
			DrawRectangleLinesEx(pair.first->GetScreenCoords(), 2, RED);
		}

		pair.second->Draw(metaData);
		if (hasStarted) {
			pair.second->Move({ -2.0f, 0.0f });
		}

		if (currentRenderingMode == RenderingMode::ShowBoundingBoxes) {
			DrawRectangleLinesEx(pair.second->GetScreenCoords(), 2, RED);
		}

		Rectangle pipeScreenCoords = pair.first->GetScreenCoords();
		if ((pipeScreenCoords.x + pipeScreenCoords.width) < 0) {
			
			pipeScreenCoords.x = width + ((PIPE_WIDTH + OFFSET_BETWEEN_PIPE_X));

			float randomYValue = GetRandomValue(height - (FLOOR_HEIGHT + pipeScreenCoords.height), height - pipeScreenCoords.height);
			pipeScreenCoords.y = randomYValue;
			pair.first->SetScreenCoords(pipeScreenCoords);
			Rectangle newScreenCoords = { pipeScreenCoords.x, randomYValue - OFFSET_BETWEEN_PIPE_Y - pipeScreenCoords.height, pipeScreenCoords.width, pipeScreenCoords.height };
			pair.second->SetScreenCoords(newScreenCoords);

			player.SetScore(player.GetScore() + 1);
		}
	}

	// drawing the floor
	for (auto floor : floors) {
		floor->Draw(metaData);
		if (hasStarted) {
			floor->Move({ -2.0f, 0.0f });
		}

		Rectangle floorScreenCoords = floor->GetScreenCoords();
		if ((floorScreenCoords.x + floorScreenCoords.width) < 0) {
			floorScreenCoords.x = width;
			floorScreenCoords.y = height - floorScreenCoords.height;
			floor->SetScreenCoords(floorScreenCoords);
		}
	}

	player.Draw(metaData);
	if (currentRenderingMode == RenderingMode::ShowBoundingBoxes) {
		DrawRectangleLinesEx(player.GetScreenCoords(), 2, RED);
	}

	// rendering the player score
	DrawText(TextFormat("%i", player.GetScore()), width / 2, 40, 44, RAYWHITE);

	if(!hasStarted) {
		getReadyText.Draw(metaData);
	}
}

GameScene::~GameScene() {
	for (auto pair : pipesPool) {
		delete pair.first;
		delete pair.second;
	}
	pipesPool.clear();

	for (auto floor : floors) {
		delete floor;
	}
	floors.clear();
}

GameOverScene::GameOverScene(SceneManager* sceneManager) : Scene("GameOverScene") {
	float height = GetScreenHeight();
	float width = GetScreenWidth();
	this->sceneManager = sceneManager;
	gameOverImg.textureAtlasCoords = { 790, 120, 190, 54 };
	gameOverImg.screenCoords = { ((width - gameOverImg.textureAtlasCoords.width) / 2), ((height - gameOverImg.textureAtlasCoords.height) / 2), gameOverImg.textureAtlasCoords.width, gameOverImg.textureAtlasCoords.height };

	playBtn.textureAtlasCoords = { 700, 230, 125, 70 };
	playBtn.screenCoords = { (width - 140) / 2, gameOverImg.screenCoords.y + gameOverImg.screenCoords.height + 80, 140, 96.0f };
}

void GameOverScene::Update() {
	Vector2 mousePos = GetMousePosition();

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		if (IsClicked(mousePos, playBtn.screenCoords)) {
			sceneManager->RemoveAndRecreateScene("GameScene");
		}
	}
}

void GameOverScene::Render(RenderMetaData metaData) {
	float height = GetScreenHeight();
	float width = GetScreenWidth();
	int noOfRenderingTimesHor = round(width / 512.0f);
	for (int i = 0; i < noOfRenderingTimesHor; ++i) {
		DrawTexturePro(*(metaData.textureAtlas), { 0, 0, 288, 512 }, { i * 512.0f, 0, 512, height }, { 0, 0 }, 0.0, RAYWHITE);
	}

	gameOverImg.Draw(metaData);

	if (sceneManager->CacheContainsKey("score")) {
		const std::string& playerScore = sceneManager->GetCacheEntryValue("score");
		DrawText(playerScore.c_str(), (width - 20) / 2, gameOverImg.screenCoords.y + gameOverImg.screenCoords.height + 20, 44, RAYWHITE);
	}

	playBtn.Draw(metaData);
}

GameOverScene::~GameOverScene() {

}