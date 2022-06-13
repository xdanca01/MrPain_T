// ################################################################################
// Common Framework for Computer Graphics Courses at FI MUNI.
// 
// Copyright (c) 2021-2022 Visitlab (https://visitlab.fi.muni.cz)
// All rights reserved.
// ################################################################################

#pragma once

#include <cstdint>

// Diamond
const float diamond_positions[] = {
    -0.75f, 0.5f, 0.6f, //
    -0.5f, 0.75f, 0.6f, //
    -0.5f, 0.5f, 0.6f,  //
    //
    -0.5f, 0.75f, 0.6f, //
    -0.25f, 0.5f, 0.6f, //
    -0.5f, 0.5f, 0.6f,  //
    //
    -0.5f, 0.5f, 0.6f,  //
    -0.25f, 0.5f, 0.6f, //
    -0.5f, 0.25f, 0.6f, //
    //
    -0.5f, 0.5f, 0.6f,  //
    -0.5f, 0.25f, 0.6f, //
    -0.75f, 0.5f, 0.6f, //
};

const float diamond_colors[] = {
    1.0f, 1.0f, 1.0f, 1.0f, //
    1.0f, 1.0f, 1.0f, 1.0f, //
    1.0f, 1.0f, 1.0f, 1.0f, //
    1.0f, 1.0f, 1.0f, 1.0f, //
    1.0f, 1.0f, 1.0f, 1.0f, //
    1.0f, 1.0f, 1.0f, 1.0f, //
    1.0f, 1.0f, 1.0f, 1.0f, //
    1.0f, 1.0f, 1.0f, 1.0f, //
    1.0f, 1.0f, 1.0f, 1.0f, //
    1.0f, 1.0f, 1.0f, 1.0f, //
    1.0f, 1.0f, 1.0f, 1.0f, //
    1.0f, 1.0f, 1.0f, 1.0f, //
};

// Square
const float square_data[] = {
    // positions
    0.0, 0.25, 0.5f, //
    0.5, 0.25, 0.5f, //
    0.5, 0.75, 0.5f, //
    0.0, 0.25, 0.5f, //
    0.5, 0.75, 0.5f, //
    0.0, 0.75, 0.5f, //
    // colors
    0.7f, 0.0f, 0.0f, 0.5f, //
    0.7f, 0.0f, 0.0f, 0.5f, //
    0.7f, 0.0f, 0.0f, 0.5f, //
    0.7f, 0.0f, 0.0f, 0.5f, //
    0.7f, 0.0f, 0.0f, 0.5f, //
    0.7f, 0.0f, 0.0f, 0.5f, //
};

// Triangle
struct Vertex {
    float position[3];
    uint8_t color[4];
};

const Vertex triangle_vertices[] = {
    Vertex{{-0.25, -0.5, 0.5}, {255, 0, 0, 255}}, //
    Vertex{{0.25, -0.5, 0.5}, {0, 255, 0, 255}},  //
    Vertex{{0.0, 0.0, 0.5}, {0, 0, 255, 255}}     //
};

// Indexed Diamond
const uint32_t diamond_indices[] = {
    0, 1, 2, //
    0, 2, 3, //
    0, 3, 4, //
    0, 4, 1  //
};

const float diamond_positions_indexed[] = {
    0.50f, 0.50f, 0.6f, // 0
    0.75f, 0.50f, 0.6f, // 1
    0.50f, 0.75f, 0.6f, // 2
    0.25f, 0.50f, 0.6f, // 3
    0.50f, 0.25f, 0.6f, // 4
};

const float diamond_colors_indexed[] = {
    0.0f, 0.0f, 0.7f, 0.5f, // 0
    0.0f, 0.0f, 0.7f, 0.5f, // 1
    0.0f, 0.0f, 0.7f, 0.5f, // 2
    0.0f, 0.0f, 0.7f, 0.5f, // 3
    0.0f, 0.0f, 0.7f, 0.5f, // 4
};
