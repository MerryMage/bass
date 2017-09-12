//bass
//license: GPLv3
//author: byuu
//project started: 2013-09-27

#include "bass.hpp"
#include "core/core.cpp"
#include "arch/table/table.cpp"

int main(int argc, char** argv) {
  if(argc == 1) {
    print("bass v12.05\n");
    print("usage: bass [options] [-o target] source [source ...]\n");
    print("\n");
    print("options:\n");
    print("  -d name          create define\n");
    print("  -d name=value    create define with value\n");
    print("  -c name          create constant\n");
    print("  -c name=value    create constant with value\n");
    print("  -strict          upgrade warnings to errors\n");
    print("  -create          overwrite target file if it already exists\n");
    print("  -benchmark       benchmark performance\n");
    return 0;
  }

  string targetFilename;
  lstring constants;
  lstring defines;
  bool create = false;
  bool strict = false;
  bool benchmark = false;
  lstring sourceFilenames;

  for(unsigned n = 1; n < argc;) {
    string s = argv[n];

    if(s == "-o") {
      targetFilename = argv[n + 1];
      n += 2;
      continue;
    }

    if(s == "-c") {
      constants.append(argv[n + 1]);
      n += 2;
      continue;
    }

    if(s == "-d") {
      defines.append(argv[n + 1]);
      n += 2;
      continue;
    }

    if(s == "-create") {
      create = true;
      n += 1;
      continue;
    }

    if(s == "-strict") {
      strict = true;
      n += 1;
      continue;
    }

    if(s == "-benchmark") {
      benchmark = true;
      n += 1;
      continue;
    }

    if(!s.beginswith("-")) {
      sourceFilenames.append(s);
      n += 1;
      continue;
    }

    print("error: unrecognized argument: ", s, "\n");
    return -1;
  }

  clock_t clockStart = clock();
  BassTable bass;
  bass.target(targetFilename, create);
  for(auto& sourceFilename : sourceFilenames) {
    bass.source(sourceFilename);
  }
  for(auto& constant : constants) {
    lstring p = constant.split<1>("=");
    bass.constant(p(0), p(1, "1"));
  }
  for(auto& define : defines) {
    lstring p = define.split<1>("=");
    bass.define(p(0), p(1));
  }
  if(bass.assemble(strict) == false) {
    print("bass: assembly failed\n");
    return -1;
  }
  clock_t clockFinish = clock();
  if(benchmark) {
    print("bass: assembled in ", (double)(clockFinish - clockStart) / CLOCKS_PER_SEC, " seconds\n");
  }
  return 0;
}
