#include "application.hpp"

#include <memory>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using std::make_shared;




GLuint load_texture_2d(const std::filesystem::path filename) {
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(filename.generic_string().data(), &width, &height, &channels, 4);

    GLuint texture;
    glCreateTextures(GL_TEXTURE_2D, 1, &texture);

    glTextureStorage2D(texture, std::log2(width), GL_RGBA16, width, height);

    glTextureSubImage2D(texture,
                        0,                         //
                        0, 0,                      //
                        width, height,             //
                        GL_RGBA, GL_UNSIGNED_BYTE, //
                        data);

    stbi_image_free(data);

    glGenerateTextureMipmap(texture);

    glTextureParameteri(texture, GL_TEXTURE_WRAP_R, GL_MIRRORED_REPEAT);
    glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    

    return texture;
}

Application::Application(int initial_width, int initial_height, std::vector<std::string> arguments)
    : PV112Application(initial_width, initial_height, arguments) {
    this->width = initial_width;
    this->height = initial_height;

    images_path = lecture_folder_path / "images";
    objects_path = lecture_folder_path / "objects";

    // --------------------------------------------------------------------------
    //  Load/Create Objects
    // --------------------------------------------------------------------------
    // You can use from_file function to load a Geometry from .obj file
    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "mustang.obj")));
    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "parking lot.obj")));
    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "Barrier.obj")));
    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "cone.obj")));

    car = geometries[0];
    parking = geometries[1];
    barrier = geometries[2];
    cone = geometries[3];

    concrete_texture = load_texture_2d(images_path / "concrete.jpg");
    rock_texture = load_texture_2d(images_path / "rocks.jpg");
    textures.push_back(load_texture_2d(images_path / "concrete_wall.jpg"));
    textures.push_back(load_texture_2d(images_path / "asphalt.jpg"));
    textures.push_back(load_texture_2d(images_path / "barricade.jpg"));
    textures.push_back(load_texture_2d(images_path / "mustang.png"));
    textures.push_back(load_texture_2d(images_path / "cone.jpg"));
    normals.push_back(load_texture_2d(images_path / "concrete_wall_normals.jpg"));
    normals.push_back(load_texture_2d(images_path / "asphalt_normals.jpg"));

    // --------------------------------------------------------------------------
    // Initialize UBO Data
    // --------------------------------------------------------------------------
    camera_ubo.position = glm::vec4(camera.get_eye_position(), 1.0f);
    camera_ubo.projection = glm::perspective(glm::radians(45.0f), float(width) / float(height), 0.01f, 1000.0f);
    camera_ubo.view = glm::lookAt(camera.get_eye_position(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));


    //lights

    //Light 1
    LightUBO light;
    light.position = glm::vec4(0.0f, 0.8f, 0.0f, 1.0f);
    light.ambient_color = glm::vec4(0.98f, 0.79f, 0.03f, 1.0f);
    light.diffuse_color = glm::vec4(0.98f, 0.79f, 0.03f, 1.0f);
    light.specular_color = glm::vec4(0.98f, 0.79f, 0.03f, 1.0f);
    lights.push_back(light);

    //Light 2
    light.position = glm::vec4(0.0f, 0.8f, 2.0f, 1.0f);
    light.ambient_color = glm::vec4(0.98f, 0.79f, 0.03f, 1.0f);
    light.diffuse_color = glm::vec4(0.98f, 0.79f, 0.03f, 1.0f);
    light.specular_color = glm::vec4(0.98f, 0.79f, 0.03f, 1.0f);
    lights.push_back(light);

    //Light 3
    light.position = glm::vec4(0.0f, 0.8f, -2.0f, 1.0f);
    light.ambient_color = glm::vec4(0.98f, 0.79f, 0.03f, 1.0f);
    light.diffuse_color = glm::vec4(0.98f, 0.79f, 0.03f, 1.0f);
    light.specular_color = glm::vec4(0.98f, 0.79f, 0.03f, 1.0f);
    lights.push_back(light);
    //Light 4
    light.position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    light.ambient_color = glm::vec4(0.98f, 0.79f, 0.03f, 1.0f);
    light.diffuse_color = glm::vec4(0.98f, 0.79f, 0.03f, 1.0f);
    light.specular_color = glm::vec4(0.98f, 0.79f, 0.03f, 1.0f);
    lights.push_back(light);

    //Light 5
    light.position = glm::vec4(0.0f, 0.0f, 2.0f, 1.0f);
    light.ambient_color = glm::vec4(0.98f, 0.79f, 0.03f, 1.0f);
    light.diffuse_color = glm::vec4(0.98f, 0.79f, 0.03f, 1.0f);
    light.specular_color = glm::vec4(0.98f, 0.79f, 0.03f, 1.0f);
    lights.push_back(light);

    //Light 6
    light.position = glm::vec4(0.0f, 0.0f, -2.0f, 1.0f);
    light.ambient_color = glm::vec4(0.98f, 0.79f, 0.03f, 1.0f);
    light.diffuse_color = glm::vec4(0.98f, 0.79f, 0.03f, 1.0f);
    light.specular_color = glm::vec4(0.98f, 0.79f, 0.03f, 1.0f);
    lights.push_back(light);

    //Light 7
    light.position = glm::vec4(0.0f, -0.8f, 0.0f, 1.0f);
    light.ambient_color = glm::vec4(0.98f, 0.79f, 0.03f, 1.0f);
    light.diffuse_color = glm::vec4(0.98f, 0.79f, 0.03f, 1.0f);
    light.specular_color = glm::vec4(0.98f, 0.79f, 0.03f, 1.0f);
    lights.push_back(light);

    //Light 8
    light.position = glm::vec4(0.0f, -0.8f, 2.0f, 1.0f);
    light.ambient_color = glm::vec4(0.98f, 0.79f, 0.03f, 1.0f);
    light.diffuse_color = glm::vec4(0.98f, 0.79f, 0.03f, 1.0f);
    light.specular_color = glm::vec4(0.98f, 0.79f, 0.03f, 1.0f);
    lights.push_back(light);

    //Light 9
    light.position = glm::vec4(0.0f, -0.8f, -2.0f, 1.0f);
    light.ambient_color = glm::vec4(0.98f, 0.79f, 0.03f, 1.0f);
    light.diffuse_color = glm::vec4(0.98f, 0.79f, 0.03f, 1.0f);
    light.specular_color = glm::vec4(0.98f, 0.79f, 0.03f, 1.0f);
    lights.push_back(light);

    //Light 10 - MOON
    light.position = glm::vec4(10.0f, 10.0f, 0.0f, 1.0f);
    light.ambient_color = glm::vec4(0.4f);
    light.diffuse_color = 5*glm::vec4(1.0f);
    light.specular_color = glm::vec4(1.0f);
    lights.push_back(light);

    //Light 11
    light.position = glm::vec4(0.0f, 0.9f, 4.0f, 1.0f);
    light.ambient_color = glm::vec4(0.98f, 0.79f, 0.03f, 1.0f);
    light.diffuse_color = glm::vec4(0.98f, 0.79f, 0.03f, 1.0f);
    light.specular_color = glm::vec4(0.98f, 0.79f, 0.03f, 1.0f);
    lights.push_back(light);


    auto T = glm::radians(-90.0f);
    //car
    objects_ubos.push_back({.model_matrix = glm::translate(
                                            glm::rotate(glm::mat4(1.0f), T, glm::vec3(0.0f,1.0f,0.0f)), //rotace podle y 
                                            glm::vec3(3.0f, 0.61f, 0.0f)), //posun,
                            .ambient_color = glm::vec4(0.0f),
                            .diffuse_color = glm::vec4(0.9f),
                            .specular_color = glm::vec4(0.01f,0.01f,0.01f,1.0f)});
    //parking
    objects_ubos.push_back({.model_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(10.0f)),
                            .ambient_color = glm::vec4(0.0f),
                            .diffuse_color = glm::vec4(1.0f),
                            .specular_color = glm::vec4(0.2f,0.2f,0.2f,1.0f)});
    //barrier
    objects_ubos.push_back({.model_matrix = glm::translate(glm::rotate(glm::rotate(glm::mat4(1.0f), T, glm::vec3(1.0f,0.0f,0.0f)),
                                                        T, glm::vec3(0.0f,0.0f,1.0f)), //second rotate
                                                        glm::vec3(4.35f, 1.2f, -1.25f)), //translate
                            .ambient_color = glm::vec4((0.0f)),
                            .diffuse_color = glm::vec4(0.41f, 0.39f, 0.37f, 1.0f),
                            .specular_color = glm::vec4(0.01f,0.01f,0.01f,1.0f)});

    //car2
    objects_ubos.push_back({.model_matrix = glm::translate(
                                            glm::rotate(glm::mat4(1.0f), T, glm::vec3(0.0f,1.0f,0.0f)), //rotace podle y 
                                            glm::vec3(3.0f, 0.61f, 0.55f)), //posun,
                            .ambient_color = glm::vec4(0.0f),
                            .diffuse_color = glm::vec4(0.9f),
                            .specular_color = glm::vec4(0.01f,0.01f,0.01f,1.0f)});

    //car3
    objects_ubos.push_back({.model_matrix = glm::translate(
                                            glm::rotate(glm::mat4(1.0f), T, glm::vec3(0.0f,1.0f,0.0f)), //rotace podle y 
                                            glm::vec3(3.0f, 0.61f, -0.55f)), //posun,
                            .ambient_color = glm::vec4(0.0f),
                            .diffuse_color = glm::vec4(0.9f),
                            .specular_color = glm::vec4(0.01f,0.01f,0.01f,1.0f)});

    //car4
    objects_ubos.push_back({.model_matrix = glm::translate(
                                            glm::rotate(glm::mat4(1.0f), T, glm::vec3(0.0f,1.0f,0.0f)), //rotace podle y 
                                            glm::vec3(3.0f, 0.61f, 1.1f)), //posun,
                            .ambient_color = glm::vec4(0.0f),
                            .diffuse_color = glm::vec4(0.9f),
                            .specular_color = glm::vec4(0.01f,0.01f,0.01f,1.0f)});

    //car5
    objects_ubos.push_back({.model_matrix = glm::translate(
                                            glm::rotate(glm::mat4(1.0f), T, glm::vec3(0.0f,1.0f,0.0f)), //rotace podle y 
                                            glm::vec3(3.0f, 0.61f, -1.1f)), //posun,
                            .ambient_color = glm::vec4(0.0f),
                            .diffuse_color = glm::vec4(0.9f),
                            .specular_color = glm::vec4(0.01f,0.01f,0.01f,1.0f)});
    
    T = glm::radians(-15.0f);

    //cone
    objects_ubos.push_back({.model_matrix = glm::rotate(
                                                glm::translate(
                                                    glm::scale(glm::mat4(1.0f), glm::vec3(0.2f)),
                                                glm::vec3(-1.0f, 0.8f, 20.3f)),
                                            T, glm::vec3(0.0f, 0.0f, 1.0f)),
                            .ambient_color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
                            .diffuse_color = glm::vec4(1.0f),
                            .specular_color = glm::vec4(0.01f,0.01f,0.01f,1.0f)});

    //cone2
    objects_ubos.push_back({.model_matrix = glm::rotate(
                                                glm::translate(
                                                    glm::scale(glm::mat4(1.0f), glm::vec3(0.2f)),
                                                glm::vec3(-1.0f, 0.8f, 22.7f)),
                                            T, glm::vec3(0.0f, 0.0f, 1.0f)),
                            .ambient_color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
                            .diffuse_color = glm::vec4(1.0f),
                            .specular_color = glm::vec4(0.01f,0.01f,0.01f,1.0f)});

    // --------------------------------------------------------------------------
    // Load Squares
    // --------------------------------------------------------------------------
    square_ubos.push_back({.model_matrix = glm::mat4(1.0f),
                            .ambient_color = glm::vec4(0.0f),
                            .diffuse_color = glm::vec4(1.0f),
                            .specular_color = glm::vec4(0.3f,0.3f,0.3f,1.0f)});
    // --------------------------------------------------------------------------
    // Create Buffers
    // --------------------------------------------------------------------------
    glCreateBuffers(1, &camera_buffer);
    glNamedBufferStorage(camera_buffer, sizeof(CameraUBO), &camera_ubo, GL_DYNAMIC_STORAGE_BIT);

    //lights
    glCreateBuffers(1, &lights_buffer);
    glNamedBufferStorage(lights_buffer, sizeof(LightUBO) * lights.size(), lights.data(), GL_DYNAMIC_STORAGE_BIT);

    //objects
    glCreateBuffers(1, &objects_buffer);
    glNamedBufferStorage(objects_buffer, sizeof(ObjectUBO) * objects_ubos.size(), objects_ubos.data(), GL_DYNAMIC_STORAGE_BIT);

    glCreateBuffers(1, &square_buffer_id);
    glNamedBufferStorage(square_buffer_id, sizeof(ObjectUBO) * square_ubos.size(), square_ubos.data(), GL_DYNAMIC_STORAGE_BIT);

    compile_shaders();
}

Application::~Application() {
    delete_shaders();

    glDeleteBuffers(1, &camera_buffer);
    glDeleteBuffers(1, &lights_buffer);
    glDeleteBuffers(1, &objects_buffer);
    glDeleteBuffers(1, &square_buffer_id);
}

// ----------------------------------------------------------------------------
// Methods
// ----------------------------------------------------------------------------

double timeOfNextAnimationStep = 2;
double animationDelayTime = 0.05; //in seconds
double step = 0;
unsigned steps = 200;

void Application::animate() {
    double currentTime = glfwGetTime();
    if(currentTime < timeOfNextAnimationStep) return;
    timeOfNextAnimationStep = currentTime + animationDelayTime;

    float car_angle = glm::radians(1.0f);
    glm::vec3 car_rotate {0.0f, 1.0f, 0.0f};
    glm::vec3 car_trans {0.015f,0,0};
    unsigned modulo_step = step % steps;
    if(modulo_step < 100){
        car_trans = {-0.015f,0,0,};
        if(modulo_step >= 50){
            car_angle = glm::radians(-1.0f);
        }
    }
    auto model_matrix = objects_ubos.at(0).model_matrix;
    model_matrix = glm::translate(model_matrix, car_trans);
    //rotate only between steps 50 and 150
    if(modulo_step >= 50 && modulo_step < 150){
        model_matrix = glm::rotate(model_matrix, car_angle, car_rotate);
    }
    objects_ubos.at(0).model_matrix = model_matrix;
    glNamedBufferSubData(objects_buffer, 0, sizeof(ObjectUBO) * objects_ubos.size(), objects_ubos.data());
    ++step;
}

void Application::delete_shaders() {
}

void Application::compile_shaders() {
    delete_shaders();
    main_program = create_program(lecture_shaders_path / "main.vert", lecture_shaders_path / "main.frag");
    square_program = create_program(lecture_shaders_path / "square.vert", lecture_shaders_path / "square.frag");
    parking_program = create_program(lecture_shaders_path / "parking.vert", lecture_shaders_path / "parking.frag");
}

void Application::update(float delta) {}

void Application::render() {
    animate();
    // --------------------------------------------------------------------------
    // Update UBOs
    // --------------------------------------------------------------------------
    // Camera
    camera_ubo.position = glm::vec4(camera.get_eye_position(), 1.0f);
    camera_ubo.projection = glm::perspective(glm::radians(45.0f), static_cast<float>(width) / static_cast<float>(height), 0.01f, 1000.0f);
    camera_ubo.view = glm::lookAt(camera.get_eye_position(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glNamedBufferSubData(camera_buffer, 0, sizeof(CameraUBO), &camera_ubo);

    // --------------------------------------------------------------------------
    // Draw scene
    // --------------------------------------------------------------------------
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);

    // Clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Configure fixed function pipeline
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glDisable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glUseProgram(parking_program);

    glBindBufferBase(GL_UNIFORM_BUFFER, 0, camera_buffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, lights_buffer);
    glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 1 * 256, sizeof(ObjectUBO));
    
    glUniform1i(glGetUniformLocation(parking_program, "has_texture"), true);
    glUniform1i(glGetUniformLocation(parking_program, "fog_enable"), fog);
    glUniform4f(glGetUniformLocation(parking_program, "F.color"), 1.0f, 1.0f, 1.0f, 0.03f);
    glUniform2f(glGetUniformLocation(parking_program, "F.position"), camera.get_eye_position().z, camera.get_eye_position().z+15.0f);
    glBindTextureUnit(3, textures[0]);
    glBindTextureUnit(4, textures[0]);
    parking->draw();

    glUseProgram(main_program);
    

    glBindBufferBase(GL_UNIFORM_BUFFER, 0, camera_buffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, lights_buffer);
    glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 2 * 256, sizeof(ObjectUBO));
    
    glUniform4f(glGetUniformLocation(main_program, "F.color"), 1.0f, 1.0f, 1.0f, 0.03f);
    glUniform2f(glGetUniformLocation(main_program, "F.position"), camera.get_eye_position().z, camera.get_eye_position().z+15.0f);
    glUniform1i(glGetUniformLocation(main_program, "has_texture"), true);
    glUniform1i(glGetUniformLocation(main_program, "bright_textures"), this->bright);
    glUniform1i(glGetUniformLocation(main_program, "fog_enable"), fog);
    glUniform1i(glGetUniformLocation(main_program, "phong_enable"), this->phong);
    glUniform1i(glGetUniformLocation(main_program, "toon_enable"), this->toon);
    glBindTextureUnit(3, textures[2]);
    barrier->draw();

    glBindBufferBase(GL_UNIFORM_BUFFER, 0, camera_buffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, lights_buffer);
    glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 0 * 256, sizeof(ObjectUBO));
    
    glBindTextureUnit(3, textures[3]);
    car->draw();

    glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 3 * 256, sizeof(ObjectUBO));
    car->draw();

    glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 4 * 256, sizeof(ObjectUBO));    
    car->draw();

    glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 5 * 256, sizeof(ObjectUBO));    
    car->draw();

    glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 6 * 256, sizeof(ObjectUBO));    
    car->draw();

    glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 7 * 256, sizeof(ObjectUBO)); 
    glBindTextureUnit(3, textures[4]);   
    cone->draw();

    glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 8 * 256, sizeof(ObjectUBO)); 
    cone->draw();

    //draw square on ground
    glUseProgram(square_program);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, lights_buffer);
    glBindBufferRange(GL_UNIFORM_BUFFER, 2, square_buffer_id, 0 * 256, sizeof(ObjectUBO));
    glUniform1i(glGetUniformLocation(square_program, "has_texture"), true);
    glUniform1i(glGetUniformLocation(square_program, "fog_enable"), fog);
    glUniform4f(glGetUniformLocation(square_program, "F.color"), 1.0f, 1.0f, 1.0f, 0.03f);
    glUniform2f(glGetUniformLocation(square_program, "F.position"), camera.get_eye_position().z, camera.get_eye_position().z+15.0f);
    glBindTextureUnit(3, textures[1]);
    glBindTextureUnit(4, normals[1]);
    
    glDrawArrays(GL_TRIANGLES, 0, 12);
    

}

void Application::render_ui() { const float unit = ImGui::GetFontSize(); }

// ----------------------------------------------------------------------------
// Input Events
// ----------------------------------------------------------------------------

void Application::on_resize(int width, int height) {
    // Calls the default implementation to set the class variables.
    PV112Application::on_resize(width, height);
}

void Application::on_mouse_move(double x, double y) { camera.on_mouse_move(x, y); }
void Application::on_mouse_button(int button, int action, int mods) { camera.on_mouse_button(button, action, mods); }
void Application::on_key_pressed(int key, int scancode, int action, int mods) {
    // Calls default implementation that invokes compile_shaders when 'R key is hit.
    PV112Application::on_key_pressed(key, scancode, action, mods);
    if(action == GLFW_PRESS){
        switch (key) {
            case GLFW_KEY_B:
                this->bright = !bright;
                break;
            case GLFW_KEY_F:
                this->fog = !fog;
                break;
            case GLFW_KEY_P:
                this->phong = !phong;
                break;
            case GLFW_KEY_T:
                this->toon = !toon;
                break;
        }
    }
}