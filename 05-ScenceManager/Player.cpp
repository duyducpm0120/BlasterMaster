#include "PLAYER.h"
#include <algorithm>

#include "Game.h"
#include "Portal.h"
#include "Utils.h"
#include "Golem.h"
#include "Item.h"
#include "Flame.h"
CPlayer::CPlayer(float x, float y) : CGameObject()
{
	bulletLevel = 1;
	enableRocket = true;
	isJumping = false;
	health = PLAYER_START_HEALTH;
	damage = PLAYER_START_DAMAGE;
	untouchable = 0;
	SetState(PLAYER_STATE_IDLE_RIGHT);
	PLAYER_width = PLAYER_NORMAL_WIDTH;
	PLAYER_height = PLAYER_NORMAL_HEIGHT;
	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
}

void CPlayer::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	
}

void CPlayer::Render()
{

	

}

void CPlayer::SetState(int state)
{
	

}

void CPlayer::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	
}
void CPlayer::SetDimension(int width, int height)
{
	this->PLAYER_width = width;
	this->PLAYER_height = height;
}
void CPlayer::GetDimension(int& width, int& height)
{
	width = this->PLAYER_width;
	height = this->PLAYER_height;
}
void CPlayer::Reset()
{
	SetState(PLAYER_STATE_IDLE_RIGHT);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}