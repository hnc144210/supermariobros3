#pragma once
#include "GameObject.h"

#define KOOPAS_GRAVITY 0.002f

#define KOOPAS_WALKING_SPEED 0.04f
#define KOOPAS_SHELL_SPEED 0.25f


#define KOOPAS_BBOX_WIDTH 16
#define KOOPAS_BBOX_HEIGHT 26
#define KOOPAS_SHELL_BBOX_HEIGHT 16

#define KOOPAS_RESPAWN_TIME 5000

#define KOOPAS_STATE_WALKING 100
#define KOOPAS_STATE_SHELL 200
#define KOOPAS_STATE_COMEBACK 300
#define KOOPAS_STATE_KICKED 400
#define KOOPAS_STATE_HELD 500

#define ID_ANI_KOOPAS_WALKING_RIGHT 16001
#define ID_ANI_KOOPAS_WALKING_LEFT 16002
#define ID_ANI_KOOPAS_SHELL 16003
#define ID_ANI_KOOPAS_COMEBACK 16004
#define ID_ANI_KOOPAS_KICKED 16005

class CKoopas : public CGameObject
{
protected:
	float ax, ay;
	bool isDefend;
	DWORD shell_start;
	bool isHeld;

public:
	CKoopas(float x, float y) : CGameObject(x, y) {
		ax = 0;
		ay = KOOPAS_GRAVITY;
		nx = -1;
		isDefend = false;
		isHeld = false;
		shell_start = 0;
		SetState(KOOPAS_STATE_WALKING);
	}

	void GetBoundingBox(float& l, float& t, float& r, float& b) override;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
	void Render() override;
	void OnNoCollision(DWORD dt) override;
	void OnCollisionWith(LPCOLLISIONEVENT e) override;

	void SetState(int state) override;

	void OnStomp();
	void OnKick(float mario_vx);
	void OnPickedUp();
	void OnReleased(float mario_vx);
	void Respawn();

	int IsCollidable() override { return 1; };
	int IsBlocking() override { return 0; }
};