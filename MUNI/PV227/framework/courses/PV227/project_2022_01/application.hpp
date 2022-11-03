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
    /** The grassy castle base. */
    SceneObject castel_base;
    /** The castle object. */
    SceneObject castle_object;

    // ----------------------------------------------------------------------------
    // Variables (Particles)
    // ----------------------------------------------------------------------------
  protected:
    /** The current number of particles in simulation. */
    int current_particle_count;
    /** The maximum number of particles for which the memory is allocated. */
    int max_particle_count = 131072;
    /** The positions of all particles. */
    GLuint particle_positions_bo;
    /** The velocities of all particles. */
    GLuint particle_velocities_bo;
    /** The colors of all particles. */
    GLuint particle_colors_bo;
    /** VAOs for rendering particles, one for each buffer with positions. */
    GLuint particle_vao;
    /** The UBO storing the data about lights - positions, colors, etc. */
    PhongLightsUBO phong_lights_bo;

    // ----------------------------------------------------------------------------
    // Variables (Textures)
    // ----------------------------------------------------------------------------
  protected:
    /** The particle texture. */
    GLuint particle_tex;

    // ----------------------------------------------------------------------------
    // Variables (Camera)
    // ----------------------------------------------------------------------------
  protected:
    /** The UBO storing the information about the normal camera. */
    CameraUBO normal_camera_ubo;

    // ----------------------------------------------------------------------------
    // Variables (Shaders)
    // ----------------------------------------------------------------------------
  protected:
    /** The program for rendering particles as a textured quad (generated in compute shader). */
    ShaderProgram particle_textured_program;
    /** The simple program for updating particles. */
    ShaderProgram update_particles_program;

    // ----------------------------------------------------------------------------
    // Variables (Frame Buffers)
    // ----------------------------------------------------------------------------
  protected:

    // ----------------------------------------------------------------------------
    // Variables (GUI)
    // ----------------------------------------------------------------------------
  protected:
    /** The number of fireworks. */
    int fireworks_count = 1;
    /** The number of particles to be used in simulation. */
    int desired_particle_count = 256;
    /** The flag determining if there should be waves on the water. */
    bool waves = false;
    /** The factor that determines how perfect the mirror is. */
    float mirror_factor = 0.8f;
    /** The gravitational acceleration. */
    float gravity;
    /** The size of one particle (in world/view space) when using geometry shaders. */
    float particle_size;
    /** The life span of each particle */
    float lifespan;
    /** The minimum initial velocity. */
    float min_initial_velocity;
    /** The maximum initial velocity. */
    float max_initial_velocity;
    /** The time for which the rocket will be sitting on the ground. */
    float init_delay;
    /** The explosion time - defines a time after which the firework goes boom. */
    float explosion_delay;
    /** The explosion thrust time - defines the time during which the particles accelerate from each other. */
    float explosion_thrust_time;
    /** The force applied during the thrust time. */
    float explosion_force;
    /** The time that it takes before the the particles are increased in size after explosion. */
    float explosion_light_delay;
    /** The time after which the exploded fireworks starts to fade away. */
    float fade_delay;
    /** The time it takes for the particles to completely fade away. */
    float fade_time;
    /** The spread of the initial directions. */
    float rocket_spread;
    /** The time it takes to start blinking. */
    float blink_delay;
    /** The blinking frequency. */
    float blink_frequency;
    /** The blink ratio. */
    float blink_ratio;

    glm::vec4 particle_color;

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
     * {@copydoc PV227Application::compile_shaders}
     */
    void compile_shaders() override;

    // ----------------------------------------------------------------------------
    // Initialize Scene
    // ----------------------------------------------------------------------------
  public:
    /** Prepares the required cameras. */
    void prepare_cameras();

    /** Prepares the lights. */
    void prepare_lights();

    /** Prepares the required materials. */
    void prepare_materials();

    /** Prepares the required textures. */
    void prepare_textures();

    /** Prepares the scene objects. */
    void prepare_scene();

    /** Prepares the frame buffer objects. */
    void prepare_framebuffers();

    /** Resizes the full screen textures match the window. */
    void resize_fullscreen_textures();

    // ----------------------------------------------------------------------------
    // Update
    // ----------------------------------------------------------------------------
    /**
     * {@copydoc PV227Application::update}
     */
    void update(float delta) override;

    /** Resets the positions and velocities of all particles in the simulation. */
    void reset_particles();

    /** Updates the positions and velocities of all particles on GPU. */
    void update_particles_gpu(float delta);

    // ----------------------------------------------------------------------------
    // Render
    // ----------------------------------------------------------------------------
  public:
    /** @copydoc PV227Application::render */
    void render() override;

    /** Renders the whole scene. */
    void render_scene(const ShaderProgram& program);

    /** Renders the specified object. */
    void render_object(const SceneObject& object, const ShaderProgram& program);

    /** Sets up the frame buffer and evaluates the lighting using forward shading. */
    void evaluate_lighting_forward();

    /** Renders the particles. */
    void render_particles();

    // ----------------------------------------------------------------------------
    // GUI
    // ----------------------------------------------------------------------------
  public:
    /** @copydoc PV227Application::render_ui */
    void render_ui() override;

    /** Resets the UI to the default values. */
    void reset_simulation_settings();

    // ----------------------------------------------------------------------------
    // Input Events
    // ----------------------------------------------------------------------------
  public:
    /** @copydoc PV227Application::on_resize */
    void on_resize(int width, int height) override;
};
