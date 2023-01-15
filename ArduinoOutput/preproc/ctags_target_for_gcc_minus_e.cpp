# 1 "/Users/rpoleynick/Documents/GitHub/aniMATE/aniMATE/aniMATE.ino"
/*********************************************************************
 Adafruit invests time and resources providing this open source code,
 please support Adafruit and open-source hardware by purchasing
 products from Adafruit!

 MIT license, check LICENSE for more information
 Copyright (c) 2019 Ha Thach for Adafruit Industries
 All text above, and the splash screen below must be included in
 any redistribution
*********************************************************************/

# 13 "/Users/rpoleynick/Documents/GitHub/aniMATE/aniMATE/aniMATE.ino" 2
# 14 "/Users/rpoleynick/Documents/GitHub/aniMATE/aniMATE/aniMATE.ino" 2
# 15 "/Users/rpoleynick/Documents/GitHub/aniMATE/aniMATE/aniMATE.ino" 2
# 16 "/Users/rpoleynick/Documents/GitHub/aniMATE/aniMATE/aniMATE.ino" 2

# 18 "/Users/rpoleynick/Documents/GitHub/aniMATE/aniMATE/aniMATE.ino" 2
# 19 "/Users/rpoleynick/Documents/GitHub/aniMATE/aniMATE/aniMATE.ino" 2

# 21 "/Users/rpoleynick/Documents/GitHub/aniMATE/aniMATE/aniMATE.ino" 2
# 22 "/Users/rpoleynick/Documents/GitHub/aniMATE/aniMATE/aniMATE.ino" 2

// Start address and size should matches value in the CircuitPython (INTERNAL_FLASH_FILESYSTEM = 1)
// to make it easier to switch between Arduino and CircuitPython



// Internal Flash object
Adafruit_InternalFlash flash((0x00040000 - 256 - 0 - (200*1024)), (200*1024));

// file system object from SdFat
FatVolume fatfs;

FatFile root;
FatFile file;

// USB MSC object
Adafruit_USBD_MSC usb_msc;

Adafruit_ImageReader reader(fatfs);

Adafruit_DotStar dotStrip(1, (3u), (4u), (2 | (1 << 2) | (0 << 4)) /*|< Transmit as B,G,R*/);

Adafruit_NeoPixel strip(60, 1, ((1 << 6) | (1 << 4) | (0 << 2) | (2)) /*|< Transmit as G,R,B*/ + 0x0000 /*|< 800 KHz data transmission*/);

// Set to true when PC write to flash
bool fs_changed;

// the setup function runs once when you press reset or power the board
void setup()
{
  // Initialize internal flash
  flash.begin();

  // Set disk vendor id, product id and revision with string up to 8, 16, 4 characters respectively
  usb_msc.setID("Adafruit", "Internal Flash", "1.0");

  // Set callback
  usb_msc.setReadWriteCallback(msc_read_callback, msc_write_callback, msc_flush_callback);
  usb_msc.setWritableCallback(msc_writable_callback);

  // Set disk size, block size should be 512 regardless of flash page size
  usb_msc.setCapacity(flash.size()/512, 512);

  // Set Lun ready
  usb_msc.setUnitReady(true);

  usb_msc.begin();

  // Init file system on the flash
  fatfs.begin(&flash);

  Serial.begin(115200);
  //while ( !Serial ) delay(10);   // wait for native usb

  fs_changed = true; // to print contents initially

  dotStrip.begin(); // Initialize pins for output
  dotStrip.setBrightness(80);
  dotStrip.show(); // Turn all LEDs off ASAP

  strip.begin();
  strip.clear();
  strip.show();
  strip.setBrightness(50);
}

GFXcanvas16* canvas = nullptr;

void loop() {
  if ( fs_changed ) {
    fs_changed = false;

    ImageReturnCode stat;
    int32_t width, height;
    Adafruit_Image img;
    // Query the dimensions of image 'parrot.bmp' WITHOUT loading to screen:
    Serial.print((reinterpret_cast<const __FlashStringHelper *>(("Querying pix.bmp image size..."))));
    stat = reader.bmpDimensions("/pix.bmp", &width, &height);
    reader.printStatus(stat); // How'd we do?
    if(stat == IMAGE_SUCCESS) { // If it worked, print image size...
      Serial.print((reinterpret_cast<const __FlashStringHelper *>(("Image dimensions: "))));
      Serial.print(width);
      Serial.write('x');
      Serial.println(height);

      stat = reader.loadBMP("/pix.bmp", img);
      if (stat == IMAGE_SUCCESS) {
        Serial.println("Horray!");
        canvas = (GFXcanvas16*) img.getCanvas();
      } else {
        Serial.println("Oh nose!");
      }

    } else {
      Serial.println("No image :(");
    }

    Serial.println();
    delay(1000); // refresh every 1 second
  }

  if (canvas == nullptr) {
    delay(100);
    return;
  }

  uint16_t frame = (millis() * (canvas->height()) / 1500) % (canvas->height());

  //Serial.print(frame);
  //Serial.print(" ");

  //Serial.print(canvas->getPixel(0, frame));
  uint16_t rgb565 = canvas->getPixel(0, frame);
  //Serial.print(rgb565); Serial.print(" (");
  uint8_t red = (rgb565 & 0b1111100000000000) >> 8;
  //Serial.print(red); Serial.print(", ");
  uint8_t green = (rgb565 & 0b0000011111100000) >> 3;
  //Serial.print(green); Serial.print(", ");
  uint8_t blue = (rgb565 & 0b0000000000011111) << 3;
  //Serial.print(blue); Serial.print(")\n");

  dotStrip.setPixelColor(0, (red << 16) | (green << 8) | blue);
  dotStrip.show();

  for (uint8_t i = 0; i < strip.numPixels(), i < canvas->width(); i++) {
    uint16_t rgb565 = canvas->getPixel(i, frame);
    uint8_t red = (rgb565 & 0b1111100000000000) >> 8;
    uint8_t green = (rgb565 & 0b0000011111100000) >> 3;
    uint8_t blue = (rgb565 & 0b0000000000011111) << 3;
    uint32_t c = (red << 16) | (green << 8) | blue;
    strip.setPixelColor(i, c);
  }
  strip.show();

  //Serial.println("");
  delay(50);
}

// Callback invoked when received READ10 command.
// Copy disk's data to buffer (up to bufsize) and
// return number of copied bytes (must be multiple of block size)
int32_t msc_read_callback (uint32_t lba, void* buffer, uint32_t bufsize)
{
  // Note: InternalFlash Block API: readBlocks/writeBlocks/syncBlocks
  // already include sector caching (if needed). We don't need to cache it, yahhhh!!
  return flash.readBlocks(lba, (uint8_t*) buffer, bufsize/512) ? bufsize : -1;
}

// Callback invoked when received WRITE10 command.
// Process data in buffer to disk's storage and
// return number of written bytes (must be multiple of block size)
int32_t msc_write_callback (uint32_t lba, uint8_t* buffer, uint32_t bufsize)
{
  // Note: InternalFlash Block API: readBlocks/writeBlocks/syncBlocks
  // already include sector caching (if needed). We don't need to cache it, yahhhh!!
  return flash.writeBlocks(lba, buffer, bufsize/512) ? bufsize : -1;
}

// Callback invoked when WRITE10 command is completed (status received and accepted by host).
// used to flush any pending cache.
void msc_flush_callback (void)
{
  // sync with flash
  flash.syncBlocks();

  // clear file system's cache to force refresh
  fatfs.cacheClear();

  fs_changed = true;
}

// Invoked to check if device is writable as part of SCSI WRITE10
// Default mode is writable
bool msc_writable_callback(void)
{
  // true for writable, false for read-only
  return true;
}
