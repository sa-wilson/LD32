#include <Windows.h>
#include "game.h"

char* filetobuf(char *file)
{
	FILE *fptr;
	long length;
	char *buf;

	fptr = fopen(file, "rb"); /* Open file for reading */
	if (!fptr) /* Return NULL on failure */
		return NULL;
	fseek(fptr, 0, SEEK_END); /* Seek to the end of the file */
	length = ftell(fptr); /* Find out how many bytes into the file we are */
	buf = (char*) malloc(length + 1); /* Allocate a buffer for the entire length of the file and a null terminator */
	fseek(fptr, 0, SEEK_SET); /* Go back to the beginning of the file */
	fread(buf, length, 1, fptr); /* Read the contents of the file in to the buffer */
	fclose(fptr); /* Close the file */
	buf[length] = 0; /* Null terminator */
	
	return buf; /* Return the buffer */
}

void doGameLoop(GLFWwindow* glfwWindow, FMOD_SYSTEM* fmodSystem)
{
	int i;
	GLuint vao, vbo[2];
	int isCompiled_VS, isCompiled_FS;
	int isLinked;
	int maxLength;
	char *vertexInfoLog;
	char *fragmentInfoLog;
	char *shaderProgramInfoLog;

	/* We're going to create a simple diamond made from lines */
	const GLfloat diamond[5][2] = {
		{ 0.0, 1.0 }, /* Top point */
		{ 1.0, 0.0 }, /* Right point */
		{ 0.0, -1.0 }, /* Bottom point */
		{ -1.0, 0.0 }, /* Left point */
		{ 0.0, 1.0 } }; /* Top point */

	const GLfloat colors[5][3] = {
		{ 1.0, 0.0, 0.0 }, /* Red */
		{ 0.0, 1.0, 0.0 }, /* Green */
		{ 0.0, 0.0, 1.0 }, /* Blue */
		{ 1.0, 1.0, 1.0 }, /* White */
		{ 1.0, 0.0, 0.0 } }; /* Red */

	/* These pointers will receive the contents of our shader source code files */
	GLchar *vertexsource, *fragmentsource;
	/* These are handles used to reference the shaders */
	GLuint vertexshader, fragmentshader;
	/* This is a handle to the shader program */
	GLuint shaderprogram;

	double lastTime = glfwGetTime();
	int frames = 0;

	fprintf(stdout, "doGameLoop()\n");

	gameState state = GAMESTATE_INIT;
	
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(2, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 10 * sizeof(GLfloat), diamond, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	vertexsource = filetobuf("resources/shaders/tutorial2.vert");
	fragmentsource = filetobuf("resources/shaders/tutorial2.frag");

	vertexshader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexshader, 1, (const GLchar**) &vertexsource, 0);
	glCompileShader(vertexshader);
	glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &isCompiled_VS);
	if (!isCompiled_VS) {
		glGetShaderiv(vertexshader, GL_INFO_LOG_LENGTH, &maxLength);
		vertexInfoLog = (char*) malloc(maxLength);
		glGetShaderInfoLog(vertexshader, maxLength, &maxLength, vertexInfoLog);

		fprintf(stderr, "OpenGL Error: Failed to compile Vertex Shader\n%s", vertexInfoLog);
		free(vertexInfoLog);
		return;
	}

	fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentshader, 1, (const GLchar**) &fragmentsource, 0);
	glCompileShader(fragmentshader);
	glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &isCompiled_FS);
	if (!isCompiled_FS) {
		glGetShaderiv(fragmentshader, GL_INFO_LOG_LENGTH, &maxLength);
		fragmentInfoLog = (char*) malloc(maxLength);
		glGetShaderInfoLog(fragmentshader, maxLength, &maxLength, fragmentInfoLog);

		fprintf(stderr, "OpenGL Error: Failed to compile fragment Shader\n%s", fragmentInfoLog);
		free(fragmentInfoLog);
		return;
	}

	shaderprogram = glCreateProgram();
	glAttachShader(shaderprogram, vertexshader);
	glAttachShader(shaderprogram, fragmentshader);
	glBindAttribLocation(shaderprogram, 0, "in_Position");
	glBindAttribLocation(shaderprogram, 1, "in_Color");
	glLinkProgram(shaderprogram);
	glGetProgramiv(shaderprogram, GL_LINK_STATUS, &isLinked);
	if (!isLinked) {
		glGetProgramiv(shaderprogram, GL_INFO_LOG_LENGTH, &maxLength);
		shaderProgramInfoLog = (char*) malloc(maxLength);
		glGetProgramInfoLog(shaderprogram, maxLength, &maxLength, shaderProgramInfoLog);

		fprintf(stderr, "OpenGL Error: Failed to link program\n%s", shaderProgramInfoLog);
		free(shaderProgramInfoLog);
		return;
	}

	glUseProgram(shaderprogram);

	/*for (i = 2; i <= 4; i++) {
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_LINE_LOOP, 0, i);
		glfwSwapBuffers(glfwWindow);
		Sleep(2000);
	} */

	glClearColor(0.0, 0.0, 0.0, 1.0);

	state = GAMESTATE_INTRO;
	/* Play the intro */

	state = GAMESTATE_MENU;
	do {
		double currentTime = glfwGetTime();
		frames++;
		if (currentTime - lastTime >= 1.0) {
			char title[256];;

			sprintf_s(title, 255, "Irsee - [FPS: %3.2f Frames: %d Time: %3.2f]", (float) frames / (currentTime - lastTime), frames, (float) (currentTime - lastTime));

			glfwSetWindowTitle(glfwWindow, title);

			frames = 0;
			lastTime = glfwGetTime();
		}

		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawArrays(GL_TRIANGLES, 2, 3);

		FMOD_System_Update(fmodSystem);
		glfwSwapBuffers(glfwWindow);
		glfwPollEvents();
	} while (!glfwWindowShouldClose(glfwWindow));

	/* Cleanup */
	glUseProgram(0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDetachShader(shaderprogram, vertexshader);
	glDetachShader(shaderprogram, fragmentshader);
	glDeleteProgram(shaderprogram);
	glDeleteShader(vertexshader);
	glDeleteShader(fragmentshader);
	glDeleteBuffers(2, vbo);
	glDeleteVertexArrays(1, &vao);
	free(vertexsource);
	free(fragmentsource);

	fprintf(stdout, "End doGameLoop()\n");
}