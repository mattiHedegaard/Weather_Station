#pragma once
// Landscape UI for the 416x240 e-paper (GxEPD2 + Adafruit_GFX).
// Put this file next to your .ino and include it AFTER the other includes
// (it needs `display`, the weather structs and the room getters).

#include <time.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeSansBold18pt7b.h>
#include <Fonts/FreeSansBold24pt7b.h>

#define UI_W   416
#define UI_H   240
#define UI_HDR 22

#ifndef UI_CITY
#define UI_CITY "AALBORG"   // shown in the page 1 header
#endif

enum { UI_SUN, UI_PART, UI_CLOUD, UI_RAIN, UI_SNOW, UI_STORM, UI_FOG };

struct UiRoom { float temp; int hum; int eco2; int tvoc; int aqi; };

// ---------------------------------------------------------------- text ----

// align: 0 = left, 1 = centre, 2 = right. y is the text BASELINE (GFX fonts).
static void uiText(const String &s, int x, int y, const GFXfont *f, uint16_t col, int align = 0) {
  int16_t x1, y1; uint16_t w, h;
  display.setFont(f);
  display.getTextBounds(s, 0, 0, &x1, &y1, &w, &h);
  int sx = x;
  if (align == 1) sx = x - (int)w / 2;
  else if (align == 2) sx = x - (int)w;
  display.setTextColor(col);
  display.setCursor(sx - x1, y);
  display.print(s);
}

// Cut a string until it fits in maxW pixels.
static String uiFit(String s, const GFXfont *f, int maxW) {
  int16_t x1, y1; uint16_t w, h;
  display.setFont(f);
  display.getTextBounds(s, 0, 0, &x1, &y1, &w, &h);
  while ((int)w > maxW && s.length() > 1) {
    s.remove(s.length() - 1);
    display.getTextBounds(s, 0, 0, &x1, &y1, &w, &h);
  }
  return s;
}

// Number with 1 decimal plus a drawn degree sign (GFX fonts have no degree glyph).
// cap = digit height of the font in px.
static void uiTemp(float v, int x, int y, const GFXfont *f, int cap, int align, int dec = 1) {
  String s = String(v, dec);
  int16_t x1, y1; uint16_t w, h;
  display.setFont(f);
  display.getTextBounds(s, 0, 0, &x1, &y1, &w, &h);
  int r = cap / 9 + 1;
  int total = (int)w + 3 + 2 * r + 1;
  int sx = x;
  if (align == 1) sx = x - total / 2;
  else if (align == 2) sx = x - total;
  display.setTextColor(GxEPD_BLACK);
  display.setCursor(sx - x1, y);
  display.print(s);
  int cx = sx + (int)w + 3 + r;
  int cy = y - cap + r;
  display.drawCircle(cx, cy, r, GxEPD_BLACK);
  if (cap >= 17) display.drawCircle(cx, cy, r - 1, GxEPD_BLACK);
}

// "Label ........ value" row with a dotted line under it.
static void uiRow(const String &label, const String &val, int xl, int xr, int y) {
  uiText(label, xl, y, &FreeSans9pt7b, GxEPD_BLACK, 0);
  uiText(val, xr, y, &FreeSans9pt7b, GxEPD_BLACK, 2);
  for (int x = xl; x <= xr; x += 3) display.drawPixel(x, y + 5, GxEPD_BLACK);
}

// Inverted label like "OUTSIDE" / "ROOM".
static void uiTag(int x, int y, const char *label) {
  int16_t x1, y1; uint16_t w, h;
  display.setFont(&FreeSansBold9pt7b);
  display.getTextBounds(label, 0, 0, &x1, &y1, &w, &h);
  display.fillRect(x, y, w + 10, 17, GxEPD_BLACK);
  display.setTextColor(GxEPD_WHITE);
  display.setCursor(x + 5 - x1, y + 14);
  display.print(label);
}

static void uiHeader(const char *title, int page) {
  display.fillRect(0, 0, UI_W, UI_HDR, GxEPD_BLACK);
  uiText(title, 8, 16, &FreeSansBold9pt7b, GxEPD_WHITE, 0);
  for (int i = 0; i < 3; i++) {
    int x = UI_W - 16 - (2 - i) * 12;
    if (i == page) display.fillRect(x, 7, 8, 8, GxEPD_WHITE);
    else           display.drawRect(x, 7, 8, 8, GxEPD_WHITE);
  }
}

// "2026-11-05" -> "THU 5". Falls back to the raw string if it isn't an ISO date.
static String uiDayLabel(const String &t, bool getMonth = false) {
  int y, m, d;
  if (sscanf(t.c_str(), "%d-%d-%d", &y, &m, &d) == 3) {
    struct tm tmv = {};
    tmv.tm_year = y - 1900; tmv.tm_mon = m - 1; tmv.tm_mday = d; tmv.tm_hour = 12;
    mktime(&tmv);  // fills in tm_wday, no NTP needed
    static const char *wd[] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
    return String(wd[tmv.tm_wday]) + " " + String(d) + (!getMonth ? "" : "/" + String(m));
  }
  return t;
}

// --------------------------------------------------------------- icons ----

// WMO weather code -> icon (assumes the codes come from Open-Meteo).
static int uiKind(int code) {
  if (code == 0 || code == 1) return UI_SUN;
  if (code == 2) return UI_PART;
  if (code == 3) return UI_CLOUD;
  if (code == 45 || code == 48) return UI_FOG;
  if ((code >= 51 && code <= 67) || (code >= 80 && code <= 82)) return UI_RAIN;
  if ((code >= 71 && code <= 77) || code == 85 || code == 86) return UI_SNOW;
  if (code >= 95) return UI_STORM;
  return UI_CLOUD;
}

static void uiThickLine(int x0, int y0, int x1, int y1, int t) {
  int o = t / 2;
  for (int a = 0; a < t; a++)
    for (int b = 0; b < t; b++)
      display.drawLine(x0 + a - o, y0 + b - o, x1 + a - o, y1 + b - o, GxEPD_BLACK);
}

static void uiRing(int cx, int cy, int r, int t) {
  display.fillCircle(cx, cy, r, GxEPD_BLACK);
  if (r - t > 0) display.fillCircle(cx, cy, r - t, GxEPD_WHITE);
}

// Coordinates are on a 24x24 grid scaled by u. Cloud = filled union of shapes,
// then the same shapes shrunk by t in white, which leaves an outline.
static void uiCloud(int x, int y, float u, float ox, float oy, float k, int t) {
  float s = k * u;
  int rx = x + (int)lroundf((ox + 3 * k) * u);
  int ry = y + (int)lroundf((oy + 12 * k) * u);
  int rw = (int)lroundf(18 * s), rh = (int)lroundf(7 * s), rr = rh / 2;
  int ax = x + (int)lroundf((ox + 9 * k) * u);
  int ay = y + (int)lroundf((oy + 12 * k) * u);
  int ar = (int)lroundf(4.5f * s);
  int bx = x + (int)lroundf((ox + 14.5f * k) * u);
  int by = y + (int)lroundf((oy + 9.5f * k) * u);
  int br = (int)lroundf(6 * s);

  display.fillRoundRect(rx, ry, rw, rh, rr, GxEPD_BLACK);
  display.fillCircle(ax, ay, ar, GxEPD_BLACK);
  display.fillCircle(bx, by, br, GxEPD_BLACK);

  int ir = rr - t; if (ir < 0) ir = 0;
  display.fillRoundRect(rx + t, ry + t, rw - 2 * t, rh - 2 * t, ir, GxEPD_WHITE);
  if (ar - t > 0) display.fillCircle(ax, ay, ar - t, GxEPD_WHITE);
  if (br - t > 0) display.fillCircle(bx, by, br - t, GxEPD_WHITE);
}

static void uiSun(int x, int y, float u, float cx, float cy, float r, float ri, float ro, int t) {
  int px = x + (int)lroundf(cx * u), py = y + (int)lroundf(cy * u);
  uiRing(px, py, (int)lroundf(r * u), t);
  for (int a = 0; a < 8; a++) {
    float an = a * 0.785398f;
    uiThickLine(px + (int)lroundf(cosf(an) * ri * u), py + (int)lroundf(sinf(an) * ri * u),
                px + (int)lroundf(cosf(an) * ro * u), py + (int)lroundf(sinf(an) * ro * u), t);
  }
}

// Draws the icon for a weather code inside an s x s box with top-left (x, y).
static void uiIcon(int code, int x, int y, int s) {
  float u = s / 24.0f;
  int t = (int)lroundf(s * 1.6f / 24.0f);
  if (t < 1) t = 1;
  int dr = (int)lroundf(1.3f * u); if (dr < 1) dr = 1;

  switch (uiKind(code)) {
    case UI_SUN:
      uiSun(x, y, u, 12, 12, 4.5f, 7.3f, 10.2f, t);
      break;
    case UI_PART:
      uiSun(x, y, u, 8, 8, 3.2f, 5.2f, 7.2f, t);
      uiCloud(x, y, u, 5.5f, 5.0f, 0.75f, t);
      break;
    case UI_CLOUD:
      uiCloud(x, y, u, 0, 0, 1, t);
      break;
    case UI_RAIN:
      uiCloud(x, y, u, 1.0f, -2.5f, 0.9f, t);
      for (int i = 0; i < 3; i++) {
        float dx = 8 + i * 4;
        uiThickLine(x + (int)lroundf(dx * u), y + (int)lroundf(16.5f * u),
                    x + (int)lroundf((dx - 1) * u), y + (int)lroundf(20.5f * u), t);
      }
      break;
    case UI_SNOW:
      uiCloud(x, y, u, 1.0f, -2.5f, 0.9f, t);
      display.fillCircle(x + (int)lroundf(8 * u),  y + (int)lroundf(18 * u), dr, GxEPD_BLACK);
      display.fillCircle(x + (int)lroundf(12 * u), y + (int)lroundf(21 * u), dr, GxEPD_BLACK);
      display.fillCircle(x + (int)lroundf(16 * u), y + (int)lroundf(18 * u), dr, GxEPD_BLACK);
      break;
    case UI_STORM:
      uiCloud(x, y, u, 1.0f, -2.5f, 0.9f, t);
      uiThickLine(x + (int)lroundf(13 * u),   y + (int)lroundf(14.5f * u), x + (int)lroundf(10 * u),   y + (int)lroundf(18.5f * u), t);
      uiThickLine(x + (int)lroundf(10 * u),   y + (int)lroundf(18.5f * u), x + (int)lroundf(13.5f * u), y + (int)lroundf(18.5f * u), t);
      uiThickLine(x + (int)lroundf(13.5f * u), y + (int)lroundf(18.5f * u), x + (int)lroundf(11 * u),   y + (int)lroundf(23 * u), t);
      break;
    case UI_FOG:
      uiCloud(x, y, u, 1.0f, -2.5f, 0.9f, t);
      uiThickLine(x + (int)lroundf(6 * u), y + (int)lroundf(17.5f * u), x + (int)lroundf(18 * u), y + (int)lroundf(17.5f * u), t);
      uiThickLine(x + (int)lroundf(9 * u), y + (int)lroundf(21 * u),   x + (int)lroundf(21 * u), y + (int)lroundf(21 * u), t);
      break;
  }
}

String GetTimeString(){
  struct tm ti;
  if (!getLocalTime(&ti, 100)) return "--:--";
  char buf[6];
  strftime(buf, sizeof(buf), "%H:%M", &ti);
  return String(buf);
}

// --------------------------------------------------------------- pages ----

// Page 1: dashboard. Left: outside now + room strip. Right: next 6 hours + next 5 days.
static void uiPageNow(const currentWeather &c, const UiRoom &room) {
  
  dailyWeather today = GetDailyWeather(0);
  String title = String(UI_CITY) + " - " + GetTimeString();
  uiHeader(title.c_str(), 0);
  uiText(uiDayLabel(String(today.time), true), UI_W - 48, 16, &FreeSansBold9pt7b, GxEPD_WHITE, 2);
  display.fillRect(158, UI_HDR, 2, UI_H - UI_HDR, GxEPD_BLACK);  // column divider

  // --- left: outside now ---
  uiIcon(c.code, 8, 30, 52);
  uiTemp(c.temp, 150, 73, &FreeSansBold24pt7b, 34, 2);
  uiText(uiFit(String(GetWeatherDescription(c.code)), &FreeSansBold9pt7b, 142),
         8, 100, &FreeSansBold9pt7b, GxEPD_BLACK, 0);
  uiRow("Feels",    String(c.feels, 1) + " C",          8, 150, 121);
  uiRow("Humidity", String((int)lroundf(c.hum)) + " %", 8, 150, 141);
  uiRow("Wind",     String(c.windSpeed, 1) + " m/s",    8, 150, 161);
  uiRow("Precip",   String(c.prec, 1) + " mm",          8, 150, 181);

  // --- left: room strip ---
  uiTag(8, 193, "IN");
  uiTemp(room.temp, 42, 207, &FreeSans9pt7b, 13, 0);
  uiText(String(room.hum) + "%", 150, 207, &FreeSans9pt7b, GxEPD_BLACK, 2);
  uiText("AQI", 8, 230, &FreeSansBold9pt7b, GxEPD_BLACK, 0);
  for (int i = 0; i < 5; i++) {
    int x = 44 + i * 15;
    if (i < room.aqi) display.fillRect(x, 221, 11, 11, GxEPD_BLACK);
    else              display.drawRect(x, 221, 11, 11, GxEPD_BLACK);
  }
  uiText(String(room.eco2), 150, 230, &FreeSans9pt7b, GxEPD_BLACK, 2);

  // --- right top: next 6 hours (temp rounded, precip in mm) ---
  const int rx = 164, cw = 41;
  for (int i = 0; i < 6; i++) {
    hourlyWeather hr = GetHourlyWeather(i);
    int cx = rx + i * cw + cw / 2;
    String hh = String(hr.time);
    if (hh.length() < 2) hh = "0" + hh;
    uiText(hh, cx, 41, &FreeSansBold9pt7b, GxEPD_BLACK, 1);
    uiIcon(hr.code, cx - 12, 47, 24);
    uiTemp(hr.temp, cx, 92, &FreeSansBold12pt7b, 17, 1, 0);
    uiText(String(hr.prec, 1), cx, 110, &FreeSans9pt7b, GxEPD_BLACK, 1);
  }
  display.fillRect(160, 120, UI_W - 160, 2, GxEPD_BLACK);

  // --- right bottom: next 5 days ---
  float mn = 1e9f, mx = -1e9f;
  for (int i = 0; i < 5; i++) {
    dailyWeather d = GetDailyWeather(i);
    if (d.minTemp < mn) mn = d.minTemp;
    if (d.maxTemp > mx) mx = d.maxTemp;
  }
  float span = mx - mn; if (span < 1) span = 1;
  const int barX = 256, barW = 74;
  for (int i = 0; i < 5; i++) {
    dailyWeather d = GetDailyWeather(i);
    int ry = 124 + i * 23;
    int base = ry + 17;
    uiText(uiDayLabel(String(d.time)), 164, base, &FreeSansBold9pt7b, GxEPD_BLACK, 0);
    uiIcon(d.code, 230, ry + 1, 20);
    int bl = (int)lroundf((d.minTemp - mn) / span * (barW - 2));
    int bw = (int)lroundf((d.maxTemp - d.minTemp) / span * (barW - 2));
    if (bw < 3) bw = 3;
    display.drawRect(barX, ry + 6, barW, 7, GxEPD_BLACK);
    display.fillRect(barX + 1 + bl, ry + 6, bw, 7, GxEPD_BLACK);
    uiText(String(d.minTemp, 1) + " / " + String(d.maxTemp, 1), 412, base, &FreeSans9pt7b, GxEPD_BLACK, 2);
  }
}

// Page 2: next 12 hours as a 6 x 2 grid
static void uiPageHourly() {
  uiHeader("NEXT 12 HOURS", 1);
  const int cw = 69, ch = 109;
  for (int i = 0; i < 12; i++) {
    int col = i % 6, row = i / 6;
    int cx = col * cw + cw / 2;
    int y0 = UI_HDR + row * ch;
    hourlyWeather hr = GetHourlyWeather(i);

    String hh = String(hr.time);
    if (hh.length() < 2) hh = "0" + hh;
    uiText(hh + ":00", cx, y0 + 19, &FreeSansBold9pt7b, GxEPD_BLACK, 1);
    uiIcon(hr.code, cx - 15, y0 + 28, 30);
    uiTemp(hr.temp, cx, y0 + 78, &FreeSansBold12pt7b, 17, 1);
    uiText(String(hr.prec, 1) + " mm", cx, y0 + 98, &FreeSans9pt7b, GxEPD_BLACK, 1);
  }
  for (int k = 1; k < 6; k++) display.drawFastVLine(k * cw, UI_HDR, UI_H - UI_HDR, GxEPD_BLACK);
  display.drawFastHLine(0, UI_HDR + ch, UI_W, GxEPD_BLACK);
}

// Page 3: 7 days with a min/max range bar
static void uiPageDaily() {
  uiHeader("7 DAYS", 2);

  float mn = 1e9f, mx = -1e9f;
  for (int i = 0; i < 7; i++) {
    dailyWeather d = GetDailyWeather(i);
    if (d.minTemp < mn) mn = d.minTemp;
    if (d.maxTemp > mx) mx = d.maxTemp;
  }
  float span = mx - mn; if (span < 1) span = 1;

  const int barX = 214, barW = 88;
  for (int i = 0; i < 7; i++) {
    dailyWeather d = GetDailyWeather(i);
    int ry = UI_HDR + i * 31;
    int base = ry + 21;

    uiText(uiDayLabel(String(d.time)), 8, base, &FreeSansBold9pt7b, GxEPD_BLACK, 0);
    uiIcon(d.code, 72, ry + 4, 22);
    uiText(uiFit(String(GetWeatherDescription(d.code)), &FreeSans9pt7b, 104),
           102, base, &FreeSans9pt7b, GxEPD_BLACK, 0);

    int bl = (int)lroundf((d.minTemp - mn) / span * (barW - 2));
    int bw = (int)lroundf((d.maxTemp - d.minTemp) / span * (barW - 2));
    if (bw < 3) bw = 3;
    display.drawRect(barX, ry + 9, barW, 7, GxEPD_BLACK);
    display.fillRect(barX + 1 + bl, ry + 9, bw, 7, GxEPD_BLACK);

    uiText(String(d.minTemp, 1) + " / " + String(d.maxTemp, 1) + " C",
           408, base, &FreeSans9pt7b, GxEPD_BLACK, 2);

    if (i < 6) display.drawFastHLine(8, ry + 30, UI_W - 16, GxEPD_BLACK);
  }
}


static void uiRefresh(int page, int aqiLedPin) {
  currentWeather cur = GetCurrentWeather();

  // Read the sensors once, not on every pass of the page loop below.
  UiRoom room = {0, 0, 0, 0, 0};
  if (page == 0) {
    room.temp = (float)GetRoomTemp();
    room.hum  = (int)lroundf((float)GetRoomHumidity());
    room.eco2 = (int)GetEco2();
    room.tvoc = (int)GetTvoc();
    room.aqi  = String(GetAqi(aqiLedPin)).toInt();
    if (room.aqi < 0) room.aqi = 0;
    if (room.aqi > 5) room.aqi = 5;
  }

  display.setTextWrap(false);
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
    if (page == 0)      uiPageNow(cur, room);
    else if (page == 1) uiPageHourly();
    else                uiPageDaily();
  } while (display.nextPage());
}