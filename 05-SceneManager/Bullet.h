#pragma once
#include "GameObject.h"

#define BULLET_BBOX_WIDTH 8
#define BULLET_BBOX_HEIGHT 8

#define BULLET_SPEED 0.1f

#define ID_ANI_BULLET 18000

class CBullet : public CGameObject {
protected:
	float ax, ay;
	bool isDestroyed = false;
	int direction;
	DWORD lifeTimeStart = 0;

public:
	CBullet(float x, float y, float targetX, float targetY);

	void Render() override;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
	void GetBoundingBox(float& l, float& t, float& r, float& b) override;
	void OnNoCollision(DWORD dt) override;

	int GetDirection() { return direction; }
	bool IsDestroyed() { return isDestroyed; }
};
