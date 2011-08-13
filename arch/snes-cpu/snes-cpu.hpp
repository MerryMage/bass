struct BassSnesCpu : public Bass {
  void seek(unsigned offset);
  bool assembleBlock(const string &block);
  BassSnesCpu();

protected:
  int64_t eval(const string &s);

  enum Mapper : unsigned { None, LoROM, HiROM } mapper;

  enum Mode : unsigned {
    Implied,
    ImpliedRepeat,
    Immediate,
    ImmediateM,
    ImmediateX,
    Direct,
    DirectX,
    DirectY,
    DirectS,
    Indirect,
    IndirectX,
    IndirectY,
    IndirectSY,
    IndirectLong,
    IndirectLongY,
    Absolute,
    AbsoluteX,
    AbsoluteY,
    IndirectAbsolute,
    IndirectAbsoluteX,
    IndirectLongAbsolute,
    Long,
    LongX,
    Relative,
    RelativeLong,
    BlockMove,
  };

  struct Opcode {
    uint8_t prefix;
    string mnemonic;
    bool priority;
    Mode mode;
    string name;
    string pattern;
  };

  struct Family {
    string pattern;
    linear_vector<Opcode> opcode;
  };

  linear_vector<Family> family;

  //assembleBlock()
  string block, name, param;
  signed relative, repeat;
  bool priority;
};
