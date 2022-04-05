#include "fps.h"
#include <string.h>
#include <numeric>

//flexxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

Fps::Fps ()
{   
    fps_string_format = "fps: %.3lg";
    curr_time = 0;
    last_index = 0;
    fps_values.resize (4096);

    prev_time = clock.getElapsedTime().asSeconds();
    
    if (!fps_font.loadFromFile ("data/fonts/base_font.ttf"))
    {
        fprintf (stderr, "Can not load font!\n");
        return;
    }

    fps_text.setFont (fps_font);
    fps_text.setCharacterSize (32); 
    fps_text.setFillColor (sf::Color::Cyan);
}

//flexxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

void Fps::UpdateFps ()
{
    curr_time = clock.getElapsedTime ().asSeconds();

    double fps = 1 / (curr_time - prev_time);
    
    fps_values[last_index++ % fps_values.size()] = fps;

    fps = std::accumulate (fps_values.begin(), fps_values.end(), 0.0) / fps_values.size(); 

    sprintf (fps_string, fps_string_format, fps);
    prev_time = curr_time;

    fps_text.setString (fps_string);

    return;
}

//flexxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

char* Fps::GetFpsString ()
{
    return fps_string;
}

const sf::Text& Fps::GetFpsText ()
{
    return fps_text;
}

//flexxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

void Fps::SetFpsFormat (const char* fps_format)
{
    fps_string_format = fps_format;
    return;
}