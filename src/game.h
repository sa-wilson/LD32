#pragma once
#include "main.h"

void doGameLoop(GLFWwindow* glfwWindow, FMOD_SYSTEM *fmodSystem);

typedef int gameState;

static const int GAMESTATE_INIT = 0;
static const int GAMESTATE_INTRO = 1;
static const int GAMESTATE_MENU = 2;
static const int GAMESTATE_PLAYING = 3;
