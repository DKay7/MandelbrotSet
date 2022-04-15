#include "mandelbrot.h"
#include <immintrin.h>
#include <x86intrin.h>

//flexxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

Mandelbrot::Mandelbrot (unsigned int width, unsigned int heigth): bg_texture(), bg_sprite()
{   
    this->width = width;
    this->heigth = heigth;
    pixels_colors = new uint32_t[width * heigth]; 
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
    for (unsigned int dy = 0; dy < heigth; dy++)
    {   
        double y0_double = ((double) dy / heigth + y_shift) * 2 / scale;
        for (unsigned int dx = 0; dx < width; dx+=4)
        {   
            __m256d x0 = _mm256_set_pd (3, 2, 1, 0);            // offsets
            __m256d y0 = _mm256_set1_pd (y0_double);

            x0 = _mm256_add_pd (x0, _mm256_set1_pd (dx));
            x0 = _mm256_div_pd (x0, _mm256_set1_pd (width));
            x0 = _mm256_add_pd (x0, _mm256_set1_pd (x_shift));
            x0 = _mm256_mul_pd (x0, _mm256_set1_pd (3.0));
            x0 = _mm256_div_pd (x0, _mm256_set1_pd (scale));
 
            __m256d x = x0;
            __m256d y = y0;
            __m256d colored = _mm256_set1_pd (0);

            int n = 0;
            for (int i_pixel = 0; i_pixel < 4; i_pixel++)
                SetPixelColor (dy * width + dx + i_pixel, 0, 0, 0, 0xFF);

            for (; n < max_iterations; n++)
            {   
                __m256d x_sq = _mm256_mul_pd (x, x);
                __m256d y_sq = _mm256_mul_pd (y, y);
                __m256d xy   = _mm256_mul_pd (x, y);
                x = _mm256_add_pd(_mm256_sub_pd(x_sq, y_sq), x0);
                y = _mm256_add_pd(_mm256_add_pd(xy, xy), y0);

                __m256d r2 = _mm256_add_pd(x_sq, y_sq);
                __m256d cmp = _mm256_cmp_pd(r2, _mm256_set1_pd(max_distance_sq), _CMP_GT_OQ);
                
                if (_mm256_movemask_pd(colored) == 0xFFFF)
                {
                    break;
                }

                cmp = _mm256_andnot_pd (colored, cmp);
                
                for (int i_cmp = 0; i_cmp < 4; i_cmp++)
                {   
                    
                    if (*((double*)&cmp + i_cmp))
                    {   
                        SetPixelColor(dy * width + dx + i_cmp, 7 * n, 128 - 7 * n, 255 - 7 * n, 0xFF);
                    }
                }

                colored = _mm256_or_pd (colored, cmp);
                
            }
        }
    }  
    
    bg_texture.update ((uint8_t*)pixels_colors);
    return;
}

double Mandelbrot::CountDistanceSq (double x, double y)
{
    return x * x + y * y;
}

void Mandelbrot::SetPixelColor (int cur_pixel, uint8_t R, uint8_t G, uint8_t B, uint8_t A)
{

    pixels_colors[cur_pixel] = 0x1000000 * A + 0x10000 * B + 0x100 * G + R;
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

