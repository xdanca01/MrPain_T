// ################################################################################
// Common Framework for Computer Graphics Courses at FI MUNI.
// 
// Copyright (c) 2021-2022 Visitlab (https://visitlab.fi.muni.cz)
// All rights reserved.
// ################################################################################

#include "application.hpp"
#include "data.hpp"

// ----------------------------------------------------------------------------
// Constructors & Destructors
// ----------------------------------------------------------------------------

Application::Application(int initial_width, int initial_height, std::vector<std::string> arguments)
    : PV112Application(initial_width, initial_height, arguments) {
    this->width = initial_width;
    this->height = initial_height;

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Diamond
    glCreateBuffers(1, &diamond_positions_vbo);
    glNamedBufferStorage(diamond_positions_vbo, sizeof(diamond_positions), diamond_positions, NULL);
    glCreateBuffers(1, &diamond_colors_vbo);
    glNamedBufferStorage(diamond_colors_vbo, sizeof(diamond_colors), diamond_colors, NULL);

    glCreateVertexArrays(1, &diamond_vao);
    glVertexArrayVertexBuffer(diamond_vao, 0, diamond_positions_vbo, 0, 3 * sizeof(float));
    glVertexArrayVertexBuffer(diamond_vao, 1, diamond_colors_vbo, 0, 4 * sizeof(float));

    glEnableVertexArrayAttrib(diamond_vao, 0);
    glVertexArrayAttribFormat(diamond_vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(diamond_vao, 0, 0);

    glEnableVertexArrayAttrib(diamond_vao, 1);
    glVertexArrayAttribFormat(diamond_vao, 1, 4, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(diamond_vao, 1, 1);

    // Square
    glCreateBuffers(1, &square_vbo);
    glNamedBufferStorage(square_vbo, sizeof(square_data), square_data, NULL);

    glCreateVertexArrays(1, &square_vao);
    glVertexArrayVertexBuffer(square_vao, 0, square_vbo, 0, 3 * sizeof(float));
    glVertexArrayVertexBuffer(square_vao, 1, square_vbo, 6 * 3 * sizeof(float), 4 * sizeof(float));

    glEnableVertexArrayAttrib(square_vao, 0);
    glVertexArrayAttribFormat(square_vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(square_vao, 0, 0);

    glEnableVertexArrayAttrib(square_vao, 1);
    glVertexArrayAttribFormat(square_vao, 1, 4, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(square_vao, 1, 1);

    // Triangle
    glCreateBuffers(1, &triangle_vbo);
    glNamedBufferStorage(triangle_vbo, sizeof(triangle_vertices), triangle_vertices, NULL);

    glCreateVertexArrays(1, &triangle_vao);
    glVertexArrayVertexBuffer(triangle_vao, 0, triangle_vbo, 0, sizeof(Vertex));

    glEnableVertexArrayAttrib(triangle_vao, 0);
    glVertexArrayAttribFormat(triangle_vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(triangle_vao, 0, 0);

    glEnableVertexArrayAttrib(triangle_vao, 1);
    glVertexArrayAttribFormat(triangle_vao, 1, 4, GL_UNSIGNED_BYTE, GL_TRUE, offsetof(Vertex, color));
    glVertexArrayAttribBinding(triangle_vao, 1, 0);

    // Indexed Diamond
    glCreateBuffers(1, &diamond_indices_vbo);
    glCreateBuffers(1, &diamond_positions_indexed_vbo);
    glCreateBuffers(1, &diamond_colors_indexed_vbo);

    glNamedBufferStorage(diamond_indices_vbo, sizeof(diamond_indices), diamond_indices, NULL);
    glNamedBufferStorage(diamond_positions_indexed_vbo, sizeof(diamond_positions_indexed), diamond_positions_indexed, NULL);
    glNamedBufferStorage(diamond_colors_indexed_vbo, sizeof(diamond_colors_indexed), diamond_colors_indexed, NULL);

    glCreateVertexArrays(1, &diamond_indexed_vao);
    glVertexArrayVertexBuffer(diamond_indexed_vao, 0, diamond_positions_indexed_vbo, 0, 3 * sizeof(float));
    glVertexArrayVertexBuffer(diamond_indexed_vao, 1, diamond_colors_indexed_vbo, 0, 4 * sizeof(float));

    glVertexArrayElementBuffer(diamond_indexed_vao, diamond_indices_vbo);

    glEnableVertexArrayAttrib(diamond_indexed_vao, 0);
    glVertexArrayAttribFormat(diamond_indexed_vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(diamond_indexed_vao, 0, 0);

    glEnableVertexArrayAttrib(diamond_indexed_vao, 1);
    glVertexArrayAttribFormat(diamond_indexed_vao, 1, 4, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(diamond_indexed_vao, 1, 1);

    compile_shaders();
}

Application::~Application() {
    glDeleteVertexArrays(1, &diamond_vao);
    glDeleteBuffers(1, &diamond_positions_vbo);
    glDeleteBuffers(1, &diamond_colors_vbo);

    glDeleteVertexArrays(1, &square_vao);
    glDeleteBuffers(1, &square_vbo);

    glDeleteVertexArrays(1, &triangle_vao);
    glDeleteBuffers(1, &triangle_vbo);

    glDeleteVertexArrays(1, &diamond_indexed_vao);
    glDeleteBuffers(1, &diamond_indices_vbo);
    glDeleteBuffers(1, &diamond_positions_indexed_vbo);
    glDeleteBuffers(1, &diamond_colors_indexed_vbo);

    delete_shaders();
}

// ----------------------------------------------------------------------------
// Methods
// ----------------------------------------------------------------------------

void Application::delete_shaders() {
    glDeleteProgram(program);
}

void Application::compile_shaders() {
    delete_shaders();

    program = create_program(lecture_shaders_path / "main.vert", lecture_shaders_path / "main.frag");
}

void Application::update(float delta) { }

void Application::render() {
    glViewport(0, 0, width, height);
    glUseProgram(program);

    // Task 4.3: Enable multisampling on OpenGl side using glEnable.
    glEnable(GL_MULTISAMPLE);

    // Task 4.1: Change polygon mode
    switch (ui_polygon_mode) {
    case 0:
        // Point
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        break;
    case 1:
        // Line
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        break;
    case 2:
        // Fill
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        break;
    }

    // Task 4.2: Modify the glClear call to clear the depth buffer as well.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Task 4.2: Enable the depth test using glEnable function.
    if (ui_depth) {
       glEnable(GL_DEPTH_TEST);
    } else {
        glDisable(GL_DEPTH_TEST);
    }

    // Task 4.4 : Enable blending.
    if (ui_blending) {
        glEnable(GL_BLEND);
    } else {
        glDisable(GL_BLEND);
        // ...
    }

    // Task 4.5: Enable culling.
    if (ui_face_cull > 0) {
       glEnable(GL_CULL_FACE);
       
        switch (ui_face_cull) {
        case 1:
            // Back
            glCullFace(GL_BACK);
            break;
        case 2:
            // Front
            glCullFace(GL_FRONT);
            break;
        case 3:
            // Both
            glCullFace(GL_FRONT_AND_BACK);
            break;
        }
    } else {
        // Disable
        glDisable(GL_CULL_FACE);
    }

    glBindVertexArray(diamond_vao);
    glDrawArrays(GL_TRIANGLES, 0, 12);

    glBindVertexArray(triangle_vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindVertexArray(square_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    if (ui_blending) {
        // Task 4.4 : Enable blending and try out different configurations of blend equation.
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBlendEquation(GL_FUNC_ADD);
        // Note that we disable the test before rendering the indexed diamond to avoid problems with blending.
        // ...
    }

    glBindVertexArray(diamond_indexed_vao);
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, nullptr);
}

void Application::render_ui() {
    const float unit = ImGui::GetFontSize();

    ImGui::Begin("Configuration", nullptr, ImGuiWindowFlags_NoDecoration);
    ImGui::SetWindowSize(ImVec2(20 * unit, 8 * unit));
    ImGui::SetWindowPos(ImVec2(2 * unit, 2 * unit));

    const char* polygon_mode_names[3] = {"Point", "Line", "Fill"};
    if (ImGui::BeginCombo("Polygon Mode", polygon_mode_names[ui_polygon_mode])) {
        if (ImGui::Selectable(polygon_mode_names[0], ui_polygon_mode == 0))
            ui_polygon_mode = 0;
        if (ImGui::Selectable(polygon_mode_names[1], ui_polygon_mode == 1))
            ui_polygon_mode = 1;
        if (ImGui::Selectable(polygon_mode_names[2], ui_polygon_mode == 2))
            ui_polygon_mode = 2;
        ImGui::EndCombo();
    }

    ImGui::Checkbox("Depth", &ui_depth);
    ImGui::Checkbox("Blending", &ui_blending);

    const char* culling_names[4] = {"None", "Front", "Back", "Front and Back"};
    if (ImGui::BeginCombo("Culling Mode", culling_names[ui_face_cull])) {
        if (ImGui::Selectable(culling_names[0], ui_face_cull == 0))
            ui_face_cull = 0;
        if (ImGui::Selectable(culling_names[1], ui_face_cull == 1))
            ui_face_cull = 1;
        if (ImGui::Selectable(culling_names[2], ui_face_cull == 2))
            ui_face_cull = 2;
        if (ImGui::Selectable(culling_names[3], ui_face_cull == 3))
            ui_face_cull = 3;
        ImGui::EndCombo();
    }
    ImGui::End();
}

// ----------------------------------------------------------------------------
// Input Events
// ----------------------------------------------------------------------------

void Application::on_resize(int width, int height) {
    // Calls the default implementation to set the class variables.
    PV112Application::on_resize(width, height);
}

void Application::on_mouse_move(double x, double y) {}

void Application::on_mouse_button(int button, int action, int mods) {}

void Application::on_key_pressed(int key, int scancode, int action, int mods) {
    // Calls default implementation that invokes compile_shaders when 'R key is hit.
    PV112Application::on_key_pressed(key, scancode, action, mods);

    // Task 4.1: Change the polygon mode to GL_LINE when L is pressed on the keyboard and GL_FILL when F is pressed on the keyboard.
    if(key == GLFW_KEY_L) ui_polygon_mode = 1;
    else if(key == GLFW_KEY_F) ui_polygon_mode = 2;
}
