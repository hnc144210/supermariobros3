#include "PiranhaPlant.h"
#include "Animations.h"
#include "Game.h"
#include "PlayScene.h"
#include "Mario.h"
#include "Bullet.h"

CPiranhaPlant::CPiranhaPlant(float x, float y, int type) : CGameObject(x, y) {
    this->startY = y;
    this->type = type;
    this->state_start = GetTickCount();
    this->isRising = true;
    this->isShooting = false;
    SetState(PIRANHA_STATE_HIDE);
}

void CPiranhaPlant::GetBoundingBox(float& l, float& t, float& r, float& b) {
    if (state == PIRANHA_STATE_HIDE) {
        l = t = r = b = 0.0f;
        return;
    }

    l = x - PIRANHA_BBOX_WIDTH / 2;
    t = y - PIRANHA_BBOX_HEIGHT / 2;
    r = l + PIRANHA_BBOX_WIDTH;
    b = t + PIRANHA_BBOX_HEIGHT;
}

int CPiranhaPlant::IsCollidable() {
    return y < startY ? 1 : 0;
}


void CPiranhaPlant::Render() {
    int aniId = -1;

    if (type == PIRANHA_TYPE_IDLE_LOW) {
        aniId = ID_ANI_PIRANHA2_IDLE;
    }
    else {
        LPGAME game = CGame::GetInstance();
        LPPLAYSCENE scene = (LPPLAYSCENE)game->GetCurrentScene();
        CMario* mario = dynamic_cast<CMario*>(scene->GetPlayer());

        if (!mario) return;

        float mx, my;
        mario->GetPosition(mx, my);

        bool isLeft = mx < x;
        bool isAbove = my < y;

        if (type == PIRANHA_TYPE_SHOOT_HIGH) {
            if (isLeft)
                aniId = isAbove ? ID_ANI_PIRANHA_LEFT_TOP : ID_ANI_PIRANHA_LEFT_BOTTOM;
            else
                aniId = isAbove ? ID_ANI_PIRANHA_RIGHT_TOP : ID_ANI_PIRANHA_RIGHT_BOTTOM;
        }
        else if (type == PIRANHA_TYPE_SHOOT_MEDIUM) {
            if (isLeft)
                aniId = isAbove ? ID_ANI_PIRANHA3_LEFT_TOP : ID_ANI_PIRANHA3_LEFT_BOTTOM;
            else
                aniId = isAbove ? ID_ANI_PIRANHA3_RIGHT_TOP : ID_ANI_PIRANHA3_RIGHT_BOTTOM;
        }
    }

    if (aniId != -1)
        CAnimations::GetInstance()->Get(aniId)->Render(x, y);
}



void CPiranhaPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
    float riseHeight = PIRANHA_RISE_HEIGHT;
    if (type == PIRANHA_TYPE_SHOOT_MEDIUM)
        riseHeight = 47.0f;
    else if (type == PIRANHA_TYPE_IDLE_LOW)
        riseHeight = 40.0f;

    if (state == PIRANHA_STATE_RISE) {
        y -= PIRANHA_RISE_SPEED * dt;
        if (y <= startY - riseHeight) {
            y = startY - riseHeight;

            if (!isShooting && type != PIRANHA_TYPE_IDLE_LOW) {
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
        if (y < startY) {
            y += PIRANHA_RISE_SPEED * dt;
            if (y > startY)
                y = startY;
        }

        if (y >= startY) {
            LPGAME game = CGame::GetInstance();
            LPPLAYSCENE scene = (LPPLAYSCENE)game->GetCurrentScene();
            CMario* mario = dynamic_cast<CMario*>(scene->GetPlayer());

            float marioX, marioY;
            mario->GetPosition(marioX, marioY);

            float dx = abs(marioX - x);
            if ((type != PIRANHA_TYPE_IDLE_LOW && dx <= 120.0f) || type == PIRANHA_TYPE_IDLE_LOW) {
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
    if (type == PIRANHA_TYPE_IDLE_LOW) return;

    LPGAME      game = CGame::GetInstance();
    LPPLAYSCENE scene = (LPPLAYSCENE)game->GetCurrentScene();
    CMario* mario = dynamic_cast<CMario*>(scene->GetPlayer());
    if (!mario) return;

    float marioX, marioY;
    mario->GetPosition(marioX, marioY);

    float bx = x;
    float by = y - (PIRANHA_BBOX_HEIGHT / 2.0f);

    float vx = 0.0f;
    float vy = 0.0f;

    bool marioLeft = marioX < x;
    bool marioAbove = marioY < y;

    const float bulletVX = 0.08f;
    const float bulletVY = 0.05f;

    if (marioLeft) {
        vx = -bulletVX;
        vy = marioAbove ? -bulletVY : bulletVY;
    }
    else {
        vx = bulletVX;
        vy = marioAbove ? -bulletVY : bulletVY;
    }

    CBullet* bullet = new CBullet(bx, by, vx, vy);
    scene->AddObject(bullet);

    isShooting = true;
}


