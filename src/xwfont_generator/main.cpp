// xwfont_generator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int main()
{
    xeekworx::bitmap_fonts::xwf_generation_config config;
    config.font_path = "C:\\Windows\\Fonts\\ariali.ttf";
    config.font_size = 32;
    config.begin_char = 0;
    config.end_char = 255;
    config.foreground = 0xFF0000FF;
    config.background = 0x00000000;
    config.border = 0x00FF00FF;
    //config.border_thickness = 1;
    //config.padding = 3;
    //config.page_size = 1024;

    xeekworx::bitmap_fonts::xwf_font * font = xeekworx::bitmap_fonts::generate_font(&config);
    if (!font) return -1;
    else {
        xeekworx::bitmap_fonts::generate_sample(font, "MWll!", -1, 300, 100);

        xeekworx::bitmap_fonts::delete_font(font);
    }

    return 0;
}

