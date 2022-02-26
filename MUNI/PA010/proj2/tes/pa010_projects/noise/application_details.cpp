#include "application.hpp"
#include <vector>

Application::SpectralSystesisInfo::SpectralSystesisInfo()
    : noise_frequency(1.0f)
    , sampling_frequency(64U)
    , start_amplitude(1.0f)
    , amplitude_multiplier(0.5f)
    , frequency_multiplier(2.0f)
    , arg0(10.0f)
{}

std::map<std::string, Application::noise_function_type> const Application::noise_functions_map {
    { "1. Plain noise", [](glm::vec3 const& pos, SpectralSystesisInfo const& info) { return Application::perlin_noise_improved(info.noise_frequency * pos); } },
    { "2. Fractal sum", Application::fractalsum },
    { "3. Turbulence", Application::turbulence },
    { "4. Marble", Application::marble },
};

Application::Application(int initial_width, int initial_height, std::vector<std::string> arguments)
        : IApplication(initial_width, initial_height, arguments)
        , systesis_info()
        , is_systesis_info_changed(true)
        , current_noise_function_name(noise_functions_map.begin()->first)
        , shader_box(shaders_path / "box.vert", shaders_path / "box.frag")
        , shader_axes(shaders_path / "axes.vert", shaders_path / "axes.frag")
        , geometry_box(
                GL_TRIANGLES,
                []() -> std::vector<float> {
                    static std::vector<float> vertices = {
                        // Front face
	                       -1.0f, 1.0f, 1.0f,
	                       -1.0f,-1.0f, 1.0f,
	                        1.0f,-1.0f, 1.0f,
	                        1.0f, 1.0f, 1.0f,
                        // Right face
	                        1.0f, 1.0f, 1.0f,
	                        1.0f,-1.0f, 1.0f,
	                        1.0f,-1.0f,-1.0f,
	                        1.0f, 1.0f,-1.0f,
                        // Back face
	                        1.0f, 1.0f,-1.0f,
	                        1.0f,-1.0f,-1.0f,
	                       -1.0f,-1.0f,-1.0f,
	                       -1.0f, 1.0f,-1.0f,
                        // Left face
	                       -1.0f, 1.0f,-1.0f,
	                       -1.0f,-1.0f,-1.0f,
	                       -1.0f,-1.0f, 1.0f,
	                       -1.0f, 1.0f, 1.0f,
                        // Top face
	                       -1.0f, 1.0f,-1.0f,
	                       -1.0f, 1.0f, 1.0f,
	                        1.0f, 1.0f, 1.0f,
	                        1.0f, 1.0f,-1.0f,
                        // Bottom face
	                       -1.0f,-1.0f, 1.0f,
	                       -1.0f,-1.0f,-1.0f,
	                        1.0f,-1.0f,-1.0f,
	                        1.0f,-1.0f, 1.0f,
                        };
                    return vertices;
                    }(),
                []() -> std::vector<uint32_t> {
                    static std::vector<uint32_t> indices = {
                        0, 1, 2,		0, 2, 3,		// Front face
                        4, 5, 6,		4, 6, 7,		// Right face
                        8, 9, 10,		8, 10, 11,		// Back face
                        12, 13, 14,		12, 14, 15,		// Left face
                        16, 17, 18,		16, 18, 19,		// Top face
                        20, 21, 22,		20, 22, 23,		// Bottom face
                        };
                    return indices;
                    }(),
                {},
                {},
                []() -> std::vector<float> {
                    static std::vector<float> uvs = {
                        // Front face
	                       0.0f / 6.0f, 1.0f,
	                       0.0f / 6.0f, 0.0f,
	                       1.0f / 6.0f, 0.0f,
	                       1.0f / 6.0f, 1.0f,
                        // Right face
	                       1.0f / 6.0f, 1.0f,
	                       1.0f / 6.0f, 0.0f,
	                       2.0f / 6.0f, 0.0f,
	                       2.0f / 6.0f, 1.0f,
                        // Back face
	                       2.0f / 6.0f, 1.0f,
	                       2.0f / 6.0f, 0.0f,
	                       3.0f / 6.0f, 0.0f,
	                       3.0f / 6.0f, 1.0f,
                        // Left face
	                       3.0f / 6.0f, 1.0f,
	                       3.0f / 6.0f, 0.0f,
	                       4.0f / 6.0f, 0.0f,
	                       4.0f / 6.0f, 1.0f,
                        // Top face
	                       4.0f / 6.0f, 1.0f,
	                       4.0f / 6.0f, 0.0f,
	                       5.0f / 6.0f, 0.0f,
	                       5.0f / 6.0f, 1.0f,
                        // Bottom face
	                       5.0f / 6.0f, 1.0f,
	                       5.0f / 6.0f, 0.0f,
	                       6.0f / 6.0f, 0.0f,
	                       6.0f / 6.0f, 1.0f,
                        };
                    return uvs;
                    }(),
                {},
                {},
                Geometry_Base::DEFAULT_POSITION_LOC,
                -1,
                -1,
                Geometry_Base::DEFAULT_TEX_COORD_LOC,
                -1,
                -1
                )
        , geometry_axes(
                GL_LINES,
                []() -> std::vector<float> {
                    static std::vector<float> vertices = {
                         0.0f, 0.0f, 0.0f,  2.0f, 0.0f, 0.0f,   // x-axis
                         0.0f, 0.0f, 0.0f,  0.0f, 2.0f, 0.0f,   // y-axis
                         0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 2.0f,   // z-axis
                        };
                    return vertices;
                    }(),
                []() -> std::vector<float> {
                    static std::vector<float> colours = {
                         1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,   // x-axis
                         0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,   // y-axis
                         0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f    // z-axis
                        };
                    return colours;
                    }()
                )
        , texture_box(0U) {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
}

GLuint Application::make_noise_texture(SpectralSystesisInfo const& info, noise_function_type const& content_generator) {
    // NOTE: The texture is 2x larger than the info.sampling_frequency along both axes (for each cube side).
    std::vector<unsigned char> data;
    data.resize((6UL * 4UL * info.sampling_frequency * info.sampling_frequency) * 4U, 127U);
    auto const coord_fn = [&info](int c) { return (c - (float)info.sampling_frequency) / (float)info.sampling_frequency; };
    std::vector<std::function<glm::vec3(int,int)> > position_fns = {
        [&coord_fn](int i, int j) -> glm::vec3 { return glm::vec3(coord_fn(i), coord_fn(j), 1.0f); },
        [&coord_fn](int i, int j) -> glm::vec3 { return glm::vec3(1.0f, coord_fn(j), -coord_fn(i)); },
        [&coord_fn](int i, int j) -> glm::vec3 { return glm::vec3(-coord_fn(i), coord_fn(j), -1.0f); },
        [&coord_fn](int i, int j) -> glm::vec3 { return glm::vec3(-1.0f, coord_fn(j), coord_fn(i)); },
        [&coord_fn](int i, int j) -> glm::vec3 { return glm::vec3(coord_fn(i), 1.0f, -coord_fn(j)); },
        [&coord_fn](int i, int j) -> glm::vec3 { return glm::vec3(coord_fn(i), -1.0f, coord_fn(j)); },
    };
    auto const clamp = [](float x, float a, float b) -> float { return x < a ? a : (x > b) ? b : x; };
    for (int s = 0; s != 6; ++s)
        for (int i = 0; i != 2U * info.sampling_frequency; ++i)
            for (int j = 0; j != 2U * info.sampling_frequency; ++j)
            {
                glm::vec3 pos = position_fns.at(s)(i,j);
                float const noise = content_generator(pos, info);
                float const intensity = clamp((noise + 1.0f) / 2.0f, 0.0f, 1.0f);
                unsigned char colour = (unsigned char)(255.0f * intensity);
                unsigned char* rgb = &data.at((j * (6UL * 2UL * info.sampling_frequency) + i + s * (2UL * info.sampling_frequency)) * 4UL);
                rgb[0] = rgb[1] = rgb[2] = colour; rgb[3] = colour;
            }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 6UL * 2UL * info.sampling_frequency, 2UL * info.sampling_frequency, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    return texture;
}

GLuint Application::rebuild_noise_texture() {
    if (texture_box != 0 && texture_box != -1)
        glDeleteTextures(1, &texture_box);
    texture_box = make_noise_texture(systesis_info, noise_functions_map.at(current_noise_function_name));
    return texture_box;
}

void Application::update(float delta) {
    if (is_systesis_info_changed)
    {
        is_systesis_info_changed = false;
        rebuild_noise_texture();
    }
}

void Application::render() {
    if (!shader_box.is_valid()|| !shader_axes.is_valid())
        return;

    activate_render_buffer();

    glm::mat4 projection_matrix = glm::perspective(glm::radians(45.0f), static_cast<float>(width) / static_cast<float>(height), 0.1f, 100.0f);
    glm::mat4 view_matrix = lookAt(camera.get_eye_position(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 model_matrix = translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

    shader_axes.use();
    glUniformMatrix4fv(shader_axes.get_uniform_location("projection"), 1, GL_FALSE, value_ptr(projection_matrix));
    glUniformMatrix4fv(shader_axes.get_uniform_location("view"), 1, GL_FALSE, value_ptr(view_matrix));
    glUniformMatrix4fv(shader_axes.get_uniform_location("model"), 1, GL_FALSE, value_ptr(model_matrix));
    geometry_axes.draw();

    shader_box.use();
    glUniformMatrix4fv(shader_box.get_uniform_location("projection"), 1, GL_FALSE, value_ptr(projection_matrix));
    glUniformMatrix4fv(shader_box.get_uniform_location("view"), 1, GL_FALSE, value_ptr(view_matrix));
    glUniformMatrix4fv(shader_box.get_uniform_location("model"), 1, GL_FALSE, value_ptr(model_matrix));
    glUniform4f(shader_box.get_uniform_location("input_color"), 0.5f, 0.5f, 0.5f, 1.0f);
    glBindTexture(GL_TEXTURE_2D, texture_box);
    geometry_box.draw();

}

void Application::render_ui() {
    ImGui::Begin("Settings", nullptr);

    static bool size_set = false;
    if (!size_set) {
        const float unit = ImGui::GetFontSize();
        ImGui::SetWindowSize(ImVec2(32 * unit, 14 * unit));
        ImGui::SetWindowPos(ImVec2(0,0));
        size_set = true;
    }

    bool b00 = false;
    if (ImGui::BeginCombo("Noise generator", current_noise_function_name.c_str())) {
        for (auto const& name_and_level : noise_functions_map) {
            bool const is_selected = name_and_level.first == current_noise_function_name;
            if (ImGui::Selectable(name_and_level.first.c_str(), is_selected)) {
                if (current_noise_function_name != name_and_level.first)
                    b00 = true;
                current_noise_function_name = name_and_level.first;
            }
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    bool b01 = ImGui::SliderInt("Sampling frequency", (int*)&systesis_info.sampling_frequency, 1U, 128U);
    bool b02 = ImGui::SliderFloat("Noise frequency", &systesis_info.noise_frequency, 1.0f, 100.0f);
    bool b05 = ImGui::SliderFloat("Frequency multiplier", &systesis_info.frequency_multiplier, 1.75f, 5.0f);
    bool b03 = ImGui::InputFloat("Start amplitude", &systesis_info.start_amplitude, 0.1f);
    bool b04 = ImGui::InputFloat("Amplitude multiplier", &systesis_info.amplitude_multiplier, 0.1f);
    bool b06 = ImGui::InputFloat("Arg 0", &systesis_info.arg0, 0.1f);

    if (b00 || b01 || b02 || b03 || b04 || b05 || b06)
        is_systesis_info_changed = true;

    ImGui::End();
}
