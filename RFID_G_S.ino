// Include the PNG decoder library
#include <PNGdec.h>
#include "resize.h" // Image is stored here in an 8-bit array


#include <WiFi.h>
#include <HTTPClient.h>
#include <SPI.h>
#include <MFRC522.h>
#include <TFT_eSPI.h>
#include <AnimatedGIF.h>
AnimatedGIF gif;
PNG png; // PNG decoder instance
TFT_eSPI tft = TFT_eSPI();
#include "out.h"

#define SS_PIN 21
#define RST_PIN 22
#define GIF_IMAGE cards   //  No DMA  63 fps, DMA:  71fps
#define NORMAL_SPEED  // Comment out for rame rate for render speed test

#define MAX_IMAGE_WIDTH 240 // Adjust for your images

// Stock font and GFXFF reference handle
#define GFXFF 1f
#define FF18 &FreeSans12pt7b
// Custom are fonts added to library "TFT_eSPI\Fonts\Custom" folder
// a #include must also be added to the "User_Custom_Fonts.h" file
// in the "TFT_eSPI\User_Setups" folder. See example entries.
#define CF_OL24 &Orbitron_Light_24
#define CF_OL32 &Orbitron_Light_32
#define CF_RT24 &Roboto_Thin_24
#define CF_S24  &Satisfy_24
#define CF_Y32  &Yellowtail_32
// Load GIF library
#define STATUS_X 120 // Centred on this
#define STATUS_Y 200

int16_t xpos = 10;
int16_t ypos = 163;
////////////////////////////
int textX = 120;
int textY = 100;
String textMessage = "Scan your Card please";
int textWidth = tft.textWidth(textMessage); // Get the width of the text
int textHeight = tft.fontHeight(); // Get the height of the font
int frameThickness = 2; // Thickness of the frame
int uid_frameThickness = 1;
int frameColor = TFT_BROWN; 
int uid_framecolor =TFT_WHITE;
int rectX = 20 - frameThickness; 
int rectY = 130 - frameThickness; 
int rectWidth = 200 + 2 * frameThickness;
int rectHeight = 20 + 2 * frameThickness; 



const char* ssid = "WE8DA83E";
const char* password = "ka027528";
const char *scriptUrl = "https://script.google.com/macros/s/AKfycbxOY47_CeKkZXBw7JdxYx2LDevzrxjMActY5D-eOVuymx1k3wbN3B7JKXN9itwAYwOz/exec";

MFRC522 mfrc522(SS_PIN, RST_PIN);



void setup() {
  
  tft.begin();
  SPI.begin();        // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522

  // Connect to Wi-Fi
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  ////////////////////////////////////////

  #ifdef USE_DMA
  tft.initDMA();
#endif
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  gif.begin(BIG_ENDIAN_PIXELS);
  /////////////////////////////////////////////////////////
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_PURPLE, TFT_ORANGE);
  tft.setFreeFont(FF18); 
  int textWidth = tft.textWidth(textMessage); 
  int textHeight = tft.fontHeight();
  Serial.print("Height=");
  Serial.print(textHeight);
  tft.drawString(textMessage,  textX, textY, GFXFF);
  tft.drawRect(0, 91, textWidth + 2 * frameThickness, 25 , frameColor);
  tft.drawRect(0, 90, textWidth + 2 * frameThickness, 27, frameColor);
  tft.drawRect(0, 89, textWidth + 2 * frameThickness, 29, frameColor);
  tft.drawRect(0, 88, textWidth + 2 * frameThickness, 31, frameColor);
  //----------------------------------------------------------
  tft.fillRect(20, 130, 200, 20, TFT_YELLOW);
  tft.drawRect(rectX, rectY, rectWidth, rectHeight, uid_framecolor);
  tft.drawRect(rectX, 127, rectWidth, rectHeight +2, uid_framecolor);
  tft.drawRect(rectX, 126, rectWidth, rectHeight +2, uid_framecolor);
  tft.setTextColor(TFT_BLACK); // Adjust the text color
  tft.setCursor(29, 147); // Set the cursor position for the variable
  tft.print("UID=");

//height =29
 // tft.drawRect(textX - frameThickness, textY - frameThickness, textWidth + 2 * frameThickness, textHeight + 2 * frameThickness, frameColor);

  ///////   End of void setup ///////////////////////

}

////////////////////////////////////////////////////


#ifdef NORMAL_SPEED // Render at rate that is GIF controlled
void loop() {
  //This part for GIF_Image_
  if (gif.open((uint8_t *)GIF_IMAGE, sizeof(GIF_IMAGE), GIFDraw))
    {
      Serial.printf("Successfully opened GIF; Canvas size = %d x %d\n", gif.getCanvasWidth(), gif.getCanvasHeight());
      tft.startWrite(); // The TFT chip slect is locked low
      while (gif.playFrame(true, NULL))
      {
        yield();
      }
      gif.close();
      tft.endWrite(); // Release TFT chip select for other SPI devices
    }
  

  //This part for Faculty Image
  int16_t rc = png.openFLASH((uint8_t *)panda, sizeof(panda), pngDraw);
  if (rc == PNG_SUCCESS) {
    Serial.println("Successfully opened png file");
    Serial.printf("image specs: (%d x %d), %d bpp, pixel type: %d\n", png.getWidth(), png.getHeight(), png.getBpp(), png.getPixelType());
    tft.startWrite();
    uint32_t dt = millis();
    rc = png.decode(NULL, 0);
    Serial.print(millis() - dt); Serial.println("ms");
    tft.endWrite();
    // png.close(); // not needed for memory->memory decode
  }


  /////////Rfid setup//////////////////////////
 if (!mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }

  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial())
  {
    return;
  }

  unsigned long cardId = 0;

for (size_t i = 0; i < mfrc522.uid.size; i++)
{
  cardId = cardId * 256 + mfrc522.uid.uidByte[i];
    
}
    //The part of Display the UID On TFT
    
    tft.setTextSize(1.5); // Adjust the text size
    tft.setTextColor(TFT_WHITE); // Adjust the text color
    tft.setCursor(30, 147); // Set the cursor position for the variable
    tft.print("UID=");
    tft.setCursor(85, 147); // Set the cursor position for the variable
    tft.print(cardId);

  // Send POST request to Google Apps Script
  HTTPClient http;
  http.begin(scriptUrl);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  // Construct the request body with the card ID
  String postData = "card_id=" + String(cardId);

  // Send the POST request
  int httpResponseCode = http.POST(postData);
  
  if (httpResponseCode > 0) {
    Serial.println("HTTP POST successful");
    Serial.print("Response Code: ");
    Serial.println(httpResponseCode);
  } else {
    Serial.println("HTTP POST failed");
    Serial.print("Error Code: ");
    Serial.println(httpResponseCode);
  }

  http.end();

  mfrc522.PICC_HaltA();
  // Stop encryption on PCD
  mfrc522.PCD_StopCrypto1();

}

#else // Test maximum rendering speed
void loop()
{
  long lTime = micros();
  int iFrames = 0;

  if (gif.open((uint8_t *)GIF_IMAGE, sizeof(GIF_IMAGE), GIFDraw))
  {
    tft.startWrite(); // For DMA the TFT chip slect is locked low
    while (gif.playFrame(false, NULL))
    {
      // Each loop renders one frame
      iFrames++;
      yield();
    }
    gif.close();    
    tft.endWrite(); // Release TFT chip select for other SPI devices
    lTime = micros() - lTime;
    Serial.print(iFrames / (lTime / 1000000.0));
    Serial.println(" fps");
  }
}
#endif


// This next function will be called during decoding of the png file to
// render each image line to the TFT.  If you use a different TFT library
// you will need to adapt this function to suit.
// Callback function to draw pixels to the display
void pngDraw(PNGDRAW *pDraw) {
  uint16_t lineBuffer[MAX_IMAGE_WIDTH];
  png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0xffffffff);
  tft.pushImage(xpos, ypos + pDraw->y, pDraw->iWidth, 1, lineBuffer);
}
