#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

#define MAXLEN 128

int
main(int argc, char *argv[])
{
  char *args[MAXLEN];
  int res = 1;

  if (argc < 2){
    fprintf(2, "xargs: invalid number of arguments\n");
  }

  for (int i = 1; i < argc; ++i) {
    args[i - 1] = argv[i];
  }

  for (int i = argc - 1; i < MAXLEN; ++i) {
    args[i] = (char *) malloc(MAXLEN);
  }

  while (res > 0) {
    char ch;
    int i = argc - 1;
    int j = 0;
    while ((res = read(0, &ch, 1)) == 1) {
      if (ch == '\n') {
        args[i++][j] = '\0';
        args[i] = 0;

        if (fork() == 0) {
          // child
          exec(args[0], args);
        } else {
          // parent
          wait((int *) 0);
        }
        break;
      } else if (ch == ' ') {
        args[i++][j] = '\0';
        j = 0;
      } else {
        args[i][j++] = ch;
      }
    }
  }
  exit(0);
}
