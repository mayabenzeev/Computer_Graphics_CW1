#include "helpers.hpp"

#include "../draw2d/color.hpp"
#include "../draw2d/surface.hpp"
#include <cstddef>
#include <algorithm>


ColorU8_sRGB find_most_red_pixel( Surface const& aSurface )
{
	ColorU8_sRGB ret{ 0, 0, 0 };

	auto const stride = aSurface.get_width() << 2;
	for( std::uint32_t x = 0; x < aSurface.get_width(); ++x )
	{
		for( std::uint32_t y = 0; y < aSurface.get_height(); ++y )
		{
			auto const idx = y*stride + (x<<2);
			auto const ptr = aSurface.get_surface_ptr() + idx;

			// Not really needed.
			//if( 0 == ptr[0] && 0 == ptr[1] && 0 == ptr[2] )
			//	continue;

			if( ptr[0] >= ret.r )
			{
				ret.r = ptr[0];
				ret.g = ptr[1];
				ret.b = ptr[2];
			}
		}
	}

	return ret;
}

ColorU8_sRGB find_least_red_nonzero_pixel( Surface const& aSurface )
{
	ColorU8_sRGB ret{ 255, 255, 255 };

	auto const stride = aSurface.get_width() << 2;
	for( std::uint32_t x = 0; x < aSurface.get_width(); ++x )
	{
		for( std::uint32_t y = 0; y < aSurface.get_height(); ++y )
		{
			auto const idx = y*stride + (x<<2);
			auto const ptr = aSurface.get_surface_ptr() + idx;

			if( 0 == ptr[0] && 0 == ptr[1] && 0 == ptr[2] )
				continue;

			if( ptr[0] < ret.r )
			{
				ret.r = ptr[0];
				ret.g = ptr[1];
				ret.b = ptr[2];
			}
		}
	}

	return ret;
}

std::size_t count_colored_pixels(Surface const& aSurface)
{
    std::size_t count = 0;
    auto const stride = aSurface.get_width() << 2;
    for(std::uint32_t y = 0; y < aSurface.get_height(); ++y)
    {
        for(std::uint32_t x = 0; x < aSurface.get_width(); ++x)
        {
            auto const idx = y * stride + (x << 2);
            auto const ptr = aSurface.get_surface_ptr() + idx;

            if (ptr[0] != 0 || ptr[1] != 0 || ptr[2] != 0)
            { 
                count++;
            }
        }
    }
    return count;
}

bool is_point_inside_triangle(Vec2f aP0, Vec2f aP1, Vec2f aP2, Vec2f aNewP) {  
    // Calculate barycentric coordinates
    float denom = (aP1.y - aP2.y) * (aP0.x - aP2.x) + (aP2.x - aP1.x) * (aP0.y - aP2.y);
    float alpha = ((aP1.y - aP2.y) * (aNewP.x - aP2.x) + (aP2.x - aP1.x) * (aNewP.y - aP2.y)) / denom;
    float beta = ((aP2.y - aP0.y) * (aNewP.x - aP2.x) + (aP0.x - aP2.x) * (aNewP.y - aP2.y)) / denom;
    float gamma = 1.0f - alpha - beta;

    //  New Point is inside the triangle if all positive
    return (alpha >= 0 && beta >= 0 && gamma >= 0);
}

bool is_triangle_filled(Surface& surface, Vec2f aP0, Vec2f aP1, Vec2f aP2) {
    int minX = std::min({aP0.x, aP1.x, aP2.x});
    int maxX = std::max({aP0.x, aP1.x, aP2.x});
    int minY = std::min({aP0.y, aP1.y, aP2.y});
    int maxY = std::max({aP0.y, aP1.y, aP2.y});

    for ( int x = minX; x <= maxX; ++x ) 
	{
        for ( int y = minY; y <= maxY; ++y ) 
		{
            if (is_point_inside_triangle({static_cast<float>(x), static_cast<float>(y)}, aP0, aP1, aP2)) 
			{
				auto idx = surface.get_linear_index(x, y);
				auto ptr = surface.get_surface_ptr() + idx;
				if (ptr[0] == 0 && ptr[1] == 0 && ptr[2] == 0) 
				{
					return false;
				}
			}
		}
	}
    return true;
}