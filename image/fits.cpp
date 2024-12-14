/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (C) 2024, Project Simon, Martin TOUZOT.
 *  
 * fits.cpp - Save raw image as FITS file.
 */

#include <libcamera/control_ids.h>
#include <libcamera/formats.h>

#include "fitsio.h"

#include "core/still_options.hpp"
#include "core/stream_info.hpp"

#include "image.hpp"

#ifndef MAKE_STRING
#define MAKE_STRING "Raspberry Pi"
#endif

using namespace libcamera;

void unpack_16bit(uint8_t const *src, StreamInfo const &info, uint16_t *dest)
{
	/* Assume the pixels in memory are already in native byte order */
	unsigned int w = info.width;
	for (unsigned int y = 0; y < info.height; y++)
	{
		memcpy(dest, src, 2 * w);
		dest += w;
		src += info.stride;
	}
}

void fits_save(std::vector<libcamera::Span<uint8_t>> const &mem, StreamInfo const &info,
			  libcamera::ControlList const &metadata, std::string const &filename, std::string const &cam_model,
			  StillOptions const *options)
{
	fitsfile *fptr;
	int status=0, fpixel = 1, naxis = 2;
	std::string fits_filename = filename.at(0) == '!' ? filename : '!' + filename;
	fits_create_file(&fptr, fits_filename.c_str(), &status); /* create new file */

	unsigned int buf_stride_pixels = info.width;
	unsigned int buf_stride_pixels_padded = (buf_stride_pixels + 7) & ~7;
	long naxes[2] = { info.height, buf_stride_pixels_padded};

	/* Create the primary array image (16-bit short integer pixels) */
	std::vector<uint16_t> buf(buf_stride_pixels_padded * info.height, 0);

	unpack_16bit(mem[0].data(), info, &buf[0]);

	fits_create_img(fptr, SHORT_IMG, naxis, (long*)naxes, &status);

	// Metadatas
	std::time_t raw_time;
	std::time(&raw_time);
	char time_string[32];
	std::tm *time_info = std::localtime(&raw_time);
	std::strftime(time_string, sizeof(time_string), "%Y-%m-%dT%H:%M:%S", time_info);
	fits_write_key(fptr, TSTRING, "DATE", &time_string, "File creation date", &status);

	auto exp = metadata.get(controls::ExposureTime);
	float exp_time = 10000;
	if (exp)
		exp_time = *exp;
	else
		LOG_ERROR("WARNING: default to exposure time of " << exp_time << "us");
	exp_time /= 1e6;
	fits_write_key(fptr, TFLOAT, "EXPOSURE", &exp_time, "Total Exposure Time", &status);

	fits_write_key(fptr, TSTRING, "INSTRUME", (void*)cam_model.c_str(), "Camera model", &status);
	
	// Image writing
	if (fits_write_img(fptr, TSHORT, fpixel, naxes[0] * naxes[1], buf.data(), &status))
		LOG(1, status);

	fits_close_file(fptr, &status);
	fits_report_error(stderr, status); /* print out any error messages */
}