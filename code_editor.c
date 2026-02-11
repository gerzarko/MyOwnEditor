#include "sys/ioctl.h"
#include <asm-generic/ioctls.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct State {

  FILE *fd;
  char *byteBuffer;
  uint32_t bufferSize;
  uint32_t colAmount;
  uint32_t rowAmount;
  uint32_t pointer;

} State;

void DrawToScreen(State *state);

int main(int argc, char **argv) {

  // if (argc != 2) {
  //   return 1;
  // }
  //

  // struct que tiene el tamano de la pantalla en pixeles
  struct winsize windowSize;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &windowSize) != 0) {
    return 1;
  }

  printf("\033[?1049h");
  // printf("\033[H");
  printf("\033[0;0f");
  // printf("\033[H");
  fflush(stdout);

  // Declare State struct
  State state = {};
  state.bufferSize = windowSize.ws_col * windowSize.ws_row;
  state.byteBuffer = malloc(state.bufferSize);
  state.colAmount = windowSize.ws_col;
  state.rowAmount = windowSize.ws_row;
  state.pointer = 0;
  state.fd = fopen(argv[1], "r");
  FILE *log = fopen(argv[2], "w");
  if (state.fd == NULL) {
    return 10;
  }

  int input = 0;
  int itemsRead = 0;

  do {
    if (itemsRead <= state.pointer) {
      uint32_t itemsRead =
          fread(state.byteBuffer, 1, state.bufferSize, state.fd);
      state.pointer = 0;
    }
    char logLine[4096];
    sprintf(logLine, "el germancito estuvo aqui\n");
    fwrite(logLine, strlen(logLine), 1, log);
    if (itemsRead == 0) {
      // char logLine[4096];
      // sprintf(logLine, "el germancito estuvo aqui\n");
      // fwrite(logLine, strlen(logLine), 1, log);
      // printf("aca");
      // state.byteBuffer = state.byteBuffer + 200;
    }
    DrawToScreen(&state);
    input = getchar();
    // char logLine[4096];
    sprintf(logLine, "%d\n", input);
    fwrite(logLine, strlen(logLine), 1, log);
  } while (input != 133);

  // fd es el puntero al inicio del archivo, bytebuffer es la canatidad de
  // bytes que vamos leyendo de ese archivo
  fclose(state.fd);
  fclose(log);

  printf("\033[?1049l");
  fflush(stdout);
}

void DrawToScreen(State *state) {

  for (uint32_t y = 0; y < state->rowAmount; y++) {
    for (uint32_t x = 0; x < state->colAmount; x++) {
      if (state->bufferSize <= state->pointer) {
        return;
      }
      if (state->byteBuffer[state->pointer] == '\n') {
        state->pointer++;
        break;
      }
      printf("\033[%d;%df", y + 1, x + 1);
      fflush(stdout);
      printf("%c", state->byteBuffer[state->pointer]);
      state->pointer++;
    }
  }
}
