#include <catch2/catch_amalgamated.hpp>

#include "helpers.hpp"

#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"

TEST_CASE("Very small triangle", "[precision]")
{
    Surface surface(200, 200);
    surface.clear();

    SECTION( "extremely small triangle" )
    {
        draw_triangle_interp(surface,
            {150.3f, 100.2f}, 
            {150.2f, 100.5f}, 
            {150.1f, 100.2f},
            {1.f, 0.f, 0.f},
            {0.f, 1.f, 0.f}, 
            {0.f, 0.f, 1.f}
        );
        auto const pixelCount = count_colored_pixels(surface);
        REQUIRE( 0 < pixelCount ); // Ensure tiny triangle result in at least one colored pixel 
    }
}

