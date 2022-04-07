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
		double count; // 초 세는거 (타임포인트 더하는 변수) 1초인지 아닌지 카운트
		double FPS; // 출력용 함수
		chrono::system_clock::time_point startRenderTimePoint;
		chrono::duration<double> renderDuration;
		
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
			FPS = 0;
		}
		void Release() 
		{
		}

		void Input()
		{
			count += renderDuration.count();
			FPS = 1.0 / count;

			string fps = "FPS : " + to_string(FPS);
			cRenderer.DrawString(fps);
			count = 0;
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

			renderDuration = chrono::system_clock::now() - startRenderTimePoint;
			
		}


			////cout << "Rendering speed : " << renderDuration.count() << "sec" << endl;

			//int remainingFrameTime = 100 - (int)(renderDuration.count() * 1000.0);
			//if (remainingFrameTime > 0)
			//	this_thread::sleep_for(chrono::milliseconds(remainingFrameTime));
		

				
		
	};

	


	

}