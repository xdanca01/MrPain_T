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

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_MULTISAMPLE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    compile_shaders();
}

Application::~Application() { Application::delete_shaders(); }

// ----------------------------------------------------------------------------
// Methods
// ----------------------------------------------------------------------------

void Application::delete_shaders() {
    glDeleteProgram(main_program);
}

void Application::compile_shaders() {
    delete_shaders();

    main_program = create_program(lecture_shaders_path / "main.vert", lecture_shaders_path / "main.frag");
}

void Application::update(float delta) {}

void Application::render() {
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // TIPS: Always use f after floating numbers, e.g 1.0f. This helps in avoiding glm template errors
    // Task 5.1: Setup the camera.
    //           1. Choose camera position (for example 1.5f, 2.0f, -8.0f is good to see all our objects) as glm::vec3.
    //           2. Create perspective matrix using glm::perspective(float field_of_view, float aspect_ratio, float near, float far).
    //              - glm works in radians by default, you can convert degrees to radians using glm::radians(degrees)
    //              - aspect_ratio is width of the window divided by height of the window
    //              - near and far planes should be: positive, near < far, near should be much smaller than 1.0f, e.g 0.01f
    //           3. Create view matrix using glm::lookAt(glm::vec3 eye_position, glm::vec3 look_at_position, glm::vec3 up_vector).
    //           4. Leave model matrix as identity for now.
    // Task 5.1: Upload MVP matrices to 'program' - use glm::value_ptr to get pointers for your matrices.
    glm::vec3 camera = glm::vec3(1.5f, 2.0f, -8.0f);
    glm::mat4 projection = glm::perspective(glm::radians(90.0f), (GLfloat)width/(GLfloat)height, 0.01f, 100.0f);
    glm::mat4 view = glm::lookAt(camera, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 model = glm::mat4(1.0f);
    glProgramUniformMatrix4fv(main_program, 0, 1, GL_FALSE, glm::value_ptr(projection));
    glProgramUniformMatrix4fv(main_program, 1, 1, GL_FALSE, glm::value_ptr(view));
    glProgramUniformMatrix4fv(main_program, 2, 1, GL_FALSE, glm::value_ptr(model));
    
    // Task 5.2: Set uniform data for light and upload camera(eye) position to 'program'.
    glm::vec4 light_pos = glm::vec4(0.0f, 3.0f, -5.0f, light);
    //light to camera
    //light_pos = glm::vec4(camera, light);
    glm::vec3 light_ambient = glm::vec3(0.3f, 0.2f, 0.9f);
    glm::vec3 light_diffuse = glm::vec3(0.6f, 0.6f, 0.3f);
    glm::vec3 light_specular = glm::vec3(1.0f, 1.0f, 1.0f);
    glProgramUniform3fv(main_program, 3, 1, glm::value_ptr(camera));
    glProgramUniform4fv(main_program, 4, 1, glm::value_ptr(light_pos));
    glProgramUniform3fv(main_program, 5, 1, glm::value_ptr(light_ambient));
    glProgramUniform3fv(main_program, 6, 1, glm::value_ptr(light_diffuse));
    glProgramUniform3fv(main_program, 7, 1, glm::value_ptr(light_specular));
    // Task 5.2: Set uniform data for material. Remember: it's ambient, diffuse, specular, and additionally shininess for material.
    glm::vec3 material_ambient  = glm::vec3(0.83f, 0.35f, 0.5f);
    glm::vec3 material_diffuse  = glm::vec3(0.83f, 0.35f, 0.5f);
    glm::vec3 material_specular = glm::vec3(0.83f, 0.35f, 0.5f);
    float shine = 60.0f;
    glProgramUniform3fv(main_program, 8, 1, glm::value_ptr(material_ambient));
    glProgramUniform3fv(main_program, 9, 1, glm::value_ptr(material_diffuse));
    glProgramUniform3fv(main_program, 10, 1, glm::value_ptr(material_specular));
    glProgramUniform1f(main_program, 11, shine);
    glUseProgram(main_program);

    // Task 5.1: Separate each object by moving them to different locations using glm::translate(glm::mat4 matrix_to_translate, glm::vec3).
    //           Play with other glm functions of the same form:
    //            - rotate (mat4x4 matrix_to_rotate, float angle, glm::vec3 axis_to_rotate_around)
    //            - translate (mat4x4 matrix_to_translate, glm::vec3 move_by_vector)
    cube.draw();
    
    model = glm::translate(model, glm::vec3(4.0f, 0.0f, 0.0f));
    glProgramUniformMatrix4fv(main_program, 2, 1, GL_FALSE, glm::value_ptr(model));
    sphere.draw();

    model = glm::translate(model, glm::vec3(-8.0f, 0.0f, 0.0f));
    glProgramUniformMatrix4fv(main_program, 2, 1, GL_FALSE, glm::value_ptr(model));
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

void Application::on_mouse_move(double x, double y) {}

void Application::on_mouse_button(int button, int action, int mods) {}

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
            // Task 5.2: Map the change between directional and point light to a key on a keyboard.
            light = light == 1.0f ? 0.0f : 1.0f;
            break;
        }
    }
}
