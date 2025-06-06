#include "Brick2.h"

void CBrick2::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_BRICK2)->Render(x, y);
}

void CBrick2::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - BRICK2_BBOX_WIDTH / 2;
	t = y - BRICK2_BBOX_HEIGHT / 2;
	r = l + BRICK2_BBOX_WIDTH;
	b = t + BRICK2_BBOX_HEIGHT;
}