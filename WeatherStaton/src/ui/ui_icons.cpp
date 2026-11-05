#define UI_W   416
#define UI_H   240
#define UI_HDR 22

#ifndef UI_CITY
#define UI_CITY "AALBORG"

// UI select icon
enum { UI_SUN, UI_PART, UI_CLOUD, UI_RAIN, UI_SNOW, UI_STORM, UI_FOG };

// Get matching icon from weather code
/* ------------TO DO------------ */
static int uiKind(int code) {
  if () return UI_SUN;
  if () return UI_PART;
  if () return UI_CLOUD;
  if () return UI_FOG;
  if () return UI_RAIN;
  if () return UI_SNOW;
  if () return UI_STORM;
  return UI_CLOUD;
}

static void DrawUiIcon(int code, int x, int y, int s) {
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

// Sun
static void uiSun(int x, int y, float u, float cx, float cy, float r, float ri, float ro, int t) {
  int px = x + (int)lroundf(cx * u), py = y + (int)lroundf(cy * u);
  uiRing(px, py, (int)lroundf(r * u), t);
  for (int a = 0; a < 8; a++) {
    float an = a * 0.785398f;
    uiThickLine(px + (int)lroundf(cosf(an) * ri * u), py + (int)lroundf(sinf(an) * ri * u),
                px + (int)lroundf(cosf(an) * ro * u), py + (int)lroundf(sinf(an) * ro * u), t);
  }
}

// Clouds
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

// Line
static void uiThickLine(int x0, int y0, int x1, int y1, int t) {
  int o = t / 2;
  for (int a = 0; a < t; a++)
    for (int b = 0; b < t; b++)
      display.drawLine(x0 + a - o, y0 + b - o, x1 + a - o, y1 + b - o, GxEPD_BLACK);
}

// Ring
static void uiRing(int cx, int cy, int r, int t) {
  display.fillCircle(cx, cy, r, GxEPD_BLACK);
  if (r - t > 0) display.fillCircle(cx, cy, r - t, GxEPD_WHITE);
}