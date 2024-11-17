/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (C) 2021, Raspberry Pi (Trading) Ltd.
 * Copyright (C) 2022 - 2024, Csaba Nagy.
 * Copyright (C) 2024, Project Simon, Martin TOUZOT.
 * 
 */
#pragma once

#include <string>
#include <fstream>

const std::string &PinotonCamAppVersion();
const std::string &PinotonCamAppCapabilities();
void getHwInfo(std::string &serial, std::string &model);