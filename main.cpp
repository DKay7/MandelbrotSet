#include "mandelbrot.h"
#include "fps.h"
#include "SFML/Graphics.hpp"

int main (void)
{   
    const unsigned int width    = 810;
    const unsigned int heigth   = 540;
    const double x_shift_delta  = 0.1;
    const double y_shift_delta  = 0.1;
    const double scale_delta    = 1.1;

    sf::RenderWindow window(sf::VideoMode(width, heigth), "Mandelbrot");
    sf::Event event;
    Fps fps;
    Mandelbrot mandelbrot(width, heigth);

    while (window.isOpen())
    {
        
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) window.close();
			if (event.type == sf::Event::KeyPressed)
			{
                switch (event.key.code)
                {
                    case sf::Keyboard::Escape:
                        window.close();
                        break;

                    case sf::Keyboard::Left:
                        mandelbrot.AddXShift (-x_shift_delta);
                        break;
                    
                    case sf::Keyboard::Right:
                        mandelbrot.AddXShift (x_shift_delta);
                        break;
                    
                    case sf::Keyboard::Up:
                        mandelbrot.AddYShift (-y_shift_delta);
                        break;
                    
                    case sf::Keyboard::Down:
                        mandelbrot.AddYShift (y_shift_delta);
                        break;

                    case sf::Keyboard::Add:
                        mandelbrot.AddScale (scale_delta);
                        break;

                    case sf::Keyboard::Subtract:
                        mandelbrot.AddScale (1 / scale_delta);
                        break;
                    default:
                        break;
                }
			}
		}

        fps.UpdateFps ();
        mandelbrot.DrawMandelbrotSet ();

        window.clear();
        window.draw (mandelbrot.GetBgSprite ());
        window.draw (fps.GetFpsText());
        window.display();
    }

    return 0;
}