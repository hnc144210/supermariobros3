#pragma once 
#include "GameObject.h"

#define PIPE_BBOX_WIDTH 32
#define PIPE_BBOX_HEIGHT 32

#define ID_ANI_PIPE 12000


class CPipe : public CGameObject {
public:
	CPipe(float x, float y) : CGameObject(x, y) {
	}
	int IsCollidable() override { return 0; }
	int IsBlocking() override { return 1; }

	void GetBoundingBox(float& l, float& t, float& r, float& b) override;
	void Render() override;
};