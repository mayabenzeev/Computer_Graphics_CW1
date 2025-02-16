inline
ColorU8_sRGB_Alpha ImageRGBA::get_pixel( Index aX, Index aY ) const
{
	assert( aX < mWidth && aY < mHeight ); // Leave this at the top of the function.

	Index pixelLinearIndex = get_linear_index(aX, aY); // Get the memorty location of the pixel at (aX, aY)
	// Get the pixel components from mData 
	return {mData[pixelLinearIndex + 0],
			mData[pixelLinearIndex + 1],
			mData[pixelLinearIndex + 2],
			mData[pixelLinearIndex + 3]}; 
}

inline
auto ImageRGBA::get_width() const noexcept -> Index
{
	return mWidth;
}
inline
auto ImageRGBA::get_height() const noexcept -> Index
{
	return mHeight;
}

inline
std::uint8_t* ImageRGBA::get_image_ptr() noexcept
{
	return mData;
}
inline
std::uint8_t const* ImageRGBA::get_image_ptr() const noexcept
{
	return mData;
}

inline
ImageRGBA::Index ImageRGBA::get_linear_index( Index aX, Index aY ) const noexcept
{
	return (aY * mWidth + aX) * 4; // Compute the linear index of pixel (aX,aY)
}
