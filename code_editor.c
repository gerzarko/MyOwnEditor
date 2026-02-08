#include "sys/ioctl.h"
#include <asm-generic/ioctls.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct State {

  FILE *fd;
  char *byteBuffer;
  uint32_t bufferSize;
  uint32_t colAmount;
  uint32_t rowAmount;

} State;

int main(int argc, char **argv) {

  // if (argc != 2) {
  //   return 1;
  // }

  // struct que tiene el tamano de la pantalla en pixeles
  struct winsize windowSize;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &windowSize) != 0) {
    return 1;
  }

  printf("\033[?1049h");
  printf("\033[0;0f");
  // printf("\033[H");
  fflush(stdout);

  uint32_t sizeOfbuffer = windowSize.ws_row * windowSize.ws_col;
  char *byteBuffer = malloc(sizeOfbuffer);

  FILE *fd = fopen(argv[1], "r");
  if (fd == NULL) {
    return 1;
  }

  FILE *log = fopen(argv[2], "w");
  if (log == NULL) {
    return 1;
  }
  // fd es el puntero al inicio del archivo, bytebuffer es la canatidad de bytes
  // que vamos leyendo de ese archivo
  uint32_t itemsRead = fread(byteBuffer, 1, sizeOfbuffer, fd);
  if (itemsRead == 0) {
    return 1;
  }

  fclose(fd);

  getchar();
  printf("\033[?1049l");
  fflush(stdout);
}

void PrintToScreen(State state, FILE *log) {

  uint32_t i = 0;
  for (uint32_t y = 1; y < state.rowAmount; y++) {
    for (uint32_t x = 1; x < state.colAmount; x++) {
      if (state.byteBuffer[i] == '\n') {
        i++;
        break;
      }
      printf("\033[%d;%df", y, x);
      fflush(stdout);
      printf("%c", state.byteBuffer[i]);

      char logline[4096];
      uint32_t written = sprintf(logline, "%c", state.byteBuffer[i]);
      fwrite(logline, written, 1, log);
      i++;
    }
  }
}
