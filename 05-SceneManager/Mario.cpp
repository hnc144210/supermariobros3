#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Coin.h"
#include "Portal.h"
#include "ParaGoomba.h"
#include "MysteryBlock.h"
#include "Collision.h"
#include "PlayScene.h"
#include "Mushroom.h"
#include "Bullet.h"
#include "PiranhaPlant.h"
#include "Koopas.h"
#include "Leaf.h"
#include "GameData.h"
#include "PipePortal.h"
#include "RandomItem.h"

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if (abs(vx) > abs(maxVx)) vx = maxVx;

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	ULONGLONG now = GetTickCount64();

	bool isRunning = abs(vx) > MARIO_WALKING_SPEED;

	if (isRunning)
	{
		if (now - lastPowerUpdate >= MARIO_POWER_TIME)
		{
			if (power < MARIO_POWER_MAX)
				power++;
			lastPowerUpdate = now;
		}
	}
	else
	{
		if (now - lastPowerUpdate >= MARIO_POWER_TIME)
		{
			if (power > 0)
				power--;
			lastPowerUpdate = now;
		}
	}

	if (holdingKoopas)
	{
		float holdX = x + nx * KOOPAS_HOLD_OFFSET_X;
		float holdY = y - KOOPAS_HOLD_OFFSET_Y;
		holdingKoopas->SetPosition(holdX, holdY);
	}

	CCollision::GetInstance()->Process(this, dt, coObjects);

	ReleaseKoopas();
}

void CMario::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
	isOnPlatform = false;
}

void CMario::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
		if (e->ny < 0) isOnPlatform = true;
	}
	else
		if (e->nx != 0 && e->obj->IsBlocking())
		{
			vx = 0;
		}

	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	else if (dynamic_cast<CCoin*>(e->obj))
		OnCollisionWithCoin(e);
	else if (dynamic_cast<CPortal*>(e->obj))
		OnCollisionWithPortal(e);
	else if (dynamic_cast<CParaGoomba*>(e->obj))
		OnCollisionWithParagoomba(e);
	else if (dynamic_cast<CMysteryBlock*>(e->obj))
		OnCollisionWithMysteryBlock(e);
	else if (dynamic_cast<CMushroom*>(e->obj))
		OnCollisionWithMushroom(e);
	else if (dynamic_cast<CBullet*>(e->obj))
		OnCollisionWithBullet(e);
	else if (dynamic_cast<CPiranhaPlant*>(e->obj))
		OnCollisionWithPiranhaPlant(e);
	else if (dynamic_cast<CKoopas*>(e->obj))
		OnCollisionWithKoopas(e);
	else if (dynamic_cast<CLeaf*>(e->obj))
		OnCollisionWithLeaf(e);
	else if (dynamic_cast<CPipePortal*>(e->obj))
		OnCollisionWithPipePortal(e);
	else if (dynamic_cast<CRandomItem*>(e->obj))
		OnCollisionWithRandomItem(e);
}

void CMario::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

	// jump on top >> kill Goomba and deflect a bit 
	if (e->ny < 0)
	{
		if (goomba->GetState() != GOOMBA_STATE_DIE)
		{
			goomba->SetState(GOOMBA_STATE_DIE);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
			CGameData::GetInstance()->AddPoint(100);
		}
	}
	else // hit by Goomba
	{
		if (untouchable == 0)
		{
			if (goomba->GetState() != GOOMBA_STATE_DIE)
			{
				if (level > MARIO_LEVEL_SMALL)
				{
					level = MARIO_LEVEL_SMALL;
					StartUntouchable();
				}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DIE);
				}
			}
		}
	}
}

void CMario::OnCollisionWithParagoomba(LPCOLLISIONEVENT e)
{
	CParaGoomba* paraGoomba = dynamic_cast<CParaGoomba*>(e->obj);
	if (!paraGoomba) return;

	if (e->ny < 0)
	{
		int state = paraGoomba->GetState();
		if (state == PARAGOOMBA_STATE_WALKING)
		{
			paraGoomba->SetState(PARAGOOMBA_STATE_NO_WING);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
			CGameData::GetInstance()->AddPoint(200);
		}
		else if (state == PARAGOOMBA_STATE_NO_WING)
		{
			paraGoomba->SetState(PARAGOOMBA_STATE_DIE);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
			CGameData::GetInstance()->AddPoint(100);
		}
	}
	else
	{
		if (untouchable == 0 && paraGoomba->GetState() != PARAGOOMBA_STATE_DIE)
		{
			if (level > MARIO_LEVEL_SMALL)
			{
				level = MARIO_LEVEL_SMALL;
				StartUntouchable();
			}
			else
			{
				DebugOut(L">>> Mario DIE >>> \n");
				SetState(MARIO_STATE_DIE);
			}
		}
	}
}

void CMario::OnCollisionWithMysteryBlock(LPCOLLISIONEVENT e) {
	CMysteryBlock* block = dynamic_cast<CMysteryBlock*>(e->obj);
	if (!block) return;

	if (e->ny > 0 && !block->IsUsed())
	{
		block->SetState(MYSTERYBLOCK_STATE_UNBOX);
		CGameData::GetInstance()->AddCoin(1);
		CGameData::GetInstance()->AddPoint(100);
	}
}
void CMario::OnCollisionWithCoin(LPCOLLISIONEVENT e)
{
	e->obj->Delete();
	coin++;
	CGameData::GetInstance()->AddCoin(1);
	CGameData::GetInstance()->AddPoint(50);
}
void CMario::OnCollisionWithMushroom(LPCOLLISIONEVENT e)
{
	CMushroom* mushroom = dynamic_cast<CMushroom*>(e->obj);
	if (!mushroom) return;
	if (!mushroom->IsUsed())
	{
		mushroom->SetState(MUSHROOM_STATE_EATEN);
		SetLevel(MARIO_LEVEL_BIG);
		CGameData::GetInstance()->AddPoint(1000);
	}
}
void CMario::OnCollisionWithBullet(LPCOLLISIONEVENT e) {
	CBullet* bullet = dynamic_cast<CBullet*>(e->obj);

	if (level > MARIO_LEVEL_BIG)
	{
		level = MARIO_LEVEL_BIG;
		StartUntouchable();
	}
	else if (level > MARIO_LEVEL_SMALL)
	{
		level = MARIO_LEVEL_SMALL;
		StartUntouchable();
	}
	else
	{
		SetState(MARIO_STATE_DIE);
	}

	bullet->Delete();
}

void CMario::OnCollisionWithPiranhaPlant(LPCOLLISIONEVENT e) {
	CPiranhaPlant* piranhaplant = dynamic_cast<CPiranhaPlant*>(e->obj);
	if (untouchable) return;
	if (level > MARIO_LEVEL_SMALL)
	{
		level = MARIO_LEVEL_SMALL;
		StartUntouchable();
	}
	else
	{
		SetState(MARIO_STATE_DIE);
	}
}

void CMario::OnCollisionWithLeaf(LPCOLLISIONEVENT e) {
	CLeaf* leaf = dynamic_cast<CLeaf*>(e->obj);
	SetLevel(MARIO_LEVEL_FLY);

	leaf->Delete();
}

void CMario::OnCollisionWithRandomItem(LPCOLLISIONEVENT e)
{
	CRandomItem* item = dynamic_cast<CRandomItem*>(e->obj);
	item->Delete();
}

void CMario::OnCollisionWithPipePortal(LPCOLLISIONEVENT e)
{
	CPipePortal* portal = dynamic_cast<CPipePortal*>(e->obj);
	if (!portal) return;

	CGame* game = CGame::GetInstance();

	int keyDownDir = 0;
	if (portal->GetDirection() == -1 && game->IsKeyDown(DIK_UP))
		keyDownDir = -1;
	else if (portal->GetDirection() == 1 && game->IsKeyDown(DIK_DOWN))
		keyDownDir = 1;

	if (keyDownDir == portal->GetDirection())
	{
		int life = CGameData::GetInstance()->GetLife();
		int point = CGameData::GetInstance()->GetPoint();
		int coin = CGameData::GetInstance()->GetCoin();

		CGameData::GetInstance()->SetMarioState(level, life, point, coin);
		CGameData::GetInstance()->SetCurrentSceneId(portal->GetSceneId());

		portal->MarioEnterPipe();
		int nextScene = CGameData::GetInstance()->GetCurrentSceneId();
		CGame::GetInstance()->InitiateSwitchScene(nextScene);
	}
}

void CMario::OnCollisionWithKoopas(LPCOLLISIONEVENT e)
{
	CKoopas* koopa = dynamic_cast<CKoopas*>(e->obj);
	if (!koopa) return;

	if (holdingKoopas == koopa)
		return;

	if (e->ny < 0)
	{
		if (koopa->IsSpinning())
		{
			koopa->SetState(KOOPAS_STATE_SHELL);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
		else
		{
			koopa->OnStomp();
			vy = -MARIO_JUMP_DEFLECT_SPEED;
			CGameData::GetInstance()->AddPoint(100);
		}
	}
	else if (e->nx != 0)
	{
		if (!koopa->IsDefend())
		{
			if (untouchable == 0)
			{
				if (level > MARIO_LEVEL_SMALL)
				{
					SetLevel(MARIO_LEVEL_SMALL);
					StartUntouchable();
				}
				else
				{
					SetState(MARIO_STATE_DIE);
				}
			}
		}
		else
		{
			if (koopa->IsSpinning())
			{
				if (untouchable == 0)
				{
					if (level > MARIO_LEVEL_SMALL)
					{
						SetLevel(MARIO_LEVEL_SMALL);
						StartUntouchable();
					}
					else
					{
						SetState(MARIO_STATE_DIE);
					}
				}
			}
			else
			{
				LPGAME game = CGame::GetInstance();
				if (game->IsKeyDown(DIK_A))
				{
					HoldKoopas(koopa);
				}
				else
				{
					koopa->OnKick(vx);
				}
			}
		}
	}
}

void CMario::HoldKoopas(CKoopas* koopa)
{
	holdingKoopas = koopa;
	koopa->SetState(KOOPAS_STATE_HELD);
	koopa->SetHeld(true);
}

void CMario::ReleaseKoopas()
{
	if (holdingKoopas && !CGame::GetInstance()->IsKeyDown(DIK_A))
	{
		holdingKoopas->SetState(KOOPAS_STATE_SPINNING);
		holdingKoopas->OnReleased(nx);
		holdingKoopas->SetHeld(false);
		holdingKoopas = nullptr;
	}
}

void CMario::OnCollisionWithPortal(LPCOLLISIONEVENT e)
{
	CPortal* p = (CPortal*)e->obj;
	CGame::GetInstance()->InitiateSwitchScene(p->GetSceneId());
}

//
// Get animation ID for small Mario
//
int CMario::GetAniIdSmall()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_SMALL_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_SMALL_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_SMALL_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_SMALL_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_SMALL_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;

	return aniId;
}


//
// Get animdation ID for big Mario
//
int CMario::GetAniIdBig()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_IDLE_RIGHT;

	return aniId;
}

int CMario::GetAniIdFly() {
	int aniId = -1;

	if (!isOnPlatform)
	{
		if (isFlying)
		{
			aniId = (nx > 0) ? ID_ANI_MARIO_FLY_RIGHT : ID_ANI_MARIO_FLY_LEFT;
		}
		else if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			aniId = (nx > 0) ? ID_ANI_MARIO_FLY_JUMP_RIGHT : ID_ANI_MARIO_FLY_JUMP_LEFT;
		}
		else
		{
			aniId = (nx > 0) ? ID_ANI_MARIO_FLY_JUMP_RIGHT : ID_ANI_MARIO_FLY_JUMP_LEFT;
		}
	}
	else {
		if (isSitting)
		{
			aniId = (nx > 0) ? ID_ANI_MARIO_FLY_SIT_RIGHT : ID_ANI_MARIO_FLY_SIT_LEFT;
		}
		else {
			if (vx == 0)
			{
				aniId = (nx > 0) ? ID_ANI_MARIO_FLY_IDLE_RIGHT : ID_ANI_MARIO_FLY_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_FLY_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_FLY_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_FLY_WALKING_RIGHT;
			}
			else
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_FLY_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_FLY_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_FLY_WALKING_LEFT;
			}
		}
	}

	if (aniId == -1) aniId = ID_ANI_MARIO_FLY_IDLE_RIGHT;
	return aniId;
}

void CMario::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;

	if (state == MARIO_STATE_DIE)
		aniId = ID_ANI_MARIO_DIE;
	else if (level == MARIO_LEVEL_FLY)
		aniId = GetAniIdFly();
	else if (level == MARIO_LEVEL_BIG)
		aniId = GetAniIdBig();
	else if (level == MARIO_LEVEL_SMALL)
		aniId = GetAniIdSmall();

	animations->Get(aniId)->Render(x, y);

	//RenderBoundingBox();

	DebugOutTitle(L"Coins: %d", coin);
}

void CMario::SetState(int state)
{
	// DIE is the end state, cannot be changed! 
	if (this->state == MARIO_STATE_DIE) return;

	switch (state)
	{
	case MARIO_STATE_RUNNING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_RUNNING_SPEED;
		ax = MARIO_ACCEL_RUN_X;
		nx = 1;
		break;
	case MARIO_STATE_RUNNING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_RUNNING_SPEED;
		ax = -MARIO_ACCEL_RUN_X;
		nx = -1;
		break;
	case MARIO_STATE_WALKING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_WALKING_SPEED;
		ax = MARIO_ACCEL_WALK_X;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_WALKING_SPEED;
		ax = -MARIO_ACCEL_WALK_X;
		nx = -1;
		break;
	case MARIO_STATE_JUMP:
		if (isSitting) break;
		if (isOnPlatform)
		{
			if (abs(this->vx) == MARIO_RUNNING_SPEED)
				vy = -MARIO_JUMP_RUN_SPEED_Y;
			else
				vy = -MARIO_JUMP_SPEED_Y;
		}
		break;

	case MARIO_STATE_RELEASE_JUMP:
		if (vy < 0) vy += MARIO_JUMP_SPEED_Y / 2;
		break;

	case MARIO_STATE_SIT:
		if (isOnPlatform && level != MARIO_LEVEL_SMALL)
		{
			state = MARIO_STATE_IDLE;
			isSitting = true;
			vx = 0; vy = 0.0f;
			y += MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_FLYING_RIGHT:
		isFlying = true;
		nx = 1;
		vx = MARIO_WALKING_SPEED;
		vy = -MARIO_FLYING_SPEED;
		break;

	case MARIO_STATE_FLYING_LEFT:
		isFlying = true;
		nx = -1;
		vx = -MARIO_WALKING_SPEED;
		vy = -MARIO_FLYING_SPEED;
		break;

	case MARIO_STATE_SIT_RELEASE:
		if (isSitting)
		{
			isSitting = false;
			state = MARIO_STATE_IDLE;
			y -= MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_IDLE:
		ax = 0.0f;
		vx = 0.0f;
		break;

	case MARIO_STATE_DIE:
		vy = -MARIO_JUMP_DEFLECT_SPEED;
		vx = 0;
		ax = 0;
		break;
	}

	CGameObject::SetState(state);
}

void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (level == MARIO_LEVEL_BIG)
	{
		if (isSitting)
		{
			left = x - MARIO_BIG_SITTING_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_BIG_SITTING_BBOX_HEIGHT;
		}
		else
		{
			left = x - MARIO_BIG_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_BBOX_WIDTH;
			bottom = top + MARIO_BIG_BBOX_HEIGHT;
		}
	}
	else if (level == MARIO_LEVEL_SMALL)
	{
		left = x - MARIO_SMALL_BBOX_WIDTH / 2;
		top = y - MARIO_SMALL_BBOX_HEIGHT / 2;
		right = left + MARIO_SMALL_BBOX_WIDTH;
		bottom = top + MARIO_SMALL_BBOX_HEIGHT;
	}
	else {
		if (isSitting)
		{
			left = x - MARIO_FLYING_SITTING_BBOX_WIDTH / 2;
			top = y - MARIO_FLYING_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_FLYING_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_FLYING_SITTING_BBOX_HEIGHT;
		}
		else
		{
			left = x - MARIO_FLYING_BBOX_WIDTH / 2;
			top = y - MARIO_FLYING_BBOX_HEIGHT / 2;
			right = left + MARIO_FLYING_BBOX_WIDTH;
			bottom = top + MARIO_FLYING_BBOX_HEIGHT;
		}
	}
}

void CMario::SetLevel(int l)
{
	// Adjust position to avoid falling off platform
	if (this->level == MARIO_LEVEL_SMALL)
	{
		y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2;
	}
	level = l;
}

