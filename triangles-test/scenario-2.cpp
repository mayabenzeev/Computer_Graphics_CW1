#include <catch2/catch_amalgamated.hpp>

#include "helpers.hpp"

#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"

TEST_CASE("Triangle is filled", "[fill]")
{
    Surface surface(200, 200);
    surface.clear();

    SECTION( "flat top" )
    {
        Vec2f p0 {50.f, 50.f};
        Vec2f p1 {100.f, 50.f}; 
        Vec2f p2 {60.f, 0.f};

        draw_triangle_interp(surface,
            p0,
            p1,
            p2,
            {1.f, 0.f, 0.f},
            {0.f, 1.f, 0.f}, 
            {0.f, 0.f, 1.f}
        );
        REQUIRE( is_triangle_filled(surface, p0, p1, p2) ); 
    }

    SECTION( "flat bottom" )
    {
        Vec2f p0 {50.f, 50.f};
        Vec2f p1 {100.f, 50.f}; 
        Vec2f p2 {60.f, 100.f};

        draw_triangle_interp(surface,
            p0,
            p1,
            p2,
            {1.f, 0.f, 0.f},
            {0.f, 1.f, 0.f}, 
            {0.f, 0.f, 1.f}
        );
        REQUIRE( is_triangle_filled(surface, p0, p1, p2) ); 
    }

    SECTION( "non trivial" )
    {
        Vec2f p0 {50.f, 50.f};
        Vec2f p1 {100.f, 60.f}; 
        Vec2f p2 {60.f, 100.f};

        draw_triangle_interp(surface,
            p0,
            p1,
            p2,
            {1.f, 0.f, 0.f},
            {0.f, 1.f, 0.f}, 
            {0.f, 0.f, 1.f}
        );
        REQUIRE( is_triangle_filled(surface, p0, p1, p2) ); 
    }
}
