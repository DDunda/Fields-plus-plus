#pragma once

#include <SDL.h>

#ifdef ERROR_LOGGING
#include <string>
#include <iostream>
#endif

#ifndef INPUT_HANDLED
#include <map>
#include <vector>
#endif

// SDL-GAME
namespace SDLG
{

	typedef Uint32 millitime;

	// Programmer has own game loop
#ifndef LOOP_HANDLED

// Called every frame
	void OnFrame();

#endif //!LOOP_HANDLED

	// Called upon initialisation
	void OnStart();
	// Called upon exit
	void OnQuit();

	static bool gameRunning = true;

	static int windowWidth = 500;
	static int windowHeight = 300;

	static SDL_Window* gameWindow = nullptr;
	static SDL_Renderer* gameRenderer = nullptr;
	static Uint32 gameWindowID = 0;

	static Uint32 windowFlags = SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN;

	// Programmer handles time stuff
#ifndef TIME_HANDLED
	static millitime currentTime = 0;
	static millitime previousTime = 0;
	static millitime deltaTime = 0;

	static millitime minFrameDelta = 0;

	static void HandleTime() {
		previousTime = currentTime;
		currentTime = SDL_GetTicks();
		deltaTime = currentTime - previousTime;

		// FPS is capped when a min delta time is given
		if (minFrameDelta > 0 && deltaTime < minFrameDelta) {
			millitime delayTime = minFrameDelta - deltaTime;

			deltaTime = minFrameDelta;
			currentTime = previousTime + deltaTime;

			SDL_Delay(delayTime);
		}
	}
#endif // !TIME_HANDLED

#ifdef ERROR_LOGGING

	static void MakeLog(std::string message) {
		std::cout << message;
		SDL_Log(message.c_str());
	}

#endif // ERROR_LOGGING

#ifndef INPUT_HANDLED

	typedef Uint32 eventType;

	class EventCallback {
	public:
		bool active = true;
		virtual void Callback(SDL_Event&) = 0;
	};

	static std::map<eventType, std::vector<EventCallback*>> callbacks;

	static void TriggerEventCallbacks(SDL_Event& e) {
		auto it = callbacks.find(e.type);
		if (it == callbacks.end()) return;

		for (auto callback : it->second)
			if (callback->active) callback->Callback(e);
	}

	static void HandleInput() {
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_QUIT:
				gameRunning = false;
				return; // Exit immediately

			case SDL_WINDOWEVENT:
				if (e.window.windowID == gameWindowID) {
					switch (e.window.event)
					{
					case SDL_WINDOWEVENT_SIZE_CHANGED:
					case SDL_WINDOWEVENT_RESIZED:
						windowWidth = e.window.data1;
						windowHeight = e.window.data2;
						break;

					case SDL_WINDOWEVENT_CLOSE:
						e.type = SDL_QUIT;
						SDL_PushEvent(&e);
						break;

					}
				}
			}

			TriggerEventCallbacks(e);
		}
	}
#endif // !INPUT_HANDLED


	static void CleanupSDL() {
		if (gameWindow == nullptr) SDL_DestroyWindow(gameWindow);
		if (gameRenderer == nullptr) SDL_DestroyRenderer(gameRenderer);
		gameWindow = nullptr;
		gameRenderer = nullptr;

		if(SDL_WasInit(NULL)) SDL_Quit();
	}

	static int StartSDL() {

		if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
#ifdef ERROR_LOGGING
			MakeLog("Unable to initialise SDL: " + std::string(SDL_GetError()));
#endif // !ERROR_LOGGING
			CleanupSDL();
			return 1;
		}

		if (SDL_CreateWindowAndRenderer(windowWidth, windowHeight, windowFlags, &gameWindow, &gameRenderer) < 0) {
#ifdef ERROR_LOGGING
			MakeLog("Unable to create window: " + std::string(SDL_GetError()));
#endif // !ERROR_LOGGING
			CleanupSDL();
			return 2;
		}

		gameWindowID = SDL_GetWindowID(gameWindow);

		OnStart();

		currentTime = SDL_GetTicks();
		deltaTime = currentTime;

#ifndef LOOP_HANDLED
		while (gameRunning) {

			HandleInput();

			OnFrame();

#ifndef TIME_HANDLED
			HandleTime();
#endif // !TIME_HANDLED

		}
#endif // !LOOP_HANDLED

		OnQuit();

		CleanupSDL();

		return 0;
	}

}