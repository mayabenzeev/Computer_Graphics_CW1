#include "draw.hpp"

#include <algorithm>

#include <cmath>

#include "surface.hpp"

// Helper Functions Declarations
bool cohen_sutherland_clip( Surface& aSurface, Vec2f& aBegin, Vec2f& aEnd, Vec2f aMin, Vec2f aMax );
int get_point_region_code( Vec2f aPoint, Vec2f aMin, Vec2f aMax );

void draw_line_solid( Surface& aSurface, Vec2f aBegin, Vec2f aEnd, ColorU8_sRGB aColor )
{
	// Define the clipping region
	Vec2f aMin{0, 0};
	Vec2f aMax{static_cast<float>(aSurface.get_width()) - 1, static_cast<float>(aSurface.get_height()) - 1};	
	
	// Clipping
    if (!cohen_sutherland_clip(aSurface, aBegin, aEnd, aMin, aMax)) 
	{
        return; // No need to draw the line
    }

	aBegin.x = std::clamp(aBegin.x, aMin.x, aMax.x);
    aBegin.y = std::clamp(aBegin.y, aMin.y, aMax.y);
    aEnd.x = std::clamp(aEnd.x, aMin.x, aMax.x);
    aEnd.y = std::clamp(aEnd.y, aMin.y, aMax.y);
	
	// Extra logging for post-clipping coordinates ********************************
    printf("Drawing line after clipping, aBegin: (%f, %f), aEnd: (%f, %f)\n", aBegin.x, aBegin.y, aEnd.x, aEnd.y);

	// Calculate the length of the line in each direction
	int dx = abs(aEnd.x - aBegin.x); 
	int dy = -abs(aEnd.y - aBegin.y);
	// Calculate the direction in which to step along each axis
	int sx = (aBegin.x < aEnd.x) ? 1 : -1;
	int sy = (aBegin.y < aEnd.y) ? 1 : -1;
	int error = dx + dy; // Calculate the error at the start

	for (;;)
	{
		// // Set the current pixel color
		// aSurface.set_pixel_srgb(aBegin.x, aBegin.y, aColor);
		// Strict bounds check: Skip drawing if the pixel is out of bounds
        if (aBegin.x >= 0 && aBegin.x < aSurface.get_width() &&
            aBegin.y >= 0 && aBegin.y < aSurface.get_height())
        {
            // Set the current pixel color
            aSurface.set_pixel_srgb(aBegin.x, aBegin.y, aColor);
        }
        else
        {
            printf("Pixel out of bounds during drawing: aX = %f, aY = %f, mWidth = %d, mHeight = %d\n", aBegin.x, aBegin.y, aSurface.get_width(), aSurface.get_height());
            return; // Stop drawing if we encounter out-of-bounds pixels
        }

		// Check if the endpoint has been reached
		if (aBegin.x == aEnd.x && aBegin.y == aEnd.y)
		{
			break;
		}

		// Step wise error calculation for keeping calculations in integer space
		int error2 = error * 2; 
		// Check which direction to move in
		if (error2 >= dy)
		{
			error += dy;
			aBegin.x += sx;
		}
		if (error2 <= dx)
		{
			error += dx;
			aBegin.y += sy;
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
       
    printf("Clipping started, Region code for aBegin: %d, Region code for aEnd: %d\n", beginCode, endCode);

	float dx = aEnd.x - aBegin.x;
	float dy = aEnd.y - aBegin.y;

	for (;;)
	{
		// printf("entered the cohen sutherland with begin = %d, end = %d", beginCode, endCode);

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

		float x, y;
		// Choose the point that is outside the window.
        int codeOut = (beginCode != 0) ? beginCode : endCode;
		
		if (codeOut & 1) // left boundary - if bit 1 is "1"
		{
			x = aMin.x;
			y = (dx != 0) ? aBegin.y + (dy / dx) * (aMin.x - aBegin.x) : aBegin.y;		}

		else if (codeOut & 2) // Right boundary - if bit 2 is "1"
		{
			x = aMax.x;
			y = (dx != 0) ? aBegin.y + (dy / dx) * (aMax.x - aBegin.x) : aBegin.y;
		}
		
		else if (codeOut & 4) // Bottom boundary - if bit 3 is "1"
		{
			y = aMin.y;
			x = (dy != 0) ? aBegin.x + (dx / dy) * (aMin.y - aBegin.y) : aBegin.x;
		}
		
		else if (codeOut & 8) // Top boundary - if bit 4 is "1"
		{
			y = aMax.y;
			x = (dy != 0) ? aBegin.x + (dx / dy) * (aMax.y - aBegin.y) : aBegin.x;
		}

		if (codeOut == beginCode) 
		{
			aBegin.x = std::clamp(x, aMin.x, aMax.x);
            aBegin.y = std::clamp(y, aMin.y, aMax.y);	
			// aBegin.x = x;
            // aBegin.y = y;
            beginCode = get_point_region_code(aBegin, aMin, aMax);  // Recalculate the region code
        } 
		else 
		{
			aEnd.x = std::clamp(x, aMin.x, aMax.x);
            aEnd.y = std::clamp(y, aMin.y, aMax.y);
            // aEnd.x = x;
            // aEnd.y = y;
            endCode = get_point_region_code(aEnd, aMin, aMax);  // Recalculate the region code
		}
        printf("After clipping, aBegin: (%f, %f), aEnd: (%f, %f)\n", aBegin.x, aBegin.y, aEnd.x, aEnd.y);

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
	// printf("point region code is %d", code);
	return code;
}
