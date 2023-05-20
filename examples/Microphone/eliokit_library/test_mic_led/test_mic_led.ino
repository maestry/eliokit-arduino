/*	
 *	LEDctrl.ino - ESP32 LED Controller Program that controls 
 *				  LEDs using a Blynk Mobile App over WiFi.
 * 	
 *	Author: Abdullah Almarzouq (anm_e36@hotmail.com)
 *	LEDctrl Github: https://github.com/an36/LED-Controller-ESP32-Blynk
 *	
 *  Finalized on: June 27, 2021
 */


#include <Adafruit_NeoPixel.h>
#include <driver/i2s.h>



bool systemPower = false;


#define LED_PIN    3		//Designated pin to set addressable LEDs signal (LEDs' Data In)
#define LED_COUNT  18		//Number of LEDs/Pixel in a strip
#define MAX_RGB    255.0

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

bool Static = false;
bool Pulse = false;
bool Cycle = false;
bool Dance = false;

int LEDsRate = 15;
float LEDsBrightness = 255;
float zeRed, zeGreen, zeBlue;

float PulseLevel = 0;
bool PulseSwitch = false;

int currCycleRed = 0;
int currCycleGreen = 0;
int currCycleBlue = 0;
bool CycleRedTurn = true;
bool CycleGreenTurn = false;
bool CycleBlueTurn = false;



/*<------------------------ I2S PDM Mic Macros and Global Variables ------------------------>*/
#define I2S_PORT       I2S_NUM_0   //I2S port to read Mic input
#define I2S_WS_PIN     42          //I2S Word Select pin on ESP32 (GPIO 21)
#define I2S_DATA_PIN   43          //I2S Data pin on ESP32 (GPIO 17)
i2s_config_t i2s_config;           //will hold i2s bus configuration
i2s_pin_config_t pin_config;       //will hold i2s pin configuration

const int DMA_BUFF_CT = 2;         //number of DMA buffer
const int DMA_BUFF_LEN = 32;       //samples per DMA buffer
const int SAMPLE_RATE = 16600;     //44.1kHz
uint16_t samples[1024];            //will hold the samples read from DMA buffer(s)
size_t num_bytes_read;             //will hold the number of bytes read from DMA buffer(s)

/*<------------------------ #**Functions Prototypes**# ------------------------>*/
void _resetBlynkBtnflags(uint8_t flag);                 /*Resets Blynk button/widget fLags. I.e., Sets LED flag to false*/
void colorWipe(uint32_t color);                         /*Sets all of the pixels on the strip to the specified color argument*/
void LEDsOff();                                         /*Sets all the LEDs off*/
void SetLEDsPulse();                                    /*Makes the LEDs Pulse*/
void SetLEDsCycle();                                    /*Makes the LEDs Cycle thorugh RGB colors*/
void SetLEDsDance(float value);                         /*Sets the NeoPixel LEDs in response to the Mic sound level received value*/
void MicInit();                                         /*Initializes I2S bus to start reading Mic input*/
float MicNormValue();                                   /*Return the Mic volume level, a float value */
float mean(uint16_t *samples, int len);                 /*Computes the mean*/
float normalize(uint16_t *samples, int len);            /*converts raw acceleration values to actual*/


void setup()
{
  // Debug console
  Serial.begin(115200);

  /*<------------------------------- NeoPixel LED Ring setup ------------------------------->*/
  strip.begin();            // INITIALIZE NeoPixel strip object (REQUIRED)
  LEDsOff();
  strip.setBrightness(51);  // Set BRIGHTNESS to about 1/5 (max = 255)

  /*<------------------------------- PDM Mic & I2S setup ------------------------------->*/
  
  i2s_driver_uninstall(I2S_NUM_0);
  Serial.println("Init microphone");
  delay(3000);
  MicInit();
  for (int i=0; i<10; i++) {
    Serial.print(".");
  }
  Serial.println(".");
  Serial.println("Microphone ok");
  //initialize I2S bus (Comment this line if there's no Microphone)
  Serial.println("I2S driver installed.");
}

void loop(){
      float norm_sample = MicNormValue();
      //Serial.println(norm_sample);     //return the Mic volume value (normalized float value)
      SetLEDsDance(norm_sample);
}

/*Sets all of the pixels on the strip to the specified color argument*/
void colorWipe(uint32_t color) {
  for(int i=0; i<strip.numPixels(); i++) {    // For each pixel in strip...
    strip.setPixelColor(i, color);            //  Set pixel's color (in RAM)
    strip.show();                             //  Update strip to match
  }
}

/*Sets all the LEDs off*/
void LEDsOff(){
  strip.clear();           //set all LEDs off
  strip.show();            // Turn OFF all pixels ASAP
}

/*Makes the LEDs Pulse*/
void SetLEDsPulse(){
  float pulseRed = (PulseLevel/MAX_RGB)*zeRed;
  float pulseGreen = (PulseLevel/MAX_RGB)*zeGreen;
  float pulseBlue = (PulseLevel/MAX_RGB)*zeBlue;
  
  colorWipe(strip.Color(pulseRed, pulseGreen, pulseBlue));

  if(PulseSwitch){
    PulseLevel-=LEDsRate;
  }
  else{
    PulseLevel+=LEDsRate;
  }
  
  if(PulseLevel>=255){
    PulseLevel = 255;
    PulseSwitch = true;
  }
  else if(PulseLevel<=5){
    PulseLevel = 5;
    PulseSwitch = false;
  }
}

/*Makes the LEDs Cycle thorugh RGB colors*/
void SetLEDsCycle(){
  colorWipe(strip.Color(currCycleRed, currCycleGreen, currCycleBlue));
  
  if(CycleRedTurn){
    if(currCycleRed>=255){
      if(currCycleBlue>0){
        currCycleBlue-=LEDsRate;
        if(currCycleBlue<0){
          currCycleBlue=0;
        }
        return;
      }
      else{
        CycleGreenTurn = true;
        CycleRedTurn = false;
      }
    }
    else{
        currCycleRed+=LEDsRate;
        if(currCycleRed>=255){
          currCycleRed=255;
        }
      return;
    }
  }
  
  if(CycleGreenTurn){
    if(currCycleGreen>=255){
      if(currCycleRed>0){
        currCycleRed-=LEDsRate;
        if(currCycleRed<0){
          currCycleRed=0;
        }
        return;
      }
      else{
        CycleGreenTurn = false;
        CycleBlueTurn = true;
      }
    }
    else{
      currCycleGreen+=LEDsRate;
      if(currCycleGreen>255){
        currCycleGreen=255;
      }
      return;
    }
  }

  if(CycleBlueTurn){
    if(currCycleBlue>=255){
      if(currCycleGreen>0){
        currCycleGreen-=LEDsRate;
        if(currCycleGreen<0){
          currCycleGreen=0;
        }
        return;
      }
      else{
        CycleBlueTurn = false;
        CycleRedTurn = true;
        return;
      }
    }
    else{
      currCycleBlue+=LEDsRate;
      if(currCycleBlue>255){
        currCycleBlue=255;
      }
      return;
    }
  }
}

/*Sets the NeoPixel LEDs in response to the Mic sound level received value*/
void SetLEDsDance(float value){
  if(value>250.0 && value<=300.0){
    colorWipe(0x2200ff);
  }
  else if(value>300.0 && value<=400.0){
    colorWipe(0x9500ff);
  }
  else if(value>400.0 && value<=500.0){
    colorWipe(0x00ff2f);
  }
  else if(value>500.0 && value<=700.0){
    colorWipe(0xf5ff38);
  }
  else if(value>700.0 && value<=1500.0){
    colorWipe(0xff8400);
  }
  else if(value>1500.0 && value<=3000.0){
    colorWipe(0xff00cc);
  }
  else if(value>3000.0 && value<=7000.0){
    colorWipe(0x00fbff);
  }
  else if(value>7000.0 && value<=12000.0){
    colorWipe(0xff0044);
  }
  else if(value>12000.0){
    colorWipe(0xff0000);
  }
  else if(value<250.0){
    LEDsOff();
  }
}

/*Initializes I2S bus to start reading Mic input*/
void MicInit(){
  esp_err_t err;

  // The I2S config
  i2s_config = {
    .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_PDM),     // Receive, not transfer
    .sample_rate = SAMPLE_RATE,                    
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,                         //Number of bits per sample
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,                         //Do not connect SEL pin.
    .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_PCM),
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,                             // Interrupt level 1
    .dma_buf_count = DMA_BUFF_CT,                                         // number of buffers
    .dma_buf_len = DMA_BUFF_LEN                                           // samples per buffer
  };

  // The pin config as per the setup
  pin_config = {
    .bck_io_num = -1,             // not used
    .ws_io_num = I2S_WS_PIN,      // CLOCK
    .data_out_num = -1,           // not used (only for speakers)
    .data_in_num = I2S_DATA_PIN   // DOUT from mic
  };

  // Configuring the I2S driver and pins.
  // This function must be called before any I2S driver read/write operations.
  err = i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
  
  if (err != ESP_OK) {
    Serial.printf("Failed installing driver: %d\n", err);
    while (true);
  }
  
  err = i2s_set_pin(I2S_PORT, &pin_config);
  
  if (err != ESP_OK) {
    Serial.printf("Failed setting pin: %d\n", err);
    while (true);
  }
}

/* Return the Mic volume level, a float value */
float MicNormValue(){
  // Read multiple samples at once
  int err = i2s_read(I2S_PORT, &samples, sizeof(uint16_t)*DMA_BUFF_LEN, &num_bytes_read,  portMAX_DELAY);  //reads two bytes (16-bit)

  if (err != ESP_OK) {  //if reading error
    Serial.printf("Failed rading i2s: %d\n", err);
    while (true);
  }

  if(num_bytes_read>0){
    return normalize(*&samples,num_bytes_read/2);  //get the normalized value
  }
}

/*Computes the mean*/
float mean(uint16_t *samples, int len){
  uint16_t sum = 0;
  for(int i=0; i<len; i++){
    sum+=samples[i];
  }
  return ((float)sum)/len;
}

/*Computes the normalized value of the obtained samples*/
float normalize(uint16_t *samples, int len){
  float avg = mean(samples,len);
  float samples_sum = 0;
  for(int i=0; i<len; i++){
    samples_sum += ((float)samples[i] - avg) * ((float)samples[i] - avg);
  }
  return sqrt(samples_sum/len);
}