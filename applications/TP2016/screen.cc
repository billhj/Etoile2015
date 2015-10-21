#include "screen.h"


void Screen :: _line(SDL_Surface *surf,
                     int x0, int y0, int x1, int y1,
                     Uint32 color)
{
  x0 = std::max(x0, 0); x0 = std::min(x0, surf->w - 1);
  x1 = std::max(x1, 0); x1 = std::min(x1, surf->w - 1);
  y0 = std::max(y0, 0); y0 = std::min(y0, surf->h - 1);
  y1 = std::max(y1, 0); y1 = std::min(y1, surf->h - 1);
  int dy = y1 - y0;
  int dx = x1 - x0;
  int stepx, stepy;

  if (dy < 0) { dy = -dy;  stepy = -1; } else { stepy = 1; }
  if (dx < 0) { dx = -dx;  stepx = -1; } else { stepx = 1; }
  dy <<= 1; // dy is now 2*dy
  dx <<= 1; // dx is now 2*dx

  _put_pixel(surf, color, x0, y0);
  if (dx > dy)
  {
    int fraction = dy - (dx >> 1); // same as 2*dy - dx
    while (x0 != x1)
    {
      if (fraction >= 0)
      {
        y0 += stepy;
        fraction -= dx; // -= 2*dx
      }
      x0 += stepx;
      fraction += dy; //  -= 2*dy
      _put_pixel(surf, color, x0, y0);
    }
  }
  else
  {
    int fraction = dx - (dy >> 1);
    while (y0 != y1)
    {
      if (fraction >= 0)
      {
        x0 += stepx;
        fraction -= dy;
      }
      y0 += stepy;
      fraction += dx;
      _put_pixel(surf, color, x0, y0);
    }
  }
}

void Screen :: _rect(SDL_Surface *surf, int x, int y, int w, int h, Uint32 color)
{
  SDL_Rect r = {x - w / 2, y - h / 2, w, h};
  SDL_FillRect(_screen, &r, color);
}

void Screen :: _disc(SDL_Surface *surf,
                     int x_center, int y_center, int radius,
                     Uint32 color)
{
  int f = 1 - radius;
  int ddF_x = 1;
  int ddF_y = -2 * radius;
  int x = 0;
  int y = radius;
  int x0 = x_center;
  int y0 = y_center;

  _line(surf, x0, y0 - radius, x0, y0 + radius, color);
  _line(surf, x0 - radius, y0, x0 + radius, y0, color);
  while (x < y)
  {
     // ddF_x == 2 * x + 1;
     // ddF_y == -2 * y;
     // f == x*x + y*y - radius*radius + 2*x - y + 1;
    if (f >= 0)
    {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
    _line(surf, x0 - x, y0 + y, x0 + x, y0 + y, color);
    _line(surf, x0 - x, y0 - y, x0 + x, y0 - y, color);
    _line(surf, x0 - y, y0 + x, x0 + y, y0 + x, color);
    _line(surf, x0 - y, y0 - x, x0 + y, y0 - x, color);
  }

}

