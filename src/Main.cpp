
#include "include\Models.h"

int main() {
	int width = GetScreenWidth();
	int height = GetScreenHeight();
	InitWindow(width, height, "Flappy Birds");

	Font font = LoadFont("resources/flappy_font.TTF");

	SetTargetFPS(60);

	ClearBackground(BLACK);
	BeginDrawing();
	Vector2 loadingTxtWidth = MeasureTextEx(font, "Loading...", 35.0f, 0.0f);
	DrawTextEx(font, "Loading...", { (GetScreenWidth() - loadingTxtWidth.x) / 2, (GetScreenHeight() - 35.0f) / 2.0f }, 35.0f, 0.0f, WHITE);
	EndDrawing();

	InitAudioDevice();

	Texture2D textureAtlas = LoadTexture("resources/texture_atlas.png");

	SceneManager sceneManager;
	RenderMetaData renderMetaData;
	renderMetaData.textureAtlas = &textureAtlas;
	renderMetaData.curFont = font;

	while (!WindowShouldClose()) {
		ClearBackground(RAYWHITE);

		BeginDrawing();

		sceneManager.RenderActiveScene(renderMetaData);

		EndDrawing();
	}

	UnloadTexture(textureAtlas);

	UnloadFont(font);

	CloseAudioDevice();

	CloseWindow();
}