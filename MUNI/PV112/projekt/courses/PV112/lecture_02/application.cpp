// ################################################################################
// Common Framework for Computer Graphics Courses at FI MUNI.
//
// Copyright (c) 2021-2022 Visitlab (https://visitlab.fi.muni.cz)
// All rights reserved.
// ################################################################################

#include "application.hpp"

// ----------------------------------------------------------------------------
// Constructors & Destructors
// ----------------------------------------------------------------------------

Application::Application(int initial_width, int initial_height, std::vector<std::string> arguments) : PV112Application(initial_width, initial_height, arguments) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Reserved for future lesson.
    glCreateVertexArrays(1, &vao);

    compile_shaders();
}

Application::~Application() {
    glDeleteVertexArrays(1, &vao);
    delete_shaders();
}

// ----------------------------------------------------------------------------
// Methods
// ----------------------------------------------------------------------------
    const char* get_shader_data(std::filesystem::path path){
        const std::string shader_data = load_file(path);
        return shader_data.data();
    }

void Application::delete_shaders() {
    if(program != 0)
    {
        glDeleteProgram(program);
        program = 0;
    }
}

void Application::compile_shaders() {
    delete_shaders();
    // Task 2.1: Compile vertex & fragment shaders.
    // Help: - There is a variable 'lecture_shaders_path' that contains path to the folder with shaders.
    //       - Use the helper function load_file from utilities to load the contents of a file into a string.
    //       - Method data() callable on std::string type can give you raw C99 pointer that OpenGL takes as an input.
    //         Example: const std::string vertex_shader_string = load_file(lecture_shaders_path / "main.vert");
    //                  const char* vertex_shader_source = vertex_shader_string.data();
    //       - Create a function so that you don't need to copy paste the code for both vertex and the fragment shaders.
    //create_my_shader(lecture_shaders_path);
    // Task 2.2: Link the shaders to create a program. Do not forget to delete shaders after linking.
    /*GLuint VS = glCreateShader(GL_VERTEX_SHADER);
    GLuint FS = glCreateShader(GL_FRAGMENT_SHADER);
    const char* VS_data = get_shader_data(lecture_shaders_path / "main.vert");
    const char* FS_data = get_shader_data(lecture_shaders_path / "main.frag");
    glShaderSource(VS, 1, &VS_data, nullptr);
    glShaderSource(FS, 1, &FS_data, nullptr);
    glCompileShader(VS);
    glCompileShader(FS);*/
    // Task 2.4: Link the second set of shaders.
    program = create_program(lecture_shaders_path/"main.vert", lecture_shaders_path/"main.frag");
}

void Application::update(float delta) {}

void Application::render() {
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindVertexArray(vao);
    glUseProgram(program);
    // Task 2.3: Use the compiled program for drawing.
    // Task 2.4: Use glViewport to render two triangles side by side.
    // Task 2.4: Use the new program to draw the second triangle.
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

// ----------------------------------------------------------------------------
// Input Events
// ----------------------------------------------------------------------------

void Application::render_ui() {
    const float unit = ImGui::GetFontSize();

    ImGui::Begin("Configuration", nullptr, ImGuiWindowFlags_NoDecoration);
    ImGui::SetWindowSize(ImVec2(10 * unit, 20 * unit));
    ImGui::SetWindowPos(ImVec2(2 * unit, 2 * unit));
    ImGui::RadioButton("Default", &ui_reverse_sides, 0);
    ImGui::RadioButton("Switch sides", &ui_reverse_sides, 1);
    ImGui::End();
}

void Application::on_resize(int width, int height) {
    // Calls the default implementation to set the class variables.
    IApplication::on_resize(width, height);
}

void Application::on_mouse_move(double x, double y) {}

void Application::on_mouse_button(int button, int action, int mods) {}

void Application::on_key_pressed(int key, int scancode, int action, int mods) {
    // Calls the default implementation that invokes compile_shaders when 'R key is hit.
    PV112Application::on_key_pressed(key, scancode, action, mods);

    // Task 2.5: Switch the order of used programs for drawing on pressing keyboard key of your choice.
    //           Use for example if(action == GLFW_PRESS && key == GLFW_KEY_S){} to decide when the switch happens.
    //           For the interoperation of UI and Inputs use the same variable used in UI `ui_reverse_sides`
}
