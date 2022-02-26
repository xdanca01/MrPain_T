################################################################################
# Common Framework for Computer Graphics Courses at FI MUNI.
#
# Copyright (c) 2021-2022 Visitlab (https://visitlab.fi.muni.cz)
# All rights reserved.
#
# Module: OPENGL
################################################################################

# Creates the module.
visitlab_create_module(module_name)

# Specifies the include directories to use when compiling the module.
target_include_directories(${module_name} PUBLIC include include/opengl include/scene)

# Collects the source files and specified them as target sources.
target_sources(
    ${module_name} 
    PRIVATE 
        include/opengl/opengl_object.hpp
        include/opengl/program.hpp
        include/opengl/program_map.hpp
        include/opengl/shader.hpp
        include/opengl/texture.hpp
        include/opengl/ubo.hpp
        include/scene/camera_ubo.hpp
        include/scene/light_ubo.hpp
        include/scene/material_ubo.hpp
        include/scene/model_ubo.hpp
        include/scene/pbr_material_ubo.hpp
        include/scene/phong_material_ubo.hpp
        include/scene/scene_object.hpp
        include/scene/scene_object_deprecated.hpp
        include/color.hpp
        include/opengl_application.hpp
        include/utils.hpp
        src/opengl/program.cpp
        src/opengl/program_map.cpp
        src/opengl/shader.cpp
        src/opengl/texture.cpp
        src/color.cpp
        src/opengl_application.cpp
        src/utils.cpp
)