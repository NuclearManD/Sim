
#include "IkeGfx.h"

SDL_Surface** images;
char** image_names;
int num_images_loaded = 0;

/* ====================================================
 * ikeGfxStart()
 * ====================================================
 * 
 * Starts Ike Graphics and SDL2
 * 
 * returns true for success, false for failure.  If it fails then an error message is printed to stdout.
 */
bool ikeGfxStart(){
	if (SDL_Init(SDL_INIT_VIDEO) != 0){
		printf("Error: SDL init failed = %s\n",SDL_GetError());
		return false;
	}
	images = (SDL_Surface**)malloc(0);
	image_names = (char**)malloc(0);
	
	/*if(!(IMG_Init(IMG_INIT_JPG)&IMG_INIT_JPG)){
		printf("Error: IMG_init failed = %s\n",SDL_GetError());
		return false;
	}*/
	
	return true;
}

/* ====================================================
 * openWindow(name, width, height)
 * ====================================================
 * 
 * Fairly self explanitory.  Will allocate memory as needed, returns pointer to the created Ike Window.
 * Will return NULL if something goes wrong, printing an error message to stdout.
 * 
 * name is the label the window will get.
 * 
 * The created window will be immediately visible.
 */
Window* openWindow(char* name, int width, int height){
	SDL_Window *win = SDL_CreateWindow(name, 100, 100, width, height, SDL_WINDOW_SHOWN);
	if (win == NULL){
		printf("Error: SDL create window failed = %s\n",SDL_GetError());
		return NULL;
	}
	SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == NULL){
		SDL_DestroyWindow(win);
		printf("Error: SDL create renderer failed = %s\n",SDL_GetError());
		return NULL;
	}
	Window* window = (Window*)malloc(sizeof(Window));
	window->renderer = ren;
	window->window = win;
	window->num_textures = 0;
	window->textures = (SDL_Texture**)malloc(0);
	window->keyhandler = NULL;
	window->render = NULL;
	return window;
}


/* ===================================================
 * destroyWindow(window)
 * ===================================================
 * 
 * Immediately closes the window and deallocates the memory of the Window structure.
 * 
 * Since the memory is deallocated, using a pointer after it has been passed to this function will cause a segmentation fault.
 */
void destroyWindow(Window* window){
	SDL_DestroyRenderer(window->renderer); // destroy the renderer and window
	SDL_DestroyWindow(window->window);
	
	// destroy the textures in the window to prevent memory leaks
	for(int i=0;i<window->num_textures;i++){
		SDL_DestroyTexture(window->textures[i]);
	}
	free(window->textures);
	
	// free the memory to prevent memory leaks
	free(window);
}

/*
 * Internal function, loads images into the main Ike image list.
 * 
 * Returns a number corresponding to the image ID.  If something fails then -1 is returned and an error is printed.
 */
int loadImage(char* path){
	for(int i=0;i<num_images_loaded;i++){
		if(!strcmp(image_names[i], path)) return i;
	}
	char* stro = (char*)malloc(strlen(path));
	strcpy(stro, path);
	image_names = realloc((void*)image_names, sizeof(char*)*(num_images_loaded+1));
	image_names[num_images_loaded] = stro;
	SDL_Surface *surface = IMG_Load(path);
	if(surface==NULL){
		printf("Failed to load %s.  Error: %s\n",path,IMG_GetError());
		return -1;
	}
	images = realloc((void*)images, sizeof(SDL_Surface*)*(num_images_loaded+1));
	images[num_images_loaded] = surface;
	num_images_loaded++;
	return num_images_loaded-1;
}

int loadTexture(Window* window, char* path){
	int image_id = loadImage(path);
	if(image_id==-1)return -1;
	SDL_Texture *tex = SDL_CreateTextureFromSurface(window->renderer, images[image_id]);
	
	if (tex == NULL){
		printf("Error: SDL create texture from surface failed = %s\n",SDL_GetError());
		return -1;
	}
	
	window->textures = realloc((void*)window->textures, sizeof(SDL_Texture*)*(window->num_textures+1));
	window->textures[window->num_textures] = tex;
	window->num_textures++;
	return window->num_textures-1;
}	

void freeAllImages(){
	for(int i=0;i<num_images_loaded;i++){
		SDL_FreeSurface(images[i]);
	}
	num_images_loaded = 0;
	image_names = realloc((void*)image_names, 0);
	images = realloc((void*)images, 0);
}

bool __IkeRender(Window* window){
	SDL_Event event;
	while(SDL_PollEvent(&event)){
		if (event.type == SDL_QUIT || (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE)) {
			return false;
		}else if(event.type == SDL_KEYDOWN){
			if(window->keyhandler!=NULL){
				SDL_KeyboardEvent* k = &event.key;
				window->keyhandler(k->keysym.scancode, KEY_DOWN);
			}
		}else if(event.type == SDL_KEYUP){
			if(window->keyhandler!=NULL){
				SDL_KeyboardEvent* k = &event.key;
				window->keyhandler(k->keysym.scancode, KEY_UP);
			}
		}
	}
	SDL_RenderClear(window->renderer);
	bool cont = window->render(window);
	SDL_RenderPresent(window->renderer);
	return cont;
}

void startIkeLoop(Window* window){
	while(__IkeRender(window));
}

void fillWindow(Window* window, int texture){
	SDL_RenderCopy(window->renderer, window->textures[texture], NULL, NULL);
}
void fillWindowSDL(Window* window, SDL_Texture* texture){
	SDL_Rect rect;
	rect.x = rect.y = 0;
	int w, h;
	SDL_GetWindowSize(window->window, &w, &h);
	rect.w = w;
	rect.h = h;
	SDL_RenderCopy(window->renderer, texture, NULL, &rect);
}
void drawTexture(Window* window, int texture, int x, int y){
	SDL_Texture* tex = window->textures[texture];
	SDL_Rect rect;
	rect.x = x;		// x and y are saved, now we can use them as throwaway variables
	rect.y = y;
	SDL_QueryTexture(tex, NULL, NULL, &x, &y);
	rect.w = x;
	rect.h = y;
	SDL_RenderCopy(window->renderer, tex, NULL, &rect);
}
