#include "Leaf.h"
#include "Animations.h"
#include "PlayScene.h"
#include "Mario.h"

CLeaf::CLeaf(float x, float y) : CGameObject(x, y)
{
    ay = LEAF_GRAVITY;
    SetState(LEAF_STATE_ARISE);
}

void CLeaf::OnNoCollision(DWORD dt) {
    x += vx * dt;
    y += vy * dt;
}

void CLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    vy += ay * dt;

    CGameObject::Update(dt, coObjects);
    CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CLeaf::Render()
{
    CAnimations::GetInstance()->Get(ID_ANI_LEAF)->Render(x, y);
}

void CLeaf::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    l = x - LEAF_BBOX_WIDTH / 2;
    t = y - LEAF_BBOX_HEIGHT / 2;
    r = l + LEAF_BBOX_WIDTH;
    b = t + LEAF_BBOX_HEIGHT;
}

void CLeaf::SetState(int state)
{
    CGameObject::SetState(state);

    switch (state)
    {
    case LEAF_STATE_ARISE:
        vy = -LEAF_RISE_SPEED;
        vx = 0;
        break;
    }
}