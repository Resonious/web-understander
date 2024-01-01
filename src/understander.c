/*******************************************************************************************
*
*   raylib game template
*
*   <Game title>
*   <Game description>
*
*   This game has been created using raylib (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2021 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include "js_interop.h"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

//----------------------------------------------------------------------------------
// Shared Variables Definition (global)
// NOTE: Those variables are shared between modules through screens.h
//----------------------------------------------------------------------------------
Font font = { 0 };
Music music = { 0 };
Sound fxCoin = { 0 };

enum AppState {
    STATE_NOT_CONNECTED,
    STATE_CONNECTING,
};

static void UpdateDrawFrame(void);          // Update and draw one frame

//----------------------------------------------------------------------------------
// Main entry point
//----------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //---------------------------------------------------------
    InitWindow(SysScreenWidth(), SysScreenHeight(), "Web Understander");
#if !defined(PLATFORM_WEB)
    SetWindowState(FLAG_WINDOW_RESIZABLE);
#endif
    StartListeningForResizeEvents();

    InitAudioDevice();      // Initialize audio device

    // Load global data (assets that must be available in all screens, i.e. font)
    font = LoadFont("resources/mecha.png");
    music = LoadMusicStream("resources/ambient.ogg");
    fxCoin = LoadSound("resources/coin.wav");

    SetMusicVolume(music, 1.0f);
    // PlayMusicStream(music);

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(60);       // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        UpdateDrawFrame();
    }
#endif

    // Unload global data loaded
    UnloadFont(font);
    UnloadMusicStream(music);
    UnloadSound(fxCoin);

    CloseAudioDevice();     // Close audio context

    CloseWindow();          // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

// Update and draw game frame
static void UpdateDrawFrame(void)
{
    // Update
    //----------------------------------------------------------------------------------
    // UpdateMusicStream(music);       // NOTE: Music keeps playing between screens

    int w = GetScreenWidth();
    int h = GetScreenHeight();

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
    NativeBeginDrawing();

        ClearBackground(RAYWHITE);

        DrawRectangle(w / 2, h / 2, 100, 100, ColorFromHSV(0.2, 1.0, 1.0));

        bool submitted = NativeTextInput("uniqueid", w / 2 + 10, h / 2 + 10, 140, 20);
        if (submitted) {
            DrawRectangle(w / 2 + 10, h / 2 + 10, 100, 100, ColorFromHSV(0.7, 1.0, 1.0));
            PlaySound(fxCoin);
        }

        //DrawFPS(10, 10);

    EndDrawing();
    NativeEndDrawing();
    //----------------------------------------------------------------------------------
}

#if defined(PLATFORM_WEB)
// This is the "resize" event listener callback (referenced in js_interop.c)
EMSCRIPTEN_KEEPALIVE
void WindowResized(int width, int height)
{
    SetWindowSize(width, height);
}
#endif
