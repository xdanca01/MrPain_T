// ################################################################################
// Common Framework for Computer Graphics Courses at FI MUNI.
//
// Copyright (c) 2021-2022 Visitlab (https://visitlab.fi.muni.cz)
// All rights reserved.
// ################################################################################

#pragma once

#include "draw_primitives.hpp"
#include "l_system.hpp"
#include "camera_ubo.hpp"
#include "light_ubo.hpp"
#include "pa213_application.h"

/**The definition of a tree ubo.*/
class TreeUBO : public UBO<Branch> {
    using UBO<Branch>::UBO; // copies constructors from the parent class
};

/**The definition of a leafs ubo.*/
class LeavesUBO : public UBO<Leaf> {
    using UBO<Leaf>::UBO; // copies constructors from the parent class
};

class Application : public PA213Application {

    // ----------------------------------------------------------------------------
    // Variables
    // ----------------------------------------------------------------------------
    /** The OpenGL query object that we use to get render time of one frame. */
    GLuint render_time_query;
    /** The current FPS on GPU. */
    float fps_gpu;
    /** The configuration for the L-system's turtle. */
    LTurtle::Config lconfig;
    /** Edit props of rules of the L-system . */
    std::array<std::pair<char[50], char[50]>, 4> lrule_edit_props;
    /** The axiom (inital sentence) for the L-system. */
    char laxiom[50];
    /** Whether to compose all iterations of the L-system or use only the last one */
    bool compose_literations;

    // ----------------------------------------------------------------------------
    // Variables (Geometry)
    // ----------------------------------------------------------------------------
  protected:
    /** The branches. */
    std::vector<Branch> branches;
    /** The buffer with the branches. */
    TreeUBO tree_ubo;
    /** The leafs. */
    std::vector<Leaf> leaves;
    /** The buffer with the leaves. */
    LeavesUBO leaves_ubo;
    /** Empty VAO. */
    GLuint empty_vao;
    // ----------------------------------------------------------------------------
    // Variables (Materials)
    // ----------------------------------------------------------------------------
  protected:
    // ----------------------------------------------------------------------------
    // Variables (Textures)
    // ----------------------------------------------------------------------------
  protected:
    /** The meadow skybox texture. */
    GLuint meadow_skybox_tex;
    /** The wood texture. */
    GLuint wood_tex;
    /** The leaf texture. */
    GLuint leaf_tex;
    // ----------------------------------------------------------------------------
    // Variables (Light)
    // ----------------------------------------------------------------------------
  protected:
    /** The UBO storing the data about lights - positions, colors, etc. */
    PhongLightsUBO phong_lights_ubo;
    // ----------------------------------------------------------------------------
    // Variables (Camera)
    // ----------------------------------------------------------------------------
  protected:
    /** The UBO storing the information about camera. */
    CameraUBO camera_ubo;

    // ----------------------------------------------------------------------------
    // Variables (Shaders)
    // ----------------------------------------------------------------------------
  protected:
    /** The main ray tracing program. */
    ShaderProgram ray_tracing_program;

    // ----------------------------------------------------------------------------
    // Variables (GUI)
    // ----------------------------------------------------------------------------
  protected:
    /** The light position set in the GUI. */
    float gui_light_position = glm::radians(45.f);

    /** The flag determining if tone mapping should be used. */
    bool use_tone_mapping = false;
    /** The exposure value. */
    float exposure = 1.0f;
    /** The gamma value. */
    float gamma = 1.0f;

    // ----------------------------------------------------------------------------
    // Constructors
    // ----------------------------------------------------------------------------
  public:
    Application(int initial_width, int initial_height, std::vector<std::string> arguments = {});

    /** Destroys the {@link Application} and releases the allocated resources. */
    virtual ~Application();

    // ----------------------------------------------------------------------------
    // Shaders
    // ----------------------------------------------------------------------------
    /**
     * {@copydoc PA213Application::compile_shaders}
     */
    void compile_shaders() override;

    // ----------------------------------------------------------------------------
    // Initialize Scene
    // ----------------------------------------------------------------------------
  public:
    /** Prepares the required cameras. */
    void prepare_cameras();

    /** Prepares the required materials. */
    void prepare_materials();

    /** Prepares the required textures. */
    void prepare_textures();

    /** Prepares the lights. */
    void prepare_lights();

    /** Sets 'lrule_edit_props' and 'laxiom' to default values. */
    void set_default_lrules();

    /** Prepares the scene objects. */
    void prepare_scene();

    // ----------------------------------------------------------------------------
    // Update
    // ----------------------------------------------------------------------------
    /**
     * {@copydoc PA213Application::update}
     */
    void update(float delta) override;

    // ----------------------------------------------------------------------------
    // Render
    // ----------------------------------------------------------------------------
  public:
    /** @copydoc PA213Application::render */
    void render() override;

    /** Uses ray tracing on GPU to render the scene. */
    void ray_tracing();

    // ----------------------------------------------------------------------------
    // GUI
    // ----------------------------------------------------------------------------
  public:
    /** @copydoc PA213Application::render_ui */
    void render_ui() override;
};
