#pragma once

struct PlanetData {
    // all units are relative to Earth.FOO = 1;
    const char* name;
    double diameter, distance_from_sun_au, orbital_period;
    double starting_angle; // assume radians
                           // float obliquity_to_orbit=1;
                           // planet_colour colour ;
                           // bool rings=false;
    const char* colour;
};

struct Sun {
    double diameter;
};

// TODO
// obliquity to orbit

constexpr int PLANET_COUNT = 9;
extern const PlanetData planet_data[PLANET_COUNT];
