#include "Mushroom.h"
#include "Mario.h"
#include "Animations.h"

CMushroom::CMushroom(float x, float y) : CGameObject(x, y) {
    vx = MUSHROOM_SPEED_X;
    ax = 0;
    ay = MUSHROOM_GRAVITY;
}

void CMushroom::Render() {
    if (isUsed) return;

    CAnimations* animations = CAnimations::GetInstance();
    animations->Get(ID_ANI_MUSHROOM)->Render(x, y);
}

void CMushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
    vy += ay * dt;
	vx += ax * dt;
    CGameObject::Update(dt, coObjects);
    CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CMushroom::GetBoundingBox(float& l, float& t, float& r, float& b) {
	l = x - MUSHROOM_BBOX_WIDTH / 2;
	t = y - MUSHROOM_BBOX_HEIGHT / 2;
	r = l + MUSHROOM_BBOX_WIDTH;
	b = t + MUSHROOM_BBOX_HEIGHT;
}

void CMushroom::OnCollisionWith(LPCOLLISIONEVENT e) {
   if (e->ny != 0) {
        vy = 0;
    }
    else if (e->nx != 0) {
        vx = -vx;
    }
}
void CMushroom::OnNoCollision(DWORD dt) {
	x += vx * dt;
	y += vy * dt;
}
void CMushroom::SetState(int state) {
    CGameObject::SetState(state);
    switch (state) {
    case MUSHROOM_STATE_MOVE:
        vx = MUSHROOM_SPEED_X;
        vy = 0;
        break;

    case MUSHROOM_STATE_ARISE:
        vy = -0.08f;
        break;

    case MUSHROOM_STATE_EATEN:
        isDeleted = true;
        break;
    }
}