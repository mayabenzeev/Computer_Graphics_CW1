#include <catch2/catch_amalgamated.hpp>

#include "../draw2d/surface.hpp"

#include "../draw2d/draw.hpp"

#include "helpers.hpp"

TEST_CASE( "2 lines with mutual pixels", "[mutual pixels]" )
{
    Surface surface(200, 200);
    surface.clear();
            
    Vec2f mutualP { 20.f, 100.f };
    ColorU8_sRGB expectedColor = {255, 255, 255};

    SECTION( "continuous trivial horizontal" ) 
    {
        // Tests when (p0.x < p1.x < p2.x) and (p0.y = p1.y = p2.y)
        draw_line_solid( surface, 
            { mutualP.x - 10.f, mutualP.y }, 
            mutualP,
            expectedColor );

        draw_line_solid( surface,
            mutualP,
            { mutualP.x + 30.f, mutualP.y }, 
            expectedColor);

        auto const counts = count_pixel_neighbours(surface);

        // There are exactly two pixels with one neighbour: one at each end of the line
        REQUIRE( 2 == counts[1] );

        // There is a non-zero number of pixels with two neighbours, as each line containes more then 1 pixel,
        // and the connected line should be at least 3 pixels.
        REQUIRE( counts[2] > 0 );

        // No pixels should have zero neighbours (=isolated)
        REQUIRE(0 == counts[0]);
        
        // There should be no pixels with more than two neighbours
        for (std::size_t i = 3; i < counts.size(); ++i)
            REQUIRE(0 == counts[i]);
    }

    SECTION( "sharp angle" ) 
    {
        // Tests when (p0.x < p1.x < p2.x) and (p0.y = p1.y < p2.y) - sharp angle (_/)
        draw_line_solid( surface, 
            { mutualP.x - 10.f, mutualP.y }, 
            mutualP,
            expectedColor );

        draw_line_solid( surface,
            mutualP,
            { mutualP.x + 30.f, mutualP.y * 2 }, 
            expectedColor);

        // Ensure that common pixel at p1 that is correctly rendered.
        auto idx = surface.get_linear_index(mutualP.x, mutualP.y);
        auto ptr = surface.get_surface_ptr() + idx;
        REQUIRE( ptr[0] == expectedColor.r );
        REQUIRE( ptr[1] == expectedColor.g );
        REQUIRE( ptr[2] == expectedColor.b );
    }

    SECTION( "overlap horizontal" ) 
    {
        // Tests when (p1.x < p0.x < p2.x) and (p0.y = p1.y = p2.y)
        draw_line_solid( surface, 
            { mutualP.x + 30.f , mutualP.y }, 
            mutualP,
            expectedColor );

        draw_line_solid( surface,
            mutualP, 
            { mutualP.x + 80.f , mutualP.y },
            expectedColor );

        // Ensure that common pixels on the overlapping line from p1.x to p0.x are correctly rendered
        for (int x = static_cast<int>(mutualP.x); x < mutualP.x + 30.f; x++) {
            auto idx = surface.get_linear_index(x, mutualP.y);
            auto ptr = surface.get_surface_ptr() + idx;
            REQUIRE( ptr[0] == expectedColor.r );
            REQUIRE( ptr[1] == expectedColor.g );
            REQUIRE( ptr[2] == expectedColor.b );
        }
    }
}

TEST_CASE( "3 lines with mutual pixels", "[mutual pixels]" )
{
    Surface surface(200, 200);
    surface.clear();
            
    Vec2f mutualP0 { 20.f, 100.f };
    ColorU8_sRGB expectedColor = {255, 0, 0};

    SECTION( "short multi lines" ) 
    {
        // Tests when (p0.x < p1.x < p2.x < p3.x) and (p0.y = p1.y = p2.y = p3.y) 
        draw_line_solid( surface, 
            { mutualP0.x - 1.f, mutualP0.y }, 
            mutualP0,
            expectedColor );

        draw_line_solid( surface,
            mutualP0,
            { mutualP0.x + 1.f, mutualP0.y }, 
            expectedColor);

        draw_line_solid( surface,
            { mutualP0.x + 1.f, mutualP0.y },
            { mutualP0.x + 2.f, mutualP0.y }, 
            expectedColor);


        auto const counts = count_pixel_neighbours(surface);

        // No gaps - There are exactly two pixels with one neighbour: one at each end of the line 
        REQUIRE( 2 == counts[1] );

        // There is a non-zero number of pixels with two neighbours, as each line containes more then 1 pixel,
        // and the connected line should be at least 3 pixels.
        REQUIRE( counts[2] > 0 );

        // No pixels should have zero neighbours (=isolated)
        REQUIRE(0 == counts[0]);
        
        // There should be no pixels with more than two neighbours
        for (std::size_t i = 3; i < counts.size(); ++i)
            REQUIRE(0 == counts[i]);
    }

    SECTION( "crossing lines" ) 
    {
        // Tests when there is an intersection point between p0-p1 and p2-p3 
        // p0-p1 -> y = 100
        // p2-p3 -> y = x + 50
        draw_line_solid( surface, 
            { 0.f, 100.f }, 
            { 100.f, 100.f },
            expectedColor );

        draw_line_solid( surface,
            { 100.f, 100.f },
            { 100.f, 150.f }, 
            expectedColor);

        draw_line_solid( surface,
            { 100.f, 150.f },
            { 50.f, 100.f }, 
            expectedColor);


        // Calculate intersection point
        float intersectX = 50.f;
        float intersectY = 100.f;
        // Ensure that intersected pixel is correctly rendered.
        auto idx = surface.get_linear_index(intersectX, intersectY);
        auto ptr = surface.get_surface_ptr() + idx;

        // Color at intersected
        REQUIRE( ptr[0] == expectedColor.r );
        REQUIRE( ptr[1] == expectedColor.g );
        REQUIRE( ptr[2] == expectedColor.b );
    
    }
}
