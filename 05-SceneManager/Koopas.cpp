#include "Koopas.h"
#include "Goomba.h"
#include "Paragoomba.h"
#include "Platform.h"
#include "Brick.h"
#include "MysteryBlock.h"
#include "Animations.h"
#include "PlayScene.h"
#include "debug.h"
#include "Brick3.h"

CKoopas::CKoopas(float x, float y, int type) : CGameObject(x, y)
{
    this->type = type;
    ax = 0;
    ay = KOOPAS_GRAVITY;
    direction = -1;
    SetState(KOOPAS_STATE_WALKING);
}


void CKoopas::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    l = x - KOOPAS_BBOX_WIDTH / 2;
    r = x + KOOPAS_BBOX_WIDTH / 2;

    if (isDefending)
        t = y - KOOPAS_BBOX_HEIGHT_SHELL / 2, b = y + KOOPAS_BBOX_HEIGHT_SHELL / 2;
    else
        t = y - KOOPAS_BBOX_HEIGHT / 2, b = y + KOOPAS_BBOX_HEIGHT / 2;
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    vy += ay * dt;
    vx += ax * dt;
    isOnGround = false;

    if (isDefending && !isWakingUp && GetTickCount64() - defend_start >= KOOPAS_DEFEND_TIMEOUT)
    {
        SetState(KOOPAS_STATE_WAKEUP);
    }
    else if (isWakingUp && GetTickCount64() - wakeup_start >= KOOPAS_WAKEUP_TIMEOUT)
    {
        y -= (KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_SHELL) / 2;
        SetState(KOOPAS_STATE_WALKING);
    }


    CGameObject::Update(dt, coObjects);
    CCollision::GetInstance()->Process(this, dt, coObjects);


    if (type == KOOPAS_TYPE_GREEN_WING &&
        state == KOOPAS_STATE_WALKING &&
        isOnGround &&
        GetTickCount64() - jump_start > KOOPAS_JUMP_INTERVAL)
    {
        vy = -KOOPAS_JUMP_SPEED;
        isOnGround = false;
        jump_start = GetTickCount64();
    }

    if (type == KOOPAS_TYPE_RED)
        TurnAround(coObjects);
}


void CKoopas::Render()
{
    int aniId = -1;

    if (isDefending)
    {
        if (isSpinning)
        {
            aniId = (type == KOOPAS_TYPE_GREEN) ? ID_ANI_KOOPAS_GREEN_SPINNING : ID_ANI_KOOPAS_SPINNING;
        }
        else if (isWakingUp)
        {
            aniId = (type == KOOPAS_TYPE_GREEN) ? ID_ANI_KOOPAS_GREEN_WAKEUP : ID_ANI_KOOPAS_WAKEUP;
        }
        else
        {
            aniId = (type == KOOPAS_TYPE_GREEN) ? ID_ANI_KOOPAS_GREEN_SHELL : ID_ANI_KOOPAS_SHELL;
        }
    }
    else
    {
        switch (type)
        {
        case KOOPAS_TYPE_GREEN:
            aniId = (vx > 0) ? ID_ANI_KOOPAS_GREEN_WALKING_RIGHT : ID_ANI_KOOPAS_GREEN_WALKING_LEFT;
            break;
        case KOOPAS_TYPE_GREEN_WING:
            aniId = (vx > 0) ? ID_ANI_KOOPAS_GREEN_WING_WALKING_RIGHT : ID_ANI_KOOPAS_GREEN_WING_WALKING_LEFT;
            break;
        default:
            aniId = (vx > 0) ? ID_ANI_KOOPAS_WALKING_RIGHT : ID_ANI_KOOPAS_WALKING_LEFT;
            break;
        }
    }

    CAnimations::GetInstance()->Get(aniId)->Render(x, y);
}


void CKoopas::OnNoCollision(DWORD dt)
{
    x += vx * dt;
    y += vy * dt;
}

void CKoopas::OnCollisionWith(LPCOLLISIONEVENT e)
{
    if (dynamic_cast<CGoomba*>(e->obj) || dynamic_cast<CParaGoomba*>(e->obj))
        OnCollisionWithEnemy(e);

    if (!e->obj->IsBlocking()) return;

    if (e->ny != 0)
    {
        vy = 0;
        isOnGround = true;
    }
    else if (e->nx != 0 && !isHeld)
    {
        vx = -vx;
        nx = -nx;
    }

    if (dynamic_cast<CMysteryBlock*>(e->obj))
        OnCollisionWithMysteryBlock(e);
    if (dynamic_cast<CBrick3*>(e->obj))
        OnCollisionWithBrick3(e);
}


void CKoopas::SetState(int state)
{
    CGameObject::SetState(state);

    switch (state)
    {
    case KOOPAS_STATE_WALKING:
        isDefending = isSpinning = isHeld = isWakingUp = false;

        if (type == KOOPAS_TYPE_GREEN_WING)
            vx = direction * KOOPAS_GREEN_WING_SPEED;
        else
            vx = direction * KOOPAS_WALKING_SPEED;

        break;

    case KOOPAS_STATE_SHELL:
        isDefending = true;
        isSpinning = isHeld = isWakingUp = false;
        vx = 0;
        defend_start = GetTickCount64();
        break;

    case KOOPAS_STATE_SPINNING:
        isDefending = true;
        isSpinning = true;
        isHeld = isWakingUp = false;
        vx = KOOPAS_KICK_SPEED * nx;
        break;

    case KOOPAS_STATE_HELD:
        isHeld = true;
        isSpinning = false;
        vx = 0;
        vy = 0;
        break;

    case KOOPAS_STATE_WAKEUP:
        isWakingUp = true;
        wakeup_start = GetTickCount64();
        break;
    }
}

void CKoopas::OnStomp()
{
    if (!isDefending)
    {
        SetState(KOOPAS_STATE_SHELL);
    }
    else if (isSpinning)
    {
        SetState(KOOPAS_STATE_SHELL);
    }
}

void CKoopas::OnKick(float vxMario)
{
    nx = (vxMario > 0) ? 1 : -1;
    SetState(KOOPAS_STATE_SPINNING);
}

void CKoopas::OnPickedUp()
{
    SetState(KOOPAS_STATE_HELD);
}

void CKoopas::OnReleased(float vxMario)
{
    OnKick(vxMario);
    isHeld = false;
}

void CKoopas::Respawn()
{
    SetState(KOOPAS_STATE_WALKING);
}

void CKoopas::OnCollisionWithEnemy(LPCOLLISIONEVENT e)
{
    if (!isSpinning) return;

    if (dynamic_cast<CGoomba*>(e->obj))
    {
        CGoomba* g = dynamic_cast<CGoomba*>(e->obj);
        g->SetState(GOOMBA_STATE_DIE);
    }
    else if (dynamic_cast<CParaGoomba*>(e->obj))
    {
        CParaGoomba* pg = dynamic_cast<CParaGoomba*>(e->obj);
        if (pg->GetState() == PARAGOOMBA_STATE_WALKING)
            pg->SetState(PARAGOOMBA_STATE_NO_WING);
        else
            pg->SetState(PARAGOOMBA_STATE_DIE);
    }
}


void CKoopas::OnCollisionWithMysteryBlock(LPCOLLISIONEVENT e)
{
    if (!isSpinning || e->nx == 0) return;

    CMysteryBlock* block = dynamic_cast<CMysteryBlock*>(e->obj);
    if (!block->IsUsed())
    {
        block->SetState(MYSTERYBLOCK_STATE_UNBOX);
    }
}

void CKoopas::OnCollisionWithBrick3(LPCOLLISIONEVENT e)
{
    if (!isSpinning || e->nx == 0) return;

    CBrick3* brick = dynamic_cast<CBrick3*>(e->obj);
    brick->Delete();
}

void CKoopas::TurnAround(vector<LPGAMEOBJECT>* coObjects) {
    if (state != KOOPAS_STATE_WALKING || isHeld || isSpinning) return;

    float edgeX = x + (vx > 0 ? KOOPAS_BBOX_WIDTH / 2 : -KOOPAS_BBOX_WIDTH / 2);
    float probeX = edgeX + (vx > 0 ? 2 : -2);
    float probeY = y + KOOPAS_BBOX_HEIGHT / 2 + 1;

    RECT footProbe;
    footProbe.left = (int)(probeX - 1);
    footProbe.right = (int)(probeX + 1);
    footProbe.top = (int)(probeY - 1);
    footProbe.bottom = (int)(probeY + 1);

    bool hasGround = false;

    for (LPGAMEOBJECT obj : *coObjects)
    {
        if (!obj->IsBlocking()) continue;

        float l, t, r, b;
        obj->GetBoundingBox(l, t, r, b);

        if (!(footProbe.right < l || footProbe.left > r || footProbe.bottom < t || footProbe.top > b))
        {
            hasGround = true;
            break;
        }
    }

    if (!hasGround)
    {
        nx = -nx;
        vx = -vx;
    }
}
