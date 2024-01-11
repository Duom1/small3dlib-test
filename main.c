#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include <SDL2/SDL.h>

#define S3L_RESOLUTION_X 1700
#define S3L_RESOLUTION_Y 870
#define RES_CO S3L_RESOLUTION_X *S3L_RESOLUTION_Y
#define S3L_PIXEL_FUNCTION drawPixel
#include "./small3dlib.h"

#define NOT_INIT(c, t)                                                         \
  if (c == NULL) {                                                             \
    fprintf(stderr, "could not initialize " t "!!!");                          \
    return EXIT_FAILURE;                                                       \
  }

S3L_Unit cubeVertices[] = {S3L_CUBE_VERTICES(S3L_F)};
S3L_Index cubeTriangles[] = {S3L_CUBE_TRIANGLES};

S3L_Model3D cubeModel;
S3L_Scene scene;

uint32_t pixels[RES_CO];

void drawPixel(S3L_PixelInfo *p) {
  uint32_t color;

  if (p->triangleIndex == 0 || p->triangleIndex == 1 || p->triangleIndex == 4 ||
      p->triangleIndex == 5) {
    color = 0xffff0000;
  } else if (p->triangleIndex == 2 || p->triangleIndex == 3 ||
             p->triangleIndex == 6 || p->triangleIndex == 7) {
    color = 0xff00ff00;
  } else {
    color = 0xff0000ff;
  }

  pixels[p->x + p->y * (S3L_RESOLUTION_X)] = color;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("usage: %s <speed>\n", argv[0]);
    return EXIT_FAILURE;
  }
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    fprintf(stderr, "could not initialize sdl !!!");
    return EXIT_FAILURE;
  }
  SDL_Window *w = SDL_CreateWindow("hello wolrd", SDL_WINDOWPOS_UNDEFINED,
                                   SDL_WINDOWPOS_UNDEFINED, S3L_RESOLUTION_X,
                                   S3L_RESOLUTION_Y, SDL_WINDOW_SHOWN);
  NOT_INIT(w, "window")
  SDL_Renderer *r = SDL_CreateRenderer(w, -1, SDL_RENDERER_ACCELERATED);
  NOT_INIT(r, "renderer")
  SDL_Texture *t = SDL_CreateTexture(r, SDL_PIXELFORMAT_ARGB8888,
                                     SDL_TEXTUREACCESS_STREAMING,
                                     S3L_RESOLUTION_X, S3L_RESOLUTION_Y);
  NOT_INIT(t, "texture")
  S3L_model3DInit(cubeVertices, S3L_CUBE_VERTEX_COUNT, cubeTriangles,
                  S3L_CUBE_TRIANGLE_COUNT, &cubeModel);
  S3L_sceneInit(&cubeModel, 1, &scene);
  scene.camera.transform.translation.z = -2 * S3L_F;
  bool quit = false;
  SDL_Event e;
  uint64_t loop = 0;
  while (!quit) {
    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_QUIT) {
        quit = true;
      }
    }
    for (int i = 0; i < RES_CO; ++i) {
      pixels[i] = 0xffaaaaaa;
    }
    S3L_newFrame();
    S3L_drawScene(scene);
    SDL_UpdateTexture(t, NULL, pixels, S3L_RESOLUTION_X * sizeof(uint32_t));
    SDL_RenderClear(r);
    SDL_RenderCopy(r, t, NULL, NULL);
    SDL_RenderPresent(r);
    if (loop % (uint64_t)atoi(argv[1]) == 0) {
      scene.models[0].transform.rotation.y += 1;
      scene.models[0].transform.rotation.x += 2;
    }
    ++loop;
  }
  SDL_DestroyRenderer(r);
  SDL_DestroyTexture(t);
  SDL_DestroyWindow(w);
  SDL_Quit();
  return EXIT_SUCCESS;
}
