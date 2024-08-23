#include "include\Models.h"

Player::Player() : score(0) {
	float height = GetScreenHeight();
	float width = GetScreenWidth();

	img.textureAtlasCoords = { 230, 750, 43, 43 };
	img.screenCoords = { 30, (height - PLAYER_HEIGHT) / 2, PLAYER_WIDTH, PLAYER_HEIGHT };
}

void Player::Draw(RenderMetaData metaData) const {
	DrawTexturePro(*(metaData.textureAtlas), img.textureAtlasCoords, img.screenCoords, {0, 0}, 0.0f, RAYWHITE);
}

/// only allowing the movement on the y coordinate
void Player::Move(Vector2 velocity) {
	if ((img.screenCoords.y + velocity.y) > 0) {
		img.screenCoords.y += velocity.y;
	}
}

unsigned int Player::GetScore() const {
	return score;
}

void Player::SetScore(int newValue) {
	score = newValue;
}

Rectangle Player::GetScreenCoords() const {
	return img.screenCoords;
}

Pipe::Pipe(float rotation, bool isUpperPipe) : rotation(rotation), isUpperPipe(isUpperPipe) {
	float height = GetScreenHeight();
	float width = GetScreenWidth();

	if (isUpperPipe) {
		img.textureAtlasCoords = { 110, 650, 52, 320 };
	}
	else {
		img.textureAtlasCoords = { 170, 650, 52, 320 };
	}
	img.screenCoords = { width / 2, height / 2, PIPE_WIDTH, PIPE_HEIGHT };
}

Rectangle Pipe::GetScreenCoords() const {
	return img.screenCoords;
}

void Pipe::SetScreenCoords(Rectangle newValue) {
	img.screenCoords = newValue;
}

void Pipe::Draw(RenderMetaData metaData) const {
	DrawTexturePro(*(metaData.textureAtlas), img.textureAtlasCoords, img.screenCoords, { 0, 0 }, rotation, RAYWHITE);
}

/// <summary>
/// only allowing horizental moving
/// </summary>
/// <param name="velocity"></param>
void Pipe::Move(Vector2 velocity) {
	img.screenCoords.x += velocity.x;
}