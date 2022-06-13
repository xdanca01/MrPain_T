// ###############################################################################
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

Application::Application(int initial_width, int initial_height, std::vector<std::string> arguments) : PV112Application(initial_width, initial_height, arguments) {

    compile_shaders();

    glCreateVertexArrays(1, &vao_empty);

    //diamond vertex
    glCreateBuffers(1, &vbo_diamond_ver);
    glNamedBufferStorage(vbo_diamond_ver, 36* sizeof(float), diamond_positions, 0);

    //diamond colors
    glCreateBuffers(1, &vbo_diamond_col);
    glNamedBufferStorage(vbo_diamond_col, 36* sizeof(diamond_colors), diamond_colors, 0);

    //square vertex + colors
    glCreateBuffers(1, &vbo_square);
    glNamedBufferStorage(vbo_square, 36* sizeof(float), square_data, 0);

    //triangles
    glCreateBuffers(1, &vbo_triangle);
    glNamedBufferStorage(vbo_triangle, 3 * sizeof(Vertex), triangle_vertices, 0);

    //diamond indices
    glCreateBuffers(1, &vbo_diamond_indices);
    glNamedBufferStorage(vbo_diamond_indices, 12 * sizeof(uint32_t), diamond_indices, 0);

    //diamond positions indexed
    glCreateBuffers(1, &vbo_diamond_pos_i);
    glNamedBufferStorage(vbo_diamond_pos_i, 15 * sizeof(float), diamond_positions_indexed, 0);

    //diamond colors indexed
    glCreateBuffers(1, &vbo_diamond_col_i);
    glNamedBufferStorage(vbo_diamond_col_i, 15 * sizeof(float), diamond_colors_indexed, 0);

    // Task 3.3: Create Vertex Buffer Objects (VBO). Copy vertex data found in data.hpp to VBOs.
    // Task 3.4-3.6: Create Vertex Array Objects (VAO) fetching data from VBOs.
    
    //diamond
    glCreateVertexArrays(1, &vao_diamond);
    glVertexArrayVertexBuffer(vao_diamond, 0, vbo_diamond_ver, 0, 3 * sizeof(float));
    glEnableVertexArrayAttrib(vao_diamond, 0);
    glVertexArrayAttribFormat(vao_diamond, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vao_diamond, 0, 0);

    //diamond color
    glVertexArrayVertexBuffer(vao_diamond, 1, vbo_diamond_col, 0, 3 * sizeof(float));
    glEnableVertexArrayAttrib(vao_diamond, 1);
    glVertexArrayAttribFormat(vao_diamond, 1, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vao_diamond, 1, 1);

    //diamond indexes
    glCreateVertexArrays(1, &vao_diamond_index);
    glVertexArrayVertexBuffer(vao_diamond_index, 0, vbo_diamond_pos_i, 0, 3* sizeof(float));
    glVertexArrayElementBuffer(vao_diamond_index, vbo_diamond_indices);
    glEnableVertexArrayAttrib(vao_diamond_index, 0);
    glVertexArrayAttribFormat(vao_diamond_index, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vao_diamond_index, 0, 0);

    //diamond indexed pos
    glVertexArrayVertexBuffer(vao_diamond_index, 1, vbo_diamond_col_i, 0, 3* sizeof(float));
    glEnableVertexArrayAttrib(vao_diamond_index, 1);
    glVertexArrayAttribFormat(vao_diamond_index, 1, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vao_diamond_index, 1, 1);

    //square position
    glCreateVertexArrays(1, &vao_square);
    glVertexArrayVertexBuffer(vao_square, 0, vbo_square, 0, 3 * sizeof(float));
    glEnableVertexArrayAttrib(vao_square, 0);
    glVertexArrayAttribFormat(vao_square, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vao_square, 0, 0);

    //square color
    glVertexArrayVertexBuffer(vao_square, 1, vbo_square, 18*sizeof(float), 3* sizeof(float));
    glEnableVertexArrayAttrib(vao_square, 1);
    glVertexArrayAttribFormat(vao_square, 1, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vao_square, 1, 1);

    //triangle
    glCreateVertexArrays(1, &vao_triangle);
    glVertexArrayVertexBuffer(vao_triangle, 0, vbo_triangle, 0, sizeof(Vertex));

    //triangle  position
    glEnableVertexArrayAttrib(vao_triangle, 0);
    glVertexArrayAttribFormat(vao_triangle, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vao_triangle, 0, 0);

    //color
    glEnableVertexArrayAttrib(vao_triangle, 1);
    glVertexArrayAttribFormat(vao_triangle, 1, 3, GL_UNSIGNED_BYTE, GL_TRUE, 3*sizeof(float));
    glVertexArrayAttribBinding(vao_triangle, 1, 0);

}

Application::~Application() {
    glDeleteVertexArrays(1, &vao_empty);

    // Task 3.3: Delete the Vertex Buffer Objects (VBOs).
    glDeleteBuffers(1, &vbo_diamond_ver);
    glDeleteBuffers(1, &vbo_diamond_col);
    glDeleteBuffers(1, &vbo_square);
    glDeleteBuffers(1, &vbo_triangle);
    glDeleteBuffers(1, &vbo_diamond_indices);
    glDeleteBuffers(1, &vbo_diamond_pos_i);
    glDeleteBuffers(1, &vbo_diamond_col_i);

    // Task 3.4-3.6: Delete Vertex Array Objects (VAOs).
    glDeleteVertexArrays(1, &vao_diamond);
    glDeleteVertexArrays(1, &vao_square);
    glDeleteVertexArrays(1, &vao_triangle);
    glDeleteVertexArrays(1, &vao_diamond_index);

    Application::delete_shaders();
}

// ----------------------------------------------------------------------------
// Methods
// ----------------------------------------------------------------------------

void Application::delete_shaders() {
    glDeleteProgram(program_triangle);
    glDeleteProgram(program_vao);
}

void Application::compile_shaders() {
    delete_shaders();

    program_triangle = create_program(lecture_shaders_path / "main.vert", lecture_shaders_path / "main.frag");
    program_vao = create_program(lecture_shaders_path / "main_vao.vert", lecture_shaders_path / "main_vao.frag");
}

void Application::update(float delta) {}

void Application::render() {
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);

    switch (ui_chosen_program) {
    case 0: {
        // Good ol' triangle.
        glUseProgram(program_triangle);
        glBindVertexArray(vao_empty);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        break;
    }
    case 1: {
        // Task 3.1: Draw a square.
        // Good ol' triangle.
        glUseProgram(program_triangle);
        glBindVertexArray(vao_empty);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        break;
    }
    case 2: {
        // Task 3.2: Draw a square using GL_TRIANGLE_STRIP.
        glUseProgram(program_triangle);
        glBindVertexArray(vao_empty);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        break;
    }
    case 3: {
        // Task 3.4-3.6: Draw all objects.
        glUseProgram(program_vao);

        glBindVertexArray(vao_diamond);
        glDrawArrays(GL_TRIANGLES, 0, 12);

        glBindVertexArray(vao_square);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glBindVertexArray(vao_triangle);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(vao_diamond_index);
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
        break;
    }
    }
}

void Application::render_ui() {
    const float unit = ImGui::GetFontSize();

    ImGui::Begin("Configuration", nullptr, ImGuiWindowFlags_NoDecoration);
    ImGui::SetWindowSize(ImVec2(15 * unit, 20 * unit));
    ImGui::SetWindowPos(ImVec2(2 * unit, 2 * unit));
    ImGui::RadioButton("Triangle", &ui_chosen_program, 0);
    ImGui::RadioButton("Square", &ui_chosen_program, 1);
    ImGui::RadioButton("Square (triangle strip)", &ui_chosen_program, 2);
    ImGui::RadioButton("VAO-based objects", &ui_chosen_program, 3);
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
}
