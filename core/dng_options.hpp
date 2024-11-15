/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (C) 2022 - 2024, Csaba Nagy.
 * Copyright (C) 2024, Project Simon, Martin TOUZOT.
 *
 * Based on raw_options.cpp by Csaba Nagy
 *  
 * dng_options.hpp - DNG encoder options.
 */

#pragma once

#include <cstdio>

#include "video_options.hpp"

struct DNGOptions : public VideoOptions
{   
    DNGOptions() : VideoOptions()
	{
		using namespace boost::program_options;
		options_.add_options();
	}

	std::string mediaDest;
	std::string folder;

	int thumbnail;

	uint16_t rawCrop[4];

	std::string model;
	std::optional<std::string> ucm;

};