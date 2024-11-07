#include <benchmark/benchmark.h>

#include <algorithm>

#include <cassert>

#include "../draw2d/image.hpp"
#include "../draw2d/draw-ex.hpp"
#include "../draw2d/surface-ex.hpp"

// Storing all image paths in a global string vector
static const std::vector<std::string> imagePaths = {
    "assets/earth.png", 
    "assets/earth128.png", 
    "assets/earth1024.png"
};

namespace
{
	void default_blit_earth_( benchmark::State& aState )
	{
		auto const width = std::uint32_t(aState.range(0));
		auto const height = std::uint32_t(aState.range(1));
    	auto imageIndex = aState.range(2);
		SurfaceEx surface( width, height );
		surface.clear();

		auto source = load_image( imagePaths[imageIndex].c_str() );
		assert( source );

		for( auto _ : aState )
		{
			blit_masked( surface, *source, {500.f, 500.f} );
			benchmark::ClobberMemory(); 
		}

		auto const maxBlitX = std::min( width, source->get_width() );
		auto const maxBlitY = std::min( height, source->get_height() );

		aState.SetBytesProcessed( 2*maxBlitX*maxBlitY*4 * aState.iterations() );
	}


	void ex_solid_blit_earth_( benchmark::State& aState )
	{
		auto const width = std::uint32_t(aState.range(0));
		auto const height = std::uint32_t(aState.range(1));
    	auto imageIndex = aState.range(2);
		SurfaceEx surface( width, height );
		surface.clear();

		auto source = load_image( imagePaths[imageIndex].c_str() );
		assert( source );

		for (auto _ : aState) 
		{
			blit_ex_solid( surface, *source, { static_cast<float>(width) / 2, static_cast<float>(height) / 2} );
			benchmark::ClobberMemory();
		}

		auto const maxBlitX = std::min( width, source->get_width() );
		auto const maxBlitY = std::min( height, source->get_height() );

		aState.SetBytesProcessed( 2*maxBlitX*maxBlitY*4 * aState.iterations() );
    }

	void ex_memcpy_blit_earth_( benchmark::State& aState )
	{
		auto const width = std::uint32_t(aState.range(0));
		auto const height = std::uint32_t(aState.range(1));
    	auto imageIndex = aState.range(2);
		SurfaceEx surface( width, height );
		surface.clear();

		auto source = load_image( imagePaths[imageIndex].c_str() );
		assert( source );

		for (auto _ : aState) 
		{
			blit_ex_memcpy( surface, *source, { static_cast<float>(width) / 2, static_cast<float>(height) / 2} );
			benchmark::ClobberMemory();
		}

		auto const maxBlitX = std::min( width, source->get_width() );
		auto const maxBlitY = std::min( height, source->get_height() );

		aState.SetBytesProcessed( 2*maxBlitX*maxBlitY*4 * aState.iterations() );
    }

}

BENCHMARK( default_blit_earth_ )
	->ArgsProduct( { {320, 1280, 1920, 7680}, {240, 720, 1080, 4320}, {0, 1, 2} } );

BENCHMARK( ex_solid_blit_earth_ )
	->ArgsProduct( { {320, 1280, 1920, 7680}, {240, 720, 1080, 4320}, {0, 1, 2} } );

BENCHMARK( ex_memcpy_blit_earth_ )
	->ArgsProduct( { {320, 1280, 1920, 7680}, {240, 720, 1080, 4320}, {0, 1, 2} } );

BENCHMARK_MAIN();
