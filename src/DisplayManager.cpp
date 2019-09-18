// include library, include base class, make path known
#include <GxEPD.h>

// select the display class to use, only one

#include <GxGDEW042T2/GxGDEW042T2.h> // 4.2" b/w

#include GxEPD_BitmapExamples

// FreeFonts from Adafruit_GFX
#include <Adafruit/Fonts/FreeMonoBold9pt7b.h>
#include <Adafruit/Fonts/FreeMonoBold12pt7b.h>
#include <Adafruit/Fonts/FreeMonoBold18pt7b.h>
#include <Adafruit/Fonts/FreeMonoBold24pt7b.h>

#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>
GxIO_Class io(SPI, /*CS=*/SS, /*DC=*/3, /*RST=*/2);
GxEPD_Class display(io, /*RST=*/2, /*BUSY=*/1);