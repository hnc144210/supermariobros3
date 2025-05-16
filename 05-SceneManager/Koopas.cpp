#include "Koopas.h"
#include "PlayScene.h"
#include "Mario.h"
#include "Platform.h"
#include "Brick.h"
#include "MysteryBlock.h"
#include "Goomba.h"
#include "Paragoomba.h"

void CKoopas::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    l = x - KOOPAS_BBOX_WIDTH / 2;
    r = x + KOOPAS_BBOX_WIDTH / 2;

    if (state == KOOPAS_STATE_WALKING)
        t = y - KOOPAS_BBOX_HEIGHT / 2, b = y + KOOPAS_BBOX_HEIGHT / 2;
    else
        t = y - KOOPAS_SHELL_BBOX_HEIGHT / 2, b = y + KOOPAS_SHELL_BBOX_HEIGHT / 2;
}

void CKoopas::OnNoCollision(DWORD dt) {
    x += vx * dt;
    y += vy * dt;
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    vy += ay * dt;
    vx += ax * dt;

    if ((state == KOOPAS_STATE_SHELL || state == KOOPAS_STATE_HELD) && GetTickCount() - shell_start > KOOPAS_RESPAWN_TIME) {
        Respawn();
    }

    CGameObject::Update(dt, coObjects);
    CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CKoopas::Render()
{
    int aniId = ID_ANI_KOOPAS_WALKING_LEFT;

    if (isDefend) {
        aniId = ID_ANI_KOOPAS_SHELL;
    }
    else if (vx > 0)
        aniId = ID_ANI_KOOPAS_WALKING_RIGHT;
    else
        aniId = ID_ANI_KOOPAS_WALKING_LEFT;

    CAnimations::GetInstance()->Get(aniId)->Render(x, y);
}

void CKoopas::SetState(int state)
{
    CGameObject::SetState(state);

    switch (state) {
    case KOOPAS_STATE_WALKING:
        vx = -KOOPAS_WALKING_SPEED;
        shell_start = 0;
        isHeld = false;
        break;

    case KOOPAS_STATE_SHELL:
        vx = 0;
        shell_start = GetTickCount();
        break;

    case KOOPAS_STATE_KICKED:
        shell_start = GetTickCount();
        break;

    case KOOPAS_STATE_HELD:
        vx = 0;
        isHeld = true;
        break;
    }
}

void CKoopas::OnCollisionWith(LPCOLLISIONEVENT e)
{
    if (!e->obj->IsBlocking()) return;
    if (dynamic_cast<CKoopas*>(e->obj)) return;

    if (e->ny != 0) vy = 0;
    else if (e->nx != 0)
    {
        vx = -vx;
        nx = -nx;
    }
}

void CKoopas::OnStomp() {
    if (state == KOOPAS_STATE_WALKING)
        SetState(KOOPAS_STATE_SHELL);
    else if (state == KOOPAS_STATE_KICKED)
        SetState(KOOPAS_STATE_SHELL);
}

void CKoopas::OnKick(float mario_vx) {
    SetState(KOOPAS_STATE_KICKED);
    vx = mario_vx > 0 ? KOOPAS_SHELL_SPEED : -KOOPAS_SHELL_SPEED;
}

void CKoopas::OnPickedUp() {
    SetState(KOOPAS_STATE_HELD);
}

void CKoopas::OnReleased(float mario_vx) {
    SetState(KOOPAS_STATE_KICKED);
    vx = mario_vx > 0 ? KOOPAS_SHELL_SPEED : -KOOPAS_SHELL_SPEED;
    isHeld = false;
}

void CKoopas::Respawn() {
    SetState(KOOPAS_STATE_WALKING);
}