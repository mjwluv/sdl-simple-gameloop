//
// Created by mwlearn on 4/14/18.
//

#include "Game.h"

namespace FixedFPSVariableRender {

  const int thickness = 25;

  Game::Game()
      : mWindow(nullptr), mRenderer(nullptr), mTicksCount(0), mIsRunning(true) {

  }

  bool Game::Initialize() {
    // Initialize SDL
    int sdlResult = SDL_Init(SDL_INIT_VIDEO);
    if (sdlResult != 0) {
      SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
      return false;
    }

    // Create a SDL Window
    mWindow = SDL_CreateWindow(
        "Game Programming in C++ (Chapter 1)",
        100, // Top left x-coordinate of window
        100, // Top left y-coordinate of window
        1024, // Width of window
        768, // Height of window
        0 // Flags (0 for no flags set)
    );

    if (!mWindow) {
      SDL_Log("Failed to create window: %s", SDL_GetError());
      return false;
    }

    // Create SDL renderer
    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (!mRenderer) {
      SDL_Log("Failed to create renderer: %s", SDL_GetError());
      return false;
    }

    mBallPos.x = 1024.f / 2.f;
    mBallPos.y = 768.f / 2.f;
    mBallPosPrev.x = mBallPos.x;
    mBallPosPrev.y = mBallPos.y;
    mBallVel.x = 0;
    mBallVel.y = 0;
    mBallAcc.x = 0.3f;
    mBallAcc.y = 0.3f;
    mBallVelMax.x = 2.f;
    mBallVelMax.y = 2.f;
    mBallDec.x = 0.08f;
    mBallDec.y = 0.08f;

    return true;
  }

  void Game::RunLoop() {
    const float fps = 60.f;
    const float dt = 1.f / fps;
    float lag = 0.f;

    float frameStart = SDL_GetTicks() / 1000.f;

    while (mIsRunning) {
      const float currentTime = SDL_GetTicks() / 1000.f;
      const float elapsed = currentTime - frameStart;

      // Record the starting of this frame.
      frameStart = currentTime;

      // Store the time elapsed since the last frame began.
      lag += elapsed;

      // Avoid spiral of death and clamp dt, thus clamping
      // how many times the UpdatePhysics can be called in
      // a single game loop.
      if (lag > 0.2f) {
        lag = 0.2f;
      }

      while (lag >= dt) {
        ProcessInput();
        UpdateGame();
        lag -= dt;
      }

      Render(lag / dt);
    }
  }

  void Game::ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          mIsRunning = false;
          break;
      }
    }

    // Get the state of the keyboard.
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    // If escaped is pressed, also end loop.
    if (state[SDL_SCANCODE_ESCAPE]) {
      mIsRunning = false;
    }
  }

  void Game::UpdateGame() {
    // Get the state of the keyboard.
    const Uint8 *state = SDL_GetKeyboardState(NULL);

    // Update paddle direction based on arrow keys.
    mBallDirX = 0;

    if (state[SDL_SCANCODE_LEFT]) {
      mBallDirX = -1;
    }
    if (state[SDL_SCANCODE_RIGHT]) {
      mBallDirX = 1;
    }

    mBallDirY = 0;

    if (state[SDL_SCANCODE_UP]) {
      mBallDirY = -1;
    }
    if (state[SDL_SCANCODE_DOWN]) {
      mBallDirY = 1;
    }


    if (mBallDirX == 1) {
      mBallVel.x += mBallAcc.x;
      if (mBallVel.x >= mBallVelMax.x) {
        mBallVel.x = mBallVelMax.x;
      }
    } else if (mBallDirX == -1) {
      mBallVel.x -= mBallAcc.x;
      if (mBallVel.x <= -mBallVelMax.x) {
        mBallVel.x = -mBallVelMax.x;
      }
    } else {
      if (mBallVel.x > 0) {
        mBallVel.x -= mBallDec.x;
        if (mBallVel.x <= 0) {
          mBallVel.x = 0;
        }
      } else if (mBallVel.x < 0) {
        mBallVel.x += mBallDec.x;
        if (mBallVel.x >= 0) {
          mBallVel.x = 0;
        }
      }
    }

    if (mBallDirY == 1) {
      mBallVel.y += mBallAcc.y;
      if (mBallVel.y >= mBallVelMax.y) {
        mBallVel.y = mBallVelMax.y;
      }
    } else if (mBallDirY == -1) {
      mBallVel.y -= mBallAcc.y;
      if (mBallVel.y <= -mBallVelMax.y) {
        mBallVel.y = -mBallVelMax.y;
      }
    } else {
      if (mBallVel.y > 0) {
        mBallVel.y -= mBallDec.y;
        if (mBallVel.y <= 0) {
          mBallVel.y = 0;
        }
      } else if (mBallVel.y < 0) {
        mBallVel.y += mBallDec.y;
        if (mBallVel.y >= 0) {
          mBallVel.y = 0;
        }
      }
    }

    mBallPos.x += mBallVel.x;
    mBallPos.y += mBallVel.y;
  }

  void Game::Render(float alpha) {
    // Set draw color to blue
    SDL_SetRenderDrawColor(
        mRenderer,
        0,    // R
        0,    // G
        255,  // B
        255   // A
    );

    // Clear back buffer
    SDL_RenderClear(mRenderer);

    SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);

    float prevX = mBallPosPrev.x * alpha;
    float prevY = mBallPosPrev.y * alpha;
    float currX = mBallPos.x * (1.0f - alpha);
    float currY = mBallPos.y * (1.0f - alpha);

    float x, y;

    // Do not interpolate the x,y positions if they there is almost no difference between current and previous positions
    if (abs(mBallPos.x) - abs(mBallPosPrev.x) <= 0.0001f) {
      x = mBallPos.x;
    } else {
      x = prevX + currX;
    }

    if (abs(mBallPos.y) - abs(mBallPosPrev.y) <= 0.0001f) {
      y = mBallPos.y;
    } else {
      y = prevY + currY;
    }

    mBallPosPrev.x = mBallPos.x;
    mBallPosPrev.y = mBallPos.y;

    // Draw ball
    SDL_Rect ball{
        static_cast<int>(x),
        static_cast<int>(y),
        thickness,
        thickness
    };

    SDL_RenderFillRect(mRenderer, &ball);

    // Swap front buffer and back buffer
    SDL_RenderPresent(mRenderer);
  }

  void Game::Shutdown() {
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
  }
}