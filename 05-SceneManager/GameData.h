#pragma once
#include <windows.h>

#define ID_HIDDEN_MAP 5

class CGameData {
private:
	static CGameData* __instance;
	int current_scene;

	int life = 4;
	int point = 0;
	int coin = 0;

	ULONGLONG startTime = 0;
	DWORD remainTime = 300;

	bool isEnterPipe = false;
	int savedMarioLevel = 0;
	int savedLife = 0;
	int savedPoint = 0;
	int savedCoin = 0;
	float respawnX = 0;
	float respawnY = 0;

public:
	CGameData();

	void Reset();

	void SetLife(int l) { life = l; }
	void AddLife(int l) { life += l; }
	int GetLife() const { return life; }

	void AddPoint(int p) { point += p; }
	void SetPoint(int p) { point = p; }
	int GetPoint() const { return point; }

	void AddCoin(int c);
	void SetCoin(int c) { coin = c; }
	int GetCoin() const { return coin; }

	void StartCountDown();
	void CountDownRemainTime();
	DWORD GetRemainTime() const { return remainTime; }

	void SetCurrentSceneId(int id) { current_scene = id; };
	int GetCurrentSceneId() { return current_scene; };
	bool IsEntryHiddenMap() { return current_scene == ID_HIDDEN_MAP; }
	bool IsExitHiddenMap() { return current_scene != ID_HIDDEN_MAP; }
	void EnterPipe() { this->isEnterPipe = true; }
	void OutPipe() { this->isEnterPipe = false; }
	bool IsEnterPipe() { return isEnterPipe; }

	void SetMarioState(int level, int life, int point, int coin);
	int GetSavedMarioLevel() const { return savedMarioLevel; }
	int GetSavedLife() const { return savedLife; }
	int GetSavedPoint() const { return savedPoint; }
	int GetSavedCoin() const { return savedCoin; }

	void SetRespawn(float x, float y);
	float GetRespawnX() const { return respawnX; }
	float GetRespawnY() const { return respawnY; }

	static CGameData* GetInstance();
};