#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <cstdio>

struct Config {
    int         screenW          = 1280;
    int         screenH          = 720;
    bool        fullscreen       = false;
    float       mouseSensitivity = 0.002f;
    float       masterVolume     = 1.f;
    float       musicVolume      = 0.7f;
    float       sfxVolume        = 1.f;
    std::string playerName       = "Operative";
    bool        vsync            = true;
    bool        showFPS          = false;
};

class ConfigManager {
public:
    static Config& get() { return instance_; }

    static bool loadFromFile(const std::string& path) {
        std::ifstream f(path);
        if (!f.is_open()) return false;
        std::string line;
        while (std::getline(f, line)) {
            if (line.empty() || line[0] == '#') continue;
            std::istringstream ss(line);
            std::string key, val;
            if (std::getline(ss, key, '=') && std::getline(ss, val)) {
                Config& c = instance_;
                if (key == "screenW")          c.screenW          = std::stoi(val);
                else if (key == "screenH")     c.screenH          = std::stoi(val);
                else if (key == "fullscreen")  c.fullscreen       = (val == "1" || val == "true");
                else if (key == "sensitivity") c.mouseSensitivity = std::stof(val);
                else if (key == "masterVol")   c.masterVolume     = std::stof(val);
                else if (key == "musicVol")    c.musicVolume      = std::stof(val);
                else if (key == "sfxVol")      c.sfxVolume        = std::stof(val);
                else if (key == "playerName")  c.playerName       = val;
                else if (key == "vsync")       c.vsync            = (val == "1" || val == "true");
                else if (key == "showFPS")     c.showFPS          = (val == "1" || val == "true");
            }
        }
        return true;
    }

    static bool saveToFile(const std::string& path) {
        std::ofstream f(path);
        if (!f.is_open()) return false;
        const Config& c = instance_;
        f << "screenW="     << c.screenW          << "\n"
          << "screenH="     << c.screenH          << "\n"
          << "fullscreen="  << (c.fullscreen ? "1" : "0") << "\n"
          << "sensitivity=" << c.mouseSensitivity  << "\n"
          << "masterVol="   << c.masterVolume      << "\n"
          << "musicVol="    << c.musicVolume       << "\n"
          << "sfxVol="      << c.sfxVolume         << "\n"
          << "playerName="  << c.playerName        << "\n"
          << "vsync="       << (c.vsync ? "1" : "0") << "\n"
          << "showFPS="     << (c.showFPS ? "1" : "0") << "\n";
        return true;
    }

private:
    static Config instance_;
};
