#include "mandelbrot.h"

//flexxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

Mandelbrot::Mandelbrot (unsigned int width, unsigned int heigth): bg_texture(), bg_sprite()
{   
    this->width = width;
    this->heigth = heigth;
    pixels_colors = new uint8_t[width * heigth * 4]; 
    bg_texture.create (width, heigth);
    bg_sprite.setTextureRect (sf::IntRect(0, 0, width, heigth));
    bg_sprite.setTexture (bg_texture, false);
}

//flexxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

const sf::Sprite& Mandelbrot::GetBgSprite ()
{
    return bg_sprite;
}

//flexxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

void Mandelbrot::DrawMandelbrotSet()
{   
    for (unsigned int dx = 0; dx < width; dx++)
    {
        for (unsigned int dy = 0; dy < heigth; dy++)
        {
            double x0 = ((double) dx / width  + x_shift) * 3 / scale; 
            double y0 = ((double) dy / heigth + y_shift) * 2 / scale;
            double x = x0;
            double y = y0;
            int n = 0;

            for (; n < max_iterations && CountDistanceSq (x, y) < max_distance_sq; n++)
            {   
                double old_x = x;
                double old_y = y;
                x = (old_x * old_x) - (old_y * old_y) + x0;
                y = 2 * old_x * old_y + y0;
            }

            int cur_pixel = 4 * (dy * width + dx);

            if (n == max_iterations)
                SetPixelColor (cur_pixel, 0, 0, 0, 0xFF);
            else
            {   
                
                int red     = n;
                int green   = 7 * n;
                int blue    = 128 + n;
                int alpha   = 0xFF;

                // TODO delete while optimizations
                for (int i = 1; i < max_iterations; i++)
                {
                    if (n % i == 0)
                    {
                        red     = 255 - red   * i;
                        green   = 255 - green * i;
                        blue    = 255 - blue  * i;
                    }
                }

                SetPixelColor (cur_pixel, red, green, blue, alpha);
            }
        }
    }

    bg_texture.update (pixels_colors);
    return;
}

double Mandelbrot::CountDistanceSq (double x, double y)
{
    return x * x + y * y;
}

void Mandelbrot::SetPixelColor (int cur_pixel, int R, int G, int B, int A)
{
    pixels_colors[cur_pixel] = R;
    pixels_colors[cur_pixel + 1] = G;
    pixels_colors[cur_pixel + 2] = B;
    pixels_colors[cur_pixel + 3] = A;

    return;
}

void Mandelbrot::AddXShift (double delta)
{
    x_shift += delta;
    return;
}

void Mandelbrot::AddYShift (double delta)
{
    y_shift += delta;
    return;
}

void Mandelbrot::AddScale (double delta)
{
    scale   *= delta;
    x_shift *= delta;
    y_shift *= delta;
    return;
}

//flexxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

Mandelbrot::~Mandelbrot()
{
    delete[] pixels_colors;
}

//flexxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

