#define _CRT_SECURE_NO_WARNINGS

#pragma once
#include <GLFW/glfw3.h>
#include <iostream>
#include <Windows.h>
#include <stdlib.h>
#include <chrono>

#pragma comment(lib,"OpenGL32")

using namespace std;

namespace Game_Engine
{
	class Unit {
	public:
		float point_x = 0, point_y = 0, width = 0, height = 0, HP = 100;
		string name;
	};

	class Wall : public Unit {
	public:
		void Resize(float x, float y, float w, float h) {
			name = "Wall";
			point_x = x;
			point_y = y;
			width = w;
			height = h;
		}
	};
	class P_HP : public Unit {
	public:
		void Resize(float x, float y, float w, float h) {
			name = "P_HP";
			point_x = x;
			point_y = y;
			width = w;
			height = h;
			HP = 100;
		}
		void Update() {
			HP += 0.01f;
			if (HP > 100) HP = 100;
			width = 0.02f * HP;
		}
	};
	class Timer : public Unit {
	private:
		chrono::system_clock::time_point TimePoint;
		chrono::duration<double> gameTimer;
	public:
		void Resize(float x, float y, float w, float h) {
			name = "Timer";
			point_x = x;
			point_y = y;
			width = w;
			height = h;
			HP = 300;
			TimePoint = chrono::system_clock::now();
		}
		int Update() {
			gameTimer = chrono::system_clock::now() - TimePoint;
			if (gameTimer.count() > 1) {
				HP -= 1;
				TimePoint = chrono::system_clock::now();
			}
			width = 0.0067f * HP;
			if (HP > 0) return 2;
			else return 4;
		}
	};
	class Enemy : public Unit {
	private:
		bool is_running; 
		float speed, cool;
		chrono::system_clock::time_point GenTimePoint;
		chrono::duration<double> RespawnTimer;
	public:
		bool is_spawn;
		Enemy() {
			name = "Enemy";
			width = 0.25f;
			height = 0.4f;
			point_x = -1.6f;
			cool = 1.3f;
			speed = 0.042f;
			is_spawn = false;
		}

		void changeD(float c, float s) {
			cool = c;
			speed = s;
		}

		void Update() {
			if (is_spawn) {
				if (is_running) {
					point_x -= speed;
					if (point_x < -1.31f) {
						is_running = false;
						GenTimePoint = chrono::system_clock::now();
					}
				}
				else {
					RespawnTimer = chrono::system_clock::now() - GenTimePoint;
					if (RespawnTimer.count() > cool) {
						Respawn();
					}
				}
			}

		}

		void Respawn() {
			point_x = 1;
			int a = rand() % 4 + 1;
			switch (a) {
			case 1: point_y = -1;
				break;
			case 2: point_y = -0.5f;
				break;
			case 3: point_y = 0;
				break;
			case 4: point_y = 0.5f;
				break;
			default: point_y = 0.5f;
			}
			is_running = true;

		}
	};


	class Player : public Unit {
	private:
		bool is_jump, is_jumpable;
		float jumpPower, jumpHeight, gravity;
	public:
		Player() {
			name = "Player";
			point_x = -0.7f;
			point_y = -1;
			width = 0.1f;
			height = 0.1f;
			is_jump = false;
			jumpPower = 0;
			gravity = 0.03f;
			jumpHeight = 0;
			is_jumpable = true;
		}
		void isLeftPressed() {
			if (point_x > -0.95) point_x -= 0.02f;
		}
		void isRightPressed() {
			if (point_x < 0.9) point_x += 0.02f;
		}
		void isDownPressed() {
			if (point_y >= 0.5f && !(is_jump) ){
				point_y = 0.01f;
			}
			else if (point_y >= 0 && !(is_jump)) {
				point_y = -0.49f;
			}
			else if (point_y >= -0.5f && !(is_jump)) {
				point_y = -0.99f;
			}
		}
		void isSpacePressed() {
			if (is_jumpable) {
				is_jumpable = false;
				is_jump = true;
				jumpHeight = point_y + 0.7f;
				if (jumpHeight > 1) jumpHeight = 0.8f;
				jumpPower = 0.1f;
			}
		}

		void Update() {
			if (is_jump) { // is_jump일때 점프
				if (point_y < jumpHeight) { // 점프 목표 높이에 도달할때까지 점프
					point_y += jumpPower;
					if (jumpPower > 0.05f) jumpPower -= 0.003f;
				}
				else { // 도달시 하강
					gravity = 0.01f;
					is_jump = false;
				}
			}
			else if(!(is_jumpable) && !(is_jump)) {
				point_y -= gravity;
				gravity += 0.002f;
			}
		}

		void CheckWall(Wall w) {
			if (point_x > w.point_x && point_x < w.point_x + w.width && point_y < w.point_y && point_y > w.point_y - w.height) {
				if (is_jump == false) {
					is_jumpable = true;
					point_y = w.point_y;
				}
			}
		}
		int CheckEnemy(Enemy w) {
			if (point_x+width > w.point_x && point_x < w.point_x + w.width && point_y+height > w.point_y && point_y < w.point_y + w.height) {
				return 1;
			}
			return 0;
		}
	};

	typedef struct tagBITMAPHEADER {
		BITMAPFILEHEADER bf;
		BITMAPINFOHEADER bi;
	}BITMAPHEADER;

	
	class Renderer
	{
	private :
		GLFWwindow* window;
		BITMAPHEADER originalHeader;	//비트맵의 헤더부분을 파일에서 읽어 저장할 구조체
		int imgSize[3];			//이미지의 크기를 저장할 변수
		BYTE* image;
		GLuint texName;

		BYTE* LoadBitmapFile(BITMAPHEADER* bitmapHeader, int* imgSize, const char* filename)
		{
			FILE* fp = fopen(filename, "rb");	//파일을 이진읽기모드로 열기
			if (fp == NULL)
			{
				printf("파일로딩에 실패했습니다.\n");	//fopen에 실패하면 NULL값을 리턴
				return NULL;
			}
			else
			{
				fread(&bitmapHeader->bf, sizeof(BITMAPFILEHEADER), 1, fp);	//비트맵파일헤더 읽기
				fread(&bitmapHeader->bi, sizeof(BITMAPINFOHEADER), 1, fp);	//비트맵인포헤더 읽기

				int imgSizeTemp = (bitmapHeader->bi.biWidth * bitmapHeader->bi.biHeight)*3;	//이미지 사이즈 계산
				*imgSize = imgSizeTemp;	// 이미지 사이즈를 상위 변수에 할당

				BYTE* image = (BYTE*)malloc(sizeof(BYTE) * imgSizeTemp);	//이미지크기만큼 메모리할당
				fread(image, sizeof(BYTE), imgSizeTemp, fp);//이미지 크기만큼 파일에서 읽어오기

				BYTE B, R;

				for (int i = 0; i < imgSize[0]; i += 3) {

					B = image[i];
					R = image[i + 2];

					image[i] = R;
					image[i + 2] = B;
				}
				fclose(fp);

				return image;
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

	public:
		Renderer() {
			glfwSetErrorCallback(error_callback);
			if (!glfwInit())
				exit(EXIT_FAILURE);
			window = glfwCreateWindow(640, 480, "Dodge_Elflee", NULL, NULL);
			if (!window)
			{
				glfwTerminate();
				exit(EXIT_FAILURE);
			}
			glfwMakeContextCurrent(window);
			glfwSetKeyCallback(window, key_callback);
		}
		~Renderer() {
			glfwDestroyWindow(window);
			glfwTerminate();
			exit(EXIT_SUCCESS);
		}
		void setTexture(const char* filename) {
			image = LoadBitmapFile(&originalHeader, &imgSize[0], filename);
			if (image == NULL) return;
			glPixelStorei(GL_UNPACK_ALIGNMENT, 3);
			glGenTextures(1, &texName);
			glBindTexture(GL_TEXTURE_2D, texName);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		}

		void Clear() {
			int width, height;
			glfwGetFramebufferSize(window, &width, &height);

			glClearColor(0.5f, 0.8f, 0.5f, 1);
			glClear(GL_COLOR_BUFFER_BIT);
		}

		void DrawScene(int A)
		{
			if (!glfwWindowShouldClose(window))
			{
				if (A == 1) {
					setTexture("Title.bmp");
					glEnable(GL_TEXTURE_2D);
					glBegin(GL_QUADS);
					glColor3f(1, 1, 1);
					glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
					glTexCoord2f(0.0f, 1.0f); glVertex3f(-1, 1, 0);
					glTexCoord2f(1.0f, 1.0f); glVertex3f(1, 1, 0);
					glTexCoord2f(1.0f, 0.0f); glVertex3f(1, -1, 0);
					glTexCoord2f(0.0f, 0.0f); glVertex3f(-1, -1, 0);
					glEnd();
					glDisable(GL_TEXTURE_2D);
				}
				if (A == 3) {
					setTexture("Lose.bmp");
					glEnable(GL_TEXTURE_2D);
					glBegin(GL_QUADS);
					glColor3f(1, 1, 1);
					glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
					glTexCoord2f(0.0f, 1.0f); glVertex3f(-1, 1, 0);
					glTexCoord2f(1.0f, 1.0f); glVertex3f(1, 1, 0);
					glTexCoord2f(1.0f, 0.0f); glVertex3f(1, -1, 0);
					glTexCoord2f(0.0f, 0.0f); glVertex3f(-1, -1, 0);
					glEnd();
					glDisable(GL_TEXTURE_2D);
				}
				if (A == 4) {
					setTexture("Win.bmp");
					glEnable(GL_TEXTURE_2D);
					glBegin(GL_QUADS);
					glColor3f(1, 1, 1);
					glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
					glTexCoord2f(0.0f, 1.0f); glVertex3f(-1, 1, 0);
					glTexCoord2f(1.0f, 1.0f); glVertex3f(1, 1, 0);
					glTexCoord2f(1.0f, 0.0f); glVertex3f(1, -1, 0);
					glTexCoord2f(0.0f, 0.0f); glVertex3f(-1, -1, 0);
					glEnd();
					glDisable(GL_TEXTURE_2D);
				}
			}
			else {
				return;
			}
		}

		int Draw(Unit A)
		{
			if (!glfwWindowShouldClose(window))
			{
				if (A.name == "Player") {
					glBegin(GL_TRIANGLE_STRIP);
					glColor3f(0.0f, 0.0f, 1.0f);
					glVertex2f(A.point_x, A.point_y);
					glVertex2f((A.point_x + A.width), A.point_y);
					glVertex2f(A.point_x, (A.point_y + A.height));
					glVertex2f((A.point_x + A.width), (A.point_y + A.height));
					glEnd();
				}
				if (A.name == "Wall") {
					glBegin(GL_TRIANGLE_STRIP);
					glColor3f(0, 0, 0);
					glVertex2f(A.point_x, A.point_y);
					glVertex2f((A.point_x + A.width), A.point_y);
					glVertex2f(A.point_x, (A.point_y - A.height));
					glVertex2f((A.point_x + A.width), (A.point_y - A.height));
					glEnd();
				}
				if (A.name == "P_HP") {
					glBegin(GL_TRIANGLE_STRIP);
					glColor3f(1, 0, 0);
					glVertex2f(A.point_x, A.point_y);
					glVertex2f((A.point_x + A.width), A.point_y);
					glVertex2f(A.point_x, (A.point_y - A.height));
					glVertex2f((A.point_x + A.width), (A.point_y - A.height));
					glEnd();
					if (A.HP <= 0) return 0;
				}
				if (A.name == "Timer") {
					glBegin(GL_TRIANGLE_STRIP);
					glColor3f(0, 0, 1);
					glVertex2f(A.point_x, A.point_y);
					glVertex2f((A.point_x + A.width), A.point_y);
					glVertex2f(A.point_x, (A.point_y - A.height));
					glVertex2f((A.point_x + A.width), (A.point_y - A.height));
					glEnd();
					if (A.HP <= 0) return 3;
				}
				if (A.name == "Enemy") {
					glEnable(GL_TEXTURE_2D);
					glBegin(GL_QUADS);
					glColor3f(1, 1, 1);
					glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
					glTexCoord2f(0.0f, 1.0f); glVertex3f((A.point_x + A.width), (A.point_y + A.height), 0);
					glTexCoord2f(1.0f, 1.0f); glVertex3f(A.point_x, (A.point_y + A.height), 0);
					glTexCoord2f(1.0f, 0.0f); glVertex3f(A.point_x, A.point_y, 0);
					glTexCoord2f(0.0f, 0.0f); glVertex3f((A.point_x + A.width), A.point_y, 0);
					glEnd();
					glDisable(GL_TEXTURE_2D);
				}
				return 1;
			}
			else {
				return 0;
			}
		}
		

		void Rendering() {
			if (!glfwWindowShouldClose(window))
			{
				glfwSwapBuffers(window);
				glfwPollEvents();
			}
		}

	};
};