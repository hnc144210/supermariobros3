#pragma once
#include "GameObject.h"

#define KOOPAS_GRAVITY 0.002f

#define KOOPAS_WALKING_SPEED 0.04f
#define KOOPAS_KICK_SPEED 0.25f

#define KOOPAS_BBOX_WIDTH 16
#define KOOPAS_BBOX_HEIGHT 26
#define KOOPAS_BBOX_HEIGHT_SHELL 16

#define KOOPAS_HOLD_OFFSET_X 10
#define KOOPAS_HOLD_OFFSET_Y 2

#define KOOPAS_STATE_WALKING   100
#define KOOPAS_STATE_SHELL     200
#define KOOPAS_STATE_SPINNING  300
#define KOOPAS_STATE_HELD      400
#define KOOPAS_STATE_WAKEUP    500

#define KOOPAS_DEFEND_TIMEOUT   5000
#define KOOPAS_WAKEUP_TIMEOUT   2000

#define ID_ANI_KOOPAS_WALKING_RIGHT   16001
#define ID_ANI_KOOPAS_WALKING_LEFT    16002
#define ID_ANI_KOOPAS_SHELL           16003
#define ID_ANI_KOOPAS_WAKEUP          16004
#define ID_ANI_KOOPAS_SPINNING        16005

class CKoopas : public CGameObject
{
protected:
    float ax, ay;

    bool isDefending = false;
    bool isSpinning = false;
    bool isHeld = false;
    bool isWakingUp = false;

    ULONGLONG defend_start = 0;
    ULONGLONG wakeup_start = 0;

    int direction = -1;

public:
    CKoopas(float x, float y);

    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
    void Render() override;
    void GetBoundingBox(float& l, float& t, float& r, float& b) override;
    void OnNoCollision(DWORD dt) override;
    void OnCollisionWith(LPCOLLISIONEVENT e) override;

    void SetState(int state) override;

    void OnStomp();
    void OnKick(float vxMario);
    void OnPickedUp();
    void OnReleased(float vxMario);
    void Respawn();

    void OnCollisionWithEnemy(LPCOLLISIONEVENT e);
    void OnCollisionWithMysteryBlock(LPCOLLISIONEVENT e);

    void TurnAround(vector<LPGAMEOBJECT>* coObjects);

    bool IsDefend() { return isDefending; }
    bool IsSpinning() { return isSpinning; }
    bool IsHeld() { return isHeld; }
    bool IsWakingUp() { return isWakingUp; }
    void SetHeld(bool held) { isHeld = held; }

    int IsCollidable() override { return 1; }
    int IsBlocking() override { return !(isDefending && !isSpinning); }
};
