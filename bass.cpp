//bass
//author: byuu
//license: GPLv3

#include "bass.hpp"
#include "core/core.cpp"
#include "arch/table/table.cpp"
#include "arch/snes-smp-canonical/snes-smp-canonical.cpp"

#if defined(BASS_BINARY)

int main(int argc, char **argv) {
  string outputFilename;
  lstring inputFilename;

  Bass *arch = 0;

  #if 0
  char argv1[] = "bass";
  char argv2[] = "-arch=table";
  char argv3[] = "-o";
  char argv4[] = "test/test.bin";
  char argv5[] = "test/test.asm";
  char *argvN[] = {
    argv1, argv2, argv3, argv4, argv5, nullptr
  };
  argc = 5;
  argv = (char**)argvN;
  #endif

  bool benchmark = false;

  for(unsigned n = 1; n < argc;) {
    if(0) {
    } else if(!arch && cstring{argv[n]} == "-arch=table") {
      arch = new BassTable;
      n++;
    } else if(!arch && !strcmp(argv[n], "-arch=snes-smp-canonical")) {
      arch = new BassSnesSmpCanonical;
      n++;
    } else if(!strcmp(argv[n], "-case-insensitive")) {
      if(arch) arch->options.caseInsensitive = true;
      n++;
    } else if(!strcmp(argv[n], "-overwrite")) {
      if(arch) arch->options.overwrite = true;
      n++;
    } else if(!strcmp(argv[n], "-benchmark")) {
      benchmark = true;
      n++;
    } else if(!strcmp(argv[n], "-o") && n + 1 < argc) {
      outputFilename = argv[n + 1];
      n += 2;
    } else if(argv[n][0] != '-') {
      inputFilename.append(argv[n]);
      n++;
    } else {
      print("unknown argument: ", argv[n], "\n");
      n++;
    }
  }

  if(!arch || outputFilename == "" || inputFilename.size() < 1) {
    print("bass v05.02\n");
    print("author: byuu\n");
    print("usage: bass -arch=(arch) [options] -o output input [input ...]\n\n");
    print("supported archs:\n");
    print("  table\n");
    print("  snes-smp-canonical\n");
    print("\n");
    print("supported options:\n");
    print("  -case-insensitive\n");
    print("  -overwrite\n");
    print("  -benchmark\n");
    print("\n");
    return 0;
  }

  clock_t startTime = clock();

  arch->open(outputFilename);
  for(auto &filename : inputFilename) arch->assemble(filename);
  arch->close();
  delete arch;

  clock_t endTime = clock();
  if(benchmark) print("Assembled in ", (endTime - startTime) / (double)CLOCKS_PER_SEC, " seconds.\n");

  return 0;
}

#endif
