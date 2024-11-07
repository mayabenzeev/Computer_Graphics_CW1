#include "draw-ex.hpp"

#include <algorithm>

#include <cstring> // for std::memcpy()

#include "image.hpp"
#include "surface-ex.hpp"

void draw_ex_line_solid( SurfaceEx& aSurface, Vec2f aBegin, Vec2f aEnd, ColorU8_sRGB aColor )
{
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	//TODO: your implementation goes here

	//TODO: remove the following when you start your implementation
	(void)aSurface; // Avoid warnings about unused arguments until the function
	(void)aBegin;   // is properly implemented.
	(void)aEnd;
	(void)aColor;
}

void blit_ex_solid( SurfaceEx& aSurface, ImageRGBA const& aImage, Vec2f aPosition )
{
	// Starting position to draw on surface
    Vec2f startPosition = Vec2f{aPosition.x - (aImage.get_width() / 2), aPosition.y - (aImage.get_height() / 2)};
    int startY = std::max(0, static_cast<int>(startPosition.y));
    int startX = std::max(0, static_cast<int>(startPosition.x));

    // Ending position to draw on surface
    Vec2f endPosition = Vec2f{aImage.get_width() + startPosition.x, aImage.get_height() + startPosition.y};
	int endY = std::min(static_cast<int>(endPosition.y), static_cast<int>(aSurface.get_height()));
	int endX = std::min(static_cast<int>(endPosition.x), static_cast<int>(aSurface.get_width()));

	for (int y = startY; y < endY; y++) 
	{
        for (int x = startX; x < endX; x++) 
		{
            // Relative position of (x,y) in the image
            int imgXPos = x - static_cast<int>(startPosition.x);
            int imgYPos = y - static_cast<int>(startPosition.y);
            
			ColorU8_sRGB_Alpha imagePixel = aImage.get_pixel(imgXPos, imgYPos); // Get the pixel from aImage

            auto idx = aSurface.get_linear_index(x, y);
            std::uint8_t* pixelPtr = aSurface.get_surface_ptr() + idx;
            pixelPtr[0] = imagePixel.r; 
            pixelPtr[1] = imagePixel.g; 
            pixelPtr[2] = imagePixel.b; 
		}
	}
}

void blit_ex_memcpy( SurfaceEx& aSurface, ImageRGBA const& aImage, Vec2f aPosition )
{
	// Starting position to draw on surface
    Vec2f startPosition = Vec2f{aPosition.x - (aImage.get_width() / 2), aPosition.y - (aImage.get_height() / 2)};
	int startY = std::max(0, static_cast<int>(startPosition.y));
    int startX = std::max(0, static_cast<int>(startPosition.x));

	// Ending position to draw on surface
    Vec2f endPosition = Vec2f{aImage.get_width() + startPosition.x, aImage.get_height() + startPosition.y};
	int endY = std::min(static_cast<int>(endPosition.y), static_cast<int>(aSurface.get_height()));
	int endX = std::min(static_cast<int>(endPosition.x), static_cast<int>(aSurface.get_width()));

	int rowWidth = endX - startX;
    if (rowWidth > 0)  // If there are rows to copy
	{
        // Pointer to the start positions in the surface 
        std::uint8_t* surfacePtr = aSurface.get_surface_ptr() + aSurface.get_linear_index(startX, startY);
        // Pointer to the start positions in the image 
        const std::uint8_t* imagePtr = aImage.get_image_ptr() + aImage.get_linear_index(startX - static_cast<int>(startPosition.x), startY - static_cast<int>(startPosition.y));

        // Copy row blocks
        for (int y = 0; y < endY - startY; ++y) 
		{
            std::memcpy(surfacePtr, imagePtr, rowWidth * 4); // 4 bytes per pixel
			// Move to next line
            surfacePtr += aSurface.get_width() * 4; 
            imagePtr += aImage.get_width() * 4; 
        }
    }
}

