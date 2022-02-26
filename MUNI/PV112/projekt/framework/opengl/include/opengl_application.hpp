// ################################################################################
// Common Framework for Computer Graphics Courses at FI MUNI.
//
// Copyright (c) 2021-2022 Visitlab (https://visitlab.fi.muni.cz)
// All rights reserved.
// ################################################################################

#pragma once
#include "capsule.hpp"
#include "cube.hpp"
#include "cylinder.hpp"
#include "glm_headers.hpp"
#include "gui_application.h"
#include "phong_material_ubo.hpp"
#include "program.hpp"
#include "sphere.hpp"
#include "teapot.hpp"
#include "torus.hpp"

class OpenGLApplication : public GUIApplication {

    // ----------------------------------------------------------------------------
    // Variables
    // ----------------------------------------------------------------------------
  protected:
    /** The elapsed time from the beginning (in milliseconds). */
    long double elapsed_time = 0;

    /** The OpenGL query object that we use to get render time of one frame. */
    GLuint render_time_query;

    /** The current FPS on GPU. */
    float fps_gpu;

    /** The current FPS on CPU. */
    float fps_cpu;

    // ----------------------------------------------------------------------------
    // Variables (Shaders)
    // ----------------------------------------------------------------------------
  protected:
    /** The default lit program. */
    ShaderProgram default_lit_program;
    /** The default unlit program. */
    ShaderProgram default_unlit_program;
    /** The absolute path to framework's shaders. Loaded from {@link configuration} if a configuration file is available. */
    std::filesystem::path framework_shaders_path;
    /** The absolute path to framework's textures. Loaded from {@link configuration} if a configuration file is available. */
    std::filesystem::path framework_textures_path;
    /** The absolute path to application shaders. Loaded from {@link configuration} if a configuration file is available. */
    std::filesystem::path lecture_shaders_path;
    /** The absolute path to application textures. Loaded from {@link configuration} if a configuration file is available. */
    std::filesystem::path lecture_textures_path;

    // ----------------------------------------------------------------------------
    // Variables (Materials)
    // ----------------------------------------------------------------------------
  protected:
    /** The UBO defining the red material. */
    PhongMaterialUBO red_material_ubo;
    /** The UBO defining the green material. */
    PhongMaterialUBO green_material_ubo;
    /** The UBO defining the blue material. */
    PhongMaterialUBO blue_material_ubo;
    /** The UBO defining the cyan material. */
    PhongMaterialUBO cyan_material_ubo;
    /** The UBO defining the magenta material. */
    PhongMaterialUBO magenta_material_ubo;
    /** The UBO defining the yellow material. */
    PhongMaterialUBO yellow_material_ubo;
    /** The UBO defining the white material. */
    PhongMaterialUBO white_material_ubo;
    /** The UBO defining the black material. */
    PhongMaterialUBO black_material_ubo;
    /** The UBO defining the gray material. */
    PhongMaterialUBO gray_material_ubo;

    // ----------------------------------------------------------------------------
    // Variables (Geometry)
    // ----------------------------------------------------------------------------
  protected:
    /** The default pre-initialized cube geometry. */
    Cube cube;

    /** The default pre-initialized sphere geometry. */
    Sphere sphere;

    /** The default pre-initialized torus geometry. */
    Torus torus;

    /** The default pre-initialized cylinder geometry. */
    Cylinder cylinder;

    /** The default pre-initialized capsule geometry. */
    Capsule capsule;

    /** The default pre-initialized teapot geometry. */
    Teapot teapot;

    /** Empty VAO. */
    GLuint empty_vao;

    // ----------------------------------------------------------------------------
    // Constructors & Destructors
    // ----------------------------------------------------------------------------
  public:
    /**
     * Constructs a new @link OpenGLApplication with a custom width and height.
     *
     * @param 	initial_width 	The initial width of the window.
     * @param 	initial_height	The initial height of the window.
     * @param 	arguments	  	The command line arguments used to obtain the application directory.
     */
    OpenGLApplication(int initial_width, int initial_height, std::vector<std::string> arguments);

    /** The destructor that releases all resources. */
    virtual ~OpenGLApplication();

    // ----------------------------------------------------------------------------
    // Methods
    // ----------------------------------------------------------------------------
    /** @copydoc	GUIApplication::compile_shaders */
    void compile_shaders() override;

    // ----------------------------------------------------------------------------
    // Update
    // ----------------------------------------------------------------------------
  public:
    /** @copydoc	GUIApplication::update */
    void update(float delta) override;

    // ----------------------------------------------------------------------------
    // Render
    // ----------------------------------------------------------------------------
  public:
    /** @copydoc	GUIApplication::render */
    void render() override;

    // ----------------------------------------------------------------------------
    // GUI
    // ----------------------------------------------------------------------------
  public:
    /** {@copydoc GUIApplication::render_ui */
    void render_ui() override {}
};
