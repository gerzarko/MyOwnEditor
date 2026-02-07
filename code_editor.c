#include "sys/ioctl.h"
#include <asm-generic/ioctls.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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

  uint32_t i = 0;
  for (uint32_t y = 0; y < windowSize.ws_row; y++) {
    for (uint32_t x = 0; x < windowSize.ws_col; x++) {
      if (byteBuffer[i] == '\n') {
        y++;
        x = 0;
        i++;
      }
      printf("\033[%d;%df", y, x);
      fflush(stdout);
      printf("%c", byteBuffer[x + (y * windowSize.ws_col)]);

      char logline[4096];
      uint32_t written =
          sprintf(logline, "%d %d", windowSize.ws_row, windowSize.ws_col);
      fwrite(logline, 1, written, log);
    }
  }

  fclose(fd);
  // sleep(5);
  getchar();
  printf("\033[?1049l");
  fflush(stdout);
}
