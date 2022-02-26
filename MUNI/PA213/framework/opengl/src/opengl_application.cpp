// ################################################################################
// Common Framework for Computer Graphics Courses at FI MUNI.
// 
// Copyright (c) 2021-2022 Visitlab (https://visitlab.fi.muni.cz)
// All rights reserved.
// ################################################################################

#include "opengl_application.hpp"

// ----------------------------------------------------------------------------
// Constructors
// ----------------------------------------------------------------------------
OpenGLApplication::OpenGLApplication(int initial_width, int initial_height, std::vector<std::string> arguments)
    : GUIApplication(initial_width, initial_height, arguments) {
    framework_shaders_path = framework_folder_path / "opengl" / "shaders";
    framework_textures_path = framework_folder_path / "opengl" / "textures";
    lecture_shaders_path = lecture_folder_path / "shaders";
    lecture_textures_path = lecture_folder_path / "textures";

    OpenGLApplication::compile_shaders();

    // Creates the empty VAO.
    glGenVertexArrays(1, &empty_vao);

    // Creates the query object.
    glCreateQueries(GL_TIME_ELAPSED, 1, &render_time_query);
    
    // Initializes the materials.
    red_material_ubo.set_material(PhongMaterialData(glm::vec3(1.0f, 0.0f, 0.0f), true, 200.0f, 1.0f));
    green_material_ubo.set_material(PhongMaterialData(glm::vec3(0.0f, 1.0f, 0.0f), true, 200.0f, 1.0f));
    blue_material_ubo.set_material(PhongMaterialData(glm::vec3(0.0f, 0.0f, 1.0f), true, 200.0f, 1.0f));
    cyan_material_ubo.set_material(PhongMaterialData(glm::vec3(0.0f, 1.0f, 1.0f), true, 200.0f, 1.0f));
    magenta_material_ubo.set_material(PhongMaterialData(glm::vec3(1.0f, 0.0f, 1.0f), true, 200.0f, 1.0f));
    yellow_material_ubo.set_material(PhongMaterialData(glm::vec3(1.0f, 1.0f, 0.0f), true, 200.0f, 1.0f));
    white_material_ubo.set_material(PhongMaterialData(glm::vec3(1.0f), true, 200.0f, 1.0f));
    black_material_ubo.set_material(PhongMaterialData(glm::vec3(0.0f), 1.0f, 0.0f, false));
    gray_material_ubo.set_material(PhongMaterialData(glm::vec3(0.7f), 1.0f, 200.0f, 1.0f));

    // Uploads the OpenGL materials.
    red_material_ubo.update_opengl_data();
    green_material_ubo.update_opengl_data();
    blue_material_ubo.update_opengl_data();
    cyan_material_ubo.update_opengl_data();
    magenta_material_ubo.update_opengl_data();
    yellow_material_ubo.update_opengl_data();
    white_material_ubo.update_opengl_data();
    black_material_ubo.update_opengl_data();
    gray_material_ubo.update_opengl_data();
}

OpenGLApplication::~OpenGLApplication() = default;


// ----------------------------------------------------------------------------
// Methods
// ----------------------------------------------------------------------------

void OpenGLApplication::compile_shaders() {
    // The default shader paths assume the standard file structure.
    // TODO: replace the original shaders in the framework with the new versions from PV227.
    default_unlit_program = ShaderProgram(framework_shaders_path / "object.vert", framework_shaders_path / "unlit.frag");
    default_lit_program = ShaderProgram(framework_shaders_path / "object.vert", framework_shaders_path / "lit.frag");
}

  // ----------------------------------------------------------------------------
// Render
// ----------------------------------------------------------------------------

void OpenGLApplication::render() {
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
}


// ----------------------------------------------------------------------------
// Update
// ----------------------------------------------------------------------------

void OpenGLApplication::update(float delta) {
    // Adds the time delta.
    elapsed_time += delta;
    // Computes FPS.
    fps_cpu = 1000 / delta;
}
