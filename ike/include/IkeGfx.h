#ifndef IKE_WINDOW_H
#define IKE_WINDOW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"

#include "IkeLib.h"

#define KEY_DOWN 0
#define KEY_UP 1

typedef bool (*__render_f_type)(struct _Window* window);

typedef struct _Window{
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture** textures;
	int num_textures;
	
	__render_f_type render;
	void (*keyhandler)(SDL_Scancode scancode, uint8_t type);
} Window;

Window* openWindow(char* name, int width, int height);
void destroyWindow(Window* window);
bool ikeGfxStart();
int loadImage(char* path);
int loadTexture(Window* window, char* path);
void freeAllImages();
void startIkeLoop(Window* window);

void fillWindow(Window* window, int texture);
void fillWindowSDL(Window* window, SDL_Texture* texture);

void drawTexture(Window* window, int texture, int x, int y);

#endif
