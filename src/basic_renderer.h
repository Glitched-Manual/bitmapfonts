#pragma once
/*
    This is the basic renderer & image buffer management for the Xeekworx Font 
    API. This renderer is used for font generation only.

    NOTE:
    The internal pixel format is RGBA, so specifying colors in parameters as 
    32-bit values are ALMOST the same as in HTML, but with a required alpha. 
    Examples:

        For opaque red use:                 0xFF0000FF
        For opaque green use:               0x00FF00FF
        For opaque blue use:                0x0000FFFF
        For transparent / no color use:     0x00000000 or 0xFFFFFF00

    Do not use HTML colors without alpha channels or the bit-math will not 
    give you desired results, since at least on x86 bits are right to left for 
    integers.

    When using save functions the pixels are converted to ABGR so that the 
    buffer will be bitmap compatible for saving to files or passing to other 
    apis that assume it's bitmap data. This also means the image constructor 
    that takes a pixel buffer as a parameter expects those pixels to be in the
    ABGR format.
*/

#include <stdint.h>
#include <memory>
#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace xeekworx {
    namespace bitmapfonts {

        class basic_renderer {
        private:

            uint32_t * m_data = nullptr;
            int32_t m_width = 0, m_height = 0; // signed, because x / y are
            bool m_external_data = false;

        public:

            static constexpr uint32_t transparent = 0xFFFFFF00;
            static constexpr uint32_t white = 0xFFFFFFFF;
            static constexpr uint32_t black = 0x000000FF;
            static constexpr uint32_t channels = 4;

            basic_renderer(const uint32_t width, const uint32_t height, const uint32_t background);
            basic_renderer(const uint32_t width, const uint32_t height);
            basic_renderer(const basic_renderer& source);
            basic_renderer(uint32_t * source_pixels, const uint32_t width, const uint32_t height, const bool copy_convert = true);
            basic_renderer(const FT_Bitmap* ftbitmap, const uint32_t foreground = black, const uint32_t background = transparent);
            ~basic_renderer();

            int32_t width() const { return m_width; }
            int32_t height() const { return m_height; }
            uint32_t * data() const { return m_data; }

            void clear(const uint32_t background = transparent);

            bool empty() const { return size() <= 0 || !m_data; }
            size_t size_in_bytes() const { return size() * channels; }
            size_t size() const { return m_width * m_height; }

            uint32_t& operator[](const int i) { return m_data[i]; }
            const uint32_t& operator[](const int i) const { return m_data[i]; }

            uint32_t from_point(const uint32_t x, const uint32_t y) const { return m_data[x + m_width * y]; }
            void to_point(const uint32_t x, const uint32_t y, const uint32_t value) { m_data[x + m_width * y] = value; }

            void draw_hline(const int32_t x1, const int32_t x2, const int32_t y, const uint32_t color);
            void draw_vline(const int32_t x, const int32_t y1, const int32_t y2, const uint32_t color);
            void draw_rect(const int32_t x, const int32_t y, const int32_t w, const int32_t h, const uint32_t thickness, const uint32_t color);
            void draw_bitmap(const FT_Bitmap* ftbitmap, const int32_t x, const int32_t y, const uint32_t foreground);
            void draw_bitmap(const basic_renderer& source_img, const int32_t source_x, const int32_t source_y, const int32_t source_w, const int32_t source_h, const int32_t x, const int32_t y);

            enum class rotation { left90degrees, right90degrees };
            void draw_bitmap_rotated(const FT_Bitmap* ftbitmap, const int32_t x, const int32_t y, const uint32_t foreground, rotation direction);
            void draw_bitmap_rotated(const basic_renderer& source_img, const int32_t source_x, const int32_t source_y, const int32_t source_w, const int32_t source_h, const int32_t x, const int32_t y, rotation direction);

            bool save(const std::string& to_file) const;
            bool save(uint32_t * to_buffer, const size_t size_in_bytes) const;

        private:

            static uint32_t blend_colors(const uint32_t foreground, const uint32_t background);

            struct pixel_abgr;

            struct pixel_rgba {
                // There is concern that this might not work the same on 
                // non-x86 based processors
                uint8_t r, g, b, a;
                pixel_rgba() {}
                pixel_rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r(r), g(g), b(b), a(a) {}
                pixel_rgba(const pixel_abgr& p) : r(p.r), g(p.g), b(p.b), a(p.a) {}
                pixel_rgba(const uint32_t& value) {
                    r = (value >> 24) & 0xFF;
                    g = (value >> 16) & 0xFF;
                    b = (value >> 8) & 0xFF;
                    a = value & 0xFF;
                }
                pixel_rgba(const uint32_t& value, const uint8_t& alpha) {
                    r = (value >> 24) & 0xFF;
                    g = (value >> 16) & 0xFF;
                    b = (value >> 8) & 0xFF;
                    a = alpha;//value & 0xFF;
                }
                operator uint32_t() { return ((a)+(b << 8) + (g << 16) + (r << 24)); }
            };

            struct pixel_abgr {
                // There is concern that this might not work the same on 
                // non-x86 based processors
                uint8_t b, g, r, a;
                pixel_abgr() {}
                pixel_abgr(uint8_t b, uint8_t g, uint8_t r, uint8_t a) : b(b), g(g), r(r), a(a) {}
                pixel_abgr(const pixel_rgba& p) : b(p.b), g(p.g), r(p.r), a(p.a) {}
                pixel_abgr(const uint32_t& value) {
                    a = (value >> 24) & 0xFF;
                    b = (value >> 16) & 0xFF;
                    g = (value >> 8) & 0xFF;
                    r = value & 0xFF;
                }
                pixel_abgr(const uint32_t& value, const uint8_t& alpha) {
                    a = alpha;// (value >> 24) & 0xFF;
                    b = (value >> 16) & 0xFF;
                    g = (value >> 8) & 0xFF;
                    r = value & 0xFF;
                }
                operator uint32_t() { return ((r)+(g << 8) + (b << 16) + (a << 24)); }
            };

        };
        
        typedef std::shared_ptr<basic_renderer> basic_renderer_ptr;

    }
}