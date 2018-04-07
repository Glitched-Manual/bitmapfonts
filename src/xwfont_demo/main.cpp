// xwfont_generator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int main()
{
    xeekworx::bitmapfonts::xwf_generation_config config;
    config.font_path = "C:\\Windows\\Fonts\\tahoma.ttf";
    config.font_size = 10;
    config.begin_char = 0;
    config.end_char = 1024;
    config.foreground = 0x000000FF;
    config.background = 0xFFFFFFFF;
    config.border = 0x00FF20FF;
    config.border_thickness = 0;
    config.padding = 3;
    config.page_size = 1024;
    int sample_w = 800, sample_h = 100;

    xeekworx::bitmapfonts::xwf_font * font = xeekworx::bitmapfonts::generate_font(&config);

    xeekworx::bitmapfonts::renderer font_renderer;
    font_renderer.setup_testenv(sample_w, sample_h, false, NULL, 0);

    if (!font) return -1;
    else {
        xeekworx::bitmapfonts::set_sample_background(0x333333FF);

        //std::wstring sample_text = "?>Friends象形字";

        std::string sample_text = "Hello World!";

        //std::string sample_text = 
        //    "Friends, Romans, countrymen, lend me your ears;\n"
        //    "I come to bury Caesar, not to praise him.\n"
        //    "The evil that men do lives after them;\n"
        //    "The good is oft interred with their bones;\n"
        //    "So let it be with Caesar.The noble Brutus\n"
        //    "Hath told you Caesar was ambitious :\n"
        //    "If it were so, it was a grievous fault,\n"
        //    "And grievously hath Caesar answer’d it.\n"
        //    "Here, under leave of Brutus and the rest–\n"
        //    "For Brutus is an honourable man;\n"
        //    "So are they all, all honourable men–\n"
        //    "Come I to speak in Caesar’s funeral.\n"
        //    "He was my friend, faithful and just to me :\n"
        //    "But Brutus says he was ambitious;\n"
        //    "And Brutus is an honourable man.\n"
        //    "He hath brought many captives home to Rome\n"
        //    "Whose ransoms did the general coffers fill :\n"
        //    "Did this in Caesar seem ambitious ?\n"
        //    "When that the poor have cried, Caesar hath wept : \n"
        //    "Ambition should be made of sterner stuff :\n"
        //    "Yet Brutus says he was ambitious;\n"
        //    "And Brutus is an honourable man.\n"
        //    "You all did see that on the Lupercal\n"
        //    "I thrice presented him a kingly crown,\n"
        //    "Which he did thrice refuse : was this ambition ?\n"
        //    "Yet Brutus says he was ambitious;\n"
        //    "And, sure, he is an honourable man.\n"
        //    "I speak not to disprove what Brutus spoke,\n"
        //    "But here I am to speak what I do know.\n"
        //    "You all did love him once, not without cause :\n"
        //    "What cause withholds you then, to mourn for him ?\n"
        //    "O judgment!thou art fled to brutish beasts,\n"
        //    "And men have lost their reason.Bear with me;\n"
        //    "My heart is in the coffin there with Caesar,\n"
        //    "And I must pause till it come back to me.\n"
        //    ;

        

       // xeekworx::bitmapfonts::measure_sample_utf16(font, sample_text.c_str(), -1, &sample_w, &sample_h);
        //xeekworx::bitmapfonts::render_sample_utf16(font, sample_text.c_str(), -1, sample_w, sample_h, 8);
        font_renderer.set_font(font);
        

        while (font_renderer.poll_testenv()) {
            font_renderer.draw(sample_text.c_str(), -1, 0, 0, sample_w, sample_h, 1);
        }

        xeekworx::bitmapfonts::delete_font(font);
    }

    return 0;
}

