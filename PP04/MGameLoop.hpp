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
		}
		void Release() 
		{
		}

		void Input()
		{
			FPS = 1.0 / renderDuration.count();
			string fps = "FPS : " + to_string(FPS);
			cRenderer.DrawString(fps);
			startRenderTimePoint = chrono::system_clock::now();
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