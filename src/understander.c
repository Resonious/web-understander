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
#include "raygui.h"
#include "js_interop.h"
#include "widgets.h"
#include <string.h>

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
    STATE_NEED_IP,
    STATE_CONNECTING,
} state;

static void UpdateDrawFrame(void);          // Update and draw one frame

char connect_endpoint[256];

//----------------------------------------------------------------------------------
// Main entry point
//----------------------------------------------------------------------------------
int main(void)
{
    memset(connect_endpoint, 0, sizeof(connect_endpoint));

    // Initialization
    //---------------------------------------------------------
    InitWindow(SysScreenWidth(), SysScreenHeight(), "Web Understander");
#if !defined(PLATFORM_WEB)
    SetWindowState(FLAG_WINDOW_RESIZABLE);
#endif
    StartListeningForResizeEvents();

    InitAudioDevice();      // Initialize audio device

    state = STATE_NEED_IP;

    // Load global data (assets that must be available in all screens, i.e. font)
    font = LoadFont("resources/mecha.png");
    global_font = LoadFont("resources/Ubunu-Regular.ttf");
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
    UnloadFont(global_font);
    UnloadMusicStream(music);
    UnloadSound(fxCoin);

    CloseAudioDevice();     // Close audio context

    CloseWindow();          // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

const Color COLOR_PRIMARY = (Color){153, 64, 64, 255};

static void DrawAskForIPAddress() {
    float w = GetScreenWidth();
    float h = GetScreenHeight();

    ClearBackground(LIGHTGRAY);

    DrawText("CONNECT", w / 2 + 10, h / 2 - 50, 16, BLACK);
    
    DrawRectangle(w / 2, h / 2, 150, 30, COLOR_PRIMARY);
    //bool submitted = NativeTextInput("ip", "127.0.0.1:9909", w / 2 + 10, h / 2 + 10, 140, 20);

    bool submitted = false;

    Rectangle input;
    input.x = w / 2;
    input.y = h / 2 + 70;
    input.width = 200;
    input.height = 100;

    GuiSetFont(global_font);
    GuiTextInputBox(input, NULL, NULL, "Connect", connect_endpoint, sizeof(connect_endpoint), NULL);
    // DrawButton(btn, "Connect", COLOR_PRIMARY);

    if (submitted) {
        DrawRectangle(w / 2 + 10, h / 2 + 10, 100, 100, ColorFromHSV(0.7, 1.0, 1.0));
        PlaySound(fxCoin);
        state = STATE_CONNECTING;
    }
}

// Update and draw game frame
static void UpdateDrawFrame(void)
{
    // Update
    //----------------------------------------------------------------------------------
    // UpdateMusicStream(music);       // NOTE: Music keeps playing between screens

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

    switch (state) {
        case STATE_NEED_IP:
            DrawAskForIPAddress();
            break;
        case STATE_CONNECTING:
            ClearBackground(DARKGRAY);
            break;
    }
    //DrawFPS(10, 10);

    EndDrawing();
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
