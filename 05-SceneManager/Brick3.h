#pragma once

#include "GameObject.h"

#define ID_ANI_BRICK3 21000

#define BRICK3_BBOX_WIDTH 16
#define BRICK3_BBOX_HEIGHT 16

class CBrick3 : public CGameObject {
public:
	CBrick3(float x, float y) : CGameObject(x, y) {}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

