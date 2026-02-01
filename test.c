#include <stdio.h>
#include <unistd.h>

int main() {
  printf("\033[?47h");
  fflush(stdout);
  sleep(5);
  printf("\033[?47l");
  fflush(stdout);
}
