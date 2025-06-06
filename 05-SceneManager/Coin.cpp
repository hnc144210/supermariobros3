#include "Coin.h"

void CCoin::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_COIN)->Render(x, y);

	//RenderBoundingBox();
}

void CCoin::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - COIN_BBOX_WIDTH / 2;
	t = y - COIN_BBOX_HEIGHT / 2;
	r = l + COIN_BBOX_WIDTH;
	b = t + COIN_BBOX_HEIGHT;
}

void CCoin::OnNoCollision(DWORD dt) {
	x += vx * dt;
	y += vy * dt;
}

void CCoin::Update(DWORD dt, std::vector<LPGAMEOBJECT>* coObjects) {
	vx += ax * dt;
	vy += ay * dt;

	if (state == COIN_STATE_ARISE) {
		if (GetTickCount64() - bornTime >= COIN_TIME_OUT) {
			this->Delete();
		}
	}

	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CCoin::SetState(int state) {
	CGameObject::SetState(state);

	switch (state) {
	case COIN_STATE_ARISE:
	{
		vy = -COIN_SPEED;
		ay = COIN_GRAVITY;
		bornTime = GetTickCount64();
		break;
	}
	case COIN_STATE_IDLE:
	{
		vx = vy = 0;
		ay = 0;
		break;
	}
	}
}