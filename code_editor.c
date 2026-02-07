#include "sys/ioctl.h"
#include <asm-generic/ioctls.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {

  if (argc != 2) {
    return 1;
  }

  // struct que tiene el tamano de la pantalla en pixeles
  struct winsize windowSize;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &windowSize) != 0) {
    return 1;
  }

  printf("\033[?47h");
  printf("\033[0;0f");
  fflush(stdout);

  uint32_t sizeOfbuffer = sizeof(windowSize.ws_row * windowSize.ws_col);
  char *byteBuffer = malloc(sizeOfbuffer);

  FILE *fd = fopen(argv[1], "r");
  if (fd == NULL) {
    printf("error aca");
    return 1;
  }

  // fd es el puntero al inicio del archivo, bytebuffer es la canatidad de bytes
  // que vamos leyendo de ese archivo
  uint32_t itemsRead = fread(byteBuffer, 1, sizeOfbuffer, fd);
  if (itemsRead == 0) {
    return 1;
  }

  for (uint32_t y = 0; y < windowSize.ws_row; y++) {
    for (uint32_t x = 0; x < windowSize.ws_col; x++) {
      printf("\033[%d;%df", y, x);
      fflush(stdout);
      printf("%c", byteBuffer[x + (y * windowSize.ws_col)]);
    }
  }

  printf("%s", byteBuffer);

  fclose(fd);
  // sleep(5);
  getchar();
  printf("\033[?1049l");
  fflush(stdout);
}
