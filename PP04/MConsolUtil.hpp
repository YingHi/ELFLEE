#pragma once
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <Windows.h>

#pragma comment(lib,"OpenGL32")

using namespace std;

namespace MuSeoun_Engine
{
	class Unit {
	public:
		float point_x = 0, point_y = 0, size_h = 0, size_w = 0;
		string name;
	};
	class Player : Unit {
	public:
		Player() {
			name = "Player";
			point_x = 0;
			point_y = 0;
			size_h = 0.005f;
		}
		void isKeyPressed() {
			point_y += 0.01f;
		}
		void isKeyUnpressed() {
			if (!(point_y <= 0)) {
				point_y -= 0.01f;
			}
		}
	};

	class Enemy : Unit {
	public:
		Enemy() {
			name = "Enemy";
			size_h = 0.0025f;
			size_w = 0.005f;
			Respawn();
		}

		void charge() {
			point_x -= 0.015f;
		}

		void Respawn() {
			point_x = 1;
			point_y = 0;
		}
	};

	class MConsoleRenderer
	{
	public :
		GLFWwindow* window;
		MConsoleRenderer() { 
			glfwSetErrorCallback(error_callback);
			if (!glfwInit())
				exit(EXIT_FAILURE);
			window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
			if (!window)
			{
				glfwTerminate();
				exit(EXIT_FAILURE);
			}
			glfwMakeContextCurrent(window);
			glfwSetKeyCallback(window, key_callback);
		}
		~MConsoleRenderer() {
			glfwDestroyWindow(window);
			glfwTerminate();
			exit(EXIT_SUCCESS);
		}

		void Clear() {
			float ratio;
			int width, height;
			glfwGetFramebufferSize(window, &width, &height);
			ratio = width / (float)height;

			glClearColor(0.7f, 0.7f, 0.7f, 1);
			glClear(GL_COLOR_BUFFER_BIT);

		}

		int Draw(Unit A)
		{
			if (!glfwWindowShouldClose(window))
			{
				glBegin(GL_TRIANGLE_STRIP);
				glColor3f(0.0f, 0.0f, 1.0f);
				//glVertex2f(a[0], a[1]);
				glEnd();

				glfwSwapBuffers(window);
				glfwPollEvents();
				return 1;
			}
			else {
				return 0;
			}
		}

		static void error_callback(int error, const char* description)
		{
			fputs(description, stderr);
		}
		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
				glfwSetWindowShouldClose(window, GL_TRUE);
		}
	};
	
}