#include <catch2/catch_amalgamated.hpp>

#include "../draw2d/surface.hpp"

#include "../draw2d/draw.hpp"

#include "helpers.hpp"

TEST_CASE("Fully Out of Bounds Endpoints", "[clip]")
{
    Surface surface(500, 300);
    surface.clear();

    SECTION( "horizontal cross screen" )
    {        
        draw_line_solid( surface,
            { -50.f, 50.f },
            { 550.f, 50.f }, 
            { 0, 0, 255 } );

        auto const pixels = max_row_pixel_count(surface);

        REQUIRE(pixels == surface.get_width());  // Full row coverage the size of the surface width
    }

    SECTION( "vertical cross screen" )
    {        
        draw_line_solid( surface,
            { 50.f, -50.f },
            { 50.f, 350.f }, 
            { 255, 0, 0 } );

        auto const pixels = max_col_pixel_count(surface);

        REQUIRE(pixels == surface.get_height());  // Full column coverage the size of the surface height
    }

    SECTION( "diagonal cross left and top edges" )
    {
        ColorU8_sRGB expectedColor = {255, 255, 0};

        // y = x + 20
        draw_line_solid( surface, 
            { -100.f, -80.f }, 
            { 600.f, 620.f }, 
            expectedColor);

        // Calculate the expected line intersection point with left bound
        int clippedStartX = 0; 
        int clippedStartY = static_cast<int>(clippedStartX) + 20;

        // Calculate the expected line intersection point with right bound
        int clippedEndY = surface.get_height() - 1;
        int clippedEndX = static_cast<int>(clippedEndY) - 20; 

        // Check the colors of the expected in-bound endpoints pixels after clipping
        auto startIdx = surface.get_linear_index(clippedStartX, clippedStartY);
        auto startPtr = surface.get_surface_ptr() + startIdx;
        REQUIRE( startPtr[0] == expectedColor.r );
        REQUIRE( startPtr[1] == expectedColor.g );
        REQUIRE( startPtr[2] == expectedColor.b );

        auto endIdx = surface.get_linear_index(clippedEndX, clippedEndY);
        auto endPtr = surface.get_surface_ptr() + endIdx;
        REQUIRE( endPtr[0] == expectedColor.r );
        REQUIRE( endPtr[1] == expectedColor.g );
        REQUIRE( endPtr[2] == expectedColor.b );
    }


    SECTION( "diagonal cross down and up edges" )
    {
        ColorU8_sRGB expectedColor = {255, 255, 0};

        // y = x - 300
        draw_line_solid( surface, 
            { -50.f, -350.f }, 
            { 500.f, 200.f }, 
            expectedColor);

        // Calculate the expected line intersection point with down bound
        int clippedStartY = 0;
        int clippedStartX = static_cast<int>(clippedStartY) + 300; 

        // Calculate the expected line intersection point with right bound
        int clippedEndX = surface.get_width() - 1;
        int clippedEndY = static_cast<int>(clippedEndX) - 300; 
    
        // Check the colors of the expected in-bound endpoints pixels after clipping
        auto startIdx = surface.get_linear_index(clippedStartX, clippedStartY);
        auto startPtr = surface.get_surface_ptr() + startIdx;
        REQUIRE( startPtr[0] == expectedColor.r );
        REQUIRE( startPtr[1] == expectedColor.g );
        REQUIRE( startPtr[2] == expectedColor.b );

        // Validate the pixel color at the clipped end point
        auto endIdx = surface.get_linear_index(clippedEndX, clippedEndY);
        auto endPtr = surface.get_surface_ptr() + endIdx;
        REQUIRE( endPtr[0] == expectedColor.r );
        REQUIRE( endPtr[1] == expectedColor.g );
        REQUIRE( endPtr[2] == expectedColor.b );
    }
}


