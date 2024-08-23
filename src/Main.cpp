
#include "include\Models.h"

int main() {
	int width = GetScreenWidth();
	int height = GetScreenHeight();
	InitWindow(width, height, "Flappy Birds");

	SetTargetFPS(60);

	Texture2D textureAtlas = LoadTexture("resources\\texture_atlas.png");

	SceneManager sceneManager;
	RenderMetaData renderMetaData;
	renderMetaData.textureAtlas = &textureAtlas;

	while (!WindowShouldClose()) {
		ClearBackground(RAYWHITE);

		BeginDrawing();

		sceneManager.RenderActiveScene(renderMetaData);

		EndDrawing();
	}

	UnloadTexture(textureAtlas);

	CloseWindow();
}