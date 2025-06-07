#include "RandomItem.h"
#include "Animations.h"
#include "Game.h"

void CRandomItem::GetBoundingBox(float& l, float& t, float& r, float& b) {
    l = x - ITEM_BBOX_WIDTH / 2;
    t = y - ITEM_BBOX_HEIGHT / 2;
    r = l + ITEM_BBOX_WIDTH;
    b = t + ITEM_BBOX_HEIGHT;
}

void CRandomItem::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    if (GetTickCount64() - lastChangeTime >= RANDOM_ITEM_CHANGE_TIME)
    {
        currentCard = (currentCard + 1) % 3;
        lastChangeTime = GetTickCount64();
    }
}

void CRandomItem::Render()
{
    int aniId = ID_ANI_ITEM_STAR;
    if (currentCard == 1) aniId = ID_ANI_ITEM_MUSHROOM;
    else if (currentCard == 2) aniId = ID_ANI_ITEM_STAR;
    else aniId = ID_ANI_ITEM_FLOWER;

    CAnimations::GetInstance()->Get(aniId)->Render(x, y);
}
