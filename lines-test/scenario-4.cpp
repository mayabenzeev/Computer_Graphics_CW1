#include <catch2/catch_amalgamated.hpp>

#include "../draw2d/surface.hpp"

#include "../draw2d/draw.hpp"

#include "helpers.hpp"

TEST_CASE("No Gaps Between Connected Lines", "[connect]") 
{
    Surface surface(200, 200);
    surface.clear();

    SECTION("Horizontal connection") 
    {
        draw_line_solid(surface, {10.f, 170.f}, {50.f, 170.f}, {255, 255, 255});
        draw_line_solid(surface, {50.f, 170.f}, {100.f, 170.f}, {255, 255, 255});

        auto const counts = count_pixel_neighbours(surface);
        REQUIRE(2 == counts[1]);
        REQUIRE(counts[2] > 0);
        REQUIRE(0 == counts[0]);

        for (std::size_t i = 3; i < counts.size(); ++i)
            REQUIRE(0 == counts[i]);
    }
}
