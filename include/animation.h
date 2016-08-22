#ifndef ANIMATION_H
#define ANIMATION_H

#include <string>
#include <vector>

#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io/png_dynamic_io.hpp>

#include <cstdint>

template<typename ProtocolT>
class Animation
{
public:
	using Image = boost::gil::rgba8_image_t;
	using View  = boost::gil::rgba8_view_t;
	using Pixel = boost::gil::rgba8_pixel_t;

	struct PixelRun
	{
		PixelRun(uint8_t l, uint8_t r, uint8_t g, uint8_t b) :
			length(l),
			r(r), g(g), b(b)
		{
		}

		uint8_t length;
		uint8_t r, g, b;
	};

public:
	Animation(ProtocolT& comm, const std::string& filename, float frame_rate) :
		comm_(comm),
		frame_rate_(frame_rate)
	{
	//	boost::gil::png_read_image(filename, image_);
	}

	void play()
	{
		std::vector<PixelRun> runs = encodePixels();
		sendAnimation(runs);
	}

private:
	void sendAnimation(const std::vector<PixelRun>& runs)
	{
		std::vector<uint8_t> buffer;

		for (const auto& run : runs)
		{
			buffer.push_back(run.length);
			buffer.push_back(run.r);
			buffer.push_back(run.g);
			buffer.push_back(run.b);
		}

		comm_.setFrame(&buffer[0], buffer.size());
	}

	std::vector<PixelRun> encodePixels()
	{
		std::vector<PixelRun> runs;

		runs.emplace_back(5, 255, 0, 0);
		runs.emplace_back(5, 0, 255, 0);
		runs.emplace_back(5, 0, 0, 255);

		return std::move(runs);
	}

private:
	ProtocolT& comm_;
	float frame_rate_;

	Image image_;
	
};

#endif // ANIMATION_H
