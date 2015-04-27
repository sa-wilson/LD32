#include "main.h"
#include "oglCallbacks.h"
#include "game.h"

int main(int argc, char** argv)
{
	GLFWwindow* glfwWindow = NULL;
	FMOD_RESULT fmodResult;
	FMOD_SYSTEM* fmodSystem = NULL;

	fprintf(stdout, "Beginning initialisation\n");

	glfwSetErrorCallback(glfwErrorCallback);

	if (!glfwInit()) exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	glfwWindow = glfwCreateWindow(512, 512, "LD32", NULL, NULL);
	if (!glfwWindow) {
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(glfwWindow);

	glfwSetKeyCallback(glfwWindow, glfwKeyCallback);

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		exit(-1);
	}
	if (!GLEW_VERSION_3_2) {
		fprintf(stderr, "OpenGL 3.2 API is not available.");
		exit(-1);
	}

	fmodResult = FMOD_System_Create(&fmodSystem);
	if (fmodResult != FMOD_OK)
	{
		fprintf(stderr, "FMOD Error! (%d) %s\n", fmodResult, FMOD_ErrorString(fmodResult));
		exit(-1);
	}

	fmodResult = FMOD_System_Init(fmodSystem, 512, FMOD_INIT_NORMAL, 0);
	if (fmodResult != FMOD_OK)
	{
		fprintf(stderr, "FMOD Error! (%d) %s\n", fmodResult, FMOD_ErrorString(fmodResult));
		exit(-1);
	}

	doGameLoop(glfwWindow, fmodSystem);

	FMOD_System_Release(fmodSystem);
	glfwDestroyWindow(glfwWindow);
	glfwTerminate();

	exit(EXIT_SUCCESS);
}
