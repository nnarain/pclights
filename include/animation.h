#ifndef ANIMATION_H
#define ANIMATION_H

#include <string>
#include <vector>
#include <fstream>
#include <thread>
#include <chrono>
#include <cstdint>

#include "lodepng.h"

template<typename ProtocolT>
class Animation
{
public:
	struct Pixel 
	{
		uint8_t r, g, b;

		bool operator==(const Pixel& p)
		{
			return r == p.r && g == p.g && b == p.b;
		}
	};

	struct PixelRun
	{
		uint8_t length;
		Pixel pixel;
	};

public:
	Animation(ProtocolT& comm, const std::string& filename, float frame_rate) :
		comm_(comm),
		frame_rate_(frame_rate)
	{
		loadPNG(filename);
	}

	void play()
	{
		unsigned int num_frames = height_;

		// calculate millisecond period for to get desired frame rate
		float frame_period_s = 1.0f / frame_rate_;
		std::chrono::milliseconds frame_period_ms((int)(frame_period_s * 1000));

		// loop for every frame
		for (unsigned int frame = 0; frame < num_frames; ++frame)
		{
			// RLE encode each frame and send driver
			std::vector<PixelRun> runs = encodeRow(frame);
			sendAnimation(runs);

			// delay desired period
			std::this_thread::sleep_for(frame_period_ms);
		}

	}

private:
	void sendAnimation(const std::vector<PixelRun>& runs)
	{
		std::vector<uint8_t> buffer;

		for (const PixelRun& run : runs)
		{
			buffer.push_back(run.length);
			buffer.push_back(run.pixel.r);
			buffer.push_back(run.pixel.g);
			buffer.push_back(run.pixel.b);
		}

		comm_.setFrame(&buffer[0], buffer.size());
	}

	std::vector<PixelRun> encodeRow(unsigned int row)
	{
		const unsigned int pixels_per_row = width_;

		std::vector<PixelRun> runs;
		
		PixelRun current_run;
		current_run.length = 1;
		current_run.pixel = getPixel(row, 0);

		for (unsigned int i = 1; i < pixels_per_row; ++i)
		{
			Pixel pixel = getPixel(row, i);

			// check if the pixel is the same 
			if (pixel == current_run.pixel)
			{
				// increment the current run length
				current_run.length++;
			}
			else
			{
				// the pixel color has changed, so append a run
				runs.push_back(current_run);

				// start a new run
				current_run.length = 1;
				current_run.pixel = pixel;
			}
		}

		runs.push_back(current_run);

		return std::move(runs);
	}

	Pixel getPixel(unsigned int row, unsigned int idx)
	{
		const unsigned int pixels_per_row = width_;
		const unsigned int pixel_size = 4; // RGBA

		unsigned int byte_offset = ((pixels_per_row * row) + idx) * pixel_size;

		Pixel p;
		p.r = image_[byte_offset + 0];
		p.g = image_[byte_offset + 1];
		p.b = image_[byte_offset + 2];

		return p;
	}

	void loadPNG(const std::string& filename)
	{
		if (lodepng::decode(image_, width_, height_, filename.c_str())) 
			throw std::runtime_error("Could not decode PNG file");
	}

private:
	ProtocolT& comm_;
	float frame_rate_;
	
	std::vector<unsigned char> image_;
	unsigned int width_, height_;
};

#endif // ANIMATION_H
