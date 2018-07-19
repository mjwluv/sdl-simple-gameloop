//
// Created by mwlearn on 4/14/18.
//

#ifndef SDL_FIXEDFPS_VARIABLE_RENDER_GAME_H
#define SDL_FIXEDFPS_VARIABLE_RENDER_GAME_H


#include <SDL2/SDL.h>

namespace FixedFPSVariableRender {
    struct Vector2 {
        float x;
        float y;
    };

    class Game {
    public:
        Game();

        bool Initialize();

        void RunLoop();

        void Shutdown();

    private:
        void ProcessInput();

        void UpdateGame();

        void Render(float alpha);

        SDL_Window *mWindow;
        SDL_Renderer *mRenderer;
        Uint32 mTicksCount; // number of ticks since start of game.
        bool mIsRunning;

        int mBallDirX;
        int mBallDirY;
        Vector2 mBallPos; // position of the ball
        Vector2 mBallPosPrev;
        Vector2 mBallVel; // velocity of the ball
        Vector2 mBallVelMax;
        Vector2 mBallAcc;
        Vector2 mBallDec;
    };
}


#endif //SDL_FIXEDFPS_VARIABLE_RENDER_GAME_H
