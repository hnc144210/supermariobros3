#pragma once
#include "GameObject.h"

#define PIRANHA_BBOX_WIDTH        16
#define PIRANHA_BBOX_HEIGHT       24

#define PIRANHA_RISE_SPEED        0.05f
#define PIRANHA_WAIT_TIME         2000

#define PIRANHA_STATE_RISE        100
#define PIRANHA_STATE_HIDE        200
#define PIRANHA_RISE_HEIGHT       33.0f

#define ID_ANI_PIRANHA_LEFT_BOTTOM     15000
#define ID_ANI_PIRANHA_LEFT_TOP        15001
#define ID_ANI_PIRANHA_RIGHT_BOTTOM    15002
#define ID_ANI_PIRANHA_RIGHT_TOP       15003

class CPiranhaPlant : public CGameObject {
protected:
    float startY;
    DWORD state_start;
    bool isRising;

public:
    CPiranhaPlant(float x, float y);

    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
    void Render() override;
    void GetBoundingBox(float& l, float& t, float& r, float& b) override;
    void SetState(int state) override;
};
