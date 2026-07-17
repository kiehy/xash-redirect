#pragma once

#include <string>

struct Config
{
    bool enabled = true;

    std::string destination = "127.0.0.1:27015";

    float delay = 0.0f;

    bool debug = true;
};

extern Config g_Config;

bool Config_Load(const char *filename);
