#pragma once
#include <chrono>
#include <thread>
#include "MConsolUtil.hpp"

using namespace std;

namespace MuSeoun_Engine
{
	class MGameLoop
	{
	private :
		bool _isGameRunning;	
		MConsoleRenderer cRenderer;
		double count; // �� ���°� (Ÿ������Ʈ ���ϴ� ����) 1������ �ƴ��� ī��Ʈ
		int counting; // �� �Լ��� ����ɶ����� ����
		int FPS; // ��¿� �Լ�
		chrono::system_clock::time_point startRenderTimePoint;
		
	public :
		MGameLoop() 	{	_isGameRunning = false;		}
		~MGameLoop() {}

		void Run()
		{
			_isGameRunning = true;
			Initialize();

			while (_isGameRunning)
			{
				Input();
				Update();
				Render();
			}
			Release();
		}
		void Stop()
		{
			_isGameRunning = false;
		}

	private :
		void Initialize()
		{
			count = 0;
			counting = 0;
			FPS = 0;
		}
		void Release() 
		{
		}

		void Input()
		{
			startRenderTimePoint = chrono::system_clock::now();
		/*	if (GetAsyncKeyState(VK_SPACE) & 0x8000 || GetAsyncKeyState(VK_SPACE) & 0x8001)
			{ 
		
			}
			else 
			{
			
			}*/

		}
		void Update()
		{
			
		}
		void Render()
		{

			cRenderer.Clear();
			cRenderer.MoveCursor(10, 20);

			chrono::duration<double> renderDuration = chrono::system_clock::now() - startRenderTimePoint;
			count += renderDuration.count();
			counting++;
			if (count > 1) { 
				FPS = counting;
				counting = 0;
				count = 0;
			}

			string fps = "FPS(milliseconds) : " + to_string(FPS);
			cRenderer.DrawString(fps);
		}


			////cout << "Rendering speed : " << renderDuration.count() << "sec" << endl;

			//int remainingFrameTime = 100 - (int)(renderDuration.count() * 1000.0);
			//if (remainingFrameTime > 0)
			//	this_thread::sleep_for(chrono::milliseconds(remainingFrameTime));
		

				
		
	};

	


	

}