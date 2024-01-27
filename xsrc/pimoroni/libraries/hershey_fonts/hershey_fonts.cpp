#include "hershey_fonts.hpp"
#include "common/unicode_sorta.hpp"
#include "common/pimoroni_common.hpp"
#include <cmath>

namespace hershey {
  std::map<std::string, const font_t*> fonts = {
    { "sans",         &futural },
    //{ "sans_bold",    &futuram },
    { "gothic",       &gothgbt },
    //{ "cursive_bold", &scriptc },
    { "cursive",      &scripts },
    { "serif_italic", &timesi  },
    { "serif",        &timesr  },
    //{ "serif_bold",   &timesrb }
  };

  inline float deg2rad(float degrees) {
    return (float)((degrees * M_PI) / 180.0f);
  }

  const font_glyph_t* glyph_data(const font_t* font, unsigned char c) {
    if(c < 32 || c > 127 + 64) { // + 64 char remappings defined in unicode_sorta.hpp
      return nullptr;
    }

    if(c > 127) {
      c = unicode_sorta::char_base_195[c - 128];
    }

    return &font->chars[c - 32];
  }

  int32_t measure_glyph(const font_t* font, unsigned char c, float s) {
    const font_glyph_t *gd = glyph_data(font, c);

    // if glyph data not found (id too great) then skip
    if(!gd) {
      return 0;
    }

    return (int32_t) (gd->width * s);
  }

  int32_t measure_text(const font_t* font, std::string message, float s) {
    int32_t width = 0;
    for(auto &c : message) {
      width += measure_glyph(font, c, s);
    }
    return width;
  }

  int32_t glyph(const font_t* font, line_func line, unsigned char c, int32_t x, int32_t y, float s, float a) {
    const font_glyph_t *gd = glyph_data(font, c);

    // if glyph data not found (id too great) then skip
    if(!gd) {
      return 0;
    }

    a = deg2rad(a);
    float as = sin(a);
    float ac = cos(a);

    const int8_t *pv = gd->vertices;
    int8_t cx = (int8_t) ((*pv++) * s);
    int8_t cy = (int8_t) ((*pv++) * s);
    bool pen_down = true;

    for(uint32_t i = 1; i < gd->vertex_count; i++) {
      if(pv[0] == -128 && pv[1] == -128) {
        pen_down = false;
        pv += 2;
      }else{
        int8_t nx = (int8_t) ((*pv++) * s);
        int8_t ny = (int8_t) ((*pv++) * s);

        int rcx = (int) ((cx * ac - cy * as) + 0.5f);
        int rcy = (int) ((cx * as + cy * ac) + 0.5f);

        int rnx = (int) ((nx * ac - ny * as) + 0.5f);
        int rny = (int) ((nx * as + ny * ac) + 0.5f);

        if(pen_down) {
          line(rcx + x, rcy + y, rnx + x, rny + y);
        }

        cx = nx;
        cy = ny;
        pen_down = true;
      }
    }

    return (int32_t) (gd->width * s);
  }

  void text(const font_t* font, line_func line, std::string message, int32_t x, int32_t y, float s, float a) {
    int32_t cx = x;
    int32_t cy = y;

    int32_t ox = 0;

    float as = sin(deg2rad(a));
    float ac = cos(deg2rad(a));

    for(auto &c : message) {
      int rcx = (int) ((ox * ac) + 0.5f);
      int rcy = (int) ((ox * as) + 0.5f);

      ox += glyph(font, line, c, cx + rcx, cy + rcy, s, a);
    }
  }
}