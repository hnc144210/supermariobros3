#pragma once
#include "GameObject.h"

#define PIRANHA_BBOX_WIDTH        16
#define PIRANHA_BBOX_HEIGHT       33

#define PIRANHA_RISE_SPEED        0.05f
#define PIRANHA_WAIT_TIME         3000

#define PIRANHA_STATE_RISE        100
#define PIRANHA_STATE_HIDE        200
#define PIRANHA_RISE_HEIGHT       33.0f

#define ID_ANI_PIRANHA_LEFT_BOTTOM     15000
#define ID_ANI_PIRANHA_LEFT_TOP        15001
#define ID_ANI_PIRANHA_RIGHT_BOTTOM    15002
#define ID_ANI_PIRANHA_RIGHT_TOP       15003

#define ID_ANI_PIRANHA3_LEFT_BOTTOM   15004
#define ID_ANI_PIRANHA3_LEFT_TOP      15005
#define ID_ANI_PIRANHA3_RIGHT_BOTTOM  15006
#define ID_ANI_PIRANHA3_RIGHT_TOP     15007

#define ID_ANI_PIRANHA2_IDLE          15008

#define PIRANHA_TYPE_SHOOT_HIGH   1
#define PIRANHA_TYPE_IDLE_LOW     2
#define PIRANHA_TYPE_SHOOT_MEDIUM 3


class CPiranhaPlant : public CGameObject {
protected:
    float startY;
    DWORD state_start;
    bool isRising;
    int type;
    bool isShooting = false;
    float triggerDistance = 100.0f;
public:
    CPiranhaPlant(float x, float y, int type);
    void ShootBullet();
    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
    void Render() override;
    void GetBoundingBox(float& l, float& t, float& r, float& b) override;
    void SetState(int state) override;
    int IsCollidable() override;
    void SetTriggerDistance(float d) { triggerDistance = d; }
};
