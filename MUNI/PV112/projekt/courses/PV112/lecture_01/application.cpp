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

Application::Application(int initial_width, int initial_height, std::vector<std::string> arguments)
    : PV112Application(initial_width, initial_height, arguments) {
}

Application::~Application() {}

// ----------------------------------------------------------------------------
// Methods
// ----------------------------------------------------------------------------

void Application::update(float delta) {}

void Application::render() {
    glViewport(0, 0, width, height);
    // Task 1.1: Set the viewport.
    glClearColor(red, green, blue, 1.0f);
    // Task 1.2: Set the clear color - you can use either hard-coded values or use variables 'red', 'green', 'blue'.
    glClear(GL_COLOR_BUFFER_BIT);
    // Task 1.2: Clear the window.
}

void Application::render_ui() {
    const float unit = ImGui::GetFontSize();

    // Creates a simple GUI that enables to set the clear color.
    ImGui::Begin("Another Window", nullptr, ImGuiWindowFlags_NoDecoration);
    ImGui::SetWindowSize(ImVec2(10 * unit, 20 * unit));
    ImGui::SetWindowPos(ImVec2(2 * unit, 2 * unit));
    ImGui::SliderFloat("Red", &red, 0.0f, 1.0f);
    ImGui::SliderFloat("Green", &green, 0.0f, 1.0f);
    ImGui::SliderFloat("Blue", &blue, 0.0f, 1.0f);
    ImGui::End();
}

void Application::on_resize(int width, int height) {
    // Calls the default implementation to set the class variables.
    IApplication::on_resize(width, height);
}

void Application::on_mouse_move(double x, double y) {}

void Application::on_mouse_button(int button, int action, int mods) {}

void Application::on_key_pressed(int key, int scancode, int action, int mods) {}
