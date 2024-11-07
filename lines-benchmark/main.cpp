#include <benchmark/benchmark.h>

#include "../draw2d/draw.hpp"
#include "../draw2d/draw-ex.hpp"
#include "../draw2d/surface-ex.hpp"

namespace {

    void benchmark_bresenham(benchmark::State& state) {
		auto width = std::uint32_t(state.range(0));
        auto height = std::uint32_t(state.range(1));
        SurfaceEx surface(width, height);
		surface.clear();
		
        ColorU8_sRGB color{255, 255, 255}; 

        Vec2f start(state.range(2), state.range(3));
        Vec2f end(state.range(4), state.range(5));

        for (auto _ : state) 
		{
            draw_line_solid(surface, start, end, color);
            benchmark::ClobberMemory();
        }
    }

    void benchmark_dda(benchmark::State& state) {
		auto width = std::uint32_t(state.range(0));
        auto height = std::uint32_t(state.range(1));
        SurfaceEx surface(width, height);
		surface.clear();    

        ColorU8_sRGB color{255, 255, 255}; 

        Vec2f start(state.range(2), state.range(3));
        Vec2f end(state.range(4), state.range(5));

        for (auto _ : state) 
		{
            draw_ex_line_solid(surface, start, end, color);
            benchmark::ClobberMemory();
        }
    }
}

BENCHMARK(benchmark_bresenham)
	->Args({320, 240, 0, 120, 319, 120})  // Horizontal, small
	->Args({320, 240, 320, 0, 320, 239}) // Vertical, small
	->Args({320, 240, 0, 0, 319, 239}) // Diagonal, small
	->Args({1920, 1080, 0, 540, 1919, 540})  // Horizontal, Full HD
	->Args({1920, 1080, 960, 0, 960, 1079}) // Vertical, Full HD
	->Args({1920, 1080, 0, 0, 1919, 1079}); // Diagonal, Full HD

BENCHMARK(benchmark_dda)
	->Args({320, 240, 0, 120, 319, 120})  // Horizontal, small
	->Args({320, 240, 320, 0, 320, 239}) // Vertical, small
	->Args({320, 240, 0, 0, 319, 239}) // Diagonal, small
	->Args({1920, 1080, 0, 540, 1919, 540})  // Horizontal, Full HD
	->Args({1920, 1080, 960, 0, 960, 1079}) // Vertical, Full HD
	->Args({1920, 1080, 0, 0, 1919, 1079}); // Diagonal, Full HD

BENCHMARK_MAIN();