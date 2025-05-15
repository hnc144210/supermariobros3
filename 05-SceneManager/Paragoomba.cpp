#include "ParaGoomba.h"
#include "Mario.h"
#include "PlayScene.h"

CParaGoomba::CParaGoomba(float x, float y) : CGameObject(x, y)
{
	ax = 0;
	ay = PARAGOOMBA_GRAVITY;
	hasWing = true;
	die_start = -1;
	last_jump_time = GetTickCount64();
	SetState(PARAGOOMBA_STATE_WALKING);
}

void CParaGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - PARAGOOMBA_BBOX_WIDTH / 2;
	right = left + PARAGOOMBA_BBOX_WIDTH;

	if (state == PARAGOOMBA_STATE_DIE)
	{
		top = y - PARAGOOMBA_BBOX_HEIGHT_DIE / 2;
		bottom = top + PARAGOOMBA_BBOX_HEIGHT_DIE;
	}
	else if (hasWing)
	{
		top = y - PARAGOOMBA_BBOX_HEIGHT / 2;
		bottom = top + PARAGOOMBA_BBOX_HEIGHT;
	}
	else
	{
		top = y - PARAGOOMBA_BBOX_HEIGHT_NO_WING / 2;
		bottom = top + PARAGOOMBA_BBOX_HEIGHT_NO_WING;
	}
}

void CParaGoomba::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CParaGoomba::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;

	if (dynamic_cast<CParaGoomba*>(e->obj)) return;

	if (e->ny != 0) vy = 0;
	else if (e->nx != 0) vx = -vx;

	if (dynamic_cast<CMario*>(e->obj) && e->ny < 0)
	{
		if (hasWing)
		{
			hasWing = false;
			SetState(PARAGOOMBA_STATE_NO_WING);
		}
		else
		{
			SetState(PARAGOOMBA_STATE_DIE);
		}
	}
}

void CParaGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if (state == PARAGOOMBA_STATE_DIE && GetTickCount64() - die_start > 500)
	{
		isDeleted = true;
		return;
	}

	if (hasWing && GetTickCount64() - last_jump_time >= PARAGOOMBA_JUMP_INTERVAL)
	{
		vy = PARAGOOMBA_JUMP_SPEED;
		last_jump_time = GetTickCount64();
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CParaGoomba::Render()
{
	int aniId = ID_ANI_PARAGOOMBA_WALKING;
	if (state == PARAGOOMBA_STATE_DIE)
		aniId = ID_ANI_PARAGOOMBA_DIE;
	else if (!hasWing)
		aniId = ID_ANI_PARAGOOMBA_NOWING;

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
}

void CParaGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case PARAGOOMBA_STATE_WALKING:
		vx = -PARAGOOMBA_WALKING_SPEED;
		break;
	case PARAGOOMBA_STATE_NO_WING:
		hasWing = false;
		vx = -PARAGOOMBA_WALKING_SPEED;
		break;
	case PARAGOOMBA_STATE_DIE:
		die_start = GetTickCount64();
		vx = 0;
		vy = 0;
		ay = 0;
		y += (hasWing ? PARAGOOMBA_BBOX_HEIGHT : PARAGOOMBA_BBOX_HEIGHT_NO_WING - PARAGOOMBA_BBOX_HEIGHT_DIE) / 2;
		break;
	}
}
