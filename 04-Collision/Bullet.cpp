#include "Bullet.h"
#include "Goomba.h"

void CBullet::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BULLET_BBOX_WIDTH;	
	bottom = y + BULLET_BBOX_HEIGHT;
}

void CBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	if (nx < 0) {
		SetSpeed(-BULLET_SPEED, 0.0f);
		if (x < startPositionX - BULLET_FLYING_SPACE)
		{
			visible = false;
			return;
			
		}
		else {
			x += dx;
			y += dy;
		}

	}
	else {
		SetSpeed(BULLET_SPEED, 0.0f);
		if (x > startPositionX + BULLET_FLYING_SPACE) {
			visible = false;
			return;

		}
		else
		{
			x += dx;
			y += dy;
		}
	}


	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	CalcPotentialCollisions(coObjects, coEvents);
			

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		// block 
		x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;

		// Collision logic with Goombas
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				if (e->nx != 0)
				{
					if (goomba->GetState() != GOOMBA_STATE_DIE)
					{
						goomba->SetState(GOOMBA_STATE_DIE);
						goomba->visible = false;
					}
				}				
			}
		}
	}


}

void CBullet::Render()
{
	int ani;
	if (nx == -1) {
		ani = BULLET_ANI_FLYING;
		animations[ani]->Render(x, y, -1);
	}
	else
		animations[0]->Render(x, y, 1);

}

void CBullet::SetState(int state)
{
	CGameObject::SetState(state);
	return;
}

void CBullet::SetStartPositon(float x, float y, int nX)
{
	startPositionX = x;
	startPositionY = y;
	nx = nX;
}
