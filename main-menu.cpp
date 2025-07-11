#include "raylib.h"
#include <iostream>
#include "game.hpp"

Game mainScreen(){
    
    bool close = false;
    enum GameState{
        MAIN_MENU,
        LOAD_GAME,
        GAME,
        OPTIONS,
        EXIT
    };

    GameState state = MAIN_MENU;
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1200;
    const int screenHeight = 630;

    InitWindow(screenWidth, screenHeight, "horrified");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    Image background = LoadImage("horrified.jpeg");
    Texture2D backgroundTexture = LoadTextureFromImage(background);
    //--------------------------------------------------------------------------------------
    
    //setting the music

    InitAudioDevice();
    Music music = LoadMusicStream("scary-music-instrumental 001.mp3");
    PlayMusicStream(music);

    //--------------------------------------------------------------------------------------
    //setting clicks

    Vector2 mousePosition = {0,0};

    //--------------------------------------------------------------------------------------

    // Main game loop

    while (!close && !WindowShouldClose())    // Detect window close button or ESC key
    {
        UpdateMusicStream(music);

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(RAYWHITE); 
        switch (state)
        {
            case MAIN_MENU:
            
                DrawTexture(backgroundTexture, 0, 0, WHITE);
                DrawRectangle(498, 313, 204, 54, YELLOW);
                DrawRectangle(500, 315, 200, 50, BLACK);
                DrawText("Start a new game", 510, 330, 20, WHITE);
                DrawRectangle(498, 373, 204, 54, YELLOW);
                DrawRectangle(500, 375, 200, 50, BLACK);
                DrawText("Load a game", 537, 390, 20, WHITE);
                DrawRectangle(498, 433, 204, 54, YELLOW);
                DrawRectangle(500, 435, 200, 50, BLACK);
                DrawText("Options", 562, 450, 20, WHITE);
                DrawRectangle(498, 493, 204, 54, YELLOW);
                DrawRectangle(500, 495, 200, 50, BLACK);
                DrawText("Exit", 580, 510, 20, WHITE);

                mousePosition = GetMousePosition();
                if(CheckCollisionPointRec(mousePosition, {498, 313, 204, 54}) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    state = GAME;
                }
                else if(CheckCollisionPointRec(mousePosition, {498, 373, 204, 54}) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    state = LOAD_GAME;
                }
                else if(CheckCollisionPointRec(mousePosition, {498, 433, 204, 54}) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    state = OPTIONS;
                }
                else if(CheckCollisionPointRec(mousePosition, {498, 493, 204, 54}) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    state = EXIT;
                }
                break;

            case GAME:
                DrawText("Game", 580, 510, 20, GREEN);
            break;

            case LOAD_GAME:
                DrawText("Load Game", 580, 510, 20, BLACK);
            break;
            
            case OPTIONS:
                DrawText("Options", 580, 510, 20, BLUE);

            break;

            case EXIT:
            close = true;
            break;
            
        }

        EndDrawing();
    }
    
    CloseWindow();        // Close window and OpenGL context
}
