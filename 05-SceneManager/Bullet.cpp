#include "Bullet.h"
#include "Animations.h"
#include "PlayScene.h"
#define BULLET_LIFE_SPAN 3000

CBullet::CBullet(float x, float y, float targetX, float targetY) : CGameObject(x, y) {
	float dx = targetX - x;
	float dy = targetY - y;
	float length = sqrt(dx * dx + dy * dy);

	vx = BULLET_SPEED * dx / length;
	vy = BULLET_SPEED * dy / length;

	ax = ay = 0;
	lifeTimeStart = GetTickCount();
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
