#include "include\Models.h"

Player::Player() : score(0), rotation(0.0f), velocity({0.0f, 0.0f}) {
	float height = GetScreenHeight();
	float width = GetScreenWidth();

	img.textureAtlasCoords = textureAtlasCoords[curAnimFrameIndex];
	img.screenCoords = { 30, (height - PLAYER_HEIGHT) / 2, PLAYER_WIDTH, PLAYER_HEIGHT };
}

void Player::Draw(RenderMetaData metaData) {
	DrawTexturePro(*(metaData.textureAtlas), textureAtlasCoords[curAnimFrameIndex], img.screenCoords, {0, 0}, rotation, RAYWHITE);

	// Player Flying Animation
	if ((int)(GetTime()* 60) % 10 == 0) {
		++curAnimFrameIndex;
		if (curAnimFrameIndex >= NO_OF_PLAYER_ANIM_FRAMES) {
			curAnimFrameIndex = 0;
		}
	}
}

/// only allowing the movement on the y coordinate
void Player::Move(Vector2 velocity) {
	this->velocity.y += velocity.y;
	if ((img.screenCoords.y + this->velocity.y) > 0) {
		float currentVelocity = this->velocity.y * GetFrameTime() * PLAYER_SPEED;
		img.screenCoords.y += currentVelocity;
		//rotation = velocity.y * PLAYER_ROTATION_SPEED;
	}
	else {
		float currentVelocity = this->velocity.y * GetFrameTime() * PLAYER_SPEED;
		img.screenCoords.y += currentVelocity;
	}
}

bool Player::IsDead() const {
	return isDead;
}

void Player::SetIsDead(bool isDead) {
	this->isDead = isDead;
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
		img.textureAtlasCoords = { 110.0f, 650.0f, 52.0f, 320.0f };
	}
	else {
		img.textureAtlasCoords = { 170.0f, 640.0f, 52.0f, 320.0f };
	}
	img.screenCoords = { width / 2.0f, height / 2.0f, PIPE_WIDTH, PIPE_HEIGHT };
}

Rectangle Pipe::GetScreenCoords() const {
	return img.screenCoords;
}

void Pipe::SetScreenCoords(Rectangle newValue) {
	img.screenCoords = newValue;
}

void Pipe::Draw(RenderMetaData metaData) const {
	DrawTexturePro(*(metaData.textureAtlas), img.textureAtlasCoords, img.screenCoords, { 0.0f, 0.0f }, rotation, RAYWHITE);
}

/// <summary>
/// only allowing horizental moving
/// </summary>
/// <param name="velocity"></param>
void Pipe::Move(Vector2 velocity) {
	img.screenCoords.x += velocity.x;
}

Floor::Floor() {
	float height = GetScreenHeight();
	float width = GetScreenWidth();

	img.textureAtlasCoords = { 586.0f, 0.0f, FLOOR_WIDTH, 112.0f };
	img.screenCoords = { 0.0f, height - FLOOR_HEIGHT, FLOOR_WIDTH, FLOOR_HEIGHT };
}

Rectangle Floor::GetScreenCoords() const {
	return img.screenCoords;
}

void Floor::SetScreenCoords(Rectangle newValue) {
	img.screenCoords = newValue;
}

void Floor::Draw(RenderMetaData metaData) const {
	DrawTexturePro(*(metaData.textureAtlas), img.textureAtlasCoords, img.screenCoords, { 0.0f, 0.0f }, 0.0f, RAYWHITE);
}

void Floor::Move(Vector2 velocity) {
	img.screenCoords.x += velocity.x;
}