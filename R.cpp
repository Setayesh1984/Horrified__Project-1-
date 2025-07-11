#include "raylib.h"
#include <iostream>

using namespace std;

int main(void){

    InitWindow(1200, 600,"horrified");
    SetTargetFPS(60);
    InitAudioDevice();
    Sound sound = LoadSound("scary-music-instrumental 001.mp3");
    Image Img = LoadImage("horrified.jpeg");
    Texture2D tex = LoadTextureFromImage(Img);
    PlaySound(sound);

    while (!WindowShouldClose()){
        BeginDrawing();
        DrawTexture(tex, 0, 0, WHITE);
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
        EndDrawing();
    }
    UnloadImage(Img);
    UnloadTexture(tex);
    UnloadSound(sound);
    CloseAudioDevice();

    CloseWindow();
    return 0;
}