#include "planets.h"

// data source
// https://nssdc.gsfc.nasa.gov/planetary/factsheet/planet_table_ratio.html
// MERCURY  	 VENUS  	 EARTH  	 MOON  	 MARS  	 JUPITER
// SATURN  	 URANUS  	 NEPTUNE  	 PLUTO 0.383 	0.949 	1
// 0.2724 	0.532 	11.21 	9.45 	4.01 	3.88 	0.187 0.387 	0.723 1
// 0.00257* 	1.52 	5.20 	9.57 	19.17 	30.18 	39.48 0.241 	0.615 1
// 0.0748* 	1.88 	11.9 	29.4 	83.7 	163.7 	247.9

const PlanetData planet_data[PLANET_COUNT] = {
    // Name  diameter  distance_au  period  starting angle, colour name
    {"Mercury", 0.383f, 0.387f, 0.241f, 4.7f, "BROWN"}, // mercury
    {"Venus", 0.949f, 0.723f, 0.615f, 6.2f, "BEIGE"},   // venus
    {"Earth", 1.0f, 1.0f, 1.0f, 0.2f, "BLUE"},          // earth
    // {"Moon", 0.2724f,  0.00257f,  0.0748f, 1.5f, "LIGHTGRAY"},  // moon*
    {"Mars", 0.532f, 1.52f, 1.88f, 1.5f, "RED"},         // mars
    {"Jupiter", 11.21f, 5.20f, 11.9f, 2.3f, "ORANGE"},   // jupiter
    {"Saturn", 9.45f, 9.57f, 29.4f, 0.2f, "MAROON"},     // saturn
    {"Uranus", 4.01f, 19.17f, 83.7f, 1.1f, "DARKBLUE"},  // uranus
    {"Neptune", 3.88f, 30.18f, 163.7f, 0.1f, "SKYBLUE"}, // neptune
    {"Pluto", 0.187f, 39.48f, 247.9f, 5.3f, "GRAY"},     // pluto
};