// ################################################################################
// Common Framework for Computer Graphics Courses at FI MUNI.
// 
// Copyright (c) 2021-2022 Visitlab (https://visitlab.fi.muni.cz)
// All rights reserved.
// ################################################################################

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "application.hpp"

// ----------------------------------------------------------------------------
// Helper Methods.
// ----------------------------------------------------------------------------

GLuint load_texture_2d(const std::filesystem::path filename) {
    int width, height, channels;
    unsigned char* data = stbi_load(filename.generic_string().data(), &width, &height, &channels, 4);

    GLuint texture;
    glCreateTextures(GL_TEXTURE_2D, 1, &texture);

    glTextureStorage2D(texture, std::log2(width), GL_RGBA8, width, height);

    glTextureSubImage2D(texture,
                        0,                         //
                        0, 0,                      //
                        width, height,             //
                        GL_RGBA, GL_UNSIGNED_BYTE, //
                        data);

    stbi_image_free(data);

    glGenerateTextureMipmap(texture);

    glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return texture;
}

// ----------------------------------------------------------------------------
// Constructors & Destructors
// ----------------------------------------------------------------------------

Application::Application(int initial_width, int initial_height, std::vector<std::string> arguments)
    : PV112Application(initial_width, initial_height, arguments) {

    images_path = lecture_folder_path / "images";

    // Configure fixed function pipeline
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    wood_texture = load_texture_2d(images_path / "wood.jpg");
    rock_texture = load_texture_2d(images_path / "rocks.jpg");

    // --------------------------------------------------------------------------
    // Task 7.1: Create Framebuffer
    // --------------------------------------------------------------------------
    // Create framebuffers (glCreateFramebuffers)
    // ...
    glCreateFramebuffers(1, &framebuffer);


    // Initialize color output texture with GL_RGBA32F format (glCreateTextures, glTextureStorage2D)
    // ...
    glCreateTextures(GL_TEXTURE_2D, 1, &framebuffer_color);
    
    glTextureStorage2D(framebuffer_color, 1, GL_RGBA32F, width, height);

    // Initialize depth output texture with GL_DEPTH_COMPONENT32F format
    // ...
    glCreateTextures(GL_TEXTURE_2D, 1, &framebuffer_depth);
    glTextureStorage2D(framebuffer_depth, 1, GL_DEPTH_COMPONENT32F, width, height);    

    // Set output 0 to GL_COLOR_ATTACHMENT0 (glNamedFramebufferDrawBuffers)
    const GLenum draw_buffers[] = {GL_COLOR_ATTACHMENT0};
    // ...
    glNamedFramebufferDrawBuffers(framebuffer, 1, draw_buffers);

    // Associate color and depth 'attachments'(GL_COLOR_ATTACHMENT0,GL_DEPTH_ATTACHMENT)
    // with (framebuffer_)color and (framebuffer_)depth 'textures' (glNamedFramebufferTexture)
    glNamedFramebufferTexture(framebuffer, GL_COLOR_ATTACHMENT0, framebuffer_color, 0);
    glNamedFramebufferTexture(framebuffer, GL_DEPTH_ATTACHMENT, framebuffer_depth, 0);

    compile_shaders();
}

Application::~Application() {
    delete_shaders();

    glDeleteTextures(1, &wood_texture);
    glDeleteTextures(1, &rock_texture);

    glDeleteFramebuffers(1, &framebuffer);
    glDeleteTextures(1, &framebuffer_color);
    glDeleteTextures(1, &framebuffer_depth);
}

// ----------------------------------------------------------------------------
// Methods
// ----------------------------------------------------------------------------

void Application::delete_shaders() {
    glDeleteProgram(main_program);
}

void Application::compile_shaders() {
    delete_shaders();

    main_program = create_program(lecture_shaders_path / "main.vert", lecture_shaders_path / "main.frag");
    postprocess_program = create_program(lecture_shaders_path / "postprocess.vert", lecture_shaders_path / "postprocess.frag");
}

void Application::update(float delta) {}

void Application::render() {
    glViewport(0,0, width, height);

    // Camera
    glm::vec3 eye_position = camera.get_eye_position();
    glm::mat4 projection_matrix = glm::perspective(glm::radians(45.0f), float(width) / float(height), 0.1f, 100.0f);
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

    // Default Material
    glProgramUniform3f(main_program, 8, 0.2f, 0.2f, 0.2f);
    glProgramUniform3f(main_program, 9, 0.5f, 0.5f, 0.5f);
    glProgramUniform3f(main_program, 10, 0.0f, 0.0f, 0.0f);
    glProgramUniform1f(main_program, 11, 4.0f);

    // --------------------------------------------------------------------------
    // Draw scene into the custom framebuffer.
    // --------------------------------------------------------------------------

    // Bind the Framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    // Clear attachments
    glClearNamedFramebufferfv(framebuffer, GL_COLOR, 0, clear_color);
    glClearNamedFramebufferfv(framebuffer, GL_DEPTH, 0, clear_depth);

    // Configure fixed function pipeline
    glEnable(GL_DEPTH_TEST);

    // Use the 'main' program
    glUseProgram(main_program);

    // Perform standard draw commands
    glBindTextureUnit(0, wood_texture);
    glProgramUniformMatrix4fv(main_program, 1, 1, false, value_ptr(model_matrix));
    cube.draw();

    model_matrix = translate(glm::mat4(1.0f), glm::vec3(3.0f, 0.0f, 0.0f));
    glBindTextureUnit(0, rock_texture);
    glProgramUniformMatrix4fv(main_program, 1, 1, false, value_ptr(model_matrix));
    glProgramUniform3f(main_program, 9, 0.0f, 0.0f, 0.8f);
    sphere.draw();

    model_matrix = translate(glm::mat4(1.0f), glm::vec3(-3.0f, 0.0f, 0.0f));
    glProgramUniformMatrix4fv(main_program, 1, 1, false, value_ptr(model_matrix));
    glProgramUniform3f(main_program, 9, 0.8f, 0.0f, 0.0f);
    teapot.draw();

    // --------------------------------------------------------------------------
    // Task 7.2: Post-process
    // --------------------------------------------------------------------------

    // Bind back the default(0) framebuffer (glBindFramebuffer).
    // ...
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Clear the color buffer.
    // ...
    glClear(GL_COLOR_BUFFER_BIT);

    // Disable depth test - we do not need it.
    // ...
    glDisable(GL_DEPTH_TEST);

    // Use post-process program.
    // ...
    glUseProgram(postprocess_program);

    // Bind the output from previous program (framebuffer_color) as input texture (binding = 0).
    // to the post-process program
    // ...
    glBindTextureUnit(0, framebuffer_color);

    // Draw the full-screen triangle.
    // ...
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Application::render_ui() { const float unit = ImGui::GetFontSize(); }

// ----------------------------------------------------------------------------
// Input Events
// ----------------------------------------------------------------------------

void Application::on_resize(int width, int height) {
    // Calls the default implementation to set the class variables.
    PV112Application::on_resize(width, height);

    glDeleteTextures(1, &framebuffer_color);
    glDeleteTextures(1, &framebuffer_depth);

    // Task 7.1: Re-Initialize color output texture.
    // Task 7.1: Re-Initialize depth output texture.
    // Task 7.1: Re-Associate color and depth 'attachments' with color and depth 'textures'.
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
