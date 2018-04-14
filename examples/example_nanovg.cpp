// XEEKWORX BITMAPFONTS EXAMPLE USING THE NANOVG FONT RENDERER
// Updated: 2018, Apr 04
//
// The only code you really need to pay attention to is the include for the 
// font library and a majority of what is in main(). This example is not 
// designed to teach basic SDL, OpenGL, or NanoVG concepts.
//
// The MIT License (MIT)
// Copyright (C) 2018 John A. Tullos. All rights reserved.
// Website: https://github.com/xeekworx
// Author E-mail: xeek@xeekworx.com
//
// Permission is hereby granted, free of charge, to any person obtaining a 
// copy of this software and associated documentation files(the "Software"), 
// to deal in the Software without restriction, including without limitation 
// the rights to use, copy, modify, merge, publish, distribute, sublicense, 
// and / or sell copies of the Software, and to permit persons to whom the 
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in 
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE 
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
// DEALINGS IN THE SOFTWARE.

#include <iostream>
#include <string>
#include <stdint.h>
#include <SDL.h>
#include <glad\glad.h>
#define NANOVG_GL3_IMPLEMENTATION
#include <nanovg.h>
#include <nanovg_gl.h>

#define XWFONT_RENDERER_NANOVG  // You must define this to use the nanovg font 
#include <xwfont.h>             // renderer before including "xwfont.h".

struct example_context {
    //  The width & height of the example's main window:
    const int width = 1024;
    const int height = 768;

    // The colors used in this example (not including the generated font's
    // colors, those are hard-coded below):
    const xeekworx::bitmapfonts::color clear_color = 0xFFFFFFFF;
    const xeekworx::bitmapfonts::color background = 0xFFFFFFFF;
    const xeekworx::bitmapfonts::color foreground = 0x000000FF;

    // The font object used in rendering and the other objects from the API
    // this example's font renderer uses.
    xeekworx::bitmapfonts::xwf_font * font = nullptr;
    SDL_Window * window = nullptr;
    SDL_GLContext gl_context = nullptr;
    NVGcontext * nvg_context = nullptr;

    // NanoVG needs these values for its nvgBeginFrame(). I calculate and set 
    // this in startup().
    float px_ratio = 0.f;
};

// You don't need to pay attention to any of these functions. These functions 
// facilitate the creation of the bitmap font and startup & shutdown of the 
// apis this example's font renderer uses.
bool generate_font(example_context& c);
bool startup(example_context& c);
bool poll_events(example_context& c);
void shutdown(example_context& c);


int main(int argc, char *argv[]) // SDL strictly requires this main signature!
{
    example_context context;
    int result = 0;

    // STARTUP
    // Generate the font and initialize the APIs that are needed for the font
    // renderer used in this example.
    // ------------------------------------------------------------------------
    try 
    {
        if (!generate_font(context)) throw("Failed to generate font!");
        if (!startup(context)) throw("Failed to startup!");
    }
    catch (const std::string& e) 
    {
        std::cout << e << std::endl;
        result = 1;
    }

    // FONT RENDERER SETUP
    // Create & configure this example's font renderer.
    // ------------------------------------------------------------------------
    std::string sample =
        "Computer programming can be a hassle\n"
        "It's like trying to take a defended castle\n"
        "When the program runs you shout with glee\n"
        "But when you get errors you cry with plea\n\n"

        "There are programming languages like C++\n"
        "But then there are others that cause a fuss\n"
        "At the end of the day you may have something done\n"
        "But by the next day you restart all the fun\n";

    xeekworx::bitmapfonts::renderer_nanovg font_renderer(context.nvg_context);
    font_renderer.set_font(context.font);
    font_renderer.set_background(context.background);
    font_renderer.set_foreground(context.foreground);

    // RENDER
    // Draw the font on the screen using this example's font renderer.
    // Note: Clearing and swapping of the frame buffer is done within the 
    // poll_events() function.
    // ------------------------------------------------------------------------
    while (poll_events(context)) {
        nvgBeginFrame(context.nvg_context, context.width, context.height, context.px_ratio);

        font_renderer.draw(sample.c_str(), -1, 150, 100, 300, 500, 25);

        nvgEndFrame(context.nvg_context);
    }

    // SHUTDOWN
    // Unset the selected font renderer's font so that it can clean-up before 
    // the shutdown() function actually destroys the font and obvoiusly, 
    // shutdown() uninitializes all of the APIs we used for this example.
    // ------------------------------------------------------------------------
    font_renderer.set_font(nullptr);
    shutdown(context);
    return result;
}


bool generate_font(example_context& c)
{
    xeekworx::bitmapfonts::xwf_generation_config config;
    config.font_path = "C:\\Windows\\Fonts\\tahoma.ttf";
    config.font_size = 10;
    config.begin_char = 0;
    config.end_char = 512;
    config.foreground = 0xFFFFFFFF;
    config.background = 0x00000000;
    config.border = 0x00FF20FF;
    config.border_thickness = 0;
    config.padding = 0;
    config.page_size = 1024;
    int sample_w = 1024, sample_h = 900;

    c.font = xeekworx::bitmapfonts::generate_font(&config);
    return c.font != nullptr;
}

bool startup(example_context& c)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
        std::cout << SDL_GetError() << std::endl;
        return false;
    }

    // Configure OpenGL Attributes:
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    if ((c.window = SDL_CreateWindow(
        "Xeekworx Bitmapfonts: NanoVG Example",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        c.width, c.height,
        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL
    )) == 0) {
        std::cout << SDL_GetError() << std::endl;
        return false;
    }

    // Create OpenGL Context:
    if (NULL == (c.gl_context = SDL_GL_CreateContext(c.window))) {
        std::cout << SDL_GetError() << std::endl;
        return false;
    }
    else SDL_GL_MakeCurrent(c.window, c.gl_context);

    // Initialize GLAD & load modern OpenGL functions:
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    // Create NanoVG Context:
    if (NULL == (c.nvg_context = nvgCreateGL3(NVG_STENCIL_STROKES | NVG_ANTIALIAS | NVG_DEBUG))) {
        std::cout << "Failed to create NanoVG context" << std::endl;
        return false;
    }

    // Calculate Pixel Ratio for hi-dpi devices:
    // This is for nvgBeginFrame used when rendering.
    int win_width = 0, win_height = 0, fb_width = 0, fb_height = 0;
    SDL_GetWindowSize(c.window, &win_width, &win_height);
    SDL_GL_GetDrawableSize(c.window, &fb_width, &fb_height);
    c.px_ratio = (float)fb_width / (float)win_width;

    // Setup the Viewport:
    glViewport(0, 0, c.width, c.height);

    // Enable Blending:
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Clear the backbuffer so it's ready for the first frame:
    glClearColor(
        c.clear_color.r != 0 ? (float)c.clear_color.r / 255.f : 0.f,
        c.clear_color.g != 0 ? (float)c.clear_color.g / 255.f : 0.f,
        c.clear_color.b != 0 ? (float)c.clear_color.b / 255.f : 0.f,
        c.clear_color.a != 0 ? (float)c.clear_color.a / 255.f : 0.f);
    glClear(GL_COLOR_BUFFER_BIT);

    return true;
}

bool poll_events(example_context& c)
{
    SDL_Event e = {};
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
        case SDL_QUIT:
            return false;
        case SDL_KEYDOWN:
            if(e.key.keysym.sym == SDLK_ESCAPE) return false; // Quit
            break;
        }
    }

    SDL_GL_SwapWindow(c.window);

    // Clear the backbuffer so it's ready for the next frame:
    glClearColor(
        c.clear_color.r != 0 ? (float)c.clear_color.r / 255.f : 0.f,
        c.clear_color.g != 0 ? (float)c.clear_color.g / 255.f : 0.f,
        c.clear_color.b != 0 ? (float)c.clear_color.b / 255.f : 0.f,
        c.clear_color.a != 0 ? (float)c.clear_color.a / 255.f : 0.f);
    glClear(GL_COLOR_BUFFER_BIT);

    return true;
}

void shutdown(example_context& c)
{
    if (c.font) xeekworx::bitmapfonts::delete_font(c.font);
    if (c.nvg_context) nvgDeleteGL3(c.nvg_context);
    if (c.gl_context) SDL_GL_DeleteContext(c.gl_context);
    if (c.window) SDL_DestroyWindow(c.window);
    SDL_Quit();
}
