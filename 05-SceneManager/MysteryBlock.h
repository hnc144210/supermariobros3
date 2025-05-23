#pragma once
#include "GameObject.h"
#define MYSTERYBLOCK_BBOX_WIDTH 16
#define MYSTERYBLOCK_BBOX_HEIGHT 16

#define MYSTERYBLOCK_STATE_UNBOX 100
#define MYSTERYBLOCK_STATE_UNBOXED 200

#define MYSTERYBLOCK_SPEED 0.2
#define MYSTERYBLOCK_GRAVITY 0.002

#define ID_ANI_MYSTERYBLOCK 13000
#define ID_ANI_MYSTERYBLOCK_UNBOXED 13001

#define ITEM_TYPE_COIN 1
#define ITEM_TYPE_MUSHROOM 2
#define ITEM_TYPE_LEAF 3


class CMysteryBlock : public CGameObject {
	bool isUsed = false;
	float ax, ay;
	float startY;
	int itemType;
public:
	CMysteryBlock(float x, float y, int itemType = ITEM_TYPE_COIN) : CGameObject(x, y) {
		ax = ay = 0;
		startY = y;
		this->itemType = itemType;
	}

	int IsCollidable() override { return 0; }
	int IsBlocking() override { return 1; }

	void GetBoundingBox(float& l, float& t, float& r, float& b) override;
	void Render() override;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
	void OnNoCollision(DWORD dt) override;
	void SetState(int state) override;
	bool IsUsed() { return isUsed; }
};