#include "GameData.h"
#include "debug.h"

CGameData* CGameData::__instance = nullptr;

CGameData::CGameData() {
	Reset();
}

void CGameData::Reset() {
	life = 4;
	point = 0;
	coin = 0;
	startTime = GetTickCount64();
	remainTime = 300;
}

void CGameData::AddCoin(int c) {
	coin += c;
	if (coin >= 100) {
		coin -= 100;
		life += 1;
	}
}

void CGameData::StartCountDown() {
	if (startTime == 0)
		startTime = GetTickCount64();
}

void CGameData::CountDownRemainTime() {
	DWORD now = GetTickCount64();
	DWORD elapsed = (now - startTime) / 1000;

	if (elapsed < 300)
		remainTime = 300 - elapsed;
	else
		remainTime = 0;
}

CGameData* CGameData::GetInstance() {
	if (!__instance)
		__instance = new CGameData();
	return __instance;
}

void CGameData::SetMarioState(int level, int life, int point, int coin) {
	savedMarioLevel = level;
	savedLife = life;
	savedPoint = point;
	savedCoin = coin;
}

void CGameData::SetRespawn(float x, float y) {
	respawnX = x;
	respawnY = y;
}