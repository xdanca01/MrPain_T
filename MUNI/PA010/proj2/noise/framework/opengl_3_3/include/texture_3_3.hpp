#pragma once

#include "texture.hpp"

/**
 * The implementation of the @link Texture class using OpenGL 3.3. The main use for this class is to support broader number of computers including MacOS
 * course.
 *
 * @author	<a href="mailto:jan.byska@gmail.com">Jan Byška</a>
 */
class Texture_3_3 : public Texture {
    // ----------------------------------------------------------------------------
    // Constructors
    // ----------------------------------------------------------------------------
  public:
    /**
     * Constructs a new @link Texture from a specified file.
     *
     * @param 	path		The path for the file with image.
     * @param 	cpu_only	The flag determining if the texture will be CPU only (should be @p true for tests as they do
     * 						not have OpenGL context).
     */
    Texture_3_3(std::filesystem::path path, bool cpu_only = false);

    /**
     * Constructs a new @link Texture with specified size and default formats.
     *
     * @param 	width   	The texture width.
     * @param 	height  	The texture height.
     * @param 	cpu_only	The flag determining if the texture will be CPU only (should be @p true for tests as they do
     * 						not have OpenGL context).
     */
    Texture_3_3(int width, int height, bool cpu_only = false) : Texture_3_3(width, height, GL_RGBA8, GL_RGBA, cpu_only) {}

    /**
     * Constructs a new custom @link Texture and initialized the OpenGL counterpart.
     *
     * @param 	width		   	The texture width.
     * @param 	height		   	The texture height.
     * @param 	internal_format	The internal texture format that specifies the number of color components in the texture.
     * @param 	format		   	The texture format that specifies the format of the pixel data.
     * @param 	cpu_only	   	The flag determining if the texture will be CPU only (should be @p true for tests as they
     * 							do not have OpenGL context).
     */
    Texture_3_3(int width, int height, GLint internal_format, GLenum format, bool cpu_only);

    // ----------------------------------------------------------------------------
    // Methods
    // ----------------------------------------------------------------------------

    /**
     * @copydoc Texture::set_texture_2d_parameters
     * 			<p>
     * 			This implementation is using OpenGL 3.3 compatible functions.
     */
    void set_texture_2d_parameters(GLint wrap_s, GLint wrap_t, GLint min_filter, GLint mag_filter) const override;

    /**
     * @copydoc Texture::update_opengl_data
     * 			<p>
     * 			This implementation is using OpenGL 3.3 compatible functions.
     */
    void update_opengl_data() override;

    /**
     * @copydoc Texture::bind
     * 			<p>
     * 			This implementation is using OpenGL 3.3 compatible functions.
     */
    virtual void bind(GLuint unit) const override;
};