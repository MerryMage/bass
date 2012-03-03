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

  Bass *arch = new BassTable;
  bool benchmark = false;

  #if 0
  char argv1[] = "bass";
  char argv2[] = "-o";
  char argv3[] = "test/test.bin";
  char argv4[] = "test/test.asm";
  char *argvN[] = { argv1, argv2, argv3, argv4, nullptr };
  argc = 4;
  argv = (char**)argvN;
  #endif

  for(unsigned n = 1; n < argc;) {
    if(0) {
    } else if(cstring{argv[n]} == "-arch=table") {
      delete arch;
      arch = new BassTable;
      n++;
    } else if(cstring{argv[n]} == "-arch=snes-smp-canonical") {
      delete arch;
      arch = new BassSnesSmpCanonical;
      n++;
    } else if(cstring{argv[n]} == "-case-insensitive") {
      arch->options.caseInsensitive = true;
      n++;
    } else if(cstring{argv[n]} == "-overwrite") {
      arch->options.overwrite = true;
      n++;
    } else if(cstring{argv[n]} == "-benchmark") {
      benchmark = true;
      n++;
    } else if(cstring{argv[n]} == "-o" && n + 1 < argc) {
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

  if(outputFilename == "" || inputFilename.size() < 1) {
    print("bass v05.04\n");
    print("author: byuu\n");
    print("usage: bass [-arch=(arch)] [options] -o output input [input ...]\n\n");
    print("supported archs:\n");
    print("  table (default)\n");
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
