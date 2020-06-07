

/*
 *   This classic game uses http://www.libsigil.com/ (v0.9.0)
 *   and https://www.dafont.com/press-start-2p.font
 *   Compiles on Code::Blocks 17.12 with MinGW32
 */

#include "sl.h"
#include <stdlib.h> // Included for the rand() function
#include <stdio.h>

void DrawCar(int X, int Y);
void GetInput(int &X, int &Y, double &GameSpeed);
void DrawSingleBrick(int X, int Y);
void DrawLines(int Pos);
bool DrawEnemiesWithCD(bool Enemies[], int Pos, int PlayerX, int PlayerY);
void DrawLevel(int GameScore, int font);
void GameOver(int font);
void Intro(int font);

#define BrickSize 10

int main(int args, char *argv[])
{

    // setup our window and a few resources we need
    slWindow(640, 480, "Classic Brick Racing", false);

    slSetBackColor(0.0, 0.0, 0.0);		// black

    int font = slLoadFont("ttf/PressStart2P.ttf");
    int PlayerX = 270, PlayerY = 100, GameLevel = 1;
    double GameSpeed = 120;
    double dt; // Delta Time
    double RoadScrollY = 1, EnemieScrollY = 1;

    // array of enemies
    bool Enemies[20];

    Intro(font);

    // Generate random enemies
    // True = enemy to the left, false = enemy to the right
    for(int i = 0; i < 19; i++){
        if(rand()%2){
            Enemies[i]=true;
        }
        else{
            Enemies[i]=false;
        }
    }

    // The window is open as long as it's not closed or the user presses escape
    while(!slShouldClose() && !slGetKey(SL_KEY_ESCAPE))
    {
        dt = slGetDeltaTime(); // Get time since last slRender() call

        // For smooth animation
        RoadScrollY -= dt*GameSpeed;
        EnemieScrollY -= dt*GameSpeed;

        // If the road has scrolled more than -80 pixels reset to 0.
        if(RoadScrollY < -80){
            RoadScrollY = 0;
        }

        // Reset to next level
        if(EnemieScrollY < -2800){
            EnemieScrollY = 400;
            GameSpeed += 20;
            GameLevel += 1;
            for(int i = 0; i < 19; i++){
                if(rand()%2){
                    Enemies[i]=true;
                }
                else{
                    Enemies[i]=false;
                }
            }
        }

        GetInput(PlayerX, PlayerY, GameSpeed);
        DrawCar(PlayerX, PlayerY);
        DrawLines(RoadScrollY);
        DrawLevel(GameLevel, font);

        // Draw enemy card and check for collision detection
        if(DrawEnemiesWithCD(Enemies, EnemieScrollY, PlayerX, PlayerY)){

            slRender();
            GameOver(font);

            double Wait = slGetTime();
            while( slGetTime() - Wait < 3.0){}
            slClose();

            return 1;

        }
        // draw everything
        slRender();
    }

    slClose();// close the window and shutdown SIGIL
    return 0;
}


void DrawCar(int X, int Y){
    // Call DrawSingleBrick 12 times to draw a car
    bool Car[4][3] = {{false, true, false},
                     {true, true, true},
                     {false, true, false},
                     {true, false, true}};

    for(int CounterY = 0; CounterY <= 3; CounterY++){
        for(int CounterX = 0; CounterX <= 2; CounterX++){
            if(Car[CounterY][CounterX] == true){
                DrawSingleBrick(X+(CounterX*BrickSize), Y-(CounterY*BrickSize));
            }
        }
    }
    return;
}

void DrawLines(int Pos){
    // Draw the road
    for(int Row = 520; Row >= 0; Row-=40){
        DrawSingleBrick(213, Row+Pos);
        DrawSingleBrick(426, Row+Pos);
        DrawSingleBrick(213, Row-BrickSize+Pos);
        DrawSingleBrick(426, Row-BrickSize+Pos);
    }
};


void DrawSingleBrick(int X, int Y){
    // draw the inner fill for a rectangle
    slSetForeColor(0.5, 0.5, 0.0, 0.5);
    slRectangleFill( X, Y, BrickSize, BrickSize);

    // draw the outline for a rectangle
    slSetForeColor(0.8, 0.8, 0.0, 0.8);
    slRectangleOutline( X, Y, BrickSize, BrickSize);
    return;
}



void GetInput(int &X, int &Y, double &GameSpeed){
    if(slGetKey(SL_KEY_UP) && GameSpeed == 60)
    {
        GameSpeed+=60;
    }
    if(slGetKey(SL_KEY_DOWN) && GameSpeed == 120)
    {
        GameSpeed-=60;
    }

    if(slGetKey(SL_KEY_LEFT) && X > 270){
        X-=80;
    }
    if(slGetKey(SL_KEY_RIGHT) && X < 350){
        X+=80;
    }

    return;
}


// Draw other cars and check for collisions
bool DrawEnemiesWithCD(bool Enemies[], int Pos, int PlayerX, int PlayerY){
    int EnemyX = 0; int EnemyY = 0;
    for(int i = 0; i < 19; i++){
        if(Enemies[i]==true){ // Draw car to the left

            EnemyX = 270; EnemyY = Pos+400+(i*120);
            DrawCar(EnemyX, EnemyY);

            // Collision detection
            if( PlayerX==EnemyX && EnemyY<140 && EnemyY>60 ){
               return true;
            }
        }

        if(Enemies[i]==false){ // Draw car to the right

            EnemyX = 350; EnemyY = Pos+400+(i*120);
            DrawCar(EnemyX, EnemyY);

            // Collision detection
            if( PlayerX==EnemyX && EnemyY<140 && EnemyY>60 ){
               return true;
            }
        }
    }
    return false;
}


void GameOver(int font){
	slSetFont(font, 18);
    slText(200, 240, "[ Game Over ]");
    slRender();
    return;
}


void Intro(int font){
	slSetForeColor(0.8, 0.8, 0.0, 0.8);
    while(!slShouldClose() && !slGetKey(SL_KEY_ENTER)){
        slSetFont(font, 18);
        slText(150, 250, "Classic Brick Racing");
        slSetFont(font, 14);
        slText(240, 200, "Press Enter!");
        slRender();
    }
    return;
}

void DrawLevel(int Level, int font){
	slSetFont(font, 12);
    char ScoreText[10];
    sprintf(ScoreText, "%d", Level); // Convert int to char
    slText(500, 240, "Level");
    slText(510, 220, ScoreText);
    return;
}

