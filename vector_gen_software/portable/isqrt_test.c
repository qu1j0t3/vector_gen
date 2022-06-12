#include <stdio.h>
#include <stdlib.h>

#include "isqrt.h"

int main(int argc, char *argv[]) {
  if (argc == 2) {
    uint32_t x = atoi(argv[1]);
    printf("%u\n", isqrt(x));
  } else {
    return EXIT_FAILURE;
  }

}