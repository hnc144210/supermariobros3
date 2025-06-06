#pragma once

#include "GameObject.h"

#define ID_ANI_BRICK2 20000

#define BRICK2_BBOX_WIDTH 16
#define BRICK2_BBOX_HEIGHT 16

class CBrick2 : public CGameObject {
public:
	CBrick2(float x, float y) : CGameObject(x, y) {}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

