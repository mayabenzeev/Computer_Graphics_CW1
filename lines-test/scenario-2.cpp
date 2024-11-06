#include <catch2/catch_amalgamated.hpp>

#include "../draw2d/surface.hpp"

#include "../draw2d/draw.hpp"

#include "helpers.hpp"

TEST_CASE("Partially Out of Bounds Endpoints", "[lines]")
{
    Surface surface(100, 100);
    surface.clear();

    SECTION( "horizontal" )
    {        
        float y = 50.f;

        // Line with one point inside the surface and one with x coordinates outside the surface.
        draw_line_solid( surface, 
            { 10.f, y }, 
            { 200.f, y }, 
            { 255, 0, 0 });

        auto const pixels = max_row_pixel_count(surface);
        int expectedPixels = surface.get_width() - static_cast<int>(y);  // Expected number of pixels 

        REQUIRE(pixels == expectedPixels);  // Pixels drawn up to boundary
    }

    SECTION( "vertical" )
    {        
        float x = 50.f;
        
        // Line with one point inside the surface and one with y coordinates outside the surface.
        draw_line_solid( surface, 
            { x, -100.f }, 
            { x, 80.f }, 
            { 255, 0, 0 });

        auto const pixels = max_col_pixel_count(surface);
        int expectedPixels = surface.get_height() - static_cast<int>(x);  // Expected number of pixels 

        REQUIRE(pixels == expectedPixels);  // Pixels drawn up to boundary
    }

    SECTION( "diagonal" )
    {
        ColorU8_sRGB expectedColor = {255, 0, 0};

        // y = 0.5x
        draw_line_solid( surface, 
            { 0.f, 0.f }, 
            { 100.f, 50.f }, 
            expectedColor);

        int clippedX = surface.get_width() - 1;
        int clippedY = static_cast<int>(clippedX * 0.5); 

        // Check the color of the expected in-bound endpoint pixel after clipping
        auto const idx = surface.get_linear_index(clippedX, clippedY);
        auto const ptr = surface.get_surface_ptr() + idx;
        REQUIRE(ptr[0] == expectedColor.r);
        REQUIRE(ptr[1] == expectedColor.g);
        REQUIRE(ptr[2] == expectedColor.b);
    }
}


