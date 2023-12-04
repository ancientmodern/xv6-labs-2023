#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void run(int base, int left[2]) {
  int n;
  int is_first = 1;
  int right[2];

  printf("prime %d\n", base);
  close(left[1]);
  pipe(right);
  while (read(left[0], &n, sizeof(n)) > 0) {
    if (n % base != 0) {
      if (is_first) {
        is_first = 0;
        if (fork() == 0) {
          // child (right)
          run(n, right);
          exit(0);
        }
        close(right[0]);
      } else {
        write(right[1], &n, sizeof(n));
      }
    }
  }
  close(left[0]);
  close(right[1]);
  wait(0);
}

int
main(int argc, char *argv[])
{
  int right[2];

  pipe(right);
  if (fork() == 0) {
    // child (right)
    run(2, right);
  } else {
    // parent (left)
    close(right[0]);
    for(int i = 3; i <= 35; ++i) {
      write(right[1], &i, sizeof(i));
    }
    close(right[1]);
    wait(0);
  }
  exit(0);
}
