#include "bass.hpp"
#include "core/core.cpp"
#include "arch/table/table.cpp"
#include "arch/snes-smp-canonical/snes-smp-canonical.cpp"

#if defined(BASS_BINARY)

int main(int argc, char** argv) {
  string outputFilename;
  lstring inputFilename;

  Bass* arch = new BassTable;
  bool benchmark = false;

  for(unsigned n = 1; n < argc;) {
    if(0) {
    } else if(string{argv[n]} == "-arch=table") {
      delete arch;
      arch = new BassTable;
      n++;
    } else if(string{argv[n]} == "-arch=snes-smp-canonical") {
      delete arch;
      arch = new BassSnesSmpCanonical;
      n++;
    } else if(string{argv[n]}.beginswith("-D")) {
      string argument = argv[n];
      argument.ltrim<1>("-D");
      lstring part = argument.split<1>("=");
      if(!part[0].position("::")) part[0] = {"global::", part[0]};
      arch->defaultMacros.append({part[0], {}, {"return ", part(1, "")}});  //no argument sets value to blank
      n++;
    } else if(string{argv[n]} == "-case-insensitive") {
      arch->options.caseInsensitive = true;
      n++;
    } else if(string{argv[n]} == "-overwrite") {
      arch->options.overwrite = true;
      n++;
    } else if(string{argv[n]} == "-benchmark") {
      benchmark = true;
      n++;
    } else if(string{argv[n]} == "-o" && n + 1 < argc) {
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

  if(inputFilename.size() < 1) {
    print("bass v11.06\n");
    print("author: byuu\n");
    print("license: GPLv3\n");
    print("usage: bass [-arch=name] [-Dname(=value) ...] [options] [-o output] input [input ...]\n");
    print("\n");
    print("supported archs:\n");
    print("  table (default)\n");
    print("  snes-smp-canonical\n");
    print("\n");
    print("supported options:\n");
    print("  -case-insensitive\n");
    print("  -overwrite\n");
    print("  -benchmark\n");
    print("\n");
    return EXIT_FAILURE;
  }

  if(arch->open(outputFilename) == false) {
    print("error: unable to open output file: ", outputFilename, "\n");
    return EXIT_FAILURE;
  }

  bool success = true;
  clock_t startTime = clock();
  for(auto& filename : inputFilename) success &= arch->assemble(filename);
  clock_t endTime = clock();
  if(benchmark) print("Assembled in ", (endTime - startTime) / (double)CLOCKS_PER_SEC, " seconds.\n");

  arch->close();
  delete arch;

  return success ? EXIT_SUCCESS : EXIT_FAILURE;
}

#endif
