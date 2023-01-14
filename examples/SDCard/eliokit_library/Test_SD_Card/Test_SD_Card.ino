#include "eliokit.h"

// IO boards
#define XL9335_ADD						0x20

void setup() {

  // put your setup code here, to run once:
  Serial.begin(115200);
    // put your setup code here, to run once:
  i2cPeripheralInitCustomSpeed(I2C_0_MASTER_NUM, I2C_0_MASTER_FREQ_HZ);
	i2cPeripheralInitCustomSpeed(I2C_1_MASTER_NUM, I2C_1_MASTER_FREQ_HZ);
  gpioExpanderInit();
  spiPeripheralInit();
  while (!sdCardBusInit()) {
    Serial.println("Errore inizializzazione card");
    delay(1000);
  }
  while (!sdCardMountFileSystem()) {
    Serial.println("Errore Mounting card");
    delay(1000);
  }
  Serial.println("Card inizializzata correttamente");
  sdCardCreateWriteCloseFileS("prova","prova1.txt","buongiorno");
  listDir("");
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Lista directory:");  
  listDir("");
  delay(5000);
}
