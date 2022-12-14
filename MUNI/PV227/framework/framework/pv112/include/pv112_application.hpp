// ################################################################################
// Common Framework for Computer Graphics Courses at FI MUNI.
// 
// Copyright (c) 2021-2022 Visitlab (https://visitlab.fi.muni.cz)
// All rights reserved.
// ################################################################################

#pragma once

#include "gui_application.h"
#include "utilities.hpp"
#include "glm_headers.hpp"

class PV112Application : public GUIApplication {

    // ----------------------------------------------------------------------------
    // Variables
    // ----------------------------------------------------------------------------
protected:
    /** The absolute path to applications shaders. Loaded from {@link configuration} if a configuration file is available. */
    std::filesystem::path lecture_shaders_path;

public:
    // ----------------------------------------------------------------------------
    // Constructors & Destructors
    // ----------------------------------------------------------------------------

    /**
     * Constructs a new @link PV112Application with a custom width and height.
     *
     * @param 	initial_width 	The initial width of the window.
     * @param 	initial_height	The initial height of the window.
     * @param 	arguments	  	The command line arguments used to obtain the application directory.
     */
    PV112Application(int initial_width, int initial_height, std::vector<std::string> arguments = {});

    /** The destructor that releases all resources. */
    virtual ~PV112Application();

    // ----------------------------------------------------------------------------
    // Methods
    // ----------------------------------------------------------------------------

    /**
     * Deletes shaders. The method is supposed to be called in destructor as well as at the beginning of @link compile_shaders method.
     */
    virtual void delete_shaders() {
    }
};
