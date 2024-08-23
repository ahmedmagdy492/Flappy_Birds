
#include "include\Models.h"

int main() {
	int width = GetScreenWidth();
	int height = GetScreenHeight();
	InitWindow(width, height, "Flappy Birds");

	SetTargetFPS(60);

	ClearBackground(BLACK);
	BeginDrawing();
	int loadingTxtWidth = MeasureText("Loading...", 35);
	DrawText("Loading...", (GetScreenWidth() - loadingTxtWidth) / 2, (GetScreenHeight() - 35) / 2, 35, WHITE);
	EndDrawing();

	InitAudioDevice();

	Texture2D textureAtlas = LoadTexture("resources/texture_atlas.png");

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

	CloseAudioDevice();

	CloseWindow();
}