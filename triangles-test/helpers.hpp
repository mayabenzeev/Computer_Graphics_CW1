#ifndef HELPERS_HPP_DD37133A_D9CE_4998_AA48_41DA09E1517C
#define HELPERS_HPP_DD37133A_D9CE_4998_AA48_41DA09E1517C

#include "../draw2d/forward.hpp"
#include <cstddef>
#include "../vmlib/vec2.hpp"

ColorU8_sRGB find_most_red_pixel( Surface const& );
ColorU8_sRGB find_least_red_nonzero_pixel( Surface const& );
std::size_t count_colored_pixels(Surface const&);
bool is_point_inside_triangle(Vec2f, Vec2f, Vec2f, Vec2f);
bool is_triangle_filled(Surface&, Vec2f, Vec2f, Vec2f);


#endif // HELPERS_HPP_DD37133A_D9CE_4998_AA48_41DA09E1517C
