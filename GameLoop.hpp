#define _CRT_SECURE_NO_WARNINGS

#pragma once
#include <thread>
#include "ConsolUtil.hpp"
#include <mmsystem.h>
#include <Digitalv.h>

#pragma comment(lib, "winmm.lib")

using namespace std;


namespace Game_Engine
{
	class GameManager
	{
	private:
		bool is_GameRunning;
		Renderer r;
		Player p;
		Enemy e[6];
		Wall w[4];
		P_HP p_hp;
		Timer timer;
		int game_state;
		chrono::system_clock::time_point coolCheckTimePoint;
		chrono::duration<double> coolTimer;
	public:
		
		GameManager() { is_GameRunning = false; }
		~GameManager() {}
		void Run()
		{
			is_GameRunning = true;
			Initialize();

			while (is_GameRunning)
			{
				Input();
				Update();
				Render();
			}
		}

	private:
		void Stop()
		{
			is_GameRunning = false;
		}
		void Initialize()
		{
			coolCheckTimePoint = chrono::system_clock::now();
			w[0].Resize(-1.0f, -1.0f, 2.0f, 0.1f);
			w[1].Resize(-1.0f, -0.5f, 2.0f, 0.1f);
			w[2].Resize(-1.0f, 0, 2.0f, 0.1f);
			w[3].Resize(-1.0f, 0.5f, 2.0f, 0.1f);
			p_hp.Resize(-1.0f, 1, 2.0f, 0.05f);
			timer.Resize(-1.0f, 0.95f, 2.0f, 0.05f);
			game_state = 1;
			PlaySound(TEXT("title.wav"), 0, SND_FILENAME | SND_ASYNC | SND_LOOP);
		}

		void Input()
		{
			coolTimer = chrono::system_clock::now() - coolCheckTimePoint;
			if (GetAsyncKeyState(VK_LEFT) & 0x8000 || GetAsyncKeyState(VK_LEFT) & 0x8001)
			{
				if (game_state == 2) {
					p.isLeftPressed();
				}
			}
			if (GetAsyncKeyState(VK_RIGHT) & 0x8000 || GetAsyncKeyState(VK_RIGHT) & 0x8001)
			{
				if (game_state == 2) {
					p.isRightPressed();\
				}
			}
			if (GetAsyncKeyState(VK_DOWN) & 0x8000 || GetAsyncKeyState(VK_DOWN) & 0x8001)
			{
				if (game_state == 2) {
					if (coolTimer.count() > 0.2) {
						p.isDownPressed();
						coolCheckTimePoint = chrono::system_clock::now();
					}
				}
			}
			if (GetAsyncKeyState(VK_SPACE) & 0x8000 || GetAsyncKeyState(VK_SPACE) & 0x8001)
			{
				if (game_state == 2) {
					p.isSpacePressed();
				}
				if (game_state == 1) {
					game_state = 2;
					PlaySound(TEXT("bgm.wav"), 0, SND_FILENAME | SND_ASYNC | SND_LOOP);
					r.setTexture("elf2.bmp");
				}
			}
			if (GetAsyncKeyState(VK_ESCAPE) & 0x8000 || GetAsyncKeyState(VK_ESCAPE) & 0x8001)
			{
				Stop();
			}
		}

		void CheckRound() {
			if (e[0].is_spawn == false) {
				e[0].is_spawn = true;
				e[0].Respawn();
			}
			if (e[1].is_spawn == false) {
				e[1].is_spawn = true;
				e[1].changeD(1.25f, 0.045f);
				e[1].Respawn();
			}
			if (timer.HP < 270 && e[2].is_spawn == false) {
				e[2].is_spawn = true;
				e[2].changeD(1.2f,0.048f);
				e[2].Respawn();
			}
			if (timer.HP < 200 && e[3].is_spawn == false) {
				e[3].is_spawn = true;
				e[3].changeD(1.1f, 0.051f);
				e[3].Respawn();
			}
			if (timer.HP < 130 && e[4].is_spawn == false) {
				e[4].is_spawn = true;
				e[4].changeD(1.0f, 0.054f);
				e[4].Respawn();
			}
			if (timer.HP < 60 && e[5].is_spawn == false) {
				e[5].is_spawn = true;
				e[5].changeD(0.9f, 0.057f);
				e[5].Respawn();
			}
		}

		void Update()
		{
			if (game_state == 2) {

				CheckRound();
				p.Update();
				for (int i = 0; i < 5; i++) {
					e[i].Update();
				}
				for (int i = 0; i < 4; i++) {
					p.CheckWall(w[i]);
				}
				for (int i = 0; i < 6; i++) {
					if (int j = p.CheckEnemy(e[i])) {
						if (j == 1) {
							p_hp.HP -= 15;
							e[i].point_x = -1.6f;
							if (p_hp.HP <= 0) {
								PlaySound(TEXT("lose.wav"), 0, SND_FILENAME | SND_ASYNC | SND_LOOP);
								game_state = 3;
								return;
							}
						}
					};
				}
				int a = timer.Update();
				if (a == 4) {
					PlaySound(TEXT("win.wav"), 0, SND_FILENAME | SND_ASYNC | SND_LOOP);
					game_state = 4;
				}
				p_hp.Update();

			}
		}
		void Render()
		{
			r.Clear();
			if (game_state == 2) {
				r.Draw(p);
				int A = r.Draw(p_hp);
				if (A == 0) {
					Stop();
				}
				int B = r.Draw(timer);
				if (B == 3) {
					p_hp.HP = 100;
				}
				for (int i = 0; i < 5; i++) {
					r.Draw(e[i]);
				}
				for (int i = 0; i < 4; i++) {
					r.Draw(w[i]);
				}
			}
			else {
				r.DrawScene(game_state);
			}
			r.Rendering();
			this_thread::sleep_for(chrono::milliseconds(15));
		}

		
	};






}