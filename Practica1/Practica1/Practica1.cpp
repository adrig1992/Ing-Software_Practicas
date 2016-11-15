#include "stdafx.h"
#include <conio.h>
#include <stdlib.h>
#include <windows.h>
#include <iostream>
#include <chrono>

//CONSTANTS
const int   LEFT_CHARACTER = 'a';
const int   RIGHT_CHARACTER = 'd';
const int   SHOOT_LEFT_CHARACTER = 'j';
const int   SHOOT_RIGHT_CHARACTER = 'k';
const int   EXIT_CHARACTER = '1';

const char  WORLD_CHARACTER = '_';
const char  PLAYER_CHARACTER = '@';
const char  BULLET_CHARACTER = '-';

const int   WORLD_WIDTH = 80;
const int	FPS = 20;
const int   BULLET_SPEED = 2;

//GAME VARIABLES
int  charPosition = WORLD_WIDTH / 2;
int  bulletPosition;
int  delay = 1000 / FPS;
int  shoot = 0;
int  actualFPS = FPS;
bool keepRunning = true;

//FUNCTIONS DECLARATION
void  keyboardManagement();
void  worldUpdate();
void  shootChecker();
void  gotoXY(const short x, const short y);
int   WaitForFPS(const int desiredFPS, std::chrono::steady_clock::time_point startTime);

//MAIN
int main() {
    do {
        std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now(); //Update the start time to calculate the fps later on.

        std::cout << "FPS: " << actualFPS << std::endl << std::endl;                        //Show the fps counter.

        keyboardManagement();	                                                            //Get the input from the keyboard.
        worldUpdate();			                                                            //Update the world with the new values.
        shootChecker();			                                                            //If the character is shooting update the bullet.
        
        gotoXY(0, 0);                                                                       //Move the cursor position to the beginning (delete all in the next iteration)

        actualFPS = WaitForFPS(FPS, startTime);                                             //Calculate frame time, update the delay, and wait.
    } while (keepRunning);

    return 0;
}

//FUNCTIONS DEFINITION
void keyboardManagement() {
    if (_kbhit()) {
        switch (_getch()) {
        case LEFT_CHARACTER:		if (charPosition > 0)				charPosition--;										break; //Left movement
        case RIGHT_CHARACTER:		if (charPosition < WORLD_WIDTH - 1)	charPosition++;										break; //Right movement
        case SHOOT_LEFT_CHARACTER:	if (shoot == 0) {                   shoot = -1; bulletPosition = charPosition - 1; }	break; //Left shoot
        case SHOOT_RIGHT_CHARACTER:	if (shoot == 0) { shoot = 1;	    bulletPosition = charPosition + 1; }	            break; //Right shoot
        case EXIT_CHARACTER:											keepRunning = false;								break; //Exit game
        }
    }
}

void worldUpdate() {
    for (int i = 0; i < WORLD_WIDTH; i++) {
        if (i == charPosition)							std::cout << PLAYER_CHARACTER;
        else if (i == bulletPosition && shoot != 0)		std::cout << BULLET_CHARACTER;
        else											std::cout << WORLD_CHARACTER;
    }
}

void shootChecker() {
    if (shoot == 0) return;

    bulletPosition += shoot*BULLET_SPEED;
    if (bulletPosition < 0 || bulletPosition > WORLD_WIDTH) {
        shoot = 0;
    }
}

int WaitForFPS(const int desiredFPS, std::chrono::steady_clock::time_point startTime) {
    std::chrono::steady_clock::time_point endTime = std::chrono::steady_clock::now();
    std::chrono::duration<double> frameTime = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime);
    delay = ((1000 / FPS) - static_cast<int>(frameTime.count() * 1000));
    Sleep(delay);
    return 1000 / (static_cast<int>(frameTime.count() * 1000) + delay);
}

void gotoXY(const short x, const short y) {
    COORD pos = { x, y };
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(output, pos);
}
