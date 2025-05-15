#include "MisteryBlock.h"

void CMisteryBlock::Render() {
	int aniId = ID_ANI_MISTERYBLOCK;

	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(aniId)->Render(x, y);
}

void CMisteryBlock::GetBoundingBox(float& l, float& t, float& r, float& b) {
	l = x - MISTERYBLOCK_BBOX_WIDTH / 2;
	t = y - MISTERYBLOCK_BBOX_HEIGHT / 2;
	r = l + MISTERYBLOCK_BBOX_WIDTH;
	b = t + MISTERYBLOCK_BBOX_HEIGHT;
}