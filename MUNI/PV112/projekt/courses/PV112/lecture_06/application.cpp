// ###############################################################################
// Common Framework for Computer Graphics Courses at FI MUNI.
// 
// Copyright (c) 2021-2022 Visitlab (https://visitlab.fi.muni.cz)
// All rights reserved.
// ################################################################################

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "application.hpp"

Application::Application(int initial_width, int initial_height, std::vector<std::string> arguments)
    : PV112Application(initial_width, initial_height, arguments) {

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    images_path = lecture_folder_path / "images";

    // Task 6.1: Initialize textures: "images/wood.jpg" and "images/rocks.jpg".    
    wood_texture = load_textures(images_path/"wood.jpg"),
    rock_texture = load_textures(images_path/"rocks.jpg");
    compile_shaders();
}

Application::~Application() {
    delete_shaders();
    glDeleteTextures(1, &wood_texture);
    glDeleteTextures(1, &rock_texture);
}

void Application::delete_shaders() {
    glDeleteProgram(main_program);
}

void Application::compile_shaders() {
    delete_shaders();

    main_program = create_program(lecture_shaders_path / "main.vert", lecture_shaders_path / "main.frag");
}

void Application::update(float delta) {}

GLuint Application::load_textures(const std::filesystem::path textureFile){
    int w, h, ch;
    unsigned char* data = stbi_load(textureFile.generic_string().data(), &w, &h, &ch, 4);
    
    GLuint textureID;
    glCreateTextures(GL_TEXTURE_2D, 1, &textureID);
    
    const GLsizei mipmapCnt = std::log2(std::min(w, h));
    glTextureStorage2D(textureID, mipmapCnt, GL_RGBA8, w, h);

    glTextureSubImage2D(textureID, 0, 0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);

    glGenerateTextureMipmap(textureID);

    glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, GL_LINEAR);
    return textureID;
}

void Application::render() {
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Camera
    const glm::vec3 eye_position = camera.get_eye_position();
    glm::mat4 projection_matrix = glm::perspective(glm::radians(45.0f), static_cast<float>(width) / static_cast<float>(height), 0.1f, 100.0f);
    glm::mat4 view_matrix = lookAt(eye_position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 model_matrix = glm::mat4(1.0f);

    glProgramUniformMatrix4fv(main_program, 0, 1, false, value_ptr(projection_matrix));
    glProgramUniformMatrix4fv(main_program, 1, 1, false, value_ptr(model_matrix));
    glProgramUniformMatrix4fv(main_program, 2, 1, false, value_ptr(view_matrix));
    glProgramUniform3f(main_program, 3, eye_position.x, eye_position.y, eye_position.z);

    // Light
    glProgramUniform4f(main_program, 4, 0.0f, 3.0f, 2.0f, light);

    glProgramUniform3f(main_program, 5, 0.2f, 0.2f, 0.2f);
    glProgramUniform3f(main_program, 6, 1.0f, 1.0f, 1.0f);
    glProgramUniform3f(main_program, 7, 0.0f, 0.0f, 0.0f);

    // Material
    glProgramUniform3f(main_program, 8, 0.2f, 0.2f, 0.2f);
    glProgramUniform3f(main_program, 9, 0.5f, 0.5f, 0.5f);
    glProgramUniform3f(main_program, 10, 0.0f, 0.0f, 0.0f);
    glProgramUniform1f(main_program, 11, 4.0f);

    glUseProgram(main_program);

    // Task 6.2: Bind a texture using glBindTextureUnit for each object (you can choose either wood or rocks texture).

    glProgramUniformMatrix4fv(main_program, 1, 1, false, value_ptr(model_matrix));
    glBindTextureUnit(0, wood_texture);
    cube.draw();

    model_matrix = translate(glm::mat4(1.0f), glm::vec3(3.0f, 0.0f, 0.0f));
    glProgramUniformMatrix4fv(main_program, 1, 1, false, value_ptr(model_matrix));
    glProgramUniform3f(main_program, 9, 0.0f, 0.0f, 0.8f);
    glBindTextureUnit(0, rock_texture);
    sphere.draw();

    model_matrix = translate(glm::mat4(1.0f), glm::vec3(-3.0f, 0.0f, 0.0f));
    glProgramUniformMatrix4fv(main_program, 1, 1, false, value_ptr(model_matrix));
    glProgramUniform3f(main_program, 9, 0.8f, 0.0f, 0.0f);
    teapot.draw();
}

void Application::render_ui() { const float unit = ImGui::GetFontSize(); }

// ----------------------------------------------------------------------------
// Input Events
// ----------------------------------------------------------------------------

void Application::on_resize(int width, int height) {
    // Calls the default implementation to set the class variables.
    PV112Application::on_resize(width, height);
}

void Application::on_mouse_move(double x, double y) { camera.on_mouse_move(x, y); }

void Application::on_mouse_button(int button, int action, int mods) { camera.on_mouse_button(button, action, mods); }

void Application::on_key_pressed(int key, int scancode, int action, int mods) {
    // Calls default implementation that invokes compile_shaders when 'R key is hit.
    PV112Application::on_key_pressed(key, scancode, action, mods);

    if (action == GLFW_PRESS) {
        switch (key) {
        case GLFW_KEY_F:
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
        case GLFW_KEY_L:
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;
        case GLFW_KEY_Q:
            light = light == 1.0f ? 0.0f : 1.0f;
            break;
        }
    }
}
