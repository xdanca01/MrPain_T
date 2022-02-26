#include "texture_3_3.hpp"

// ----------------------------------------------------------------------------
// Constructors
// ----------------------------------------------------------------------------

Texture_3_3::Texture_3_3(std::filesystem::path path, bool cpu_only)
    : Texture(path, true /* we utilize the cpu flag to avoid calling OpenGL 4.5 commands*/) {
    // Creates the GPU representation of the data using OpenGL 3.3.
    if (width > 0 && height > 0 && !cpu_only) {
        this->cpu_only =
            cpu_only; // We update CPU flag to the correct value since TRUE is always used in the base constructor.

        glGenTextures(1, &opengl_object);
        glBindTexture(GL_TEXTURE_2D, opengl_object);
        glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, type, texture_data.data());
    }
}

Texture_3_3::Texture_3_3(int width, int height, GLint internal_format, GLenum format, bool cpu_only)
    : Texture(width, height, internal_format, format,
              true /* we utilize the cpu flag to avoid calling OpenGL 4.5 commands*/) {

    // Creates the GPU representation of the data using OpenGL 3.3.
    if (width > 0 && height > 0 && !cpu_only) {
        this->cpu_only =
            cpu_only; // We update CPU flag to the correct value since TRUE is always used in the base constructor.

        glGenTextures(1, &opengl_object);
        glBindTexture(GL_TEXTURE_2D, opengl_object);
        glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, type, texture_data.data());
    }
}

// ----------------------------------------------------------------------------
// Methods
// ----------------------------------------------------------------------------

void Texture_3_3::set_texture_2d_parameters(GLint wrap_s, GLint wrap_t, GLint min_filter, GLint mag_filter) const {
    if (opengl_object != 0) {
        glBindTexture(GL_TEXTURE_2D, opengl_object);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
        glBindTexture(GL_TEXTURE_2D, 0);
    } else {
        // fail silently
    }
}

void Texture_3_3::update_opengl_data() {
    if (opengl_object != 0) {
        glBindTexture(GL_TEXTURE_2D, opengl_object);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, format, type, texture_data.data());
        glBindTexture(GL_TEXTURE_2D, 0);
    } else {
        // fail silently
    }
}

void Texture_3_3::bind(GLuint unit) const {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, opengl_object);
}
