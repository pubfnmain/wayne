#include <unistd.h>

void spawn(char *args[]) {
  if (fork() == 0) {
    execvp(args[0], args);
  }
}
