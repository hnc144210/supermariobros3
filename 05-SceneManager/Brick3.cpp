#include "Brick3.h"

void CBrick3::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_BRICK3)->Render(x, y);
}

void CBrick3::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - BRICK3_BBOX_WIDTH / 2;
	t = y - BRICK3_BBOX_HEIGHT / 2;
	r = l + BRICK3_BBOX_WIDTH;
	b = t + BRICK3_BBOX_HEIGHT;
}