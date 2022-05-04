// ################################################################################
// Common Framework for Computer Graphics Courses at FI MUNI.
//
// Copyright (c) 2021-2022 Visitlab (https://visitlab.fi.muni.cz)
// All rights reserved.
// ################################################################################

#include "application.hpp"
#include "utils.hpp"
#pragma warning(disable : 4996)

Application::Application(int initial_width, int initial_height, std::vector<std::string> arguments)
    : PA213Application(initial_width, initial_height, arguments),
      lconfig{0.25f, 1.0f, 1.0f, glm::pi<float>() / 3.0f, glm::pi<float>() / 4.0f, 0.9f, 3U}, lrule_edit_props(), laxiom(), compose_literations(false) {
    Application::compile_shaders();
    prepare_cameras();
    prepare_materials();
    prepare_textures();
    prepare_lights();
    set_default_lrules();
    prepare_scene();

    // Creates the empty VAO.
    glCreateVertexArrays(1, &empty_vao);

    // Creates the query object.
    glCreateQueries(GL_TIME_ELAPSED, 1, &render_time_query);
}

Application::~Application() {}

// ----------------------------------------------------------------------------
// Shaderes
// ----------------------------------------------------------------------------
void Application::compile_shaders() {
    ray_tracing_program = ShaderProgram(lecture_shaders_path / "full_screen_quad.vert", lecture_shaders_path / "ray_tracing.frag");
    std::cout << "Shaders are reloaded." << std::endl;
}

// ----------------------------------------------------------------------------
// Initialize Scene
// ----------------------------------------------------------------------------
void Application::prepare_cameras() {
    // Sets the default camera position.
    camera.set_eye_position(glm::radians(0.f), glm::radians(20.f), 15.f);
    // Computes the projection matrix.
    camera_ubo.set_projection(
        glm::perspective(glm::radians(45.f), static_cast<float>(this->width) / static_cast<float>(this->height), 0.1f, 1000.0f));
    camera_ubo.update_opengl_data();
}

void Application::prepare_materials() {}

void Application::prepare_textures() {
    meadow_skybox_tex = TextureUtils::load_texture_cube(
        1024, 1024, lecture_textures_path / "meadow_skybox/meadow_skybox_px.jpg",
        lecture_textures_path / "meadow_skybox/meadow_skybox_nx.jpg", lecture_textures_path / "meadow_skybox/meadow_skybox_py.jpg",
        lecture_textures_path / "meadow_skybox/meadow_skybox_ny.jpg", lecture_textures_path / "meadow_skybox/meadow_skybox_pz.jpg",
        lecture_textures_path / "meadow_skybox/meadow_skybox_nz.jpg");
    TextureUtils::set_texture_3d_parameters(meadow_skybox_tex, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE,
                                            GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    wood_tex = TextureUtils::load_texture_2d(lecture_textures_path / "wood.png");
    TextureUtils::set_texture_2d_parameters(wood_tex, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);

    leaf_tex = TextureUtils::load_texture_2d(lecture_textures_path / "leaf.png");
    TextureUtils::set_texture_2d_parameters(leaf_tex, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);
}

void Application::prepare_lights() {
    // The rest is set in the update scene method.
    phong_lights_ubo.set_global_ambient(glm::vec3(0.0f));
}

void Application::set_default_lrules() {
    std::strncpy(laxiom, "L", sizeof(laxiom) - 1U);
    std::strncpy(lrule_edit_props[0].first, "L", sizeof(lrule_edit_props[0].first) - 1U);
    std::strncpy(lrule_edit_props[0].second, "B[&L][&--L]^-L", sizeof(lrule_edit_props[0].second) - 1U);
    std::strncpy(lrule_edit_props[1].first, "B", sizeof(lrule_edit_props[1].first) - 1U);
    std::strncpy(lrule_edit_props[1].second, "B*B", sizeof(lrule_edit_props[1].second) - 1U);

    lrule_edit_props[2].first[0] = 0;
    lrule_edit_props[2].second[0] = 0;
    lrule_edit_props[3].first[0] = 0;
    lrule_edit_props[3].second[0] = 0;
}

void Application::prepare_scene() {
    branches.clear();
    leaves.clear();

    LTurtle::Rules lrules;
    for (auto const& rep : lrule_edit_props)
        if (std::strlen(rep.first) == 1U && std::strlen(rep.second) > 0)
            lrules.insert({rep.first[0], rep.second});

    for (unsigned int i = compose_literations ? 0U : lconfig.max_depth, n = lconfig.max_depth; i <= n; ++i)
    {
        lconfig.max_depth = i;
        LTurtle turtle(lconfig, lrules, branches, leaves);
        turtle.run(laxiom);
    }

    if (!branches.empty())
        tree_ubo = TreeUBO(branches, GL_DYNAMIC_STORAGE_BIT);
    if (!leaves.empty())
        leaves_ubo = LeavesUBO(leaves, GL_DYNAMIC_STORAGE_BIT);
}

// ----------------------------------------------------------------------------
// Update
// ----------------------------------------------------------------------------
void Application::update(float delta) {
    PA213Application::update(delta);

    // Updates the main camera.
    const glm::vec3 eye_position = camera.get_eye_position();
    camera_ubo.set_view(lookAt(eye_position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    camera_ubo.update_opengl_data();

    // Computes the light position.
    const glm::vec3 light_position = glm::vec3(15, 15, 15) * glm::vec3(cosf(gui_light_position / 6.0f) * sinf(gui_light_position), 1,
                                                                       cosf(gui_light_position / 6.0f) * cosf(gui_light_position));

    // Updates the OpenGL buffer storing the information about the light.
    phong_lights_ubo.clear();
    phong_lights_ubo.add(PhongLightData::CreateDirectionalLight(light_position, glm::vec3(0.1f), glm::vec3(0.9f), glm::vec3(1.0f)));
    phong_lights_ubo.update_opengl_data();
}

// ----------------------------------------------------------------------------
// Render
// ----------------------------------------------------------------------------
void Application::render() {
    // Starts measuring the elapsed time.
    glBeginQuery(GL_TIME_ELAPSED, render_time_query);

    // Binds the main window framebuffer.
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Sets the clear values and clear the framebuffer.
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    ray_tracing();

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

void Application::ray_tracing() {
    // Binds the main window framebuffer.
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, width, height);

    // Clears the framebuffer color.
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    // We do not need depth and depth test.
    glDisable(GL_DEPTH_TEST);

    // Uses the proper program.
    ray_tracing_program.use();
    ray_tracing_program.uniform("num_branches", (int)branches.size());
    ray_tracing_program.uniform("num_leaves", (int)leaves.size());
    ray_tracing_program.uniform("resolution", glm::vec2(width, height));
    ray_tracing_program.uniform("use_tone_mapping", use_tone_mapping);
    ray_tracing_program.uniform("exposure", exposure);
    ray_tracing_program.uniform("gamma", gamma);

    // Binds the textures.
    glBindTextureUnit(0, meadow_skybox_tex);
    glBindTextureUnit(1, wood_tex);
    glBindTextureUnit(2, leaf_tex);

    // Binds the data with the camera and the lights.
    camera_ubo.bind_buffer_base(CameraUBO::DEFAULT_CAMERA_BINDING);
    phong_lights_ubo.bind_buffer_base(PhongLightsUBO::DEFAULT_LIGHTS_BINDING);
    tree_ubo.bind_buffer_base(4);
    leaves_ubo.bind_buffer_base(5);

    // Renders the full screen quad to evaluate every pixel.
    // Binds an empty VAO as we do not need any state.
    glBindVertexArray(empty_vao);
    // Calls a draw command with 3 vertices that are generated in vertex shader.
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

// ----------------------------------------------------------------------------
// GUI
// ----------------------------------------------------------------------------
void Application::render_ui() {
    const float unit = ImGui::GetFontSize();

    ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_NoDecoration);
    ImGui::SetWindowSize(ImVec2(20 * unit, 44 * unit));
    ImGui::SetWindowPos(ImVec2(0 * unit, 0 * unit));
    ImGui::PushItemWidth(150);

    std::string fps_cpu_string = "FPS (CPU): ";
    ImGui::Text(fps_cpu_string.append(std::to_string(fps_cpu)).c_str());

    std::string fps_string = "FPS (GPU): ";
    ImGui::Text(fps_string.append(std::to_string(fps_gpu)).c_str());

    ImGui::Separator();
    ImGui::Text("GFX-config:");
    ImGui::SliderAngle("Light Position", &gui_light_position, 0);

    ImGui::Checkbox("Use Tone Mapping", &use_tone_mapping);
    ImGui::SliderFloat("Exposure", &exposure, 0.0f, 10.0f, "%.1f");
    ImGui::SliderFloat("Gamma", &gamma, 0.5f, 2.8f, "%.1f");

    ImGui::Separator();
    ImGui::Text("L-config:");
    bool rebuild = false;
    rebuild |= ImGui::SliderFloat("Branch radius", &lconfig.radius, 0.1f, 0.5f, "%.2f");
    rebuild |= ImGui::SliderFloat("Branch length", &lconfig.distance, 0.25f, 2.0f, "%.2f");
    rebuild |= ImGui::SliderFloat("Leaf size", &lconfig.leaf_size, 0.1f, 2.0f, "%.2f");
    rebuild |= ImGui::SliderAngle("World Y rot angle", &lconfig.angle_world_y, 0.0f, 180.0f);
    rebuild |= ImGui::SliderAngle("Turtle Z rot angle", &lconfig.angle_turtle_left, 0.0f, 180.0f);
    rebuild |= ImGui::SliderFloat("Brush decay", &lconfig.brush_decay_coef, 0.75f, 1.25f, "%.2f");
    rebuild |= ImGui::SliderInt("Max recursion depth", (int*)&lconfig.max_depth, 0, 5);
    rebuild |= ImGui::Checkbox("Compose iterations", &compose_literations);
    int i = 0;
    for (auto& rule : lrule_edit_props) {
        ImGui::PushItemWidth(2 * ImGui::GetFontSize());
        rebuild |= ImGui::InputText(("->##" + std::to_string(i)).c_str(), &rule.first[0], sizeof(rule.first) - 1U);
        ImGui::SameLine();
        ImGui::PushItemWidth(10 * ImGui::GetFontSize());
        rebuild |= ImGui::InputText((std::to_string(i + 1) + ". rule##" + std::to_string(i + 100)).c_str(), &rule.second[0],
                                    sizeof(rule.second) - 1U);
        ++i;
    }
    ImGui::PushItemWidth(7 * ImGui::GetFontSize());
    rebuild |= ImGui::InputText("Axiom", &laxiom[0], sizeof(laxiom) - 1U);
    ImGui::SameLine();
    if (ImGui::Button("Default rules")) {
        set_default_lrules();
        rebuild = true;
    }
    ImGui::Text("B - Make branch");
    ImGui::Text("L/l - Make leaf (2 orientations)");
    ImGui::Text("M - Move turtle forward (no draw)");
    ImGui::Text("+/- - Rotate turtle in +/-Y world axis");
    ImGui::Text("&/^ - Rotate turtle in its +/-left axis");
    ImGui::Text("* - Decay turtle's brush");
    ImGui::Text("[ - Push turtle's state");
    ImGui::Text("] - Pop turtle's state");
    if (rebuild)
        prepare_scene();

    ImGui::End();
}
