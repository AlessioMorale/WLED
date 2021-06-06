#pragma once

#include "tom-thumb.h"
#include "wled.h"
#include <array>
#include <bitset>

class ClockMatrixUsermod;
static constexpr size_t DIGIT_HEIGHT = 5;
static constexpr size_t DIGIT_WIDTH = 3;

ClockMatrixUsermod *instance = nullptr;
class Character : public std::bitset<DIGIT_HEIGHT * DIGIT_WIDTH>
{
};

class ClockMatrixUsermod : public Usermod
{
private:
  unsigned long lastTime = 0;
  uint8_t last_count = 0;

  uint8_t hour = 0;
  uint8_t minute = 0;
  uint8_t second = 0;
  struct tm timeinfo;

  static constexpr size_t MATRIX_WIDTH = 7;
  static constexpr size_t MATRIX_HEIGHT = 11;
  std::array<std::bitset<MATRIX_WIDTH>, MATRIX_HEIGHT> framebuffer;
  bool test = false;

  uint16_t get_led_index(int x, int y)
  {
    auto segment = strip.getSegment(x);
    auto index = ((segment.options & REVERSE) == REVERSE) ? segment.stop - y - 1 : segment.start + y;
    return index;
  }

  void set_pixel(int x, int y, uint32_t colour)
  {
    strip.setPixelColor(get_led_index(x, y), colour);
  }

  uint32_t get_pixel(int x, int y)
  {
    auto index = get_led_index(x, y);
    return strip.getPixelColor(index);
  }

  void set_pixel(int x, int y, bool set)
  {
    auto colour = get_pixel(x, y);
    CRGB c(colour);
    if (set)
    {
      c = strip.getSegment(0).colors[2];
    }
    strip.setPixelColor(get_led_index(x, y), c.r, c.g, c.b, 0);
  }

  void handleOverlay()
  {
    for (int y = 0; y < MATRIX_HEIGHT; y++)
    {
      for (int x = 0; x < MATRIX_WIDTH; x++)
      {
        set_pixel(x, y, framebuffer[y][x]);
      }
    }
  };

  unsigned short get_char_index(uint8_t ascii)
  {
    uint i = 0;

    while (i < font.Chars && ascii != font.Index[i])
      i++;
    if (font.Index[i] != ascii)
      return 10; // asterisk
    return i;
  }

  Character get_char(uint8_t ascii)
  {
    auto index = get_char_index(ascii);
    Character c;
    c.reset();
    auto disp = index * font.Height;
    int n = 0;
    for (int y = 0; y < DIGIT_HEIGHT; y++)
    {
      unsigned char line = font.Bitmap[disp + y];
      for (int x = 0; x < DIGIT_WIDTH; x++)
      {
        bool bit = (line & 0b10000000) != 0;
        line *= 2;
        c[n++] = bit;
      }
    }
    return c;
  }

  void draw_digit(int x, int y, uint8_t ascii)
  {
    auto character = get_char(ascii);
    for (int n = 0; n < character.size(); n++)
    {
      int c_x = n % DIGIT_WIDTH + x;
      int c_y = n / DIGIT_WIDTH + y;
      framebuffer[c_y][c_x] = character[n];
    }
  }

  bool update_time()
  {
    if (getLocalTime(&timeinfo))
    {
      hour = timeinfo.tm_hour;
      minute = timeinfo.tm_min;
      second = timeinfo.tm_sec;
      return true;
    }
    return false;
  }

  void draw_clock()
  {
    draw_digit(0, 0, hour / 10 + 48);
    draw_digit(4, 0, hour % 10 + 48);
    draw_digit(0, 6, minute / 10 + 48);
    draw_digit(4, 6, minute % 10 + 48);
  }

  void draw_wait()
  {
    draw_digit(0, 0, 'w');
    draw_digit(4, 0, 'i');
    draw_digit(0, 6, 'f');
    draw_digit(4, 6, 'i');
  }

public:
  typedef void (*callback)();
  // Functions called by WLED
  void setup()
  {

    instance = this;
    for (int y = 0; y < MATRIX_HEIGHT; y++)
    {
      for (int x = 0; x < MATRIX_WIDTH; x++)
      {
        framebuffer[y][x] = false; //((x + y) % 2 == 0);
      }
    }
    callback overlay_handler = []()
    { instance->handleOverlay(); };
    strip.setShowCallback(overlay_handler);
  }

  /*
   * connected() is called every time the WiFi is (re)connected
   * Use it to initialize network interfaces
   */
  void connected()
  {
    const char *ntpServer = "pool.ntp.org";

    const long gmtOffset_sec = 0;
    const int daylightOffset_sec = 3600;
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  }

  /*
   * loop() is called continuously. Here you can check for events, read sensors,
   * etc.
   */
  void loop()
  {
    if (millis() - lastTime > 1000)
    {
      if (update_time())
      {
        draw_clock();
      }
      else
      {
        draw_wait();
      }

      lastTime = millis();
    }
  }

  /*
   * addToJsonInfo() can be used to add custom entries to the /json/info part of
   * the JSON API. Creating an "u" object allows you to add custom key/value
   * pairs to the Info section of the WLED web UI. Below it is shown how this
   * could be used for e.g. a light sensor
   */
  /*
  void addToJsonInfo(JsonObject& root)
  {
    int reading = 20;
    //this code adds "u":{"Light":[20," lux"]} to the info object
    JsonObject user = root["u"];
    if (user.isNull()) user = root.createNestedObject("u");

    JsonArray lightArr = user.createNestedArray("Light"); //name
    lightArr.add(reading); //value
    lightArr.add(" lux"); //unit
  }
  */

  /*
   * addToJsonState() can be used to add custom entries to the /json/state part
   * of the JSON API (state object). Values in the state object may be modified
   * by connected clients
   */
  // void addToJsonState(JsonObject &root) {
  // root["user0"] = userVar0;
  //}

  /*
   * readFromJsonState() can be used to receive data clients send to the
   * /json/state part of the JSON API (state object). Values in the state object
   * may be modified by connected clients
   */
  // void readFromJsonState(JsonObject &root) {
  //   userVar0 =
  //       root["user0"] |
  //       userVar0; // if "user0" key exists in JSON, update, else keep old
  //       value
  // if (root["bri"] == 255) Serial.println(F("Don't burn down your
  // garage!"));
  //}

  /*
   * addToConfig() can be used to add custom persistent settings to the cfg.json
   * file in the "um" (usermod) object. It will be called by WLED when settings
   * are actually saved (for example, LED settings are saved) If you want to
   * force saving the current state, use serializeConfig() in your loop().
   *
   * CAUTION: serializeConfig() will initiate a filesystem write operation.
   * It might cause the LEDs to stutter and will cause flash wear if called too
   * often. Use it sparingly and always in the loop, never in network callbacks!
   *
   * addToConfig() will also not yet add your setting to one of the settings
   * pages automatically. To make that work you still have to add the setting to
   * the HTML, xml.cpp and set.cpp manually.
   *
   * I highly recommend checking out the basics of ArduinoJson serialization and
   * deserialization in order to use custom settings!
   */
  // void addToConfig(JsonObject &root) {
  //   JsonObject top = root.createNestedObject("exampleUsermod");
  //   top["great"] =
  //       userVar0; // save this var persistently whenever settings are saved
  //}

  /*
   * readFromConfig() can be used to read back the custom settings you added
   * with addToConfig(). This is called by WLED when settings are loaded
   * (currently this only happens once immediately after boot)
   *
   * readFromConfig() is called BEFORE setup(). This means you can use your
   * persistent values in setup() (e.g. pin assignments, buffer sizes), but also
   * that if you want to write persistent values to a dynamic buffer, you'd need
   * to allocate it here instead of in setup. If you don't know what that is,
   * don't fret. It most likely doesn't affect your use case :)
   */
  // void readFromConfig(JsonObject &root) {
  //   JsonObject top = root["top"];
  //   userVar0 =
  //       top["great"] |
  //       42; // The value right of the pipe "|" is the default value in case
  //       your
  // setting was not present in cfg.json (e.g. first boot)
  //}

  /*
   * getId() allows you to optionally give your V2 usermod an unique ID (please
   * define it in const.h!). This could be used in the future for the system to
   * determine whether your usermod is installed.
   */
  uint16_t getId() { return USERMOD_ID_EXAMPLE; }

  // More methods can be added in the future, this example will then be
  // extended. Your usermod will remain compatible as it does not need to
  // implement all methods from the Usermod base class!
};