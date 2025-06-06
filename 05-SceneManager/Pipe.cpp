#include "Pipe.h"
#include "PlayScene.h"

void CPipe::Render() {
	int aniId = ID_ANI_PIPE_LONG;
	if (pipe_type == 1) aniId = ID_ANI_PIPE_LONG;
	else if (pipe_type == 2) aniId = ID_ANI_PIPE_SHORT;
	else if (pipe_type == 3) aniId = ID_ANI_PIPE_BODY;
	else if (pipe_type == 4) aniId = ID_ANI_BLACK_PIPE_BODY;
	else if (pipe_type == 5) aniId = ID_ANI_BLACK_PIPE_HEAD;

	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(aniId)->Render(x, y);
}

void CPipe::GetBoundingBox(float& l, float& t, float& r, float& b) {
	if (pipe_type == 2) {
		l = x - PIPE_BBOX_WIDTH / 2;
		t = y - PIPE_SHORT_BBOX_HEIGHT / 2;
		r = l + PIPE_BBOX_WIDTH;
		b = t + PIPE_SHORT_BBOX_HEIGHT;
	}
	else if (pipe_type == 1) {
		l = x - PIPE_BBOX_WIDTH / 2;
		t = y - PIPE_LONG_BBOX_HEIGHT / 2;
		r = l + PIPE_BBOX_WIDTH;
		b = t + PIPE_LONG_BBOX_HEIGHT;
	}
	else if (pipe_type == 3) {
		l = x - PIPE_BODY_BBOX_WIDTH / 2;
		t = y - PIPE_BODY_BBOX_HEIGHT / 2;
		r = l + PIPE_BODY_BBOX_WIDTH;
		b = t + PIPE_BODY_BBOX_HEIGHT;
	}
	else {
		l = x - PIPE_BBOX_WIDTH / 2;
		t = y - BLACK_PIPE_BBOX_HEIGHT / 2;
		r = l + PIPE_BBOX_WIDTH;
		b = t + BLACK_PIPE_BBOX_HEIGHT;
	}
}