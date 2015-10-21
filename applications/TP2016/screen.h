#ifndef SCREEN_HPP_
#define SCREEN_HPP_

#include "SDL.h"
#include <iostream>
#include <cassert>

class Screen
{
  public:
    /**
       Constructor of Screen  open a SDL window
       @param w width of the window (e.g. 500)
       @param h height of the window (e.g. 500)
     */
    Screen(int w, int h) : _w(w), _h(h)
    {
      if (SDL_Init(SDL_INIT_VIDEO) < 0)
        std::cerr << "error init sdl " << std::endl;
      _screen = SDL_SetVideoMode(w, h, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
      assert(_screen);
    }

     /// @return the width of the screen
    int w() const { return _w; }

     /// @return the height of the screen
    int h() const { return _h; }

    /** draw a disc
       @param x x-coordinate of the center (in pixels)
       @param y y-coordinate of the center (in pixels)
       @param radius (in pixels)
       @param color 0xRRGGBBAA (default: red)
     */
    void disc(int x, int y, int radius, Uint32 color = 0xFF0000)
    {
      _disc(_screen, x, y, radius, color);
    }

	void disc(int x, int y, int radius, float r, float g, float b)
    {
		Uint32 color = SDL_MapRGB(_screen->format, r, g, b);
        disc(x, y, radius, color);
    }

    /** draw a filled rectangle
       @param x x-coordinate of the center (in pixels)
       @param y y-coordinate of the center (in pixels)
       @param w width of the rectangle (in pixels)
       @param h height of the rectangle (in pixels)
       @param color 0xRRGGBBAA (default: blue)
     */
    void rect(int x, int y, int w, int h, Uint32 color = 0xFF)
    {
      _rect(_screen, x, y, w, h, color);
    }

	void rect(int x, int y, int w, int h, float r, float g, float b)
    {
		Uint32 color = SDL_MapRGB(_screen->format, r, g, b);
        _rect(_screen, x, y, w, h, color);
    }

    /** draw a line
       @param x0 starting x-coordinate (in pixels)
       @param y0 starting y-coordinate (in pixels)
       @param x1 ending x-coordinate (in pixels)
       @param y1 ending y-coordinate (in pixels)
       @param color 0xRRGGBBAA (default: red)
     */
    void line(int x0, int y0, int x1, int y1, Uint32 color = 0xFF0000)
    {
      _line(_screen, x0, y0, x1, y1, color);
    }

    /*
       Flip the screen: swap buffer1 and buffer2; in effect, update the screen to
       take the drawings into account
     */
    void flip()
    {
      SDL_Rect r = {0, 0, _w, _h};
      SDL_Flip(_screen);
      SDL_FillRect(_screen, &r, 0xFFFFFF);
      SDL_Event event;
      while (SDL_PollEvent(&event))
        if (event.type == SDL_QUIT)
          exit(0);
    }
  protected:
    SDL_Surface *_screen;
    int _w, _h;
    void _disc(SDL_Surface *surf,
               int x_center, int y_center, int radius,
               Uint32 color);
    void _rect(SDL_Surface *surf,
               int x_center, int y_center, int w, int h,
               Uint32 color);

    void _line(SDL_Surface *surf,
               int x0, int y0, int x1, int y1,
               Uint32 color);
    void _put_pixel(SDL_Surface*surf,
                    Uint32 color, int x, int y)
    {
      if (x >= surf->w || x < 0 || y >= surf->h || y < 0)
        return;
      Uint32 *bufp = (Uint32 *)surf->pixels + y * surf->pitch / 4 + x;
      *bufp = color;
    }
    void _put_pixel(SDL_Surface*surf, int x, int y,
                    Uint8 r, Uint8 g, Uint8 b)
    {  _put_pixel(surf, SDL_MapRGB(surf->format, r, g, b), x, y); }
    ;
};


#endif
