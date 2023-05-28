#pragma once

namespace WideScreenPatch {

enum class SDResolution : int {
    _640x480 = 0,
    _800x600,
    _1024x768,
    _1152x864,
    _1280x768,
    _1280x800,
    _1280x1024,
    _1440x900,
    _1600x1200,
    Max
};

constexpr std::pair<unsigned int, unsigned int> resolve_sd(SDResolution res) {
    switch (res) {
    case SDResolution::_640x480:
        return std::make_pair(640, 480);
    case SDResolution::_800x600:
        return std::make_pair(800, 600);
    case SDResolution::_1024x768:
        return std::make_pair(1024, 768);
    case SDResolution::_1152x864:
        return std::make_pair(1152, 864);
    case SDResolution::_1280x768:
        return std::make_pair(1280, 768);
    case SDResolution::_1280x800:
        return std::make_pair(1280, 800);
    case SDResolution::_1280x1024:
        return std::make_pair(1280, 1024);
    case SDResolution::_1440x900:
        return std::make_pair(1440, 900);
    case SDResolution::_1600x1200:
        return std::make_pair(1600, 1200);
    }
    return std::make_pair(640, 480);
}

constexpr std::pair<unsigned int, unsigned int> resolve_hd(SDResolution res) {
    switch (res) {
    case SDResolution::_640x480:
        return std::make_pair(960, 540);
    case SDResolution::_800x600:
        return std::make_pair(1024, 576);
    case SDResolution::_1024x768:
        return std::make_pair(1280, 720);
    case SDResolution::_1152x864:
        return std::make_pair(1366, 768);
    case SDResolution::_1280x768:
        return std::make_pair(1600, 900);
    case SDResolution::_1280x800:
        return std::make_pair(1920, 1080);
    case SDResolution::_1280x1024:
        return std::make_pair(2560, 1440);
    case SDResolution::_1440x900:
        return std::make_pair(3200, 1800);
    case SDResolution::_1600x1200:
        return std::make_pair(3840, 2160);
    }
    return std::make_pair(1280, 720);
}
void install();
}; // namespace WideScreenPatch