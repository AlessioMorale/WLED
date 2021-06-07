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
  bool _colour_from_state = false;
  CRGB _clock_digits_colour;
  uint8_t _hour = 0;
  uint8_t _minute = 0;
  uint8_t _second = 0;
  struct tm timeinfo;

  static constexpr size_t MATRIX_WIDTH = 7;
  static constexpr size_t MATRIX_HEIGHT = 11;
  std::array<std::bitset<MATRIX_WIDTH>, MATRIX_HEIGHT> framebuffer;
  bool test = false;

  uint16_t get_led_index(int x, int y)
  {
    auto segment = strip.getSegment(x);
    auto index = ((segment.options & REVERSE) == REVERSE) ? segment.start + y : segment.stop - y - 1;
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
      c = _clock_digits_colour;
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
    updateLocalTime();
    _hour = hour(localTime);
    _minute = minute(localTime);
    _second = second(localTime);
    return true;
  }

  void draw_clock()
  {
    draw_digit(0, 0, _hour / 10 + 48);
    draw_digit(4, 0, _hour % 10 + 48);
    draw_digit(0, 6, _minute / 10 + 48);
    draw_digit(4, 6, _minute % 10 + 48);
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
        framebuffer[y][x] = false;
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
  }

  /*
   * loop() is called continuously. Here you can check for events, read sensors,
   * etc.
   */
  void loop()
  {
    if (millis() - lastTime > 1000)
    {
      if (!_colour_from_state)
      {
        _clock_digits_colour = strip.getSegment(0).colors[2];
      }

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

#define CLOCK_SETTINGS_KEY "matrix"
#define CLOCK_DIGITS_COLOUR_KEY "dig_col"

  void _saveToJson(JsonObject &root)
  {
    JsonObject top = root.createNestedObject(CLOCK_SETTINGS_KEY);
    JsonArray colarr = top.createNestedArray(CLOCK_DIGITS_COLOUR_KEY);
    colarr.add(_clock_digits_colour.r);
    colarr.add(_clock_digits_colour.g);
    colarr.add(_clock_digits_colour.b);
  }

  void _loadFromJson(JsonObject &root)
  {
    _colour_from_state = false;
    JsonObject top = root[CLOCK_SETTINGS_KEY];
    if (!top.isNull())
    {
      JsonArray colarr = top[CLOCK_DIGITS_COLOUR_KEY];

      if (!colarr.isNull())
      {
        _clock_digits_colour.r = colarr[0];
        _clock_digits_colour.g = colarr[1];
        _clock_digits_colour.b = colarr[2];
        _colour_from_state = true;
      }
    }
  }

  void addToJsonState(JsonObject &root)
  {
    _saveToJson(root);
  }

  void readFromJsonState(JsonObject &root)
  {
    _loadFromJson(root);
  }

  /*
   * getId() allows you to optionally give your V2 usermod an unique ID (please
   * define it in const.h!). This could be used in the future for the system to
   * determine whether your usermod is installed.
   */
  uint16_t getId() { return USERMOD_ID_CLOCK_MATRIX; }
};
