#include "config.h"
#include "util.h"

#include <fstream>
#include <sstream>
#include <algorithm>

Config g_Config;

static std::string Trim(std::string s)
{
    s.erase(0, s.find_first_not_of(" \t\r\n"));
    s.erase(s.find_last_not_of(" \t\r\n") + 1);
    return s;
}

bool Config_Load(const char *filename)
{
    std::ifstream file(filename);

    if (!file.is_open())
    {
        Log("Could not open %s", filename);
        return false;
    }

    std::string line;

    while (std::getline(file, line))
    {
        line = Trim(line);

        if (line.empty())
            continue;

        if (line[0] == '#')
            continue;

        auto pos = line.find('=');

        if (pos == std::string::npos)
            continue;

        std::string key = Trim(line.substr(0, pos));
        std::string value = Trim(line.substr(pos + 1));

        if (key == "enabled")
            g_Config.enabled = (value == "1");

        else if (key == "destination")
            g_Config.destination = value;

        else if (key == "delay")
            g_Config.delay = std::stof(value);

        else if (key == "debug")
            g_Config.debug = (value == "1");
    }

    Log("Configuration loaded.");
    Log("Enabled: %s", g_Config.enabled ? "yes" : "no");
    Log("Destination: %s", g_Config.destination.c_str());
    Log("Delay: %.2f", g_Config.delay);
    Log("Debug: %s", g_Config.debug ? "yes" : "no");

    return true;
}
