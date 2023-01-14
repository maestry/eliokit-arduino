#include <Esp.h>


void setup() {
  // put your setup code here, to run once:
  delay(500);
  Serial.begin(115200);
  delay(500);
  Serial.println("\n\n================================");
  Serial.printf("Chip Model: %s %s %d\n",
                ESP.getChipModel(),
                "rev.",
                (int)ESP.getChipRevision());
  Serial.printf("with number of cores = %d\n", (int)ESP.getChipCores());
  Serial.println("================================");

#ifdef EXTERNAL_NUM_INTERRUPTS
  Serial.printf("EXTERNAL_NUM_INTERRUPTS = %d\n", EXTERNAL_NUM_INTERRUPTS);
#endif
#ifdef NUM_DIGITAL_PINS
  Serial.printf("NUM_DIGITAL_PINS = %d\n", NUM_DIGITAL_PINS);
#endif
#ifdef NUM_ANALOG_INPUTS
  Serial.printf("NUM_ANALOG_INPUTS = %d\n", NUM_ANALOG_INPUTS);
#endif
  Serial.println();
  Serial.printf("Default TX:   %d\n", TX);
  Serial.printf("Default RX:   %d\n", RX);
  Serial.println();
  Serial.printf("Default SDA:  %d\n", SDA);
  Serial.printf("Default SCL:  %d\n", SCL);
  Serial.println();
  Serial.printf("Default SS:   %d\n", SS);
  Serial.printf("Default MOSI: %d\n", MOSI);
  Serial.printf("Default MISO: %d\n", MISO);
  Serial.printf("Default SCK:  %d\n", SCK);

  Serial.println();
  Serial.printf("A0\tA1\tA2\tA3\tA4\tA5\tA6\tA7\tA8\tA9\n");
  Serial.printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", 
                 A0, A1, A2, A3, A4, A5, A6, A7, A8, A9);
  Serial.println();
  Serial.printf("A10\tA11\tA12\tA13\tA14\tA15\tA16\tA17\tA18\tA19\n");
  Serial.printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", 
                 A10, A11, A12, A13, A14, A15, A16, A17, A18, A19);
  Serial.println();

  Serial.printf("T1\tT2\tT3\tT4\tT5\tT6\tT7\tT8\tT9\tT10\n");
  Serial.printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", 
                 T1, T2, T3, T4, T5, T6, T7, T8, T9, T10);
  Serial.println();
  Serial.printf("T11\tT12\tT13\tT14\n");
  Serial.printf("%d\t%d\t%d\t%d\n", 
                 T11, T12, T13, T14);

  Serial.println("================================");
}

void loop() {
  // put your main code here, to run repeatedly:

}