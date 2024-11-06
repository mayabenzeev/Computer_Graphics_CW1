#include <catch2/catch_amalgamated.hpp>

#include "../draw2d/surface.hpp"

#include "../draw2d/draw.hpp"

#include "helpers.hpp"

TEST_CASE("Partially Out of Bounds Endpoints", "[clip]")
{
    Surface surface(100, 100);
    surface.clear();

    SECTION( "horizontal" )
    {        
        Vec2f startP { 10.f, 50.f };
        Vec2f endP { 200.f, 50.f };

        // Line with one point inside the surface and one with x coordinates outside the surface.
        draw_line_solid( surface, 
            startP, 
            endP, 
            { 255, 0, 0 });

        auto const pixels = max_row_pixel_count(surface);
        int expectedPixels = surface.get_width() - startP.x;  // Expected number of pixels 

        REQUIRE(pixels == expectedPixels);  // Pixels drawn up to boundary
    }

    SECTION( "vertical" )
    {   
        Vec2f startP { 50.f, -100.f };
        Vec2f endP { 50.f, 80.f };
        
        // Line with one point inside the surface and one with y coordinates outside the surface.
        draw_line_solid( surface, 
            startP, 
            endP, 
            { 255, 0, 0 });

        auto const pixels = max_col_pixel_count(surface);
        int expectedPixels = endP.y - 0 + 1;  // Expected number of pixels 

        REQUIRE(pixels == expectedPixels);  // Pixels drawn up to boundary
    }

    SECTION( "diagonal" )
    {
        ColorU8_sRGB expectedColor = {255, 0, 0};

        // y = x + 20
        draw_line_solid( surface, 
            { 0.f, 20.f }, 
            { 100.f, 120.f }, 
            expectedColor);

        int clippedY = surface.get_height() - 1;
        int clippedX = clippedY - 20;

        // Check the color of the expected in-bound endpoint pixel after clipping
        auto const idx = surface.get_linear_index(clippedX, clippedY);
        auto ptr = surface.get_surface_ptr() + idx;
        REQUIRE( ptr[0] == expectedColor.r );
        REQUIRE( ptr[1] == expectedColor.g );
        REQUIRE( ptr[2] == expectedColor.b );
    }
}


