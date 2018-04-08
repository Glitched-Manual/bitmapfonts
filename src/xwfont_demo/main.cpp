// xwfont_generator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#define XWFONT_RENDERER_NANOVG
#include <xwfont.h>

#define XWF_UTF16 1

int main()
{
    xeekworx::bitmapfonts::xwf_generation_config config;
    config.font_path = "C:\\Windows\\Fonts\\tahoma.ttf";
    config.font_size = 10;
    config.begin_char = 0;
    config.end_char = 1024;
    config.foreground = 0xFFFFFFFF;
    config.background = 0x00000000;
    config.border = 0x00FF20FF;
    config.border_thickness = 0;
    config.padding = 3;
    config.page_size = 1024;
    int sample_w = 1024, sample_h = 900;

    xeekworx::bitmapfonts::xwf_font * font = xeekworx::bitmapfonts::generate_font(&config);

    xeekworx::bitmapfonts::renderer font_renderer;
    font_renderer.setup_testenv(sample_w, sample_h, false, NULL, 0);

    if (!font) return -1;
    else {
        xeekworx::bitmapfonts::set_sample_background(0x333333FF);

        //std::wstring sample_text = "?>Friends象形字";

        //std::string sample_text = "Hello World!";

#ifdef XWF_UTF16
        std::wstring sample_text = 
            L"Friends, Romans, countrymen, lend me your ears;\n"
            L"I come to bury Caesar, not to praise him.\n"
            L"The evil that men do lives after them;\n"
            L"The good is oft interred with their bones;\n"
            L"So let it be with Caesar.The noble Brutus\n"
            L"Hath told you Caesar was ambitious :\n"
            L"If it were so, it was a grievous fault,\n"
            L"And grievously hath Caesar answer’d it.\n"
            L"Here, under leave of Brutus and the rest–\n"
            L"For Brutus is an honourable man;\n"
            L"So are they all, all honourable men–\n"
            L"Come I to speak in Caesar’s funeral.\n"
            L"He was my friend, faithful and just to me :\n"
            L"But Brutus says he was ambitious;\n"
            L"And Brutus is an honourable man.\n"
            L"He hath brought many captives home to Rome\n"
            L"Whose ransoms did the general coffers fill :\n"
            L"Did this in Caesar seem ambitious ?\n"
            L"When that the poor have cried, Caesar hath wept : \n"
            L"Ambition should be made of sterner stuff :\n"
            L"Yet Brutus says he was ambitious;\n"
            L"And Brutus is an honourable man.\n"
            L"You all did see that on the Lupercal\n"
            L"I thrice presented him a kingly crown,\n"
            L"Which he did thrice refuse : was this ambition ?\n"
            L"Yet Brutus says he was ambitious;\n"
            L"And, sure, he is an honourable man.\n"
            L"I speak not to disprove what Brutus spoke,\n"
            L"But here I am to speak what I do know.\n"
            L"You all did love him once, not without cause :\n"
            L"What cause withholds you then, to mourn for him ?\n"
            L"O judgment!thou art fled to brutish beasts,\n"
            L"And men have lost their reason.Bear with me;\n"
            L"My heart is in the coffin there with Caesar,\n"
            L"And I must pause till it come back to me.\n"
            ;
#else
        std::string sample_text =
            "Friends, Romans, countrymen, lend me your ears;\n"
            "I come to bury Caesar, not to praise him.\n"
            "The evil that men do lives after them;\n"
            "The good is oft interred with their bones;\n"
            "So let it be with Caesar.The noble Brutus\n"
            "Hath told you Caesar was ambitious :\n"
            "If it were so, it was a grievous fault,\n"
            "And grievously hath Caesar answer’d it.\n"
            "Here, under leave of Brutus and the rest–\n"
            "For Brutus is an honourable man;\n"
            "So are they all, all honourable men–\n"
            "Come I to speak in Caesar’s funeral.\n"
            "He was my friend, faithful and just to me :\n"
            "But Brutus says he was ambitious;\n"
            "And Brutus is an honourable man.\n"
            "He hath brought many captives home to Rome\n"
            "Whose ransoms did the general coffers fill :\n"
            "Did this in Caesar seem ambitious ?\n"
            "When that the poor have cried, Caesar hath wept : \n"
            "Ambition should be made of sterner stuff :\n"
            "Yet Brutus says he was ambitious;\n"
            "And Brutus is an honourable man.\n"
            "You all did see that on the Lupercal\n"
            "I thrice presented him a kingly crown,\n"
            "Which he did thrice refuse : was this ambition ?\n"
            "Yet Brutus says he was ambitious;\n"
            "And, sure, he is an honourable man.\n"
            "I speak not to disprove what Brutus spoke,\n"
            "But here I am to speak what I do know.\n"
            "You all did love him once, not without cause :\n"
            "What cause withholds you then, to mourn for him ?\n"
            "O judgment!thou art fled to brutish beasts,\n"
            "And men have lost their reason.Bear with me;\n"
            "My heart is in the coffin there with Caesar,\n"
            "And I must pause till it come back to me.\n"
            ;
#endif

       // xeekworx::bitmapfonts::measure_sample_utf16(font, sample_text.c_str(), -1, &sample_w, &sample_h);
        //xeekworx::bitmapfonts::render_sample_utf16(font, sample_text.c_str(), -1, sample_w, sample_h, 8);
        font_renderer.set_font(font);
        font_renderer.set_background(0x333333FF);
        font_renderer.set_foreground(0xFF0000FF);
        

        while (font_renderer.poll_testenv()) {
            font_renderer.nanovg_begin();
            font_renderer.draw(sample_text.c_str(), -1, 150, 0, 300, 500, 25);
            font_renderer.nanovg_end();
        }

        xeekworx::bitmapfonts::delete_font(font);
    }

    return 0;
}

