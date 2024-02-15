#include <SPI.h>
#include <SD.h>
#include <FastLED.h>

FASTLED_USING_NAMESPACE

#define DATA_PIN    7
#define CLK_PIN     6
#define LED_TYPE    APA102
#define COLOR_ORDER GRB
//#define NUM_LEDS  144
#define NUM_LEDS    64

#define SD_SELECT 4

CRGB image[NUM_LEDS][NUM_LEDS];
CRGB leds[NUM_LEDS];
File imageFile;
unsigned long image_start_address = 0;
unsigned long image_end_address = 0;
unsigned long image_ptr = 0;
int image_row = 0;

int frame_counter = 0;
unsigned long last_time = 0;

// Code stolen from: https://arduino.stackexchange.com/questions/19795/how-to-read-bitmap-image-on-arduino
struct bmp_file_header_t {
  uint16_t signature;
  uint32_t file_size;
  uint16_t reserved[2];
  uint32_t image_offset;
};

struct bmp_image_header_t {
  uint32_t header_size;
  uint32_t image_width;
  uint32_t image_height;
  uint16_t color_planes;
  uint16_t bits_per_pixel;
  uint32_t compression_method;
  uint32_t image_size;
  uint32_t horizontal_resolution;
  uint32_t vertical_resolution;
  uint32_t colors_in_palette;
  uint32_t important_colors;
};


bmp_file_header_t fileHeader;
bmp_image_header_t imageHeader;

#define BRIGHTNESS          255
#define FRAMES_PER_SECOND  120


void setup() {
  // Setup serial
  Serial.begin(9600);

  // Setup LEDs
  delay(100); // 3 second delay for recovery
  FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);

  // Setup SD
  if (!SD.begin(BUILTIN_SDCARD)) {
    Serial.println("SD init failed");
    return;
  }
  Serial.println("SD Init done.");

  loadImage();
  
}

void loadImage(){
//  char filename[] = "0002.bmp";
  char filename[] = "tree.bmp";
  if (!SD.exists(filename))
  {
    Serial.println("File does not exist");
  }
  else
  {
    Serial.println("File exists");
  }
  imageFile = SD.open(filename, FILE_READ);
  // Read the file header
  imageFile.read(&fileHeader, sizeof(fileHeader));
  imageFile.read(&imageHeader, sizeof(imageHeader));

  inspectImage();

  image_start_address = fileHeader.image_offset;
  image_ptr = image_start_address;
  image_end_address = image_start_address + imageHeader.image_size;

  // Load image into memory
  imageFile.seek(image_start_address);
  for (int i = 0; i < NUM_LEDS; i++)
  {
    for (int j = 0; j < NUM_LEDS; j++)
    {
      byte r, g, b;
      g = imageFile.read();
      b = imageFile.read();
      r = imageFile.read();
      
//      image[i][j] = (b > 0 || g > 0 || r > 0);
      image[j][i].red = r;
      image[j][i].green = g;
      image[j][i].blue = b;
    }
  }
  
}

void inspectImage(){
  Serial.print("Signature: ");
  Serial.println(fileHeader.signature);
  Serial.print("File Size: ");
  Serial.println(fileHeader.file_size);
  Serial.print("Image Offset: ");
  Serial.println(fileHeader.image_offset);
  Serial.println("- - - - - - - - - - - -");
  Serial.print("Image Width: ");
  Serial.println(imageHeader.image_width);
  Serial.print("Image Height: ");
  Serial.println(imageHeader.image_height);
  Serial.print("Color Planes: ");
  Serial.println(imageHeader.color_planes);
  Serial.print("Bits Per Pixel: ");
  Serial.println(imageHeader.bits_per_pixel);
  Serial.print("image_size: ");
  Serial.println(imageHeader.image_size);
  Serial.print("Horizontal Resolution: ");
  Serial.println(imageHeader.horizontal_resolution);
  Serial.print("Vertical Resolution: ");
  Serial.println(imageHeader.vertical_resolution);
}


void loop()
{
    nextRow();
}

void nextRow()
{
  image_row++;
  if (image_row >= NUM_LEDS)
  {
    image_row = 0;
    frame_counter++;

//    #define FRAME_AGG 100
//    if (frame_counter == FRAME_AGG)
//    {
//      unsigned long new_time = micros();
//      long double fps = ((long double)FRAME_AGG / ((long double)new_time - (long double)last_time)) * 1000 * 1000;
//      Serial.print((int)fps);
//      Serial.println();
//      frame_counter = 0;
//      delay(5);
//      last_time = micros();
//    }
  }

  // Copy image to leds array
  memmove( &leds[0], &image[image_row][0], NUM_LEDS * sizeof(CRGB));

  FastLED.show();
}
