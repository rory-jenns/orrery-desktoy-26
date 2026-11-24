#define _USE_MATH_DEFINES
#include "planets.h"
#include "raylib.h"

#include <cmath>
#include <iostream>
#include <stdlib.h>
#include <string.h>

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif


const double UPDATE_ANGLE_INCREMENT = 0.003f;

const double MAX_DIST = 2.0f; // Cut off at Mars
// const double MAX_DIST = 10.0f; // Cut off at Saturn.
// const double MAX_DIST = 40.0f; // To include all planets

const int WINDOW_HEIGHT = 800;
const int WINDOW_WIDTH = 800;

const int SCREEN_SHIFT_X = (WINDOW_WIDTH / 2);
const int SCREEN_SHIFT_Y = (WINDOW_HEIGHT / 2);

const double SCREEN_SCALE_X = ((double)WINDOW_WIDTH / 2) / MAX_DIST;
const double SCREEN_SCALE_Y = ((double)WINDOW_HEIGHT / 2) / MAX_DIST;

const int SCREEN_DIAMETER_SCALE = 15;

struct Coords {
    double x, y;
};

struct GamePlanet {
    const char* name;
    Coords coords;
    double angle_to_sun;
    double diameter, distance_from_sun_au, orbital_period;
    Color colour;

    Coords calculate_coords() {
        double x = std::cos(this->angle_to_sun) * this->distance_from_sun_au;
        double y = std::sin(this->angle_to_sun) * this->distance_from_sun_au;
        return (Coords){x, y};
    }

    void update_coords() { coords = this->calculate_coords(); }

    void update_angle() {
        this->angle_to_sun += UPDATE_ANGLE_INCREMENT / this->orbital_period;
        if (this->angle_to_sun > 2 * M_PI) {
            this->angle_to_sun -= 2 * M_PI;
        }
    }

    void update() {
        this->update_angle();
        this->update_coords();
    }
};

// GameModel: pure data, no raylib drawing or input
struct GameModel {
    GamePlanet planets[PLANET_COUNT];

    Color colour_lookup(const char* colour) {
        const int MAX_COLORS_COUNT = 21;
        Color colors[MAX_COLORS_COUNT] = {
            DARKGRAY,  MAROON, ORANGE, DARKGREEN, DARKBLUE, DARKPURPLE, DARKBROWN,
            GRAY,      RED,    GOLD,   LIME,      BLUE,     VIOLET,     BROWN,
            LIGHTGRAY, PINK,   YELLOW, GREEN,     SKYBLUE,  PURPLE,     BEIGE};

        const char* colorNames[MAX_COLORS_COUNT] = {
            "DARKGRAY",  "MAROON", "ORANGE", "DARKGREEN", "DARKBLUE", "DARKPURPLE", "DARKBROWN",
            "GRAY",      "RED",    "GOLD",   "LIME",      "BLUE",     "VIOLET",     "BROWN",
            "LIGHTGRAY", "PINK",   "YELLOW", "GREEN",     "SKYBLUE",  "PURPLE",     "BEIGE"};
        for (int i = 0; i < MAX_COLORS_COUNT; i++) {
            if (strcmp(colour, colorNames[i]) == 0) {
                return colors[i];
            }
        }
        return PINK; // hopefully very obvious default / not-found
    }

    void setup() {
        for (int i = 0; i < PLANET_COUNT; i++) {
            GamePlanet& p = this->planets[i];
            p.name = planet_data[i].name;
            p.diameter = planet_data[i].diameter;
            p.distance_from_sun_au = planet_data[i].distance_from_sun_au;
            p.orbital_period = planet_data[i].orbital_period;
            p.angle_to_sun = planet_data[i].starting_angle;
            p.update_coords();
            p.colour = colour_lookup(planet_data[i].colour);
        }
    }
};

// Controller: reads input, updates the model
struct Controller {
    void update(GameModel& m, float dt) {
        for (int i = 0; i < PLANET_COUNT; i++) {
            GamePlanet& p = m.planets[i];
            p.update();
        }
    }
};

// View: reads the model, draws it
struct View {
    Coords relpos(Coords in_coords) {
        // the coords (0, 0) should be centre screen
        return {(in_coords.x * SCREEN_SCALE_X) + SCREEN_SHIFT_X,
                (in_coords.y * SCREEN_SCALE_Y) + SCREEN_SHIFT_Y};
    }

    double scale_diameter(double diameter) {
        return std::sqrt(std::sqrt(diameter)) * SCREEN_DIAMETER_SCALE;
    }

    void draw(const GameModel& m) {
        BeginDrawing();
        // Draw Background
        ClearBackground(BLACK);

        srand(253); // draw a random set of the same individual stars
        for (int i = 0; i < 500; i++) {
            int star_size = 2;
            DrawCircle(rand() % WINDOW_WIDTH, rand() % WINDOW_HEIGHT, star_size, LIGHTGRAY);
        }

        // Draw Planets
        for (int i = 0; i < PLANET_COUNT; i++) {
            GamePlanet p = m.planets[i];
            Coords screen_coords = relpos(p.coords);
            double draw_size = scale_diameter(p.diameter);

            DrawText(TextFormat("P#%d: %s", i + 1, p.name), (int)screen_coords.x - 10,
                     (int)screen_coords.y + 20, 20, PURPLE);

            // printf("Drawing... %d, %d\n", (int)screen_coords.x, (int)screen_coords.y);

            DrawCircle((int)screen_coords.x, (int)screen_coords.y, draw_size, p.colour);
        }

        // Draw Sun
        Coords screen_coords = relpos((Coords){0.0, 0.0}); // Draw in the centre
        double draw_size = scale_diameter(5);

        // printf("Drawing... %d, %d\n", (int)screen_coords.x, (int)screen_coords.y);

        DrawCircle((int)screen_coords.x, (int)screen_coords.y, draw_size, YELLOW);

        EndDrawing();
    }
};

static GameModel model;
static Controller controller;
static View view;

static void frame() {
    controller.update(model, GetFrameTime());
    view.draw(model);
}

int main() {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Orrery");
    SetTargetFPS(60);
    model.setup();

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(frame, 0, 1);
#else
    while (!WindowShouldClose()) {frame();}
#endif

    CloseWindow();
}