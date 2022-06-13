// ################################################################################
// Common Framework for Computer Graphics Courses at FI MUNI.
// 
// Copyright (c) 2021-2022 Visitlab (https://visitlab.fi.muni.cz)
// All rights reserved.
// ################################################################################

#pragma once

#include "pv112_application.hpp"

class Application : public PV112Application {

    // ----------------------------------------------------------------------------
    // Variables
    // ----------------------------------------------------------------------------
private:
    GLuint program = 0;

    // Diamond
    GLuint diamond_vao = 0;
    GLuint diamond_positions_vbo = 0;
    GLuint diamond_colors_vbo = 0;

    // Square
    GLuint square_vao = 0;
    GLuint square_vbo = 0;

    // Triangle
    GLuint triangle_vao = 0;
    GLuint triangle_vbo = 0;

    // Diamond Indexed
    GLuint diamond_indexed_vao = 0;
    GLuint diamond_indices_vbo = 0;
    GLuint diamond_positions_indexed_vbo = 0;
    GLuint diamond_colors_indexed_vbo = 0;

    // UI Variables
    int ui_polygon_mode = 2; // 0 = GL_POINT, 1 = GL_LINE, 2 = GL_FILL
    bool ui_depth = true;
    bool ui_blending = false;
    int ui_face_cull = 1;

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
    virtual ~Application();

    // ----------------------------------------------------------------------------
    // Methods
    // ----------------------------------------------------------------------------

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
