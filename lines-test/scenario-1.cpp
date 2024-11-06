#include <catch2/catch_amalgamated.hpp>

#include "../draw2d/surface.hpp"

#include "../draw2d/draw.hpp"

#include "helpers.hpp"

TEST_CASE("Line Symmetry", "[lines]")
{
    Surface surface( 640, 480 );

    SECTION( "horizontal" )
    {
        Vec2f p0{10.f, 50.f};
        Vec2f p1{90.f, 50.f};

        // Draw line from p0 to p1
        draw_line_solid(surface, p0, p1, {255, 255, 255});
        auto pixelsFirstRow = max_row_pixel_count(surface);
        auto pixelsFirstCol = max_col_pixel_count(surface);

        surface.clear();  

        // Draw line from p1 to p0
        draw_line_solid(surface, p1, p0, {255, 255, 255});
        auto pixelsSecondRow = max_row_pixel_count(surface);
        auto pixelsSecondCol = max_col_pixel_count(surface);

        // Check that the number of pixels drawn on screen didn't change
        REQUIRE(pixelsFirstRow == pixelsSecondRow);
        REQUIRE(pixelsFirstCol == pixelsSecondCol);
    }

    SECTION( "vertical" )
    {
        Vec2f p0{20.f, 100.f};
        Vec2f p1{20.f, 350.f};

        // Draw line from p0 to p1
        draw_line_solid(surface, p0, p1, {255, 0, 255});
        auto pixelsFirstRow = max_row_pixel_count(surface);
        auto pixelsFirstCol = max_col_pixel_count(surface);

        surface.clear();  

        // Draw line from p1 to p0
        draw_line_solid(surface, p1, p0, {255, 0, 255});
        auto pixelsSecondRow = max_row_pixel_count(surface);
        auto pixelsSecondCol = max_col_pixel_count(surface);

        // Check that the number of pixels drawn on screen didn't change
        REQUIRE(pixelsFirstRow == pixelsSecondRow);
        REQUIRE(pixelsFirstCol == pixelsSecondCol);
    }

    SECTION( "diagonal" )
    {
        Vec2f p0{50.f, 50.f};
        Vec2f p1{300.f, 350.f};

        // Draw line from p0 to p1
        draw_line_solid(surface, p0, p1, {255, 0, 255});
        auto pixelsFirstRow = max_row_pixel_count(surface);
        auto pixelsFirstCol = max_col_pixel_count(surface);

        surface.clear();  

        // Draw line from p1 to p0
        draw_line_solid(surface, p1, p0, {255, 0, 255});
        auto pixelsSecondRow = max_row_pixel_count(surface);
        auto pixelsSecondCol = max_col_pixel_count(surface);

        // Check that the number of pixels drawn on screen didn't change
        REQUIRE(pixelsFirstRow == pixelsSecondRow);
        REQUIRE(pixelsFirstCol == pixelsSecondCol);
    }

    SECTION( "diagonal" )
    {
        Vec2f p0{50.f, 50.f};
        Vec2f p1{300.f, 350.f};

        // Draw line from p0 to p1
        draw_line_solid(surface, p0, p1, {255, 0, 255});
        auto pixelsFirstRow = max_row_pixel_count(surface);
        auto pixelsFirstCol = max_col_pixel_count(surface);

        surface.clear();  

        // Draw line from p1 to p0
        draw_line_solid(surface, p1, p0, {255, 0, 255});
        auto pixelsSecondRow = max_row_pixel_count(surface);
        auto pixelsSecondCol = max_col_pixel_count(surface);

        // Check that the number of pixels drawn on screen didn't change
        REQUIRE(pixelsFirstRow == pixelsSecondRow);
        REQUIRE(pixelsFirstCol == pixelsSecondCol);
    }

}
