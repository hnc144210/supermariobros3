#pragma once
#include "GameObject.h"

#define LEAF_BBOX_WIDTH 14
#define LEAF_BBOX_HEIGHT 16

#define LEAF_GRAVITY 0.0003f
#define LEAF_FLOAT_SPEED 0.015f
#define LEAF_FLOAT_PERIOD 800

#define LEAF_RISE_SPEED 0.18f
#define LEAF_RISE_HEIGHT 22.0f

#define ID_ANI_LEAF 19000

#define LEAF_STATE_ARISE 0

class CLeaf : public CGameObject
{
protected:
    float ay;

public:
    CLeaf(float x, float y);

    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
    void Render() override;
    void GetBoundingBox(float& l, float& t, float& r, float& b) override;
    void OnNoCollision(DWORD dt) override;

    void SetState(int state) override;

    int IsCollidable() override { return 1; }
    int IsBlocking() override { return 0; }
};