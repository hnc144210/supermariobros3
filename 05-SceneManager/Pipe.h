#pragma once 
#include "GameObject.h"

#define PIPE_BBOX_WIDTH 32
#define PIPE_BODY_BBOX_WIDTH 30
#define PIPE_BODY_BBOX_HEIGHT 32
#define PIPE_SHORT_BBOX_HEIGHT 32
#define PIPE_LONG_BBOX_HEIGHT 48
#define BLACK_PIPE_BBOX_HEIGHT 16


#define ID_ANI_PIPE_LONG 12000
#define ID_ANI_PIPE_SHORT 12001
#define ID_ANI_PIPE_BODY 12002
#define ID_ANI_BLACK_PIPE_BODY 12003
#define ID_ANI_BLACK_PIPE_HEAD 12004

class CPipe : public CGameObject {
	int pipe_type;
public:
	CPipe(float x, float y, int pipe_type) : CGameObject(x, y) {
		this->pipe_type = pipe_type;
	}
	int IsCollidable() override { return 0; }
	int IsBlocking() override { return 1; }

	void GetBoundingBox(float& l, float& t, float& r, float& b) override;
	void Render() override;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override {};
};