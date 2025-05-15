#pragma once
#include "GameObject.h"
#define MISTERYBLOCK_BBOX_WIDTH 16
#define MISTERYBLOCK_BBOX_HEIGHT 16

#define ID_ANI_MISTERYBLOCK 13000

	class CMisteryBlock : public CGameObject {
	public:
		CMisteryBlock(float x, float y) : CGameObject(x, y) {
		}
		int IsCollidable() override { return 0; }
		int IsBlocking() override { return 1; }

		void GetBoundingBox(float& l, float& t, float& r, float& b) override;
		void Render() override;
	};

