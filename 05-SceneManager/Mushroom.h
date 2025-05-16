#pragma once
#include "GameObject.h"

#define MUSHROOM_BBOX_WIDTH  16
#define MUSHROOM_BBOX_HEIGHT 16

#define MUSHROOM_SPEED_X  0.03f
#define MUSHROOM_GRAVITY  0.002f

#define ID_ANI_MUSHROOM   17000

#define MUSHROOM_STATE_ARISE 50
#define MUSHROOM_STATE_MOVE 100
#define MUSHROOM_STATE_EATEN 200


class CMushroom : public CGameObject {
protected:
    float ax;
    float ay;
    bool isUsed = false;

public:
    CMushroom(float x, float y);

    void Render() override;
    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
    void GetBoundingBox(float& l, float& t, float& r, float& b) override;
    void OnCollisionWith(LPCOLLISIONEVENT e) override;
    void OnNoCollision(DWORD dt) override;
    int IsCollidable() override { return !isUsed; }
    int IsBlocking() override { return 0; }

    void SetState(int state) override;
    bool IsUsed() { return isUsed; }
};
