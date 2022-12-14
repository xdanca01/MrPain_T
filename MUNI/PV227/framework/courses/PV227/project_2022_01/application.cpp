#include "application.hpp"
#include "utils.hpp"
#include <map>

Application::Application(int initial_width, int initial_height, std::vector<std::string> arguments)
    : PV227Application(initial_width, initial_height, arguments) {
    Application::compile_shaders();
    prepare_cameras();
    prepare_lights();
    prepare_textures();
    prepare_scene();
    prepare_framebuffers();

    reset_simulation_settings();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0);
}

Application::~Application() {}

// ----------------------------------------------------------------------------
// Shaderes
// ----------------------------------------------------------------------------
void Application::compile_shaders() {
    default_unlit_program = ShaderProgram(lecture_shaders_path / "object.vert", lecture_shaders_path / "unlit.frag");
    default_lit_program = ShaderProgram(lecture_shaders_path / "object.vert", lecture_shaders_path / "lit.frag");
    display_texture_program = ShaderProgram(lecture_shaders_path / "full_screen_quad.vert", lecture_shaders_path / "display_texture.frag");


    particle_textured_program = ShaderProgram();
    particle_textured_program.add_vertex_shader(lecture_shaders_path / "particle_textured.vert");
    particle_textured_program.add_fragment_shader(lecture_shaders_path / "particle_textured.frag");
    particle_textured_program.add_geometry_shader(lecture_shaders_path / "particle_textured.geom");
    particle_textured_program.link();

    update_particles_program = ShaderProgram();
    update_particles_program.add_compute_shader(lecture_shaders_path / "fireworks.comp");
    update_particles_program.link();

    std::cout << "Shaders are reloaded." << std::endl;
}

// ----------------------------------------------------------------------------
// Initialize Scene
// ----------------------------------------------------------------------------
void Application::prepare_cameras() {
    // Sets the default camera position.
    camera.set_eye_position(glm::radians(-45.f), glm::radians(20.f), 50.f);

    // Sets the projection matrix for the normal and mirrored cameras.
    glm::mat4 projection =
        glm::perspective(glm::radians(45.f), static_cast<float>(this->width) / static_cast<float>(this->height), 0.1f, 5000.0f);

    normal_camera_ubo.set_projection(projection);
    normal_camera_ubo.update_opengl_data();
}

void Application::prepare_lights(){
    phong_lights_bo = PhongLightsUBO(100, GL_SHADER_STORAGE_BUFFER); // Note that we use SSBO
    phong_lights_bo.add(PhongLightData::CreateDirectionalLight(glm::vec4(1,1,1,0), glm::vec3(0.1f), glm::vec3(0.9f), glm::vec3(1.0f)));
    phong_lights_bo.update_opengl_data();
}

void Application::prepare_textures() {
    particle_tex = TextureUtils::load_texture_2d(lecture_textures_path / "star.png");
    // Particles are really small, use mipmaps for them.
    TextureUtils::set_texture_2d_parameters(particle_tex, GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
}

void Application::prepare_framebuffers() {
    // Here you can create your framebuffers.
    // Creates the framebuffer for rendering into the G-buffer.
    glCreateFramebuffers(1, &gbuffer_fbo);
    glCreateFramebuffers(1, &gbuffer_fbo_mirror);
    // Specifies a list of color buffers to be drawn into.
    glNamedFramebufferDrawBuffers(gbuffer_fbo, 2, FBOUtils::draw_buffers_constants);
    glNamedFramebufferDrawBuffers(gbuffer_fbo_mirror, 1, FBOUtils::draw_buffers_constants);

    // We call separate resize method where we create and attache the textures.
    resize_fullscreen_textures();
}

void Application::resize_fullscreen_textures() {
    // Here you can (re)create you textures as this method is called whenever the window is resized.
    // Removes the previously allocated textures (if any).
    glDeleteTextures(1, &gbuffer_albedo_texture);
    glDeleteTextures(1, &gbuffer_mask_texture);
    glDeleteTextures(1, &gbuffer_albedo_texture_mirror);
    glDeleteTextures(1, &gbuffer_depth_texture);
    glDeleteTextures(1, &gbuffer_depth_texture_mirror);

    // Creates new textures for G-buffer and set their basic parameters.
    glCreateTextures(GL_TEXTURE_2D, 1, &gbuffer_albedo_texture);
    glCreateTextures(GL_TEXTURE_2D, 1, &gbuffer_mask_texture);
    glCreateTextures(GL_TEXTURE_2D, 1, &gbuffer_albedo_texture_mirror);
    glCreateTextures(GL_TEXTURE_2D, 1, &gbuffer_depth_texture);
    glCreateTextures(GL_TEXTURE_2D, 1, &gbuffer_depth_texture_mirror);

    // Initializes the immutable storage.
    glTextureStorage2D(gbuffer_albedo_texture, 1, GL_RGBA32F, width, height);
    glTextureStorage2D(gbuffer_mask_texture, 1, GL_RGBA32F, width, height);
    glTextureStorage2D(gbuffer_albedo_texture_mirror, 1, GL_RGBA32F, width, height);
    glTextureStorage2D(gbuffer_depth_texture, 1, GL_DEPTH_COMPONENT24, width, height);
    glTextureStorage2D(gbuffer_depth_texture_mirror, 1, GL_DEPTH_COMPONENT24, width, height);

    // Sets the texture parameters.
    TextureUtils::set_texture_2d_parameters(gbuffer_albedo_texture, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_NEAREST, GL_NEAREST);
    TextureUtils::set_texture_2d_parameters(gbuffer_mask_texture, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_NEAREST, GL_NEAREST);
    TextureUtils::set_texture_2d_parameters(gbuffer_albedo_texture_mirror, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_NEAREST, GL_NEAREST);
    TextureUtils::set_texture_2d_parameters(gbuffer_depth_texture, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_NEAREST, GL_NEAREST);
    TextureUtils::set_texture_2d_parameters(gbuffer_depth_texture_mirror, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_NEAREST, GL_NEAREST);

    // Binds the texture to the G-buffer.
    glNamedFramebufferTexture(gbuffer_fbo, GL_COLOR_ATTACHMENT0, gbuffer_albedo_texture, 0);
    glNamedFramebufferTexture(gbuffer_fbo, GL_COLOR_ATTACHMENT1, gbuffer_mask_texture, 0);
    glNamedFramebufferTexture(gbuffer_fbo, GL_DEPTH_ATTACHMENT, gbuffer_depth_texture, 0);
    FBOUtils::check_framebuffer_status(gbuffer_fbo, "G-buffer");

    glNamedFramebufferTexture(gbuffer_fbo_mirror, GL_COLOR_ATTACHMENT0, gbuffer_albedo_texture_mirror, 0);
    glNamedFramebufferTexture(gbuffer_fbo_mirror, GL_DEPTH_ATTACHMENT, gbuffer_depth_texture_mirror, 0);
    FBOUtils::check_framebuffer_status(gbuffer_fbo_mirror, "G-buffer");

}

void Application::prepare_scene() {
    // Allocates GPU buffers.
    glCreateBuffers(1, &particle_positions_bo);
    glCreateBuffers(1, &particle_velocities_bo);
    glCreateBuffers(1, &particle_colors_bo);
    glNamedBufferStorage(particle_positions_bo, sizeof(float) * 4 * max_particle_count, nullptr, GL_DYNAMIC_STORAGE_BIT);
    glNamedBufferStorage(particle_velocities_bo, sizeof(float) * 4 * max_particle_count, nullptr, GL_DYNAMIC_STORAGE_BIT);
    glNamedBufferStorage(particle_colors_bo, sizeof(float) * 4 * max_particle_count, nullptr, GL_DYNAMIC_STORAGE_BIT);

    // Initialize positions and velocities, and uploads them into OpenGL buffers.
    current_particle_count = desired_particle_count;
    reset_particles();

    // Creates VAOs to render the particles. The positions are at location '0' and the sizes at '1'.
    glCreateVertexArrays(1, &particle_vao);

    glVertexArrayVertexBuffer(particle_vao, 0, particle_positions_bo, 0, 4 * sizeof(float));
    glEnableVertexArrayAttrib(particle_vao, 0);
    glVertexArrayAttribFormat(particle_vao, 0, 4, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(particle_vao, 0, 0);

    glVertexArrayVertexBuffer(particle_vao, 1, particle_colors_bo, 0, 4 * sizeof(float));
    glEnableVertexArrayAttrib(particle_vao, 1);
    glVertexArrayAttribFormat(particle_vao, 1, 4, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(particle_vao, 1, 1);

    outer_terrain_object = SceneObject(
        cube, ModelUBO(translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.08f, 0.0f)) * scale(glm::mat4(1.0f), glm::vec3(13.0f, 0.1f, 13.0f))),
        green_material_ubo);

    castel_base = SceneObject(
        cube, ModelUBO(translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.05f, 0.0f)) * scale(glm::mat4(1.0f), glm::vec3(3.8f, 0.1f, 3.8f))),
        green_material_ubo);

    lake_object = SceneObject(
        cube, ModelUBO(translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.05f, 0.0f)) * scale(glm::mat4(1.0f), glm::vec3(12.f, 0.1f, 12.f))),
        blue_material_ubo);

    Geometry castle = Geometry::from_file(lecture_folder_path / "models/castle2.obj");
    castle_object = SceneObject(
        castle, ModelUBO(scale(translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.06f, 0.0f)) * glm::mat4(1.0f), glm::vec3(7.f, 7.f, 7.f))),
        gray_material_ubo);
}

// ----------------------------------------------------------------------------
// Update
// ----------------------------------------------------------------------------

void Application::update(float delta) {
    PV227Application::update(delta);

    // Updates the main camera.
    glm::vec3 eye_position = camera.get_eye_position();
    glm::mat4 view_matrix = glm::lookAt(eye_position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    normal_camera_ubo.set_view(view_matrix);
    normal_camera_ubo.update_opengl_data();

    if (current_particle_count != desired_particle_count) {
        current_particle_count = desired_particle_count;
        reset_particles();
    }

    update_particles_gpu(delta);
}

void Application::reset_particles() {
    // Reset timer.
    elapsed_time = 0;

    // Creates CPU buffers with the data of particles - positions and velocities.
    std::vector<glm::vec4> particle_positions (max_particle_count, glm::vec4(0.0f));
    std::vector<glm::vec4> particle_velocities (max_particle_count, glm::vec4(0.0f));
    std::vector<glm::vec4> particle_colors (max_particle_count, glm::vec4(1.0f));

    glNamedBufferSubData(particle_positions_bo, 0, sizeof(glm::vec4) * current_particle_count, particle_positions.data());
    glNamedBufferSubData(particle_velocities_bo, 0, sizeof(glm::vec4) * current_particle_count, particle_velocities.data());
    glNamedBufferSubData(particle_colors_bo, 0, sizeof(glm::vec4) * current_particle_count, particle_colors.data());
}

void Application::update_particles_gpu(float delta) {
    // Some time relevant variables for your disposal.
    float elapsed_time_in_milliseconds = elapsed_time;
    float elapsed_time_in_seconds = elapsed_time / 1000.f;
    float time_between_frames_in_milliseconds = delta;
    float time_between_frames_in_seconds = delta / 1000.f;

     // Bind all buffers to OpenGL.
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, particle_positions_bo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, particle_velocities_bo);
    phong_lights_bo.bind_buffer_base(2);
    //glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, phong_lights_bo);

    // Memory barrier, we need the data in the buffers to be ready
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    update_particles_program.use();

    //compute color
    float time = fmod(float(elapsed_time_in_seconds), lifespan);
    float explosion_start_time = init_delay + explosion_delay;
    float fade_start_time = explosion_start_time + fade_delay;
    float fade_end_time = fade_start_time + fade_time;
    float particle_size = (fade_end_time-time)/(fade_end_time-fade_start_time);
    particle_size = glm::min(glm::max(0.0f, particle_size), 1.0f);
    //particle_size = 1.0;
    //choose color
    if(time < init_delay && time <= time_between_frames_in_seconds && time != elapsed_time_in_seconds){

        particle_color = glm::vec4(glm::rgbColor(glm::vec3(static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 360.0f, 1.0f, 1.0f)), particle_size);
    }
    //change particle_size
    if(time >= fade_start_time)
    {
        particle_color.w = particle_size;
    }

    
    // Update uniform values
    update_particles_program.uniform("current_particle_count", current_particle_count);
    update_particles_program.uniform("time_delta", time_between_frames_in_seconds);
    update_particles_program.uniform("gravity", gravity);
    update_particles_program.uniform("elapsed_time", elapsed_time_in_seconds);
    update_particles_program.uniform("fireworks_count", fireworks_count);
    update_particles_program.uniform("lifespan", lifespan);
    update_particles_program.uniform("min_initial_velocity", min_initial_velocity);
    update_particles_program.uniform("max_initial_velocity", max_initial_velocity);
    update_particles_program.uniform("init_delay", init_delay);
    update_particles_program.uniform("explosion_delay", explosion_delay);
    update_particles_program.uniform("explosion_thrust_time", explosion_thrust_time);
    update_particles_program.uniform("explosion_force", explosion_force);
    update_particles_program.uniform("explosion_light_delay", explosion_light_delay);
    update_particles_program.uniform("fade_delay", fade_delay);
    update_particles_program.uniform("fade_time", fade_time);
    update_particles_program.uniform("rocket_spread", rocket_spread);
    update_particles_program.uniform("blink_delay", blink_delay);
    update_particles_program.uniform("blink_frequency", blink_frequency);
    update_particles_program.uniform("blink_ratio", blink_ratio);
    update_particles_program.uniform("particle_color", particle_color);
    particle_textured_program.uniform("particle_color", particle_color);

    // Dispatches the compute shader
    const int local_size_x = 256;
    glDispatchCompute(current_particle_count / local_size_x, 1, 1);

    // Waits for OpenGL - don't forget OpenGL is asynchronous.
    glFinish();
}

// ----------------------------------------------------------------------------
// Render
// ----------------------------------------------------------------------------
void Application::show_g_buffer_textures() {
    // Binds the main window framebuffer.
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, width, height);

    // Clears the framebuffer color.
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    // We do not need depth and depth test.
    glDisable(GL_DEPTH_TEST);

    glBindTextureUnit(0, gbuffer_albedo_texture);
    glBindTextureUnit(1, gbuffer_mask_texture);
    glBindTextureUnit(2, gbuffer_albedo_texture_mirror);
    glBindTextureUnit(3, gbuffer_depth_texture);
    //glTextureParameteri(gbuffer_depth_texture, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    //glTextureParameteri(gbuffer_depth_texture, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

    display_texture_program.uniform("mirror_factor", mirror_factor);
    display_texture_program.use();

    // Renders the full screen quad to evaluate every pixel.
    // Calls a draw command with 3 vertices that are generated in vertex shader.
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Application::render_into_g_buffer() {
    
    glBindFramebuffer(GL_FRAMEBUFFER, gbuffer_fbo);
    glViewport(0, 0, width, height);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    phong_lights_bo.bind_buffer_base(PhongLightsUBO::DEFAULT_LIGHTS_BINDING);

    // Renders all objects in the scene, use deferred shading program
    render_scene(default_lit_program);

    // Binds G-Buffer.
    glBindFramebuffer(GL_FRAMEBUFFER, gbuffer_fbo_mirror);
    glViewport(0, 0, width, height);

    // Clears the G-buffer, clear all textures to (0,0,0,0) and depth to 1.0.
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    phong_lights_bo.bind_buffer_base(PhongLightsUBO::DEFAULT_LIGHTS_BINDING);
    
    render_scene_mirrored(default_lit_program);

}

void Application::render() {
    // Begins measuring the GPU time.
    glBeginQuery(GL_TIME_ELAPSED, render_time_query);

    normal_camera_ubo.bind_buffer_base(CameraUBO::DEFAULT_CAMERA_BINDING);
    //evaluate_lighting_forward();
    render_into_g_buffer();
    render_particles(gbuffer_fbo_mirror, true);
    render_particles(gbuffer_fbo, false);
    show_g_buffer_textures();

    // Resets the VAO and the program.
    glBindVertexArray(0);
    glUseProgram(0);

    // Evaluates the query.
    glEndQuery(GL_TIME_ELAPSED);
    glFinish();
    GLuint64 render_time;
    glGetQueryObjectui64v(render_time_query, GL_QUERY_RESULT, &render_time);
    fps_gpu = 1000.f / (static_cast<float>(render_time) * 1e-6f);
}

void Application::evaluate_lighting_forward() {
    // Binds the correct buffer.
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Uploads the information about lights.
    phong_lights_bo.bind_buffer_base(PhongLightsUBO::DEFAULT_LIGHTS_BINDING);

    // Renders all objects in the scene using forward shading program.
    render_scene(default_lit_program);
}

void Application::render_scene_mirrored(const ShaderProgram& program){
    program.uniform("is_mirrored", true);
    render_object(castle_object, program, false);
    render_object(castel_base, program, false);
    program.uniform("is_mirrored", false);
}

void Application::render_scene(const ShaderProgram& program) {
    render_object(castel_base, program, false);
    render_object(castle_object, program, false);
    render_object(outer_terrain_object, program, false);
    render_object(lake_object, program, true);
}

void Application::render_object(const SceneObject& object, const ShaderProgram& program, bool is_lake) {
    program.use();
    program.uniform("is_lake", is_lake);
    // Calls the standard rendering functions.
    object.get_model_ubo().bind_buffer_base(ModelUBO::DEFAULT_MODEL_BINDING);
    object.get_material().bind_buffer_base(PhongMaterialUBO::DEFAULT_MATERIAL_BINDING);
    object.get_geometry().bind_vao();
    object.get_geometry().draw();
}

void Application::render_particles(GLuint buffer, bool is_mirrored) {
    glBindFramebuffer(GL_FRAMEBUFFER, buffer);
    // Sets up very simple blending.
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glDepthMask(GL_FALSE);
    glBlendFunc(GL_ONE, GL_ONE);

    // Sets the necessary values
    particle_textured_program.use();
    particle_textured_program.uniform("particle_size_vs", particle_size);
    particle_textured_program.uniform("is_mirrored", is_mirrored);
    glBindTextureUnit(0, particle_tex);
    if(is_mirrored == false){
        glBindTextureUnit(1, gbuffer_depth_texture);
    }
    else{
        glBindTextureUnit(1, gbuffer_depth_texture_mirror);
    }

    // Binds the proper VAO (we use the VAO with the data we just wrote).
    glBindVertexArray(particle_vao);
    // Memory barrier, we need the data in the buffers to be ready.
    glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
    // Draws the particles as points.
    glDrawArrays(GL_POINTS, 0, current_particle_count);

    // Disables blending.
    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
}

// ----------------------------------------------------------------------------
// GUI
// ----------------------------------------------------------------------------
void Application::render_ui() {
    const float unit = ImGui::GetFontSize();

    ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_NoDecoration);
    ImGui::SetWindowSize(ImVec2(20 * unit, 40 * unit));
    ImGui::SetWindowPos(ImVec2(2 * unit, 2 * unit));

    std::string fps_cpu_string = "FPS (CPU): ";
    ImGui::Text(fps_cpu_string.append(std::to_string(fps_cpu)).c_str());

    std::string fps_string = "FPS (GPU): ";
    ImGui::Text(fps_string.append(std::to_string(fps_gpu)).c_str());

    ImGui::PushItemWidth(150.f);

    if (ImGui::SliderInt("Fireworks Count", &fireworks_count, 1, 50)) {
        reset_particles();
    }
    if (ImGui::Button("Reset Particles")) {
        reset_particles();
    }

    const char* particle_labels[10] = {"256", "512", "1024", "2048", "4096", "8192", "16384", "32768", "65536", "131072"};
    int exponent = static_cast<int>(log2(current_particle_count) - 8); // -8 because we start at 256 = 2^8
    if (ImGui::Combo("Particle Count", &exponent, particle_labels, IM_ARRAYSIZE(particle_labels))) {
        desired_particle_count = static_cast<int>(glm::pow(2, exponent + 8)); // +8 because we start at 256 = 2^8
    }

    ImGui::Checkbox("Waves", &waves);
    ImGui::SliderFloat("Mirror Factor", &mirror_factor, 0.0f, 1.0f, "%.2f");

    if (ImGui::Button("Reset Simulation Settings")) {
        reset_simulation_settings();
    }

    ImGui::SliderFloat("Particle Size", &particle_size, 0.05f, 0.5f, "%.2f");
    if (ImGui::SliderFloat("Life Span", &lifespan, 1.f, 10.0f, "%.1f")) {
        init_delay = glm::min(init_delay, lifespan);
        explosion_delay = glm::min(explosion_delay, lifespan);
        explosion_thrust_time = glm::min(explosion_thrust_time, lifespan);
        fade_delay = glm::min(fade_delay, lifespan);
        fade_time = glm::min(fade_time, lifespan);
        blink_delay = glm::min(blink_delay, lifespan);
        reset_particles();
    }
    if (ImGui::SliderFloat("Rocket Spread", &rocket_spread, 0, 20, "%.2f")) {
        reset_particles();
    }
    if (ImGui::SliderFloat("Min Init Velocity", &min_initial_velocity, 5, max_initial_velocity, "%.2f")) {
        max_initial_velocity = glm::max(max_initial_velocity, min_initial_velocity);
        reset_particles();
    }
    if (ImGui::SliderFloat("Max Init Velocity", &max_initial_velocity, min_initial_velocity, 20, "%.2f")) {

        min_initial_velocity = glm::min(min_initial_velocity, max_initial_velocity);
        reset_particles();
    }
    if (ImGui::SliderFloat("Init Delay", &init_delay, 0.05f, lifespan, "%.2f")) {
        reset_particles();
    }
    if (ImGui::SliderFloat("Explosion Delay", &explosion_delay, 0.00f, lifespan, "%.2f")) {
        reset_particles();
    }
    if (ImGui::SliderFloat("Expl. Thrust Time", &explosion_thrust_time, 0.00f, lifespan, "%.2f")) {
        reset_particles();
    }
    if (ImGui::SliderFloat("Explosion Force", &explosion_force, 1.0f, 20.f, "%.2f")) {
        reset_particles();
    }
    if (ImGui::SliderFloat("Expl. Light Delay", &explosion_light_delay, 0.00f, 1.f, "%.2f")) {
        reset_particles();
    }
    if (ImGui::SliderFloat("Fade Delay", &fade_delay, 0.00f, lifespan, "%.2f")) {
        reset_particles();
    }
    if (ImGui::SliderFloat("Fade Time", &fade_time, 0.00f, lifespan, "%.2f")) {
        reset_particles();
    }
    if (ImGui::SliderFloat("Blink Delay", &blink_delay, 0.00f, lifespan, "%.2f")) {
    }
    if (ImGui::SliderFloat("Blink Frequency", &blink_frequency, 1.0, 50.f, "%.2f")) {
    }
    if (ImGui::SliderFloat("Blink Ratio", &blink_ratio, 0.0f, 1.0f, "%.2f")) {
    }

    ImGui::End();
}

void Application::reset_simulation_settings() {
    gravity = -9.81f;
    particle_size = 0.2f;
    lifespan = 5;
    init_delay = 0.1f;
    min_initial_velocity = 14;
    max_initial_velocity = 16;
    explosion_delay = 0.9f;
    explosion_thrust_time = 0.33f;
    explosion_force = 10.0f;
    explosion_light_delay = 0.1f;
    fade_delay = 0.35f;
    fade_time = 3.0f;
    rocket_spread = 3;
    blink_delay = 0.3f;
    blink_frequency = 20.f;
    blink_ratio = 0.35f;
}

// ----------------------------------------------------------------------------
// Input Events
// ----------------------------------------------------------------------------
void Application::on_resize(int width, int height) {
    PV227Application::on_resize(width, height);
    resize_fullscreen_textures();
}