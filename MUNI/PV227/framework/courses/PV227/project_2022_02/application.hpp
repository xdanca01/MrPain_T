// ################################################################################
// Common Framework for Computer Graphics Courses at FI MUNI.
//
// Copyright (c) 2021-2022 Visitlab (https://visitlab.fi.muni.cz)
// All rights reserved.
// ################################################################################

#pragma once
#include "camera_ubo.hpp"
#include "light_ubo.hpp"
#include "pv227_application.hpp"
#include "scene_object.hpp"

class Application : public PV227Application {
    // ----------------------------------------------------------------------------
    // Variables (Geometry)
    // ----------------------------------------------------------------------------
  protected:
    /** The lake object. */
    SceneObject lake_object;
    /** The outer terrain object. */
    SceneObject outer_terrain_object;
    /** The snow terrain object. */
    SceneObject snow_terrain_object;
    /** The grassy castle base. */
    SceneObject castel_base;
    /** The castle object. */
    SceneObject castle_object;

    /** The broom position. */
    glm::vec2 broom_center;

    /** The scene object storing information about the broom model. */
    SceneObject broom_object;
    /** The scene object representing the light. */
    SceneObject light_object;

    /** The data for the particles. */
    GLuint snow_positions_bo;
    GLuint snow_vao;

    // ----------------------------------------------------------------------------
    // Variables (Materials)
    // ----------------------------------------------------------------------------
  protected:
    /** The material for the terrain. */
    PhongMaterialUBO brown_material_ubo;
    /** The material for the castle. */
    PhongMaterialUBO castle_material_ubo;
    /** The material for the snow. */
    PhongMaterialUBO snow_material_ubo;

    // ----------------------------------------------------------------------------
    // Variables (Textures)
    // ----------------------------------------------------------------------------
  protected:
    /** The texture for broom. */
    GLuint broom_tex;
    /** The ice texture. */
    GLuint ice_albedo_tex;
    /** The texture for the snow particles. */
    GLuint particle_tex;
    /** The snow albedo texture. */
    GLuint snow_albedo_tex;
    /** The snow normal texture. */
    GLuint snow_normal_tex;
    /** The snow height texture. */
    GLuint snow_height_tex;
    /** The snow roughness */
    GLuint snow_roughness_tex;
    GLuint snow_mask_1_tex;
    GLuint snow_mask_2_tex;
    GLuint shadow_texture;
    glm::mat4 shadow_matrix;

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
    /** The camera projection matrix. */
    glm::mat4 projection_matrix;
    /** The UBO storing the information about the camera. */
    CameraUBO camera_ubo;

    CameraUBO light_camera_data_ubo;
    /** The projection matrix of the light camera. */
    glm::mat4 light_camera_projection;
    /** The view matrix of the light camera. */
    glm::mat4 light_camera_view;

    // ----------------------------------------------------------------------------
    // Variables (Shaders)
    // ----------------------------------------------------------------------------
  protected:
    /** A shader for rendering snowflakes. */
    ShaderProgram snowflakes_program;
    ShaderProgram render_snow_mask_program;
    ShaderProgram render_shadow_mask_program;
    ShaderProgram blur_texture;

    // ----------------------------------------------------------------------------
    // Variables (Frame Buffers)
    // ----------------------------------------------------------------------------
  protected:
    GLuint gbuffer_fbo_1 = 0;
    GLuint gbuffer_fbo_2 = 0;
    GLuint shadow_fbo = 0;
    GLsizei shadow_tex_size = 1024;
    // ----------------------------------------------------------------------------
    // Variables (GUI)
    // ----------------------------------------------------------------------------
  protected:
    /** The light position set in the GUI. */
    float gui_light_position = glm::radians(360.f);

    /** The desired snow particle count. */
    int desired_snow_count = 2048;

    /** The current snow particle count. */
    int current_snow_count = 256;

    bool secondBuffer = false;
    bool clear_snow = false;
    int cnt = 0;
    /** The flag determining if a snow should be visible. */
    bool show_snow = true;

    /** The flag determining polygon fill mode. */
    bool wireframe = false;

    /** The flag determining if PBR model will be used. */
    bool pbr = false;

    /** The flag determining if tessellated snow will be visible. */
    bool show_tessellated_snow = true;
    // ----------------------------------------------------------------------------
    // Constructors
    // ----------------------------------------------------------------------------
  public:
    Application(int initial_width, int initial_height, std::vector<std::string> arguments = {});

    /** Destroys the {@link Application} and releases the allocated resources. */
    ~Application() override;

    // ----------------------------------------------------------------------------
    // Shaders
    // ----------------------------------------------------------------------------
    /**
     * {@copydoc PV227Application::compile_shaders}
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

    /** Prepares the scene objects. */
    void prepare_scene();

    /** Prepares the snow.*/
    void prepare_snow();

    /** Prepares the frame buffer objects. */
    void prepare_framebuffers();

    /** Resizes the full screen textures match the window. */
    void resize_fullscreen_textures();

    /** Applies blur. */
    void apply_blur();

    // ----------------------------------------------------------------------------
    // Update
    // ----------------------------------------------------------------------------
    /**
     * {@copydoc PV227Application::update}
     */
    void update(float delta) override;

    /**
     * Updates the broom location. Note that this method expects that the currently bind depth buffer contains
     * information about the whole scene (except the broom itself).
     */
    void update_broom_location();

    // ----------------------------------------------------------------------------
    // Render
    // ----------------------------------------------------------------------------
  public:
    /** @copydoc PV227Application::render */
    void render() override;

    void render_height_mask();

    /** Renders the whole scene. */
    void render_scene(const ShaderProgram& program, bool render_broom);

    /** Renders the snow.*/
    void render_snow();

    void render_snow_mask();

    void render_broom_mask();

    /** Renders the specified object. */
    void render_object(const SceneObject& object, const ShaderProgram& program, bool render_as_patches, bool use_snow = false, bool is_lit = false, float uv_multiplier = 1) const;

    /** Clears the accumulated snow. */
    void clear_accumulated_snow();

    // ----------------------------------------------------------------------------
    // GUI
    // ----------------------------------------------------------------------------
  public:
    /** @copydoc PV227Application::render_ui */
    void render_ui() override;

    // ----------------------------------------------------------------------------
    // Input Events
    // ----------------------------------------------------------------------------
  public:
    /** @copydoc PV227Application::on_resize */
    void on_resize(int width, int height) override;

    /** @copydoc PV227Application::on_mouse_move */
    void on_mouse_move(double x, double y) override;
};
