#include "PiranhaPlant.h"
#include "Animations.h"
#include "Game.h"
#include "PlayScene.h"
#include "Mario.h"
#include "Bullet.h"
CPiranhaPlant::CPiranhaPlant(float x, float y) : CGameObject(x, y) {
    this->startY = y;
    this->state_start = GetTickCount();
    this->isRising = true;
    SetState(PIRANHA_STATE_HIDE);
    state_start = GetTickCount();
}

void CPiranhaPlant::GetBoundingBox(float& l, float& t, float& r, float& b) {
    l = x - PIRANHA_BBOX_WIDTH / 2;
    t = y - PIRANHA_BBOX_HEIGHT;
    r = l + PIRANHA_BBOX_WIDTH;
    b = t + PIRANHA_BBOX_HEIGHT;
}

void CPiranhaPlant::Render() {
    int aniId = ID_ANI_PIRANHA_LEFT_BOTTOM;

    LPGAME game = CGame::GetInstance();
    CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

    float mx, my;
    mario->GetPosition(mx, my);

    if (mx < x) {
        aniId = (my < y) ? ID_ANI_PIRANHA_LEFT_TOP : ID_ANI_PIRANHA_LEFT_BOTTOM;
    }
    else {
        aniId = (my < y) ? ID_ANI_PIRANHA_RIGHT_TOP : ID_ANI_PIRANHA_RIGHT_BOTTOM;
    }

    CAnimations::GetInstance()->Get(aniId)->Render(x, y);
}

void CPiranhaPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
    if (state == PIRANHA_STATE_RISE) {
        y -= PIRANHA_RISE_SPEED * dt;
        if (y <= startY - PIRANHA_RISE_HEIGHT) {
            y = startY - PIRANHA_RISE_HEIGHT;

            if (!isShooting) {
                ShootBullet();
            }

            if (GetTickCount() - state_start > PIRANHA_WAIT_TIME) {
                state_start = GetTickCount();
                isShooting = false;
                SetState(PIRANHA_STATE_HIDE);
            }
        }
    }
    else if (state == PIRANHA_STATE_HIDE) {
        y += PIRANHA_RISE_SPEED * dt;
        if (y >= startY) {
            y = startY;

            LPGAME game = CGame::GetInstance();
            LPPLAYSCENE scene = (LPPLAYSCENE)game->GetCurrentScene();
            CMario* mario = dynamic_cast<CMario*>(scene->GetPlayer());

            float marioX, marioY;
            mario->GetPosition(marioX, marioY);

            float dx = abs(marioX - x);
            if (dx <= 150.0f) {
                if (GetTickCount() - state_start > PIRANHA_WAIT_TIME) {
                    state_start = GetTickCount();
                    SetState(PIRANHA_STATE_RISE);
                }
            }
        }
    }

}



void CPiranhaPlant::SetState(int state) {
    CGameObject::SetState(state);
    switch (state) {
    case PIRANHA_STATE_RISE:
        isRising = true;
        break;
    case PIRANHA_STATE_HIDE:
        isRising = false;
        break;
    }
}
void CPiranhaPlant::ShootBullet() {
    LPGAME      game = CGame::GetInstance();
    LPPLAYSCENE scene = (LPPLAYSCENE)game->GetCurrentScene();
    CMario* mario = dynamic_cast<CMario*>(scene->GetPlayer());
    if (!mario) return;

    float marioX, marioY;
    mario->GetPosition(marioX, marioY);

    float bx = x;
    float by = y - (PIRANHA_BBOX_HEIGHT / 2.0f);

    CBullet* bullet = new CBullet(bx, by, marioX, marioY);
    scene->AddObject(bullet);

    isShooting = true;
}
