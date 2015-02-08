/**
 * @file
 * @brief Unit tests for openshot::Timeline
 * @author Jonathan Thomas <jonathan@openshot.org>
 *
 * @section LICENSE
 *
 * Copyright (c) 2008-2014 OpenShot Studios, LLC
 * <http://www.openshotstudios.com/>. This file is part of
 * OpenShot Library (libopenshot), an open-source project dedicated to
 * delivering high quality video editing and animation solutions to the
 * world. For more information visit <http://www.openshot.org/>.
 *
 * OpenShot Library (libopenshot) is free software: you can redistribute it
 * and/or modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * OpenShot Library (libopenshot) is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with OpenShot Library. If not, see <http://www.gnu.org/licenses/>.
 */

#include "UnitTest++.h"
#include "../include/OpenShot.h"

using namespace std;
using namespace openshot;

TEST(Timeline_Constructor)
{
	// Create a default fraction (should be 1/1)
	Fraction fps(30000,1000);
	Timeline t1(640, 480, fps, 44100, 2);

	// Check values
	CHECK_EQUAL(640, t1.info.width);
	CHECK_EQUAL(480, t1.info.height);

	// Create a default fraction (should be 1/1)
	Timeline t2(300, 240, fps, 44100, 2);

	// Check values
	CHECK_EQUAL(300, t2.info.width);
	CHECK_EQUAL(240, t2.info.height);
}

TEST(Timeline_Width_and_Height_Functions)
{
	// Create a default fraction (should be 1/1)
	Fraction fps(30000,1000);
	Timeline t1(640, 480, fps, 44100, 2);

	// Check values
	CHECK_EQUAL(640, t1.info.width);
	CHECK_EQUAL(480, t1.info.height);

	// Set width
	t1.info.width = 600;

	// Check values
	CHECK_EQUAL(600, t1.info.width);
	CHECK_EQUAL(480, t1.info.height);

	// Set height
	t1.info.height = 400;

	// Check values
	CHECK_EQUAL(600, t1.info.width);
	CHECK_EQUAL(400, t1.info.height);
}

TEST(Timeline_Framerate)
{
	// Create a default fraction (should be 1/1)
	Fraction fps(24,1);
	Timeline t1(640, 480, fps, 44100, 2);

	// Check values
	CHECK_CLOSE(24.0f, t1.info.fps.ToFloat(), 0.00001);
}

TEST(Timeline_Check_Two_Track_Video)
{
	// Create a reader
	Clip clip_video("../../src/examples/test.mp4");
	clip_video.Layer(0);
	clip_video.Position(0.0);

	Clip clip_overlay("../../src/examples/front3.png");
	clip_overlay.Layer(1);
	clip_overlay.Position(0.05); // Delay the overlay by 0.05 seconds
	clip_overlay.End(0.5);	// Make the duration of the overlay 1/2 second

	// Create a timeline
	Timeline t(640, 480, Fraction(30, 1), 44100, 2);

	// Add clips
	t.AddClip(&clip_video);
	t.AddClip(&clip_overlay);

	// Open Timeline
	t.Open();

	// Get frame
	tr1::shared_ptr<Frame> f = t.GetFrame(1);

	// Get the image data
	const Magick::PixelPacket* pixels = f->GetPixels(200);

	// Check image properties on scanline, column 230
	CHECK_EQUAL(5397, pixels[230].red);
	CHECK_EQUAL(0, pixels[230].blue);
	CHECK_EQUAL(49087, pixels[230].green);
	CHECK_EQUAL(0, pixels[230].opacity);

	// Get frame
	f = t.GetFrame(2);

	// Get scanline 190 of pixels
	pixels = f->GetPixels(190);

	// Check image properties on scanline, column 230
	CHECK_EQUAL(64764, pixels[230].red);
	CHECK_EQUAL(63993, pixels[230].blue);
	CHECK_EQUAL(64764, pixels[230].green);
	CHECK_EQUAL(0, pixels[230].opacity);

	// Get frame
	f = t.GetFrame(3);

	// Get scanline 190 of pixels
	pixels = f->GetPixels(190);

	// Check image properties on scanline, column 230
	CHECK_EQUAL(64771, pixels[230].red);
	CHECK_EQUAL(63429, pixels[230].blue);
	CHECK_EQUAL(64193, pixels[230].green);
	CHECK_EQUAL(0, pixels[230].opacity);


	// Get frame
	f = t.GetFrame(24);

	// Get scanline 190 of pixels
	pixels = f->GetPixels(190);

	// Check image properties on scanline, column 230
	CHECK_EQUAL(64507, pixels[230].red);
	CHECK_EQUAL(63736, pixels[230].blue);
	CHECK_EQUAL(64507, pixels[230].green);
	CHECK_EQUAL(0, pixels[230].opacity);

	// Get frame
	f = t.GetFrame(5);

	// Get scanline 190 of pixels
	pixels = f->GetPixels(190);

	// Check image properties on scanline, column 230
	CHECK_EQUAL(6437, pixels[230].red);
	CHECK_EQUAL(0, pixels[230].blue);
	CHECK_EQUAL(48399, pixels[230].green);
	CHECK_EQUAL(0, pixels[230].opacity);

	// Get frame
	f = t.GetFrame(25);

	// Get scanline 190 of pixels
	pixels = f->GetPixels(190);

	// Check image properties on scanline, column 230
	CHECK_EQUAL(5397, pixels[230].red);
	CHECK_EQUAL(0, pixels[230].blue);
	CHECK_EQUAL(49087, pixels[230].green);
	CHECK_EQUAL(0, pixels[230].opacity);

	// Get frame
	f = t.GetFrame(4);

	// Get scanline 190 of pixels
	pixels = f->GetPixels(190);

	// Check image properties on scanline, column 230
	CHECK_EQUAL(64771, pixels[230].red);
	CHECK_EQUAL(63429, pixels[230].blue);
	CHECK_EQUAL(64193, pixels[230].green);
	CHECK_EQUAL(0, pixels[230].opacity);

	// Close reader
	t.Close();
}
