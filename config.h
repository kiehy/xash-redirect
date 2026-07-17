#pragma once

#include <string>
#include <cstdint>

struct ServerAddress
{
    std::string ip = "127.0.0.1";
    uint16_t port = 27015;
};

struct Config
{
    bool enabled = true;

    ServerAddress destination;

    float delay = 0.0f;

    bool debug = true;
};

extern Config g_Config;

bool Config_Load(const char *filename);
