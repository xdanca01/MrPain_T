// ################################################################################
// Common Framework for Computer Graphics Courses at FI MUNI.
//
// Copyright (c) 2021-2022 Visitlab (https://visitlab.fi.muni.cz)
// All rights reserved.
// ################################################################################

#pragma once

#include "camera.h"
#include "cube.hpp"
#include "pv112_application.hpp"
#include "sphere.hpp"
#include "teapot.hpp"

// ----------------------------------------------------------------------------
// UNIFORM STRUCTS
// ----------------------------------------------------------------------------

struct CameraUBO {
    glm::mat4 projection;
    glm::mat4 view;
    glm::vec4 position;
};

struct LightUBO {
    glm::vec4 position;
    glm::vec4 ambient_color;
    glm::vec4 diffuse_color;
    glm::vec4 specular_color;
};

struct alignas(256) ObjectUBO {
    glm::mat4 model_matrix;  // [  0 -  64) bytes
    glm::vec4 ambient_color; // [ 64 -  80) bytes
    glm::vec4 diffuse_color; // [ 80 -  96) bytes

    // Contains shininess in .w element
    glm::vec4 specular_color; // [ 96 - 112) bytes
};

const float square_ref_model[] = {
    0.0f, 0.0f, 0.0f, //BL
    1.0f, 0.0f, 0.0f, //BR
    0.0f, 1.0f, 0.0f, //TL

    0.0f, 1.0f, 0.0f, //TL
    1.0f, 0.0f, 0.0f, //BR
    1.0f, 1.0f, 0.0f, //TR
};


// Constants
const float clear_color[4] = {0.0, 0.0, 0.0, 1.0};
const float clear_depth[1] = {1.0};

class Application : public PV112Application {

    // ----------------------------------------------------------------------------
    // Variables
    // ----------------------------------------------------------------------------
    std::filesystem::path images_path;
    std::filesystem::path objects_path;

    // Main program
    GLuint main_program;
    GLuint square_program;
    GLuint parking_program;
    // TODO: feel free to add as many as you need/like
    GLuint square_buffer_id = 0;


    // List of geometries used in the project
    std::vector<std::shared_ptr<Geometry>> geometries;
    // Shared pointers are pointers that automatically count how many times they are used. When there are 0 pointers to the object pointed
    // by shared_ptrs, the object is automatically deallocated. Consequently, we gain 3 main properties:
    // 1. Objects are not unnecessarily copied
    // 2. We don't have to track pointers
    // 3. We don't have to deallocate these geometries
    std::shared_ptr<Geometry> car;
    std::shared_ptr<Geometry> parking;
    std::shared_ptr<Geometry> barrier;
    std::shared_ptr<Geometry> cone;

    

    // Default camera that rotates around center.
    Camera camera;

    // UBOs
    GLuint camera_buffer = 0;
    CameraUBO camera_ubo;

    GLuint objects_buffer = 0;
    std::vector<ObjectUBO> objects_ubos;

    std::vector<ObjectUBO> square_ubos;

    // Lights
    std::vector<LightUBO> lights;
    GLuint lights_buffer = 0;

    // Textures
    GLuint marble_texture = 0;
    GLuint concrete_texture = 0;
    GLuint rock_texture = 0;
    std::vector<GLuint> textures;
    std::vector<GLuint> normals;

    //Step
    unsigned int step = 0;
    //Bright textures
    bool bright = false;
    bool fog = false;
    bool phong = true;
    bool phong_parking = true;
    bool toon = false;
    bool has_cone = true;
    bool play = true;
    bool camera1 = true;
    glm::vec3 camera_target = glm::vec3(0.0f);
    float cameraStep = 0.05f;

    // ----------------------------------------------------------------------------
    // Constructors & Destructors
    // ----------------------------------------------------------------------------
  public:
    /**
     * Constructs a new @link Application with a custom width and height.
     *
     * @param 	initial_width 	The initial width of the window.
     * @param 	initial_height	The initial height of the window.
     * @param 	arguments	  	The command line arguments used to obtain the application directory.
     */
    Application(int initial_width, int initial_height, std::vector<std::string> arguments = {});

    /** Destroys the {@link Application} and releases the allocated resources. */
    ~Application() override;

    // ----------------------------------------------------------------------------
    // Methods
    // ----------------------------------------------------------------------------

    //metoda pro krok animace
    void animate();
    /** @copydoc PV112Application::compile_shaders */
    void compile_shaders() override;

    /** @copydoc PV112Application::delete_shaders */
    void delete_shaders() override;

    /** @copydoc PV112Application::update */
    void update(float delta) override;

    /** @copydoc PV112Application::render */
    void render() override;

    /** @copydoc PV112Application::render_ui */
    void render_ui() override;

    // ----------------------------------------------------------------------------
    // Input Events
    // ----------------------------------------------------------------------------

    /** @copydoc PV112Application::on_resize */
    void on_resize(int width, int height) override;

    /** @copydoc PV112Application::on_mouse_move */
    void on_mouse_move(double x, double y) override;

    /** @copydoc PV112Application::on_mouse_button */
    void on_mouse_button(int button, int action, int mods) override;

    /** @copydoc PV112Application::on_key_pressed */
    void on_key_pressed(int key, int scancode, int action, int mods) override;
};
