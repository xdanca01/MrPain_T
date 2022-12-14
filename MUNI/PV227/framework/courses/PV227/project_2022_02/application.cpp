#include "application.hpp"
#include "utils.hpp"
#include <map>

Application::Application(int initial_width, int initial_height, std::vector<std::string> arguments)
    : PV227Application(initial_width, initial_height, arguments) {
    Application::compile_shaders();
    prepare_cameras();
    prepare_materials();
    prepare_textures();
    prepare_lights();
    prepare_scene();
    prepare_framebuffers();
    render_height_mask();
}

Application::~Application() {}

// ----------------------------------------------------------------------------
// Shaderes
// ----------------------------------------------------------------------------
void Application::compile_shaders() {
    default_unlit_program = ShaderProgram(lecture_shaders_path / "object.vert", lecture_shaders_path / "unlit.frag");
    default_lit_program = ShaderProgram(lecture_shaders_path / "object.vert", lecture_shaders_path / "lit.frag");
    
    snow_tesselation_program = ShaderProgram();
    snow_tesselation_program.add_vertex_shader(lecture_shaders_path / "object.vert");
    snow_tesselation_program.add_tess_control_shader(lecture_shaders_path / "snow_control.tesc");
    snow_tesselation_program.add_tess_evaluation_shader(lecture_shaders_path / "snow_control.tese");
    snow_tesselation_program.add_fragment_shader(lecture_shaders_path / "snowTess.frag");
    snow_tesselation_program.link();
    
    shadow_map_program = ShaderProgram(lecture_shaders_path / "object.vert", lecture_shaders_path / "nothing.frag");

    snowflakes_program = ShaderProgram();
    snowflakes_program.add_vertex_shader(lecture_shaders_path / "snow.vert");
    snowflakes_program.add_geometry_shader(lecture_shaders_path / "snow.geom");
    snowflakes_program.add_fragment_shader(lecture_shaders_path / "snow.frag");
    snowflakes_program.link();

    render_snow_mask_program = ShaderProgram(lecture_shaders_path / "full_screen_quad.vert", lecture_shaders_path / "increase_snow.frag");
    render_shadow_mask_program = ShaderProgram(lecture_shaders_path / "full_screen_quad.vert", lecture_shaders_path / "display_texture.frag");
    blur_texture = ShaderProgram(lecture_shaders_path / "full_screen_quad.vert", lecture_shaders_path / "dof.frag");

    std::cout << "Shaders are reloaded." << std::endl;
}

// ----------------------------------------------------------------------------
// Initialize Scene
// ----------------------------------------------------------------------------
void Application::prepare_cameras() {
    // Sets the default camera position.
    camera.set_eye_position(glm::radians(-45.f), glm::radians(20.f), 50.f);

    // Sets the projection matrix for the normal and mirrored cameras.
    projection_matrix =
        glm::perspective(glm::radians(45.f), static_cast<float>(this->width) / static_cast<float>(this->height), 0.1f, 5000.0f);

    camera_ubo.set_projection(projection_matrix);
    camera_ubo.update_opengl_data();


    light_camera_projection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 10.0f, 16.0f);
    light_camera_view = glm::lookAt(glm::vec3(0.0f, 15.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f)); // <- modify this
    //light_camera_projection = glm::perspective(glm::radians(45.0f), 1.0f, 12.0f, 30.0f); // <- modify this
    light_camera_data_ubo.set_projection(light_camera_projection);
    //light_camera_view = glm::lookAt(glm::vec3(0.0f, 15.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)); // <- modify this
    light_camera_data_ubo.set_view(light_camera_view);
    light_camera_data_ubo.update_opengl_data();
}

void Application::prepare_materials() {
    glm::vec3 brown_color = glm::vec3(0.5f, 0.25f, 0.0f);
    brown_material_ubo.set_material(PhongMaterialData(brown_color * 0.1, brown_color * 0.9, true, glm::vec3(0.1), 2.0f));
    brown_material_ubo.update_opengl_data();

    glm::vec3 castle_color = glm::vec3(0.95f, 0.75f, 0.55f);
    castle_material_ubo.set_material(PhongMaterialData(castle_color * 0.1, castle_color * 0.9, true, glm::vec3(0.1), 2.0f));
    castle_material_ubo.update_opengl_data();

    snow_material_ubo.set_material(PhongMaterialData(glm::vec3(0.1f), glm::vec3(0.9f), true, glm::vec3(0.1), 2.0f));
    snow_material_ubo.update_opengl_data();
}

void Application::prepare_textures() {
    broom_tex = TextureUtils::load_texture_2d(lecture_textures_path / "wood.jpg");
    TextureUtils::set_texture_2d_parameters(broom_tex, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);

    ice_albedo_tex = TextureUtils::load_texture_2d(lecture_textures_path / "ice_albedo.png");
    TextureUtils::set_texture_2d_parameters(ice_albedo_tex, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);

    snow_albedo_tex = TextureUtils::load_texture_2d(lecture_textures_path / "snow_albedo.png");
    TextureUtils::set_texture_2d_parameters(snow_albedo_tex, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);

    snow_normal_tex = TextureUtils::load_texture_2d(lecture_textures_path / "snow_normal.png");
    TextureUtils::set_texture_2d_parameters(snow_normal_tex, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);

    snow_height_tex = TextureUtils::load_texture_2d(lecture_textures_path / "snow_height.png");
    TextureUtils::set_texture_2d_parameters(snow_height_tex, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);

    snow_roughness_tex = TextureUtils::load_texture_2d(lecture_textures_path / "snow_roughness.png");
    TextureUtils::set_texture_2d_parameters(snow_roughness_tex, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);

    particle_tex = TextureUtils::load_texture_2d(lecture_textures_path / "star.png");
    TextureUtils::set_texture_2d_parameters(particle_tex, GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
}

void Application::prepare_lights() {
    // The rest is set in the update scene method.
    phong_lights_ubo.set_global_ambient(glm::vec3(0.0f));
}

void Application::prepare_framebuffers() {
    glCreateFramebuffers(1, &gbuffer_fbo_1);
    glCreateFramebuffers(1, &gbuffer_fbo_2);

    glNamedFramebufferDrawBuffers(gbuffer_fbo_1, 1, FBOUtils::draw_buffers_constants);
    glNamedFramebufferDrawBuffers(gbuffer_fbo_2, 1, FBOUtils::draw_buffers_constants);

    glCreateFramebuffers(1, &shadow_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, shadow_fbo);
    
    
    glCreateTextures(GL_TEXTURE_2D, 1, &snow_mask_1_tex);
    glCreateTextures(GL_TEXTURE_2D, 1, &snow_mask_2_tex);

    glTextureStorage2D(snow_mask_1_tex, 1, GL_RGBA32F, shadow_tex_size, shadow_tex_size);
    glTextureStorage2D(snow_mask_2_tex, 1, GL_RGBA32F, shadow_tex_size, shadow_tex_size);

    TextureUtils::set_texture_2d_parameters(snow_mask_1_tex, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    TextureUtils::set_texture_2d_parameters(snow_mask_2_tex, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    glNamedFramebufferTexture(gbuffer_fbo_1, GL_COLOR_ATTACHMENT0, snow_mask_1_tex, 0);
    FBOUtils::check_framebuffer_status(gbuffer_fbo_1, "G-buffer");

    glNamedFramebufferTexture(gbuffer_fbo_2, GL_COLOR_ATTACHMENT0, snow_mask_2_tex, 0);
    FBOUtils::check_framebuffer_status(gbuffer_fbo_2, "G-buffer");

    glCreateTextures(GL_TEXTURE_2D, 1, &shadow_texture);
    glTextureStorage2D(shadow_texture, 1, GL_DEPTH_COMPONENT24, shadow_tex_size, shadow_tex_size);
    TextureUtils::set_texture_2d_parameters(shadow_texture, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_NEAREST, GL_NEAREST);

    glNamedFramebufferTexture(shadow_fbo, GL_DEPTH_ATTACHMENT, shadow_texture, 0);
    FBOUtils::check_framebuffer_status(shadow_fbo, "ShadowFBO");
    resize_fullscreen_textures();
}

void Application::resize_fullscreen_textures() {
}

void Application::prepare_scene() {

    Geometry plane = Geometry::from_file(lecture_folder_path / "models/plane.obj");
    snow_terrain_object = SceneObject(
        plane, ModelUBO(scale(translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)) * glm::mat4(1.0f), glm::vec3(26.f, 1.f, 26.f))),
        snow_material_ubo, snow_albedo_tex);

    outer_terrain_object = SceneObject(
        cube, ModelUBO(translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.08f, 0.0f)) * scale(glm::mat4(1.0f), glm::vec3(13.0f, 0.1f, 13.0f))),
        brown_material_ubo);

    castel_base = SceneObject(
        cube, ModelUBO(translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.05f, 0.0f)) * scale(glm::mat4(1.0f), glm::vec3(3.8f, 0.1f, 3.8f))),
        brown_material_ubo);

    lake_object = SceneObject(
        cube, ModelUBO(translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.05f, 0.0f)) * scale(glm::mat4(1.0f), glm::vec3(12.f, 0.1f, 12.f))),
        blue_material_ubo, ice_albedo_tex);

    Geometry castle = Geometry::from_file(lecture_folder_path / "models/castle.obj");
    castle_object = SceneObject(
        castle, ModelUBO(scale(translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.06f, 0.0f)) * glm::mat4(1.0f), glm::vec3(7.f, 7.f, 7.f))),
        castle_material_ubo);

    // Prepares the broom model - rest is set in the update method.
    Geometry broom = Geometry::from_file(lecture_folder_path / "models/broom.obj");
    broom_object = SceneObject(broom, ModelUBO(), white_material_ubo, broom_tex);

    // The light model is placed based on the value from UI so we update its model matrix later.
    // We are also not adding it to the scene_objects since we render it separately.
    light_object = SceneObject(sphere, ModelUBO(), white_material_ubo);
}

void Application::prepare_snow() {
    // Initialize snowflakes positions.
    std::vector<glm::vec3> snow_initial_positions(current_snow_count);
    for (int i = 0; i < current_snow_count; i++)
        snow_initial_positions[i] = glm::vec3((static_cast<float>(rand()) / static_cast<float>(RAND_MAX) - 0.5f) * 26,
                                              static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 50.0f,
                                              (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) - 0.5f) * 26);

    glCreateBuffers(1, &snow_positions_bo);
    glNamedBufferStorage(snow_positions_bo, sizeof(float) * 3 * current_snow_count, snow_initial_positions.data(), 0);

    glCreateVertexArrays(1, &snow_vao);
    glVertexArrayVertexBuffer(snow_vao, Geometry_Base::DEFAULT_POSITION_LOC, snow_positions_bo, 0, 3 * sizeof(float));

    glEnableVertexArrayAttrib(snow_vao, Geometry_Base::DEFAULT_POSITION_LOC);
    glVertexArrayAttribFormat(snow_vao, Geometry_Base::DEFAULT_POSITION_LOC, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(snow_vao, Geometry_Base::DEFAULT_POSITION_LOC, Geometry_Base::DEFAULT_POSITION_LOC);
}

// ----------------------------------------------------------------------------
// Update
// ----------------------------------------------------------------------------
void Application::update(float delta) {
    PV227Application::update(delta);

    // Updates the main camera.
    const glm::vec3 eye_position = camera.get_eye_position();
    camera_ubo.set_view(glm::lookAt(eye_position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    camera_ubo.update_opengl_data();

    // Computes the light position.
    glm::vec3 light_position =
        glm::vec3(15, 15, 15) * glm::vec3(cosf(gui_light_position / 6.0f) * sinf(gui_light_position), sinf(gui_light_position / 6.0f),
                                          cosf(gui_light_position / 6.0f) * cosf(gui_light_position));

    // Updates the light model visible in the scene.
    light_object.get_model_ubo().set_matrix(glm::translate(glm::mat4(1.0f), light_position) * glm::scale(glm::mat4(1.0f), glm::vec3(0.2f)));
    light_object.get_model_ubo().update_opengl_data();

    // Updates the OpenGL buffer storing the information about the light.
    phong_lights_ubo.clear();
    phong_lights_ubo.add(PhongLightData::CreatePointLight(light_position, glm::vec3(0.1f), glm::vec3(0.9f), glm::vec3(1.0f)));
    phong_lights_ubo.update_opengl_data();

    // Recreates particles if the number changed.
    if (current_snow_count != desired_snow_count) {
        current_snow_count = desired_snow_count;
        prepare_snow();
    }

    shadow_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f)) *
                    glm::scale(glm::mat4(1.0f), glm::vec3(0.5f)) *
                    light_camera_projection * light_camera_view;
}

void Application::update_broom_location() {
    // Updates the broom location.
    float winZ = 0;
    glReadPixels(broom_center.x, broom_center.y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

    glm::vec4 clip_space =
        glm::vec4((broom_center.x / width) * 2.0f - 1.0f, (broom_center.y / height) * 2.0f - 1.0f, winZ * 2.0 - 1.0, 1.0f);
    glm::vec4 world_space = glm::inverse(camera.get_view_matrix()) * glm::inverse(projection_matrix) * clip_space;
    glm::vec3 position = glm::vec3(world_space.x / world_space.w, world_space.y / world_space.w + 4.0f, world_space.z / world_space.w);

    broom_object.get_model_ubo().set_matrix(glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), glm::vec3(8.f)));
    broom_object.get_model_ubo().update_opengl_data();
}

// ----------------------------------------------------------------------------
// Render
// ----------------------------------------------------------------------------
void Application::render_broom_mask(){
    GLuint buff = gbuffer_fbo_2;
    if(secondBuffer){
        buff = gbuffer_fbo_1;
    }
    light_camera_data_ubo.bind_buffer_base(CameraUBO::DEFAULT_CAMERA_BINDING);
    glBindFramebuffer(GL_FRAMEBUFFER, buff);
    glViewport(0, 0, shadow_tex_size, shadow_tex_size);
    glDisable(GL_DEPTH_TEST);
    render_object(broom_object, shadow_map_program, false);
    //blur mask
    blur_texture.use();
    if(secondBuffer){
        glBindTextureUnit(0, snow_mask_1_tex);
    }
    else{
        glBindTextureUnit(0, snow_mask_2_tex);
    }
    glDrawArrays(GL_TRIANGLES, 0, 3);
    bool show_snow = false;
    if(show_snow){
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, width, height);
        glDisable(GL_DEPTH_TEST);
        // Clear the framebuffer, clear the color and depth
        //glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClearDepth(1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        render_shadow_mask_program.use();
        glBindVertexArray(empty_vao);
        glDisable(GL_DEPTH_TEST);
        if(secondBuffer){
            glBindTextureUnit(0, snow_mask_1_tex);
        }
        else{
            glBindTextureUnit(0, snow_mask_2_tex);
        }
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, width, height);
}

void Application::render_snow_mask(){
    GLuint buff = gbuffer_fbo_1;
    if(secondBuffer){
        buff = gbuffer_fbo_2;
    }
    
    // Sets the lights.
    glBindFramebuffer(GL_FRAMEBUFFER, buff);
    glViewport(0, 0, shadow_tex_size, shadow_tex_size);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);

    // Sets the necessary values
    render_snow_mask_program.use();
    glBindVertexArray(1);
    float increaseSnow = 0.00005 * log2(current_snow_count);
    render_snow_mask_program.uniform("increase", increaseSnow);
    render_snow_mask_program.uniform("clear_snow", clear_snow);
    //read from 1, write to 2
    if(secondBuffer){
        glBindTextureUnit(0, snow_mask_1_tex);
    }
    //read from 2, write to 1
    else{
        glBindTextureUnit(0, snow_mask_2_tex);
    }
    //
    glDrawArrays(GL_TRIANGLES, 0, 3);
    //light_camera_data_ubo.bind_buffer_base(CameraUBO::DEFAULT_CAMERA_BINDING);
    //render_object(broom_object, shadow_map_program, false, false, true);
    bool showSnowTex = false;
    if(showSnowTex){
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, width, height);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClearDepth(1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        render_shadow_mask_program.use();
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
    secondBuffer = !secondBuffer;
    clear_snow = false;
    glEnable(GL_DEPTH_TEST);
}

void Application::render() {
    
    // Starts measuring the elapsed time.
    glBeginQuery(GL_TIME_ELAPSED, render_time_query);
    // Binds the lights for everything.
    phong_lights_ubo.bind_buffer_base(PhongLightsUBO::DEFAULT_LIGHTS_BINDING);
    camera_ubo.bind_buffer_base(CameraUBO::DEFAULT_CAMERA_BINDING);
    //light_camera_data_ubo.bind_buffer_base(CameraUBO::DEFAULT_CAMERA_BINDING);

    // Binds and clears the main frame buffer.
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, width, height);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);


    if(wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    render_object(outer_terrain_object, default_lit_program, false, true, true);
    render_object(castel_base, default_lit_program, false, true, true);
    render_object(lake_object, default_lit_program, false, 10, true, true);
    render_object(castle_object, default_lit_program, false, true, true);
    render_object(snow_terrain_object, snow_tesselation_program, true /* true is necessary for tessellation */, true, true);
    update_broom_location();
    render_object(broom_object, default_lit_program, false, false, true);
    render_object(light_object, default_unlit_program, false, true);
    render_snow();
    render_snow_mask();
    render_broom_mask();
    render_height_mask();
    
    // Resets the VAO and the program.
    glBindVertexArray(0);
    glUseProgram(0);

    // Stops measuring the elapsed time.
    glEndQuery(GL_TIME_ELAPSED);

    // Waits for OpenGL - don't forget OpenGL is asynchronous.
    glFinish();

    // Evaluates the query.
    GLuint64 render_time;
    glGetQueryObjectui64v(render_time_query, GL_QUERY_RESULT, &render_time);
    fps_gpu = 1000.f / (static_cast<float>(render_time) * 1e-6f);
}

void Application::render_snow() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);
    snowflakes_program.use();
    snowflakes_program.uniform("elapsed_time_s", static_cast<float>(elapsed_time) * 1e-3f);
    glBindTextureUnit(0, particle_tex);
    glBindVertexArray(snow_vao);
    glDrawArrays(GL_POINTS, 0, current_snow_count);
    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
}

void Application::render_height_mask(){
    glBindFramebuffer(GL_FRAMEBUFFER, shadow_fbo);
    glViewport(0, 0, shadow_tex_size, shadow_tex_size);
    glClearDepth(1.0);
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    // TASK 1: Render the scene from the position of the light.
    //   Hint: See the code above for 'DISPLAY_VIEW_FROM_MAIN_CAMERA'.
    light_camera_data_ubo.bind_buffer_base(CameraUBO::DEFAULT_CAMERA_BINDING);
    // Sets the lights.
    phong_lights_ubo.bind_buffer_base(PhongLightsUBO::DEFAULT_LIGHTS_BINDING);
    // Renders the scene.
    render_object(outer_terrain_object, shadow_map_program, false, false, true);
    render_object(castel_base, shadow_map_program, false, false, true);
    render_object(lake_object, shadow_map_program, false, 10, false, true);
    render_object(castle_object, shadow_map_program, false, false, true);
    /*glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, width, height);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    blur_texture.use();
    glBindTextureUnit(0, shadow_texture);
    glDrawArrays(GL_TRIANGLES, 0, 3);*/
    //set to true if you want to see shadow texture
    //also check where this render_height_mask is called
    bool show_shadow_texture = false;
    if(show_shadow_texture){
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, width, height);

        // Clear the framebuffer, clear the color and depth
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClearDepth(1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        render_shadow_mask_program.use();
        glDisable(GL_DEPTH_TEST);

        // TASK 2: Display the shadow map (see also the TASK 2 in the display_texture.frag).
        //	 Hint: Use glBindTextureUnit as if it was any other texture.
        glBindTextureUnit(0, shadow_texture);
        // TASK 8: Use GL_NONE as GL_TEXTURE_COMPARE_MODE when using shadow texture
        glTextureParameteri(shadow_texture, GL_TEXTURE_COMPARE_MODE, GL_NONE);
        //	 Hint: It means calling glTextureParameteri(shadow_texture, GL_TEXTURE_COMPARE_MODE, GL_NONE).

        // Renders the full screen quad to evaluate every pixel.
        // Binds an empty VAO as we do not need any state.
        glBindVertexArray(empty_vao);
        // Calls a draw command with 3 vertices that are generated in vertex shader.
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, width, height);
    return;
}

void Application::render_object(const SceneObject& object, const ShaderProgram& program, bool render_as_patches, bool use_snow, bool is_lit, float uv_multiplier) const {
    program.use();

    // Handles the textures.
    program.uniform("has_texture", object.has_texture());
    program.uniform("uv_multiplier", uv_multiplier);
    if(is_lit){
        program.uniform("use_snow", false);
        program.uniform_matrix("shadow_matrix", shadow_matrix);
        glBindTextureUnit(2, shadow_texture);
        glTextureParameteri(shadow_texture, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
        glTextureParameteri(shadow_texture, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

    }
    if(use_snow){
        program.uniform("use_snow", true);
        if(secondBuffer){
            glBindTextureUnit(1, snow_mask_2_tex);
        }
        else{
            glBindTextureUnit(1, snow_mask_1_tex);
        }
    }
    if (object.has_texture()) {
        glBindTextureUnit(0, object.get_texture());
    } else {
        glBindTextureUnit(0, 0);
    }

    object.get_model_ubo().bind_buffer_base(ModelUBO::DEFAULT_MODEL_BINDING);
    object.get_material().bind_buffer_base(PhongMaterialUBO::DEFAULT_MATERIAL_BINDING);
    object.get_geometry().bind_vao();

    if (render_as_patches) {
        // We must use patches if we want to use tessellation.
        glPatchParameteri(GL_PATCH_VERTICES, 3);
        program.uniform("window_size", static_cast<float>(width), static_cast<float>(height));
        glBindTextureUnit(3, snow_normal_tex);
        glBindTextureUnit(4, snow_height_tex);
        if (object.get_geometry().draw_elements_count > 0) {
            glDrawElements(GL_PATCHES, object.get_geometry().draw_elements_count, GL_UNSIGNED_INT, nullptr);
        } else {
            glDrawArrays(GL_PATCHES, 0, object.get_geometry().draw_arrays_count);
        }
    } else {
        // Calls the standard rendering function if we do not require patches.
        object.get_geometry().draw();
    }
}

void Application::clear_accumulated_snow() {
    clear_snow = true;
}

// ----------------------------------------------------------------------------
// GUI
// ----------------------------------------------------------------------------
void Application::render_ui() {
    const float unit = ImGui::GetFontSize();

    ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_NoDecoration);
    ImGui::SetWindowSize(ImVec2(20 * unit, 17 * unit));
    ImGui::SetWindowPos(ImVec2(2 * unit, 2 * unit));

    ImGui::PushItemWidth(150.f);

    std::string fps_cpu_string = "FPS (CPU): ";
    ImGui::Text(fps_cpu_string.append(std::to_string(fps_cpu)).c_str());

    std::string fps_string = "FPS (GPU): ";
    ImGui::Text(fps_string.append(std::to_string(fps_gpu)).c_str());

    ImGui::SliderAngle("Light Position", &gui_light_position, 0);

    if(ImGui::Button("Clear Snow")) {
        clear_accumulated_snow();
    }

    ImGui::Checkbox("Show Snow", &show_snow);

    ImGui::Checkbox("Show Tessellated Snow", &show_tessellated_snow);

    ImGui::Checkbox("Wireframe", &wireframe);

    ImGui::Checkbox("Use PBR", &pbr);

    const char* particle_labels[10] = {"256", "512", "1024", "2048", "4096", "8192", "16384", "32768", "65536", "131072"};
    int exponent = static_cast<int>(log2(current_snow_count) - 8); // -8 because we start at 256 = 2^8
    if (ImGui::Combo("Particle Count", &exponent, particle_labels, IM_ARRAYSIZE(particle_labels))) {
        desired_snow_count = static_cast<int>(glm::pow(2, exponent + 8)); // +8 because we start at 256 = 2^8
    }


    ImGui::End();
}

// ----------------------------------------------------------------------------
// Input Events
// ----------------------------------------------------------------------------
void Application::on_resize(int width, int height) {
    PV227Application::on_resize(width, height);
    resize_fullscreen_textures();
}

void Application::on_mouse_move(double x, double y) {
    PV227Application::on_mouse_move(x, y);
    broom_center = glm::vec2(static_cast<float>(x), static_cast<float>(height - y));
}