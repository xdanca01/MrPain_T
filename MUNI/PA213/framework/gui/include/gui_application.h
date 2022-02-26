// ################################################################################
// Common Framework for Computer Graphics Courses at FI MUNI.
//
// Copyright (c) 2021-2022 Visitlab (https://visitlab.fi.muni.cz)
// All rights reserved.
// ################################################################################

#pragma once

#include "camera.h"
#include "iapplication.h"
#include "imgui.h"

class GUIApplication : public IApplication {

    // ----------------------------------------------------------------------------
    // Variables
    // ----------------------------------------------------------------------------
  protected:
    /** The default camera. */
    Camera camera;

    // ----------------------------------------------------------------------------
    // Constructors & Destructors
    // ----------------------------------------------------------------------------
  public:
    /**
     * Constructs a new @link GUIApplication with a custom width and height.
     *
     * @param 	initial_width 	The initial width of the window.
     * @param 	initial_height	The initial height of the window.
     * @param 	arguments	  	The command line arguments used to obtain the application directory.
     */
    GUIApplication(int initial_width, int initial_height, std::vector<std::string> arguments);

    /** The destructor that releases all resources. */
    ~GUIApplication() override;

    // ----------------------------------------------------------------------------
    // Shaders
    // ----------------------------------------------------------------------------

    /**
     * Compiles shaders and deletes previous ones (if necessary).
     * TODO: The implementation should be left to the child class.
     */
    virtual void compile_shaders() {
        delete_shaders();
    }

    /**
     * Deletes shaders. The method is supposed to be called in destructor as well as at the beginning of @link
     * compile_shaders method.
     * TODO: The implementation should be left to the child class.
     */
    virtual void delete_shaders() {}

    // ----------------------------------------------------------------------------
    // Input Events
    // ----------------------------------------------------------------------------

    /**
     * @copydoc IApplication::on_key_pressed
     * <p>
     * The implementation passes mouse movement to camera.
     */
    void on_mouse_move(double x, double y) override;

    /**
     * @copydoc IApplication::on_key_pressed
     * <p>
     * The implementation passes the event to camera.
     */
    void on_mouse_button(int button, int action, int mods) override;

    /**
     * @copydoc IApplication::on_key_pressed
     * <p>
     * The implementation passes the input to ImGui. It also invokes @link compile_shaders if 'R' is pressed.
     */
    void on_key_pressed(int key, int scancode, int action, int mods) override;
};
