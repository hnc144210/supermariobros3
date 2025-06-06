#pragma once
#include "Pipe.h"

#define PIPE_PORTAL_DIRECTION_UP 1
#define PIPE_PORTAL_DIRECTION_DOWN -1

class CPipePortal : public CPipe
{
protected:
    int sceneId;
    int direction;
    float exitX, exitY;

public:
    CPipePortal(float x, float y, int pipe_type, int sceneId, int direction, float exitX, float exitY)
        : CPipe(x, y, pipe_type)
    {
        this->sceneId = sceneId;
        this->direction = direction;
        this->exitX = exitX;
        this->exitY = exitY;
    }

    int IsCollidable() override { return 0; }
    int IsBlocking() override { return 1; }

    int GetSceneId() const { return sceneId; }
    int GetDirection() const { return direction; }

    void MarioEnterPipe();
};
