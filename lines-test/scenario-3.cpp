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

    SECTION( "diagonal cross left and right edges" )
    {
        ColorU8_sRGB expectedColor = {255, 255, 0};

        // y = 0.5x 
        draw_line_solid( surface, 
            { -100.f, -50.f }, 
            { 600.f, 300.f }, 
            expectedColor);

        // Calculate the expected line intersection point with left bound
        int clippedStartX = 0; 
        int clippedStartY = static_cast<int>(clippedStartX * 0.5);

        // Calculate the expected line intersection point with right bound
        int clippedEndX = surface.get_width() - 1;
        int clippedEndY = static_cast<int>(clippedEndX * 0.5); 

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

        // y = 2x - 200
        draw_line_solid( surface, 
            { 50.f, -100.f }, 
            { 300.f, 400.f }, 
            expectedColor);

        // Calculate the expected line intersection point with down bound
        int clippedStartY = 0;
        int clippedStartX = static_cast<int>((clippedStartY  + 200) / 2); 

        // Calculate the expected line intersection point with right bound
        int clippedEndY = surface.get_height() - 1;
        int clippedEndX = static_cast<int>((clippedStartY + 200) / 2); 
    
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


