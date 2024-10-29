#include "draw.hpp"

#include <algorithm>

#include <cmath>

#include "surface.hpp"

// Helper Functions Declarations
bool cohen_sutherland_clip( Surface& aSurface, Vec2f& aBegin, Vec2f& aEnd, Vec2f minPoint, Vec2f aMax );
int get_point_region_code( Vec2f aPoint, Vec2f minPoint, Vec2f aMax );

void draw_line_solid( Surface& aSurface, Vec2f aBegin, Vec2f aEnd, ColorU8_sRGB aColor )
{
	printf("points trying to draw: (%f, %f), (%f, %f)\n", aBegin.x, aBegin.y, aEnd.x, aEnd.y); // logging
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
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	//TODO: your implementation goes here

	//TODO: remove the following when you start your implementation
	(void)aSurface; // Avoid warnings about unused arguments until the function
	(void)aP0;      // is properly implemented.
	(void)aP1;
	(void)aP2;
	(void)aC0;
	(void)aC1;
	(void)aC2;
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
