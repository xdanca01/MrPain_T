#pragma once

#include "glad.h"
#include <vector>

/**
 * This is a base class for all geometry classes that wraps buffers and vertex array objects for geometries.
 * <p>
 * The main reason for separating the functionality withing this class is the ability to easily reuse it for both OpenGL 3.3 and
 * OpenGL 4.5+ implementations of the geometries for different courses.
 * <p>
 * The creation of the OpenGL objects is left to the
 *
 * To draw the geometry, bind this geometry's vao (using glBindVertexArray({@link Geometry::vao}) or
 * {@link Geometry::bind_vao), and call drawing commands using @link Geometry_Base::draw or @link Geometry_Base::draw_instanced.
 *
 * Example:
 * <code>
 *  Cube my_cube(...);
 *  ...
 *  my_cube.bind_vao();
 *  my_cube.draw();
 * </code>
 *
 * @author	<a href="mailto:cejka.honza@gmail.com ">Jan Čejka</a>
 * @author	<a href="mailto:jan.byska@gmail.com">Jan Byška</a>
 * @author	<a href="mailto:matus.talcik@gmail.com">Matúš Talčík</a>
 */
class Geometry_Base {

    // ----------------------------------------------------------------------------
    // Static Variables
    // ----------------------------------------------------------------------------
public:
    // The following constants define the default indices of input vertex shader variables.
    // Make sure these numbers corresponds to layout (binding=N) in shaders (or use glBindAttribLocation).
    /** The default location of position vertex attribute. */
    static const int DEFAULT_POSITION_LOC = 0;
    /** The default location of normal vertex attribute. */
    static const int DEFAULT_NORMAL_LOC = 1;
    /** The default location of texture coordinate vertex attribute. */
    static const int DEFAULT_TEX_COORD_LOC = 2;
    /** The default location of tangent vertex attribute. */
    static const int DEFAULT_TANGENT_LOC = 3;
    /** The default location of bitangent vertex attribute. */
    static const int DEFAULT_BITANGENT_LOC = 4;
    /** The default location of color vertex attribute. */
    static const int DEFAULT_COLOR_LOC = 5; // TODO make number 2

    // ----------------------------------------------------------------------------
    // Variables
    // ----------------------------------------------------------------------------
public:
    /** Type of the primitives to be drawn using this geometry, e.g. GL_TRIANGLES. */
    GLenum mode = GL_POINTS;

    /** The sizes of the buffer storing the geometry (positions, normals, texture coordinates, etc.). */
    GLsizei vertex_buffer_size = 0;

    /** The vertex buffer stride, i.e., the spacing of the elements in the array.*/
    GLsizei vertex_buffer_stride = 0;

    /** The buffer storing the interleaved data for the vertices. */
    std::vector<float> interleaved_vertices{};

    /** The offset for positions in the interleaved buffer. */
    int position_offset = 0;

    /** The offset for colors in the interleaved buffer. */
    int color_offset = -1;

    /** The offset for normals in the interleaved buffer. */
    int normal_offset = -1;

    /** The offset for texture coordinates in the interleaved buffer. */
    int tex_coord_offset = -1;

    /** The number of elements (floats) per vertex. */
    int elements_per_vertex = 0;

    /** The number of vertices to be drawn using glDrawArrays. */
    GLsizei draw_arrays_count = 0;

    /** The number of vertices to be drawn using glDrawElements. */
    GLsizei draw_elements_count = 0;

    /** The number of patch vertices. This variable is used only when mode is set to GL_PATCHES, otherwise it is ignored. */
    GLsizei patch_vertices = 0;

    /** The Vertex Array Object that describes how the vertex attributes are stored. */
    GLuint vao = 0;

    /** The OpenGL buffer with the the geometry data (positions, normals, texture coordinates, etc.). */
    GLuint vertex_buffer = 0;

    /** The OpenGL buffer with the indices describing the geometry. */
    GLuint index_buffer = 0;

    /** The location of position vertex attribute. */
    GLint position_loc = DEFAULT_POSITION_LOC;

    /** The location of normal vertex attribute. */
    GLint normal_loc = DEFAULT_NORMAL_LOC;

    /** The location of texture coordinate vertex attribute. */
    GLint tex_coord_loc = DEFAULT_TEX_COORD_LOC;

    /** The location of tangent vertex attribute. */
    GLint tangent_loc = DEFAULT_TANGENT_LOC;

    /** The location of bitangent vertex attribute. */
    GLint bitangent_loc = DEFAULT_BITANGENT_LOC;

    /** The location of bitangent vertex attribute. */
    GLint color_loc = DEFAULT_COLOR_LOC;

    // ----------------------------------------------------------------------------
    // Constructors
    // ----------------------------------------------------------------------------
public:
    Geometry_Base() {
    }

    /**
     * Creates a new @link Geometry_Base object. Note that this constructor does not initialize the OpenGL objects, this
     * is left to the child class.
     *
     * @param 	mode			   	The mode that will be used for rendering the geometry.
     * @param 	elements_per_vertex	The number of elements (floats) per vertex.
     * @param 	vertices_count	   	The number of vertices in the geometry.
     * @param 	indices_count	   	The number of indices defining the geometry.
     * @param 	position_loc	   	The location of position vertex attribute for the VAO (use -1 if not necessary).
     * @param 	normal_loc		   	The location of normal vertex attribute for the VAO (use -1 if not necessary).
     * @param 	tex_coord_loc	   	The location of texture coordinates vertex attribute for the VAO (use -1 if not	necessary).
     * @param 	tangent_loc		   	The location of tangent vertex attribute for the VAO (use -1 if not necessary).
     * @param 	bitangent_loc	   	The location of bitangent vertex attribute for the VAO (use -1 if not necessary).
     * @param 	color_loc		   	The location of color vertex attribute for the VAO (use -1 if not necessary).
     */
    Geometry_Base(GLenum mode, int elements_per_vertex, int vertices_count, int indices_count,
                  GLint position_loc = DEFAULT_POSITION_LOC, GLint normal_loc = DEFAULT_NORMAL_LOC,
                  GLint tex_coord_loc = DEFAULT_TEX_COORD_LOC, GLint tangent_loc = DEFAULT_TANGENT_LOC,
                  GLint bitangent_loc = DEFAULT_BITANGENT_LOC, GLint color_loc = DEFAULT_COLOR_LOC)
        : mode(mode), elements_per_vertex(elements_per_vertex), position_loc(position_loc), normal_loc(normal_loc),
          tex_coord_loc(tex_coord_loc), tangent_loc(tangent_loc), bitangent_loc(bitangent_loc), color_loc(color_loc) {

        vertex_buffer_stride = elements_per_vertex * sizeof(float);
        vertex_buffer_size = vertices_count * vertex_buffer_stride;

        init_patches_count();
        draw_elements_count = indices_count;
        draw_arrays_count = vertices_count;
    };

    /**
     * Creates a new @link Geometry_Base object. Note that this constructor does not initialize the OpenGL objects, this
     * is left to the child class.
     * 
     * @param 	mode		 	The mode that will be used for rendering the geometry.
     * @param 	positions	 	The the list vertex positions (if empty position_loc will be set to -1).
     * @param 	normals		 	The list of normals for each vertex (if empty @link #normal_loc will be set to -1).
     * @param 	colors		 	The list of colors for each vertex (if empty @link #color_loc will be set to -1).
     * @param 	tex_coords   	The list of texture coordinates for each vertex (if empty @link #tex_coord_loc will be set to -1).
     * @param 	tangents	 	The list of tangents for each vertex (if empty @link #tangent_loc will be set to -1).
     * @param 	bitangents   	The list of bitangents for each vertex (if empty @link #bitangent_loc will be set to -1).
     * @param 	indices		 	The list of indices.
     * @param 	position_loc 	The location of position vertex attribute for the VAO (use -1 if not necessary).
     * @param 	normal_loc   	The location of normal vertex attribute for the VAO (use -1 if not necessary).
     * @param 	tex_coord_loc	The location of texture coordinates vertex attribute for the VAO (use -1 if not necessary).
     * @param 	tangent_loc  	The location of tangent vertex attribute for the VAO (use -1 if not necessary).
     * @param 	bitangent_loc	The location of bitangent vertex attribute for the VAO (use -1 if not necessary).
     * @param 	color_loc	 	The location of color vertex attribute for the VAO (use -1 if not necessary).
     */
    Geometry_Base(GLenum mode,
                  std::vector<float> positions,
                  std::vector<uint32_t> indices,
                  std::vector<float> normals,
                  std::vector<float> colors,
                  std::vector<float> tex_coords,
                  std::vector<float> tangents,
                  std::vector<float> bitangents,
                  GLint position_loc = DEFAULT_POSITION_LOC,
                  GLint normal_loc = DEFAULT_NORMAL_LOC,
                  GLint tex_coord_loc = DEFAULT_TEX_COORD_LOC,
                  GLint tangent_loc = DEFAULT_TANGENT_LOC,
                  GLint bitangent_loc = DEFAULT_BITANGENT_LOC,
                  GLint color_loc = DEFAULT_COLOR_LOC)
        : mode(mode), position_loc(positions.size() > 0 ? position_loc : -1), normal_loc(normals.size() > 0 ? normal_loc : -1),
          tex_coord_loc(tex_coords.size() > 0 ? tex_coord_loc : -1), tangent_loc(tangents.size() > 0 ? tangent_loc : -1),
          bitangent_loc(bitangents.size() > 0 ? bitangent_loc : -1), color_loc(colors.size() > 0 ? color_loc : -1) {

        // Computes the number of elements per vertex.
        elements_per_vertex = 3 + (!normals.empty() ? 3 : 0) + (!colors.empty() ? 3 : 0) + (!tex_coords.empty() ? 2 : 0) +
                              (!tangents.empty() ? 3 : 0) + (!bitangents.empty() ? 3 : 0);

        // Computes the number of vertices.
        const int vertices_count = static_cast<int>(positions.size() / 3);

        // Builds the interleaved buffer from the input data.
        for (size_t i = 0; i < positions.size() / 3; i += 1) {
            interleaved_vertices.push_back(positions[i * 3 + 0]);
            interleaved_vertices.push_back(positions[i * 3 + 1]);
            interleaved_vertices.push_back(positions[i * 3 + 2]);

            if (!normals.empty()) {
                interleaved_vertices.push_back(normals[i * 3 + 0]);
                interleaved_vertices.push_back(normals[i * 3 + 1]);
                interleaved_vertices.push_back(normals[i * 3 + 2]);
            }

            if (!colors.empty()) {
                interleaved_vertices.push_back(colors[i * 3 + 0]);
                interleaved_vertices.push_back(colors[i * 3 + 1]);
                interleaved_vertices.push_back(colors[i * 3 + 2]);
            }

            if (!tex_coords.empty()) {
                interleaved_vertices.push_back(tex_coords[i * 2 + 0]);
                interleaved_vertices.push_back(tex_coords[i * 2 + 1]);
            }

            if (!tangents.empty()) {
                interleaved_vertices.push_back(tangents[i * 3 + 0]);
                interleaved_vertices.push_back(tangents[i * 3 + 1]);
                interleaved_vertices.push_back(tangents[i * 3 + 2]);
            }

            if (!bitangents.empty()) {
                interleaved_vertices.push_back(bitangents[i * 3 + 0]);
                interleaved_vertices.push_back(bitangents[i * 3 + 1]);
                interleaved_vertices.push_back(bitangents[i * 3 + 2]);
            }
        }

        vertex_buffer_stride = elements_per_vertex * sizeof(float);
        vertex_buffer_size = vertices_count * vertex_buffer_stride;

        init_patches_count();

        // Sets the number of elements according to the
        if (!indices.empty()) {
            draw_elements_count = static_cast<GLsizei>(indices.size());
        }

        draw_arrays_count = vertices_count;
    };

    /**
     * Creates a new @link Geometry_Base object from another geometry performing a deep copy.
     *
     * @param 	other	The other geometry.
     */
    Geometry_Base(const Geometry_Base& other)
        : mode(other.mode), vertex_buffer_size(other.vertex_buffer_size), vertex_buffer_stride(other.vertex_buffer_stride),
          interleaved_vertices(other.interleaved_vertices), position_offset(other.position_offset),
          color_offset(other.color_offset), normal_offset(other.normal_offset), tex_coord_offset(other.tex_coord_offset),
          elements_per_vertex(other.elements_per_vertex), draw_arrays_count(other.draw_arrays_count),
          draw_elements_count(other.draw_elements_count), /*vao(other.vao), vertex_buffer(other.vertex_buffer),
          index_buffer(other.index_buffer), THESE SHOULD NOT BE COPIED BUT NEEDS TO BE RECREATED*/ patch_vertices(other.patch_vertices), position_loc(other.position_loc),
          normal_loc(other.normal_loc), tex_coord_loc(other.tex_coord_loc), tangent_loc(other.tangent_loc),
          bitangent_loc(other.bitangent_loc), color_loc(other.color_loc) {
    };

    /**
    * Move constructor swapping the two geometries.
    *
    * @param 	other	The other geometry.
    */
    Geometry_Base(Geometry_Base&& other)
        : Geometry_Base() { swap_fields(*this, other); };

    // ----------------------------------------------------------------------------
    // Operators
    // ----------------------------------------------------------------------------
    Geometry_Base& operator=(Geometry_Base other) {
        swap_fields(*this, other);
        return *this;
    };

    void swap_fields(Geometry_Base& first, Geometry_Base& second) const noexcept {
        using std::swap;

        swap(first.vertex_buffer, second.vertex_buffer);
        swap(first.vertex_buffer_size, second.vertex_buffer_size);
        swap(first.index_buffer, second.index_buffer);
        swap(first.vao, second.vao);
        swap(first.mode, second.mode);
        swap(first.draw_arrays_count, second.draw_arrays_count);
        swap(first.draw_elements_count, second.draw_elements_count);
        swap(first.patch_vertices, second.patch_vertices);
        swap(first.position_loc, second.position_loc);
        swap(first.normal_loc, second.normal_loc);
        swap(first.tex_coord_loc, second.tex_coord_loc);
        swap(first.tangent_loc, second.tangent_loc);
        swap(first.bitangent_loc, second.bitangent_loc);
        swap(first.color_loc, second.color_loc);
        swap(first.elements_per_vertex, second.elements_per_vertex);
        swap(first.vertex_buffer_stride, second.vertex_buffer_stride);
        swap(first.interleaved_vertices, second.interleaved_vertices);
        swap(first.position_offset, second.position_offset);
        swap(first.color_offset, second.color_offset);
        swap(first.normal_offset, second.normal_offset);
        swap(first.tex_coord_offset, second.tex_coord_offset);
    }

    virtual ~Geometry_Base() {
    }

    // ----------------------------------------------------------------------------
    // Methods
    // ----------------------------------------------------------------------------
public:
    /** Sets the default number of patch vertices if the current mode is GL_PATCHES. */
    void init_patches_count() {
        if (mode == GL_PATCHES) {
            patch_vertices = 16;
        }
    }

    /** Binds the VAO corresponding to this geometry. */
    void bind_vao() const {
        glBindVertexArray(vao);
    }

    /**
     * Draws the geometry using either glDrawArrays or glDrawElements based on the current values of {@link draw_arrays_count} and
     * {@link draw_elements_count}.
     */
    void draw() const {
        bind_vao();

        if (mode == GL_PATCHES) {
            glPatchParameteri(GL_PATCH_VERTICES, patch_vertices);
        }

        if (draw_elements_count > 0) {
            glDrawElements(mode, draw_elements_count, GL_UNSIGNED_INT, nullptr);
        } else {
            glDrawArrays(mode, 0, draw_arrays_count);
        }
    }

    /**
     * Draws multiple instances of the geometry using either glDrawArraysInstanced or glDrawElementsInstanced based on
     * the current values of {@link draw_arrays_count} and {@link draw_elements_count}.
     *
     * @param 	count	The number of instances to render.
     */
    void draw_instanced(int count) const {
        bind_vao();

        if (mode == GL_PATCHES) {
            glPatchParameteri(GL_PATCH_VERTICES, patch_vertices);
        }

        if (draw_elements_count > 0) {
            glDrawElementsInstanced(mode, draw_elements_count, GL_UNSIGNED_INT, nullptr, count);
        } else {
            glDrawArraysInstanced(mode, 0, draw_arrays_count, count);
        }
    }
};
