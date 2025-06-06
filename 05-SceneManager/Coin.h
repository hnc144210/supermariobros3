#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_COIN 11000

#define	COIN_WIDTH 10
#define COIN_BBOX_WIDTH 10
#define COIN_BBOX_HEIGHT 16
#define COIN_SPEED 0.6f
#define COIN_GRAVITY 0.005f
#define COIN_TIME_OUT 200

#define COIN_STATE_ARISE 100
#define COIN_STATE_IDLE	200

class CCoin : public CGameObject {
	float ax, ay;
	DWORD bornTime;
public:
	CCoin(float x, float y) : CGameObject(x, y) {
		ax = ay = 0;
		SetState(COIN_STATE_IDLE);
	}
	void Render() override;
	void Update(DWORD dt, std::vector<LPGAMEOBJECT>* coObjects) override;
	void OnNoCollision(DWORD dt) override;
	void GetBoundingBox(float& l, float& t, float& r, float& b) override;

	int IsBlocking() override { return 0; }
	void SetState(int state) override;
};