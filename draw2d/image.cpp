#include "image.hpp"

#include <memory>
#include <algorithm>

#include <cstdio>
#include <cstring>
#include <cassert>

#include <stb_image.h>

#include "surface.hpp"

#include "../support/error.hpp"

namespace
{
	struct STBImageRGBA_ : public ImageRGBA
	{
		STBImageRGBA_( Index, Index, std::uint8_t* );
		virtual ~STBImageRGBA_();
	};
}

ImageRGBA::ImageRGBA()
	: mWidth( 0 )
	, mHeight( 0 )
	, mData( nullptr )
{}

ImageRGBA::~ImageRGBA() = default;


std::unique_ptr<ImageRGBA> load_image( char const* aPath )
{
	assert( aPath );

	stbi_set_flip_vertically_on_load( true );

	int w, h, channels;
	stbi_uc* ptr = stbi_load( aPath, &w, &h, &channels, 4 );
	if( !ptr )
		throw Error( "Unable to load image \"%s\"", aPath );

	return std::make_unique<STBImageRGBA_>(
		ImageRGBA::Index(w),
		ImageRGBA::Index(h),
		ptr
	);
}

void blit_masked( Surface& aSurface, ImageRGBA const& aImage, Vec2f aPosition )
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

			if (imagePixel.a >= 128) // Showing only input pixels with alpha value greater or equal to 128
			{
				ColorU8_sRGB aColor{imagePixel.r, imagePixel.g, imagePixel.b};
				aSurface.set_pixel_srgb(x, y, aColor);
			}
		}
	}
}

namespace
{
	STBImageRGBA_::STBImageRGBA_( Index aWidth, Index aHeight, std::uint8_t* aPtr )
	{
		mWidth = aWidth;
		mHeight = aHeight;
		mData = aPtr;
	}

	STBImageRGBA_::~STBImageRGBA_()
	{
		if( mData )
			stbi_image_free( mData );
	}
}
