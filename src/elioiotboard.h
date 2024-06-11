#ifndef ELIOIOTBOARD_H
#define ELIOIOTBOARD_H
class IOExpander
{
public:
  int INT;
  int A1;
  int A2;
  int IO0_0;
  int IO0_1;
  int IO0_2; // AN Selection
  int IO0_3;
  int IO0_4;
  int IO0_5;
  int IO0_6;
  int IO0_7;
  int VSS;
  int IO1_0;
  int IO1_1;
  int IO1_2;
  int IO1_3;
  int IO1_4;
  int IO1_5;
  int IO1_6;
  int IO1_7;
  int A0;
  int SCL;
  int SDA;
  int VDD;

  IOExpander(const int *values)
  {
    INT = values[0];
    A1 = values[1];
    A2 = values[2];
    IO0_0 = values[3];
    IO0_1 = values[4];
    IO0_2 = values[5];
    IO0_3 = values[6];
    IO0_4 = values[7];
    IO0_5 = values[8];
    IO0_6 = values[9];
    IO0_7 = values[10];
    VSS = values[11];
    IO1_0 = values[12];
    IO1_1 = values[13];
    IO1_2 = values[14];
    IO1_3 = values[15];
    IO1_4 = values[16];
    IO1_5 = values[17];
    IO1_6 = values[18];
    IO1_7 = values[19];
    A0 = values[20];
    SCL = values[21];
    SDA = values[22];
    VDD = values[23];
  }
};

class Microcontroller
{
public:
  int V3V3;
  int EN;
  int IO4;
  int IO5;
  int IO6;
  int IO7;
  int IO15;
  int IO16;
  int IO17;
  int IO18;
  int IO8;
  int IO19;
  int IO20;
  int IO3;
  int IO46;
  int IO9;
  int IO10;
  int IO11;
  int IO12;
  int IO13;
  int IO14;
  int IO21;
  int IO47;
  int IO48;
  int IO45;
  int IO0;
  int IO35;
  int IO36;
  int IO37;
  int IO38;
  int IO39;
  int IO40;
  int IO41;
  int IO42;
  int RXD0;
  int TXD0;
  int IO2;
  int IO1;
  int EPAD;
  Microcontroller(const int *values)
  {
    V3V3 = values[0];
    EN = values[1];
    IO4 = values[2];
    IO5 = values[3];
    IO6 = values[4];
    IO7 = values[5];
    IO15 = values[6];
    IO16 = values[7];
    IO17 = values[8];
    IO18 = values[9];
    IO8 = values[10];
    IO19 = values[11];
    IO20 = values[12];
    IO3 = values[13];
    IO46 = values[14];
    IO9 = values[15];
    IO10 = values[16];
    IO11 = values[17];
    IO12 = values[18];
    IO13 = values[19];
    IO14 = values[20];
    IO21 = values[21];
    IO47 = values[22];
    IO48 = values[23];
    IO45 = values[24];
    IO0 = values[25];
    IO35 = values[26];
    IO36 = values[27];
    IO37 = values[28];
    IO38 = values[29];
    IO39 = values[30];
    IO40 = values[31];
    IO41 = values[32];
    IO42 = values[33];
    RXD0 = values[34];
    TXD0 = values[35];
    IO2 = values[36];
    IO1 = values[37];
    EPAD = values[38];
  }
};

class Demo
{

private:
  const int pincMicrocontrollere[39] = {
      2,  // V3V3
      3,  // EN
      4,  // IO4
      5,  // IO5
      6,  // IO6
      7,  // IO7
      8,  // IO15
      9,  // IO16
      10, // IO17
      11, // IO18
      12, // IO8
      13, // IO19
      14, // IO20
      15, // IO3
      16, // IO46
      17, // IO9
      18, // IO10
      19, // IO11
      20, // IO12
      21, // IO13
      22, // IO14
      23, // IO21
      24, // IO47
      25, // IO48
      26, // IO45
      27, // IO0
      28, // IO35
      29, // IO36
      30, // IO37
      31, // IO38
      32, // IO39
      33, // IO40
      34, // IO41
      35, // IO42
      36, // RXD0
      37, // TXD0
      38, // IO2
      39, // IO1
      41  // EPAD
  };
  const int pincIOExpander[39] = {
      1,  // INT
      2,  // A1
      3,  // A2
      4,  // IO0_0
      5,  // IO0_1
      6,  // IO0_2
      7,  // IO0_3
      8,  // IO0_4
      9,  // IO0_5
      10, // IO0_6
      11, // IO0_7
      12, // VSS
      13, // IO1_0
      14, // IO1_1
      15, // IO1_2
      16, // IO1_3
      17, // IO1_4
      18, // IO1_5
      19, // IO1_6
      20, // IO1_7
      21, // A0
      22, // SCL
      23, // SDA
      24  // VDD
  };

public:
  Microcontroller microcontroller = Microcontroller(pincMicrocontrollere);
  IOExpander ioExpander = IOExpander(pincIOExpander);
  Demo()
  {
  }
};

class Mini
{
private:
  const int pincMicrocontrollere[39] = {
      2,  // V3V3
      3,  // EN
      4,  // IO4
      5,  // IO5
      6,  // IO6
      7,  // IO7
      8,  // IO15
      9,  // IO16
      10, // IO17
      11, // IO18
      12, // IO8
      13, // IO19
      14, // IO20
      15, // IO3
      16, // IO46
      17, // IO9
      18, // IO10
      19, // IO11
      20, // IO12
      21, // IO13
      22, // IO14
      23, // IO21
      24, // IO47
      25, // IO48
      26, // IO45
      27, // IO0
      28, // IO35
      29, // IO36
      30, // IO37
      31, // IO38
      32, // IO39
      33, // IO40
      34, // IO41
      35, // IO42
      36, // RXD0
      37, // TXD0
      38, // IO2
      39, // IO1
      41  // EPAD
  };
  const int pincIOExpander[39] = {
      1,  // INT
      2,  // A1
      3,  // A2
      4,  // IO0_0
      5,  // IO0_1
      6,  // IO0_2
      7,  // IO0_3
      8,  // IO0_4
      9,  // IO0_5
      10, // IO0_6
      11, // IO0_7
      12, // VSS
      13, // IO1_0
      14, // IO1_1
      15, // IO1_2
      16, // IO1_3
      17, // IO1_4
      18, // IO1_5
      19, // IO1_6
      20, // IO1_7
      21, // A0
      22, // SCL
      23, // SDA
      24  // VDD
  };

public:
  Microcontroller microcontroller = Microcontroller(pincMicrocontrollere);
  IOExpander ioExpander = IOExpander(pincIOExpander);
  Mini()
  {
  }
};

class Elioboard
{
public:
  Demo demo;
  Mini mini;
  Elioboard()
  {
  }
};

#endif
