#include "MysteryBlock.h"
#include "PlayScene.h"
#include "Coin.h"
#include "Mushroom.h"
#include "Leaf.h"

void CMysteryBlock::Render() {
	int aniId = ID_ANI_MYSTERYBLOCK;

	if (this->state == MYSTERYBLOCK_STATE_UNBOXED)
		aniId = ID_ANI_MYSTERYBLOCK_UNBOXED;

	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(aniId)->Render(x, y);
}

void CMysteryBlock::GetBoundingBox(float& l, float& t, float& r, float& b) {
	l = x - MYSTERYBLOCK_BBOX_WIDTH / 2;
	t = y - MYSTERYBLOCK_BBOX_HEIGHT / 2;
	r = l + MYSTERYBLOCK_BBOX_WIDTH;
	b = t + MYSTERYBLOCK_BBOX_HEIGHT;
}

void CMysteryBlock::OnNoCollision(DWORD dt) {
	x += vx * dt;
	y += vy * dt;

	if (y > startY) {
		this->SetState(MYSTERYBLOCK_STATE_UNBOXED);
	}
}

void CMysteryBlock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	vx += ax * dt;
	vy += ay * dt;

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CMysteryBlock::SetState(int state) {
	CGameObject::SetState(state);

	switch (state) {
	case MYSTERYBLOCK_STATE_UNBOX:
	{
		vy = -MYSTERYBLOCK_SPEED;
		ay = MYSTERYBLOCK_GRAVITY;

		isUsed = true;
		LPGAME game = CGame::GetInstance();
		LPPLAYSCENE scene = (LPPLAYSCENE)game->GetCurrentScene();
		if (itemType == ITEM_TYPE_COIN) {
			CCoin* coin = new CCoin(x, y - 16);
			coin->SetState(COIN_STATE_ARISE);
			scene->AddObject(coin);
		}
		else if (itemType == ITEM_TYPE_MUSHROOM) {
			CMushroom* mushroom = new CMushroom(x, y - 32);
			mushroom->SetState(MUSHROOM_STATE_MOVE);
			scene->AddObject(mushroom);
		}
		else if (itemType == ITEM_TYPE_LEAF) {
			CLeaf* leaf = new CLeaf(x, y - 12);
			leaf->SetState(LEAF_STATE_ARISE);
			scene->AddObject(leaf);
		}

		break;
	}
	case MYSTERYBLOCK_STATE_UNBOXED:
	{
		isUsed = true;
		y = startY;
		vx = vy = 0;
		break;
	}
	}
}