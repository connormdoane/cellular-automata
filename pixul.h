#ifndef PIXUL_H_
#define PIXUL_H_

#include <SDL2/SDL.h>
#include <stdlib.h>

typedef struct P_Window_t {
  int w; // width
  int h; // height
  int pw; // pixel width
  int ph; // pixel height
  int ps; // pixel size
  SDL_Window* window;
  SDL_Renderer* renderer;
} P_Window;

typedef struct P_Color_t {
  int r;
  int g;
  int b;
  int a;
} P_Color;

typedef P_Color** P_Image;

P_Image P_Create(char* title, P_Window* window, int width, int height, int pixel_width, int pixel_height) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    fprintf(stderr, "[SDL_Init Error] %s\n", SDL_GetError());
    return NULL;
  }

  window->w  = width;
  window->h  = height;
  window->pw = pixel_width;
  window->ph = pixel_height;

  window->ps = width / pixel_width;
  if (height / pixel_height < window->ps) window->ps = height / pixel_height;

  window->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
  if (window->window == NULL) {
    fprintf(stderr, "[SDL_CreateWindow Error] %s\n", SDL_GetError());
    SDL_Quit();
    return NULL;
  }

  window->renderer = SDL_CreateRenderer(window->window, -1, SDL_RENDERER_SOFTWARE);
  if (window->renderer == NULL) {
    fprintf(stderr, "[SDL_CreateRenderer Error] %s\n", SDL_GetError());
    SDL_DestroyWindow(window->window);
    SDL_Quit();
    return NULL;
  }

  P_Image image = calloc(pixel_width, sizeof(P_Color*));
  for (int i = 0; i < pixel_width; i++) {
    image[i] = calloc(pixel_height, sizeof(P_Color));
  }

  return image;
}

void P_Destroy(P_Window* window, P_Image image) {
  for (int i = 0; i < window->pw; i++) {
    free(image[i]);
  }
  free(image);

  SDL_DestroyRenderer(window->renderer);
  SDL_DestroyWindow(window->window);
  SDL_Quit();
}

void P_Update(P_Window* window, P_Image image) {
  SDL_SetRenderDrawColor(window->renderer, 255, 255, 255, 255);
  SDL_RenderClear(window->renderer);

  for (int i = 0; i < window->pw; i++) {
    for (int j = 0; j < window->ph; j++) {
      SDL_Rect rect = {i * window->ps, j * window->ps, window->ps, window->ps};
      SDL_SetRenderDrawColor(window->renderer, image[i][j].r,image[i][j].g, image[i][j].b, image[i][j].a);
      SDL_RenderFillRect(window->renderer, &rect);
    }
  }

  SDL_RenderPresent(window->renderer);
}

void P_Clear(P_Window* window, P_Image image, P_Color color) {
  for (int i = 0; i < window->pw; i++) {
    for (int j = 0; j < window->ph; j++) {
      image[i][j].r = color.r;
      image[i][j].g = color.g;
      image[i][j].b = color.b;
      image[i][j].a = color.a;
    }
  }
}

int P_KeyPress(SDL_Scancode key) {
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    return state[key];
}

int P_Done() {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      return 1;
    }
  }
  return 0;
}

void P_Set(P_Window* window, P_Image image, int x, int y, P_Color color) {
  if (x < 0 || y < 0 || x >= window->pw || y >= window->ph) return;
  image[x][y].r = color.r;
  image[x][y].g = color.g;
  image[x][y].b = color.b;
  image[x][y].a = color.a;
}

#endif 
