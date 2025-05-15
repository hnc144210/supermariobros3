#pragma once
#include "GameObject.h"

#define PARAGOOMBA_WALKING_SPEED 0.04f
#define PARAGOOMBA_GRAVITY 0.002f
#define PARAGOOMBA_JUMP_SPEED -0.4f

#define PARAGOOMBA_JUMP_INTERVAL 2000

#define PARAGOOMBA_BBOX_WIDTH 16
#define PARAGOOMBA_BBOX_HEIGHT 16
#define PARAGOOMBA_BBOX_HEIGHT_NO_WING 14
#define PARAGOOMBA_BBOX_HEIGHT_DIE 7

#define PARAGOOMBA_STATE_WALKING 100
#define PARAGOOMBA_STATE_NO_WING 200
#define PARAGOOMBA_STATE_DIE 300

#define ID_ANI_PARAGOOMBA_WALKING 14000
#define ID_ANI_PARAGOOMBA_NOWING 14001
#define ID_ANI_PARAGOOMBA_DIE 14002

class CParaGoomba : public CGameObject
{
protected:
	float ax;
	float ay;

	bool hasWing;
	ULONGLONG die_start;
	ULONGLONG last_jump_time;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
public:
	CParaGoomba(float x, float y);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);

	virtual int IsCollidable() { return 1; }
	virtual int IsBlocking() { return 0; }
};
