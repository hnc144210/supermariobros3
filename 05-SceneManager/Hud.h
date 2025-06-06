#pragma once

#include "GameObject.h"

#define HUD_BACKGROUND_BBOX_WIDTH 262
#define HUD_BACKGROUND_BBOX_HEIGHT 36

#define ID_ANI_HUD_BACKGROUND        22000

#define ID_ANI_MUSHROOM_CARD         22001
#define ID_ANI_FLOWER_CARD           22002
#define ID_ANI_STAR_CARD             22003

#define ID_ANI_M_BOX                 22004
#define ID_ANI_WHITE_P_BOX           22005
#define ID_ANI_BLACK_P_BOX           22006

#define ID_ANI_ARROW_WHITE           22007
#define ID_ANI_ARROW_BLACK           22008

#define ID_ANI_ICON_X                22009
#define ID_ANI_ICON_CLOCK            22010
#define ID_ANI_ICON_DOLAR            22011

#define ID_ANI_NUMBER_0              22020
#define ID_ANI_NUMBER_1              22021
#define ID_ANI_NUMBER_2              22022
#define ID_ANI_NUMBER_3              22023
#define ID_ANI_NUMBER_4              22024
#define ID_ANI_NUMBER_5              22025
#define ID_ANI_NUMBER_6              22026
#define ID_ANI_NUMBER_7              22027
#define ID_ANI_NUMBER_8              22028
#define ID_ANI_NUMBER_9              22029

#define ID_ANI_CHAR_W                22030
#define ID_ANI_CHAR_O                22031
#define ID_ANI_CHAR_R                22032
#define ID_ANI_CHAR_L                22033
#define ID_ANI_CHAR_D                22034

#define HUD_NUMBER_SPACING           8
#define HUD_CHAR_SPACING             8
#define HUD_ARROW_COUNT              6

class CHud : public CGameObject
{
protected:
    ULONGLONG fullPowerFlashStart;
    bool isFullPowerDark;

public:
    CHud(float x, float y) : CGameObject(x, y)
    {
        fullPowerFlashStart = 0;
        isFullPowerDark = true;
    }

    void Update(DWORD dt);
    void Render() override;
    void GetBoundingBox(float& l, float& t, float& r, float& b) override {}

    void RenderBackground();
    void RenderWorldText();
    void RenderIconX();
    void RenderIconClock();
    void RenderIconDolar();
    void RenderCoins();
    void RenderPoints();
    void RenderTime();
    void RenderLife();
    void RenderFrame();
    void RenderPowerBar();

    void RenderNumber(int number, float x, float y, int maxDigits = 0);
    void RenderChar(char c, float x, float y);
};
