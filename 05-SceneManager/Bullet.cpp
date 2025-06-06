#include "Bullet.h"
#include "Animations.h"
#include "PlayScene.h"
#define BULLET_LIFE_SPAN 3000

CBullet::CBullet(float x, float y, float vx, float vy) : CGameObject(x, y) {
	this->vx = vx;
	this->vy = vy;
	this->ax = 0;
	this->ay = 0;
	this->lifeTimeStart = GetTickCount();
}


void CBullet::Render() {
	int aniId = ID_ANI_BULLET;
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
}

void CBullet::GetBoundingBox(float& l, float& t, float& r, float& b) {
	l = x - BULLET_BBOX_WIDTH / 2;
	t = y - BULLET_BBOX_HEIGHT / 2;
	r = l + BULLET_BBOX_WIDTH;
	b = t + BULLET_BBOX_HEIGHT;
}

void CBullet::OnNoCollision(DWORD dt) {
	x += vx * dt;
	y += vy * dt;

	if (GetTickCount() - lifeTimeStart > BULLET_LIFE_SPAN) {
		isDestroyed = true;
	}
}

void CBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	vx += ax * dt;
	vy += ay * dt;

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}
