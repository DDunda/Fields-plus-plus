#include <SDL.h>
#include <vector>
#include <time.h>
#include <thread>
#include <string>

#define ERROR_LOGGING
#include "SDLG.h"

#include "Vec2.h"

using namespace SDLG;
using std::vector;
using std::thread;
using std::string;

int main(int argc, char* argv[]) { return StartSDL(); }

SDL_Texture* canvas;

#define CANVAS_WIDTH 1920
#define CANVAS_HEIGHT 1080

#define CAM_WIDTH (3.0*1.92)
#define CAM_HEIGHT (3.0*1.08)

#define CAM_X 0.0
#define CAM_Y 0.0

#define X_OFF CAM_X - 0.5f * CAM_WIDTH
#define Y_OFF CAM_Y + 0.5f * CAM_HEIGHT

#define HUE 0
#define XY 1
#define COLOUR_MODE XY

#define N 30
#define M 0.015
#define V 0.4
#include "config.h"

class Planet {
public:
	float mass;
	float px, py;
	float vx, vy;

	Planet(float m, float x, float y , float _vx, float _vy) : mass(m), px(x), py(y), vx(_vx), vy(_vy) {}
};

vector<Planet> planets{
};

int RandBetween(int a, unsigned r) {
	return a + (rand() % r);
}

SDL_Rect dst{ 0, 0, CANVAS_WIDTH, CANVAS_HEIGHT };

Planet* first;
Planet* last;

float dT;

float halfWidthInv = CAM_WIDTH / CANVAS_WIDTH;
float halfHeightInv = -CAM_HEIGHT / CANVAS_HEIGHT;

int threadCount;

// Makes pretty colour
void ThreadMinion(char* _pixels, int threadIndex) {
	char* pixels = _pixels + threadIndex * CANVAS_WIDTH * 3;
	for (int y = threadIndex; y < CANVAS_HEIGHT; y += threadCount) {
		for (int x = 0; x < CANVAS_WIDTH; ++x) {
			float px = x * halfWidthInv + X_OFF;
			float py = y * halfHeightInv + Y_OFF;

			float fx = 0.0f, fy = 0.0f;

			for (Planet* a = first; a < last; ++a) {
				float dx = a->px - px;
				float dy = a->py - py;
				float d2 = dx * dx + dy * dy; // Distance squared

				if (d2) { // Checks non zero
					float f = a->mass / (d2 * sqrt(d2));

					fx += dx * f; // (dx / d) * m / d^2
					fy += dy * f; // (dy / d) * m / d^2
				}
			}

#if COLOUR_MODE == XY
			float fm = sqrt(fx * fx + fy * fy);
			if (fm) {
				*(pixels++) = fx * 127.5f / fm + 127.5f;
				*(pixels++) = -fy * 127.5f / fm + 127.5f;
				*(pixels++) = 100;
			}
			else {
				*(pixels++) = 0;
				*(pixels++) = 0;
				*(pixels++) = 0;
			}
#endif

#if COLOUR_MODE == HUE
			float fsector = atan2(fy, fx) * 3.0 / M_PI + 3.0;

			//   \  1  /
			//  2 \   / 0,6
			// ----> <----
			//  3 /   \ 5
			//   /  4  \
				
			// Which sector the angle falls in, in range [0,6]
			int sector = fsector;

			// The progress through the current sector, [0,255]
			char value = (fsector - sector) * 255;

			switch (sector) {
			case 0:
			default:
				*(pixels++) = 0;
				*(pixels++) = 255;
				*(pixels++) = 255 - value;
				break;
			case 1:
				*(pixels++) = value;
				*(pixels++) = 255;
				*(pixels++) = 0;
				break;
			case 2:
				*(pixels++) = 255;
				*(pixels++) = 255 - value;
				*(pixels++) = 0;
				break;
			case 3:
				*(pixels++) = 255;
				*(pixels++) = 0;
				*(pixels++) = value;
				break;
			case 4:
				*(pixels++) = 255 - value;
				*(pixels++) = 0;
				*(pixels++) = 255;
				break;
			case 5:
				*(pixels++) = 0;
				*(pixels++) = value;
				*(pixels++) = 255;;
				break;
			}
#endif
		}
		pixels += CANVAS_WIDTH * (threadCount - 1) * 3;
	}
}

void RenderGravityWell() {
	char* pixels;
	int pitch;
	SDL_LockTexture(canvas, NULL, (void**)&pixels, &pitch);

	thread* RenderingThreads = new thread[threadCount];
	for (int i = 0; i < threadCount; i++)	RenderingThreads[i] = thread(ThreadMinion, pixels, i);
	for (int i = 0; i < threadCount; i++)	RenderingThreads[i].join();
	delete[] RenderingThreads;

	SDL_UnlockTexture(canvas);
}

// Makes pretty move
void DoPlanetPhysics() {
	for (Planet* a = first; a < last; a++) {
		for (Planet* b = a + 1; b < last; b++) {
			float dx = b->px - a->px;
			float dy = b->py - a->py;
			float d2 = dx * dx + dy * dy; // Distance squared

			if (d2) { // Checks non zero
				float dm = dT / (d2 * sqrt(d2));

				float fx = dx * dm;
				float fy = dy * dm;

				a->vx += fx * b->mass;
				a->vy += fy * b->mass;

				b->vx -= fx * a->mass;
				b->vy -= fy * a->mass;
			}
		}

		a->px += a->vx * dT;
		a->py += a->vy * dT;
	}
}

void SDLG::OnStart() {
	minFrameDelta = 1000 / 60;

	threadCount = thread::hardware_concurrency();
	string title = "Field generator++ (" + std::to_string(threadCount) + " threads)";
	SDL_SetWindowTitle(gameWindow, title.c_str());

	srand(time(0));

	for (int i = 0; i < N; i++) {
		//float m = RandBetween(1, 99) / 100.0f;
		//float x = RandBetween(-100, 199) / 100.0f * 1.92f;
		//float y = RandBetween(-100, 199) / 100.0f * 1.08f;

		float x = cos(i * 2.0 * M_PI / (float)N);
		float y = sin(i * 2.0 * M_PI / (float)N);
		float vx = -sin(i * 2.0 * M_PI / (float)N) * V;
		float vy = cos(i * 2.0 * M_PI / (float)N) * V;

		planets.push_back(Planet(M, x, y, vx, vy));
	}

	canvas = SDL_CreateTexture(gameRenderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, CANVAS_WIDTH, CANVAS_HEIGHT);
}

void SDLG::OnFrame() {
	first = &planets[0];
	last = (&planets[planets.size() - 1]) + 1;

	dT = deltaTime / 1000.0;

	DoPlanetPhysics();

	SDL_SetRenderDrawColor(gameRenderer, 0, 0, 0, 255);
	SDL_RenderClear(gameRenderer);

	RenderGravityWell();
	SDL_RenderCopy(gameRenderer,canvas,NULL,&dst);

	SDL_RenderPresent(gameRenderer);
}

void SDLG::OnQuit() {
	SDL_DestroyTexture(canvas);
}