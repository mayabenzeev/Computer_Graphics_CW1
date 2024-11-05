#include "draw.hpp"

#include <algorithm>

#include <cmath>

#include "surface.hpp"

// Helper Functions Declarations
bool cohen_sutherland_clip( Surface& aSurface, Vec2f& aBegin, Vec2f& aEnd, Vec2f minPoint, Vec2f aMax );
int get_point_region_code( Vec2f aPoint, Vec2f minPoint, Vec2f aMax );
void sort_vertices_ascending_y_order( Vec2f& aP0, Vec2f& aP1, Vec2f& aP2, ColorF aC0, ColorF aC1, ColorF aC2 );
Vec2f interpolate_line(Vec2f& aP0, Vec2f& aP1, Vec2f& aP2, float aT);
ColorF interpolate_color(const Vec2f& aP0, const Vec2f& aP1, const Vec2f& aP2, const ColorF& aC0, const ColorF& aC1, const ColorF& aC2, const Vec2f& aNewP);
void fill_top_flat_triangle( Surface& aSurface, Vec2f aP0, Vec2f aP1, Vec2f aP2, ColorF aC0, ColorF aC1, ColorF aC2 ,bool aIsTrivialCase );
void fill_bottom_flat_triangle( Surface& aSurface, Vec2f aP0, Vec2f aP1, Vec2f aP2, ColorF aC0, ColorF aC1, ColorF aC2 );
ColorU8_sRGB convert_color_to_sRGB(ColorF aColor);

void draw_line_solid( Surface& aSurface, Vec2f aBegin, Vec2f aEnd, ColorU8_sRGB aColor )
{
	// Define the clipping region
	Vec2f minPoint{0, 0};
	Vec2f maxPoint{static_cast<float>(aSurface.get_width()) - 1, static_cast<float>(aSurface.get_height()) - 1};	
	
	// Clipping
    if (!cohen_sutherland_clip(aSurface, aBegin, aEnd, minPoint, maxPoint)) 
	{
        return; // No need to draw the line
    }

	// Rounds the float coordinates to the nearest integer coordinates
    int x1 = static_cast<int>(std::round(aBegin.x));
    int y1 = static_cast<int>(std::round(aBegin.y));
    int x2 = static_cast<int>(std::round(aEnd.x));
    int y2 = static_cast<int>(std::round(aEnd.y));

	// Calculate the length of the line in each direction
	int dx = abs(x2 - x1); 
	int dy = -abs(y2 - y1);
	// Calculate the direction in which to step along each axis
	int sx = x1 < x2 ? 1 : -1;
	int sy = y1 < y2 ? 1 : -1;
	int error = dx + dy; // Calculate the error at the start

	while (true)
	{
		aSurface.set_pixel_srgb(x1, y1, aColor); // Set the current pixel color
		if (x1 == x2 && y1 == y2) // Check if the endpoint has been reached
		{
			break;
		}

		int error2 = error * 2; // Step-wise error evaluation
		// Check which direction to move in
		if (error2 >= dy)
		{
			error += dy;
			x1 += sx;
		}
		if (error2 <= dx)
		{
			error += dx;
			y1 += sy;
		}
	}
}

void draw_triangle_wireframe( Surface& aSurface, Vec2f aP0, Vec2f aP1, Vec2f aP2, ColorU8_sRGB aColor )
{
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	//TODO: your implementation goes here

	//TODO: remove the following when you start your implementation
	(void)aSurface; // Avoid warnings about unused arguments until the function
	(void)aP0;   // is properly implemented.
	(void)aP1;
	(void)aP2;
	(void)aColor;
}

void draw_triangle_solid( Surface& aSurface, Vec2f aP0, Vec2f aP1, Vec2f aP2, ColorU8_sRGB aColor )
{
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	//TODO: your implementation goes here

	//TODO: remove the following when you start your implementation
	(void)aSurface; // Avoid warnings about unused arguments until the function
	(void)aP0;   // is properly implemented.
	(void)aP1;
	(void)aP2;
	(void)aColor;
}

void draw_triangle_interp( Surface& aSurface, Vec2f aP0, Vec2f aP1, Vec2f aP2, ColorF aC0, ColorF aC1, ColorF aC2 )
{
	sort_vertices_ascending_y_order(aP0, aP1, aP2, aC0, aC1, aC2); // Sort the vertices by y
    float t = (aP1.y - aP0.y) / (aP2.y - aP0.y); // Calculate t the interpolation factor

	if (aP1.y == aP2.y) // If the triangle is a top flat triangle
	{
		fill_top_flat_triangle(aSurface, aP0, aP1, aP2, aC0, aC1, aC2, true);
	}
	else if (aP0.y == aP1.y) // If the triangle is a bottom flat triangle
	{
		fill_bottom_flat_triangle(aSurface, aP0, aP1, aP2, aC0, aC1, aC2);
	}
	else
	{
		// Find P3, C3 - the intersection point of the boundary line and the triangle long edge.
		Vec2f p3 = interpolate_line(aP0, aP1, aP2, t);
		ColorF c3 = interpolate_color(aP0, aP1, aP2, aC0, aC1, aC2, p3);


		// Draw the top and bottom flat triangles
        fill_top_flat_triangle(aSurface, aP0, p3, aP1, aC0, c3, aC1, false);
		fill_bottom_flat_triangle(aSurface, p3, aP1, aP2, c3, aC1, aC2);

	}
}


void draw_rectangle_solid( Surface& aSurface, Vec2f aMinCorner, Vec2f aMaxCorner, ColorU8_sRGB aColor )
{
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	//TODO: your implementation goes here

	//TODO: remove the following when you start your implementation
	(void)aSurface; // Avoid warnings about unused arguments until the function
	(void)aMinCorner;   // is properly implemented.
	(void)aMaxCorner;
	(void)aColor;
}

void draw_rectangle_outline( Surface& aSurface, Vec2f aMinCorner, Vec2f aMaxCorner, ColorU8_sRGB aColor )
{
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	//TODO: your implementation goes here

	//TODO: remove the following when you start your implementation
	(void)aSurface; // Avoid warnings about unused arguments
	(void)aMinCorner;
	(void)aMaxCorner;
	(void)aColor;
}

// Helper Functions Implementations
bool cohen_sutherland_clip( Surface& aSurface, Vec2f& aBegin, Vec2f& aEnd, Vec2f aMin, Vec2f aMax )
{
	// Compute the region codes for the two points
	int beginCode = get_point_region_code(aBegin, aMin, aMax);
	int endCode = get_point_region_code(aEnd, aMin, aMax);

	// If the line is completely outside the window it will be discarded
	if ((beginCode & endCode) != 0)
	{
		return false;
	}

	// If the line is completely inside the window no need to clip
	if ((beginCode | endCode) == 0)
	{
		return true;
	}
       
	float dx = aEnd.x - aBegin.x;
	float dy = aEnd.y - aBegin.y;

	// If a point is outside the left boundary - if bit 1 is "1"
	if (beginCode & 1)
	{
		aBegin.x = aMin.x;
		aBegin.y += (dy / dx) * (aMin.x - aBegin.x);
	}
	else if (endCode & 1)
	{
		aEnd.x = aMin.x;
		aEnd.y += (dy / dx) * (aMin.x - aEnd.x);
	}

	// If a point is outside the right boundary - if bit 2 is "1"
	if (beginCode & 2)
	{
		aBegin.x = aMax.x;
		aBegin.y += dy * (aMax.x - aBegin.x) / dx;
	}
	else if (endCode & 2)
	{
		aEnd.x = aMax.x;
		aEnd.y += dy * (aMax.x - aEnd.x) / dx;
	}

	// If a point is outside the bottom boundary - if bit 3 is "1"
	if (beginCode & 4)
	{
		aBegin.y = aMin.y;
		aBegin.x += dx * (aMin.y - aBegin.y) / dy;
	}
	else if (endCode & 4)
	{
		aEnd.y = aMin.y;
		aEnd.x += dx * (aMin.y - aEnd.y) / dy;
	}

	// If a point is outside the top boundary - if bit 4 is "1"
	if (beginCode & 8)
	{
		aBegin.y = aMax.y;
		aBegin.x += dx * (aMax.y - aBegin.y) / dy;
	}
	else if (endCode & 8)
	{
		aEnd.y = aMax.y;
		aEnd.x += dx * (aMax.y - aEnd.y) / dy;
	}

	return true;
}

int get_point_region_code( Vec2f aPoint, Vec2f aMin, Vec2f aMax )
{
	// Calculate region code for point
	// Point initially is considered "inside" - code is 0000
	int code = 0;
	if (aPoint.x < aMin.x) code |= 1; // Set the left bit
	if (aPoint.x > aMax.x) code |= 2; // Set the right bit
	if (aPoint.y < aMin.y) code |= 4; // Set the bottom bit
	if (aPoint.y > aMax.y) code |= 8; // Set the top bit
	return code;
}

void sort_vertices_ascending_y_order(Vec2f &aP0, Vec2f &aP1, Vec2f &aP2, ColorF aC0, ColorF aC1, ColorF aC2 )
{
	// Sort the vertices in ascending order of y-coordinates
	if (aP0.y > aP1.y)
	{
		std::swap(aP0, aP1);
		std::swap(aC0, aC1);
	}
	if (aP0.y > aP2.y)
	{
		std::swap(aP0, aP2);
		std::swap(aC0, aC2);
	}
	if (aP1.y > aP2.y)
	{
		std::swap(aP1, aP2);
		std::swap(aC1, aC2);
	}
}

Vec2f interpolate_line(Vec2f& aP0, Vec2f& aP1, Vec2f& aP2, float aT)
{
	// Calculate the x-coordinate of the intersection point
	// float x = aP0.x + aT * (aP2.x - aP0.x);
	float x = (1 - aT) * aP0.x + aT * aP2.x;
    return Vec2f{x, aP1.y};
}

ColorF interpolate_color(const Vec2f& aP0, const Vec2f& aP1, const Vec2f& aP2, const ColorF& aC0, const ColorF& aC1, const ColorF& aC2, const Vec2f& aNewP) {
    // Calculate barycentric coordinates
    float denom = (aP1.y - aP2.y) * (aP0.x - aP2.x) + (aP2.x - aP1.x) * (aP0.y - aP2.y);
    float alpha = ((aP1.y - aP2.y) * (aNewP.x - aP2.x) + (aP2.x - aP1.x) * (aNewP.y - aP2.y)) / denom;
    float beta = ((aP2.y - aP0.y) * (aNewP.x - aP2.x) + (aP0.x - aP2.x) * (aNewP.y - aP2.y)) / denom;
    float gamma = 1.0f - alpha - beta;

    // Return the blended color by the weights
    return ColorF{
        alpha * aC0.r + beta * aC1.r + gamma * aC2.r,
        alpha * aC0.g + beta * aC1.g + gamma * aC2.g,
        alpha * aC0.b + beta * aC1.b + gamma * aC2.b
    };
}

void fill_bottom_flat_triangle(Surface &aSurface, Vec2f aP0, Vec2f aP1, Vec2f aP2, ColorF aC0, ColorF aC1, ColorF aC2)
{
    float invslope1 = (aP2.x - aP0.x) / (aP2.y - aP0.y); // Inverse slope of one edge
    float invslope2 = (aP2.x - aP1.x) / (aP2.y - aP1.y); // Inverse slope of seconed edge
    float curx1 = aP0.x;
    float curx2 = aP1.x;
    int height = aSurface.get_height();
	int width = aSurface.get_width();
	ColorF interpolatedColor;

	// Clipping and Culling y-coordinates
    float startY = std::max(aP0.y, 0.0f);
    float endY = std::min(aP2.y, static_cast<float>(height));

	// Iterate from bottom (y0/y1) to top (y2) and fill the triangle
    for (float scanlineY = startY; scanlineY < endY; scanlineY++)
    {
		if (scanlineY >= 0 && scanlineY < height) { // Check that y isn't out of bounds
		int fromX = std::min(static_cast<int>(std::round(curx1)), static_cast<int>(std::round(curx2)));
		int toX = std::max(static_cast<int>(std::round(curx1)), static_cast<int>(std::round(curx2)));


		for (int x = std::max(fromX, 0); x <= std::min(toX, static_cast<int>(width - 1)); x++) {
			interpolatedColor = interpolate_color(aP0, aP1, aP2, aC0, aC1, aC2, Vec2f{static_cast<float>(x), scanlineY});
			aSurface.set_pixel_srgb(x, static_cast<int>(scanlineY), convert_color_to_sRGB(interpolatedColor));
			}
		}

        curx1 += invslope1;
        curx2 += invslope2;
    }
}

void fill_top_flat_triangle(Surface &aSurface, Vec2f aP0, Vec2f aP1, Vec2f aP2, ColorF aC0, ColorF aC1, ColorF aC2, bool aIsTrivialCase)
{
    float invslope1 = (aP2.x - aP0.x) / (aP2.y - aP0.y); // Inverse slope of one edge
    float invslope2 = (aP1.x - aP0.x) / (aP1.y - aP0.y); // Inverse slope of seconed edge
    float curx1 = aP2.x;
    float curx2 = aP1.x;
    int height = aSurface.get_height();
	int width = aSurface.get_width();
	ColorF interpolatedColor;

	// Clipping and Culling y-coordinates
	float startY = 0.0f;
	if (!aIsTrivialCase) startY = std::max(aP2.y, startY); // Decide whether to draw the first row of the triangle to prevent overlapping rendering
	else startY = std::max(aP2.y - 1, startY);
    float endY = std::min(aP0.y, static_cast<float>(height + 1));
	
	// Iterate from top (y2/y1) to bottom (y0) and fill the triangle
    for (float scanlineY = startY; scanlineY >= endY; scanlineY--)
    {
		if (scanlineY >= 0 && scanlineY < height) {  // Check that y isn't out of bounds
			int fromX = std::min(static_cast<int>(std::round(curx1)), static_cast<int>(std::round(curx2)));
			int toX = std::max(static_cast<int>(std::round(curx1)), static_cast<int>(std::round(curx2)));
			for (int x = std::max(fromX, 0); x <= std::min(toX, static_cast<int>(width - 1)); x++) {
				interpolatedColor = interpolate_color(aP0, aP1, aP2, aC0, aC1, aC2, Vec2f{static_cast<float>(x), scanlineY});
				aSurface.set_pixel_srgb(x, static_cast<int>(scanlineY), convert_color_to_sRGB(interpolatedColor));
			
			}
		}
		curx1 -= invslope1;
		curx2 -= invslope2;
	}
}

ColorU8_sRGB convert_color_to_sRGB(ColorF aColor)
{
	// Convert color to sRGB color
	return ColorU8_sRGB{linear_to_srgb(aColor.r), linear_to_srgb(aColor.g), linear_to_srgb(aColor.b)};
}


