#include <SFML/Graphics.hpp>

class Mandelbrot final
{   
    public:
        Mandelbrot (unsigned int width, unsigned int heigth);
        ~Mandelbrot();
        const sf::Sprite& GetBgSprite ();
        void DrawMandelbrotSet ();
        void AddXShift (double delta);
        void AddYShift (double delta);
        void AddScale (double delta);

    private:
        double CountDistanceSq (double x, double y);
        void SetPixelColor (int cur_pixel, uint8_t R, uint8_t G, uint8_t B, uint8_t A);
        unsigned int width;
        unsigned int heigth;
        uint32_t* pixels_colors;
        const int max_iterations = 128;
        const double max_distance_sq = 16;
        double x_shift = -0.4;
        double y_shift = -0.30;
        double scale = 0.23;
        sf::Texture bg_texture;
        sf::Sprite bg_sprite;


};
