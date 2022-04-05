#include <SFML/Graphics.hpp>

//flexxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

class Fps
{   
    public:
        Fps ();
        void UpdateFps ();
        char* GetFpsString ();
        const sf::Text& GetFpsText ();
        void SetFpsFormat (const char* fps_format);

    private:
        sf::Font fps_font;
        sf::Text fps_text;
        sf::Clock clock;

        const char* fps_string_format;
        char fps_string[32];
        double prev_time;
        double curr_time;
        std::vector<double> fps_values;
        int last_index;
};