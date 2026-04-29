#include <iostream>
#include <libgen.h>
#include <string>
#include <unistd.h>

int main(int argc, char *argv[]) {
  // Get the path to this executable
  char path[1024];
  ssize_t count = readlink("/proc/self/exe", path, sizeof(path));
  if (count == -1)
    return 1;
  path[count] = '\0';

  // Get the directory name
  char *dir = dirname(path);

  // Change working directory to the executable's directory
  if (chdir(dir) != 0)
    return 1;

  // Execute the main game binary
  std::string gamePath = "./OmnitrixUniverse";
  char *const args[] = {(char *)gamePath.c_str(), nullptr};

  execv(gamePath.c_str(), args);

  // If execv returns, it failed
  perror("execv failed");
  return 1;
}
