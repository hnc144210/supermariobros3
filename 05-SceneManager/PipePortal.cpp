#include "PipePortal.h"
#include "GameData.h"
#include "Game.h"
#include "PlayScene.h"

void CPipePortal::MarioEnterPipe() {
	CGameData::GetInstance()->SetRespawn(exitX, exitY);
	CGameData::GetInstance()->SetCurrentSceneId(sceneId);
	CGameData::GetInstance()->EnterPipe();
}