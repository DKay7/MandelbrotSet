#include "mandelbrot.h"
#include <immintrin.h>

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
    unsigned int cur_pixel = 0;
    for (unsigned int dy = 0; dy < heigth; dy++)
    {   
        double y0_double = ((double) dy / heigth + y_shift) * 2 / scale;

        for (unsigned int dx = 0; dx < width; dx+=4)
        {   
            // double x0 = ((double) dx / width  + x_shift) * 3 / scale; 
            __m256d x0 = _mm256_set_pd (0, 1, 2, 3);            // offsets
            __m256d y0 = _mm256_set1_pd (y0_double);

            x0 = _mm256_add_pd (x0, _mm256_set1_pd (dx));
            x0 = _mm256_div_pd (x0, _mm256_set1_pd (width));
            x0 = _mm256_add_pd (x0, _mm256_set1_pd (x_shift));
            x0 = _mm256_mul_pd (x0, _mm256_set1_pd (3));
            x0 = _mm256_div_pd (x0, _mm256_set1_pd (scale));

            // double x = x0;
            // double y = y0;
            __m256d x = x0;
            __m256d y = y0;
            __m256d x_sq = _mm256_mul_pd (x, x);
            __m256d y_sq = _mm256_mul_pd (y, y);

            __m256d was_all_colored = _mm256_set1_pd (0);
            int n = 0;
            
            for (; n < max_iterations && ; n++)
            {   
                x = _mm256_add_pd(_mm256_sub_pd (x_sq, y_sq), x0);
                y = _mm256_fmadd_pd(_mm256_set1_pd (2.0), _mm256_mul_pd(x, y), y0);

                x_sq = _mm256_mul_pd (x, x);
                y_sq = _mm256_mul_pd (y, y);

                __m256d cmp = _mm256_cmp_pd (_mm256_add_pd (x_sq, y_sq), _mm256_set1_pd(max_distance_sq), _CMP_GT_OQ);
                // printf ("%llX\n", *(long long*)&cmp);


                for (int i = 0; i < 4; i++)
                {   
                    // int cur_pixel = 4 * (dy * width + dx + i);
                    if (*((long long*)&cmp + i))
                    {   
                        int red     = n;
                        int green   = 7 * n;
                        int blue    = 128 + n;
                        int alpha   = 0xFF;
                        
                        SetPixelColor (cur_pixel + (3 - i), red, green, blue, alpha);
                    }
                    else
                        SetPixelColor (cur_pixel + (3 - i), 128, 0, 0, 0xFF);
                }

                was_all_colored = _mm256_or_pd (was_all_colored, cmp);
            }
            cur_pixel += 4;
                        
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

