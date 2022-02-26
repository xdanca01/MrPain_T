// ###############################################################################
// 
// Visitlab Playground - Visualization & Computer Graphics Toolkit.
// 
// Copyright (c) 2021-2022 Visitlab (https://visitlab.fi.muni.cz)
// All rights reserved.
// 
// ################################################################################

#pragma once

#include "advanced_scene_object.hpp"
#include "geometry.hpp"
#include "imgui.h"
#include "pa213_application.h"
#include "program.hpp"
#include "scene/model_ubo.hpp"
#include "texture.hpp"
#include "utils.hpp"
#include <vector>
#include <unordered_map>
#include <map>
#include <array>
#include <memory>

struct Particle {
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 force;
};

struct BoundaryPlane {
    glm::vec3 origin;
    glm::vec3 unit_normal;
    float dot_normal_origin;
};

struct ParticleSystem {
    ParticleSystem();

    ~ParticleSystem();

    void clear_particles();

    void insert_particle(glm::vec3 const& position, glm::vec3 const& velocity);

    void insert_boundary(glm::vec3 const& origin, glm::vec3 const& unit_normal);

    void update(float time_step);

    void get_triangles_of_particles(
        glm::vec3 const& camera_I,
        glm::vec3 const& camera_J,
        float particle_radius_scale,
        std::vector<float>& coords,
        std::vector<uint32_t>& indices,
        std::vector<float>& tex_coords
        ) const;

    static float const max_time_buffer;
    static float const time_step_internal;
    static float const time_step_single;
    static float const particle_radius;
    static float const particle_mass;
    static float const particle_viscous_drag_coef;
    static float const fluid_force_reach;
    static float const cell_size;
    static float const gravity_accel;
    static glm::vec3 gravity_accel_dir;
    static glm::vec2 const restitution_coef_fluid;
    static glm::vec2 const restitution_coef_boundary;
    static int const num_collision_response_iterations;
    static float const lennard_jones_m;
    static float const lennard_jones_n;
    static float const lennard_jones_k;

private:
    struct CellId {
        int x, y, z;

        bool operator==(CellId const& other) const;
    };

    struct CellIdHash {
        size_t operator()(CellId const& id) const;
    };

    using ParticlesInCells = std::unordered_map<CellId, std::vector<Particle*>, CellIdHash>;

    struct CollisionParticles {
        Particle* p1;
        Particle* p2;
        glm::vec3 unit_distance_vector;
        float penetration;
    };

    using ParticleCollisions = std::vector<CollisionParticles>;

    struct CollisionBoundary {
        Particle* particle;
        BoundaryPlane const* boundary;
        float penetration;
    };

    using BoundaryCollisions = std::vector<CollisionBoundary>;

    void update_internal(float time_step);

    void swap_cells();

    static CellId to_cell_id(glm::vec3 const& position);

    std::array<ParticlesInCells, 2> cells;
    ParticlesInCells* current_cells;
    ParticlesInCells* future_cells;
    std::vector<BoundaryPlane> planes;
    float time_buffer;
};

class Application : public PA213Application {

public:
    Application(int initial_width, int initial_height, std::vector<std::string> arguments);

    ~Application();

    void update(float time_step_milliseconds) override;

    void render() override;

    void render_ui() override;

    void compile_shaders() override;

private:
    void generate_particles();

    void rebuild_fluid_render_targets_if_needed();

    void create_fluid_render_targets();

    void destroy_fluid_render_targets();

    using TexturePtr = std::unique_ptr<Texture>;

    ParticleSystem particles;
    ShaderProgram gpu_program_billboards;
    ShaderProgram gpu_program_depth;
    ShaderProgram gpu_program_smooth;
    ShaderProgram gpu_program_normals;
    ShaderProgram gpu_program_lighting;
    ShaderProgram gpu_program_ground;
    TexturePtr texture_fluid_depth;
    GLuint texture_fluid_colour_aux;
    TexturePtr texture_fluid_depth_smooth;
    TexturePtr texture_fluid_depth_smooth2;
    GLuint texture_fluid_colour_smooth_aux;
    TexturePtr texture_fluid_depth_normals_aux;
    GLuint texture_fluid_colour_normals;
    GLuint fbo_fluid_depth;
    GLuint fbo_fluid_smooth;
    GLuint fbo_fluid_smooth2;
    GLuint fbo_fluid_normals;
    Geometry geometry_ground;
    bool paused;
    bool do_single_step;
    bool show_ground;
    bool show_billboards;
    bool show_depth_texture;
    bool show_depth_smooth_texture;
    bool show_normals_texture;
    int smoothing_radius;
    int num_smooting_passes;
    float smooting_gauss_sigma;
    float smooting_depth_falloff;
    float diffuse_coef;
    float specular_coef;
    float specular_exponent;
    float particle_radius_scale;
    int fluid_gen_hight;
    bool use_fixed_fluid_render_target_size;
    static glm::i32vec2 const ground_size;
    static glm::i32vec2 const desired_fluid_size;
    static glm::i32vec3 const desired_fluid_origin_shift;
    static glm::vec3 const light_direction;
    static glm::vec4 const light_color;
    static glm::vec4 const ambient_color;
    static glm::i32vec2 const fixed_fluid_render_target_size;
};
