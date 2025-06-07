#pragma once
#include "GameObject.h"

#define ITEM_BBOX_WIDTH 16
#define ITEM_BBOX_HEIGHT 16

#define ID_ANI_ITEM_MUSHROOM 23001
#define ID_ANI_ITEM_STAR     23002
#define ID_ANI_ITEM_FLOWER   23003

#define RANDOM_ITEM_CHANGE_TIME 150

class CRandomItem : public CGameObject
{
protected:
    int currentCard = 0;
    ULONGLONG lastChangeTime = 0;

public:
    CRandomItem(float x, float y) : CGameObject(x, y) {}

    void GetBoundingBox(float& l, float& t, float& r, float& b) override;
    void Render() override;
    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;

    int IsCollidable() override { return 1; }
    int IsBlocking() override { return 0; }
};