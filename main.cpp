#include<SDL.h>
#include<Eigen/Eigen>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

void DrawPixel(SDL_Renderer* renderer, int x, int y, Eigen::Vector3f color);
void DrawLine(SDL_Renderer* renderer, int x0, int y0, int x1, int y1, Eigen::Vector3f color);
void DrawTriangleLineSweep(SDL_Renderer* renderer, int x0, int y0, int x1, int y1, int x2, int y2, Eigen::Vector3f color);

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

		//Render
		DrawLine(renderer, 400, 300, 700, 500, Eigen::Vector3f{ 1,0,0 });

		SDL_RenderPresent(renderer);
	}
	return 0;
}

//绘制点
void DrawPixel(SDL_Renderer* renderer, int x, int y, Eigen::Vector3f color) {
	SDL_SetRenderDrawColor(renderer, (Uint8)(color.x() * 255), (Uint8)(color.y() * 255), (Uint8)(color.z() * 255), SDL_ALPHA_OPAQUE);
	SDL_Point point;
	point.x = x;
	point.y = y;
	SDL_RenderDrawPoints(renderer, &point, 1);
}

//Bresenham算法
void DrawLine(SDL_Renderer* renderer, int x0, int y0, int x1, int y1, Eigen::Vector3f color) {
	bool steep = false;

	if (abs(x1 - x0) < abs(y1 - y0)) {
		steep = true;
	}

	if ((x0 > x1 && !steep) || (y0 > y1 && steep)) {
		std::swap(x0, x1);
		std::swap(y0, y1);
	}


	int dx = x1 - x0;
	int dy = y1 - y0;

	int x = x0, y = y0, d = 1, sub = 0;

	if (!steep) {
		if (dy < 0) {
			d = -1;
			dy = -dy;
		}

		sub = 2 * dy - dx;

		while (x < x1) {
			x++;
			if (sub > 0) {
				sub += 2 * dy - 2 * dx;
				y += d;
			}
			else {
				sub += 2 * dy;
			}
			DrawPixel(renderer, x, y, color);
		}
	}
	else {
		if (dx < 0) {
			d = -1;
			dx = -dx;
		}

		sub = 2 * dx - dy;

		while (y < y1) {
			if (sub > 0) {
				sub += 2 * dx - 2 * dy;
				x += d;
			}
			else {
				sub += 2 * dx;
			}
			DrawPixel(renderer, x, y, color);
		}
	}
}

//扫描线算法
void DrawTriangleLineSweep(SDL_Renderer* renderer, int x0, int y0, int x1, int y1, int x2, int y2, Eigen::Vector3f color) {
	if (y0 == y1 && y1 == y2) return;

	if (y0 > y1) {
		std::swap(y0, y1);
		std::swap(x0, x1);
	}
	if (y0 > y2) {
		std::swap(y0, y2);
		std::swap(x0, x2);
	}
	if (y1 > y2) {
		std::swap(y1, y2);
		std::swap(x1, x2);
	}


	int height = y2 - y0;

	for (int i = 0; i < height; i++) {
		bool split = i > y1 - y0 || y1 == y0;
		int segmentHeight = split ? y2 - y1 : y1 - y0;

		float alpha = (float)i / height;
		float beta = (float)(i - (split ? y1 - y0 : 0)) / segmentHeight;

		int p0 = x0 + (x2 - x0) * alpha;	
		int p1 = split ? x1 + (x2 - x1) * beta : x0 + (x1 - x0) * beta;

		if (p1 < p0) std::swap(p1, p0);

		for (int p = p0; p <= p1; p++) {
			DrawPixel(renderer, p, y0+i, color);
		}
	}
}
