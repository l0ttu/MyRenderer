#include<SDL.h>
#include<Eigen/Eigen>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

void DrawPixel(SDL_Renderer* renderer, int x, int y, Eigen::Vector3f color);

int main(int argc,char* argv[])
{
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	window = SDL_CreateWindow("MyRenderer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, 0, 0);

	bool quit = false;
	SDL_Event sdlEvent;
	while (!quit) {
		while (SDL_PollEvent(&sdlEvent)) {
			if (sdlEvent.type == SDL_QUIT) {
				quit = true;
			}
			if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) {
				quit = true;
			}
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);

		SDL_RenderPresent(renderer);
	}
	return 0;
}

void DrawPixel(SDL_Renderer* renderer, int x, int y, Eigen::Vector3f color) {
	SDL_SetRenderDrawColor(renderer, (Uint8)(color.x() * 255), (Uint8)(color.y() * 255), (Uint8)(color.z() * 255), SDL_ALPHA_OPAQUE);
	SDL_Point point;
	point.x = x;
	point.y = y;
	SDL_RenderDrawPoints(renderer, &point, 1);
}