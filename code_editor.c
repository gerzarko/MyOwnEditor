#include "sys/ioctl.h"
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

struct winsize windowSize;

int main(int argc, char **argv) {

  if (argc != 2) {
    return 1;
  }

  printf("\033[?47h");
  printf("\033[0;0f");
  fflush(stdout);

  char byteBuffer[4096];

  FILE *fd = fopen(argv[1], "r");
  if (fd == NULL) {
    printf("error aca");
    return 1;
  }

  // fd es el puntero al inicio del archivo, bytebuffer es la canatidad de bytes
  // que vamos leyendo de ese archivo
  uint32_t itemsRead = fread(byteBuffer, 1, 4096, fd);
  if (itemsRead == 0) {
    return 1;
  }

  printf("%s", byteBuffer);

  fclose(fd);
  // sleep(5);
  getchar();
  printf("\033[?47l");
  fflush(stdout);
}
