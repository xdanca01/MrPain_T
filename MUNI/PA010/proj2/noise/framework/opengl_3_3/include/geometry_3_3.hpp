#pragma once

#include "geometry_base.hpp"

/**
 * The implementation of the @link Geometry class using OpenGL 3.3. The main use for this class is to support broader number of
 * computers including MacOS in PB009 course.
 *
 * @author	<a href="mailto:jan.byska@gmail.com">Jan Byška</a>
 */
class Geometry_3_3 : public Geometry_Base {
    // ----------------------------------------------------------------------------
    // Constructors
    // ----------------------------------------------------------------------------
public:
    /** Creates a new empty @link Geometry_3_3 object. */
    Geometry_3_3()
        : Geometry_Base(GL_POINTS, 0, 0, 0) {
    }

    /**
     * Creates a @link Geometry_3_3 object.
     *
     * @param 	mode		 	The mode that will be used for rendering the geometry.
     * @param 	positions	 	The list of vertex positions.
     * @param 	colors		 	The colors.
     */
    Geometry_3_3(GLenum mode, std::vector<float> positions, std::vector<float> colors);

    /**
     * Creates a @link Geometry_3_3 object.
     *
     * @param 	mode		 	The mode that will be used for rendering the geometry.
     * @param 	positions	 	The list of vertex positions.
     * @param 	normals		 	The list of normals associated with the vertices.
     * @param 	colors		 	The list of colors associated with the vertices.
     */
    Geometry_3_3(GLenum mode, std::vector<float> positions, std::vector<float> normals, std::vector<float> colors);

    /**
     * Creates a @link Geometry_3_3 object.
     *
     * @param 	mode		 	The mode that will be used for rendering the geometry.
     * @param 	positions	 	The list of vertex positions.
     * @param 	normals		 	The list of normals associated with the vertices.
     * @param 	colors		 	The list of colors associated with the vertices.
     * @param 	colors		 	The list of texture coordinates associated with the vertices.
     */
    Geometry_3_3(GLenum mode, std::vector<float> positions, std::vector<float> normals, std::vector<float> colors, std::vector<float> coords);

    /**
     * Creates a @link Geometry_3_3 object.
     *
     * @param 	mode		 	The mode that will be used for rendering the geometry.
     * @param 	positions	 	The list of vertex positions.
     * @param 	indices		 	The list of indices defining the geometry.
     * @param 	normals		 	The list of normals associated with the vertices.
     * @param 	colors		 	The colors.
     * @param 	tex_coords   	The list of texture coordinates associated with the vertices.
     * @param 	tangents	 	The list of tangents associated with the vertices.
     * @param 	bitangents   	The list of bitangents associated with the vertices.
     * @param 	position_loc 	The location of position vertex attribute for the VAO (use -1 if not necessary).
     * @param 	normal_loc   	The location of normal vertex attribute for the VAO (use -1 if not necessary).
     * @param 	color_loc	 	The location of color vertex attribute for the VAO (use -1 if not necessary).
     * @param 	tex_coord_loc	The location of texture coordinates vertex attribute for the VAO (use -1 if not
     * 							necessary).
     * @param 	tangent_loc  	The location of tangent vertex attribute for the VAO (use -1 if not necessary).
     * @param 	bitangent_loc	The location of bitangent vertex attribute for the VAO (use -1 if not necessary).
     */
    Geometry_3_3(
        GLenum mode,
        std::vector<float> positions,
        std::vector<uint32_t> indices,
        std::vector<float> normals = {},
        std::vector<float> colors = {},
        std::vector<float> tex_coords = {},
        std::vector<float> tangents = {},
        std::vector<float> bitangents = {},
        GLint position_loc = DEFAULT_POSITION_LOC,
        GLint normal_loc = DEFAULT_NORMAL_LOC,
        GLint color_loc = DEFAULT_COLOR_LOC,
        GLint tex_coord_loc = DEFAULT_TEX_COORD_LOC,
        GLint tangent_loc = DEFAULT_TANGENT_LOC,
        GLint bitangent_loc = DEFAULT_BITANGENT_LOC);

    /**
    * Creates a @link Geometry object. TODO add colors
    * @param   mode                The mode that will be used for rendering the geometry.
    * @param   vertex_buffer_size  The number of float values stored in the buffer for each vertex.
    * @param 	vertices_count	    The number of vertices that the created geometry will have.
    * @param 	vertices	    The actual geometry vertices.
    * @param 	indices_count 	    The number of indices wihing the geometry.
    * @param 	indices		    The actual indices.
    * @param 	position_loc  	    The location of position vertex attribute for the VAO (use -1 if not necessary).
    * @param 	normal_loc	    The location of normal vertex attribute for the VAO (use -1 if not necessary).
    * @param 	tex_coord_loc 	    The location of texture coordinates vertex attribute for the VAO (use -1 if not necessary).
    * @param 	tangent_loc   	    The location of tangent vertex attribute for the VAO (use -1 if not necessary).
    * @param 	bitangent_loc 	    The location of bitangent vertex attribute for the VAO (use -1 if not necessary).
    */
    Geometry_3_3(
        GLenum mode,
        int vertex_buffer_size,
        int vertices_count,
        const float* vertices,
        int indices_count,
        const unsigned int* indices,
        GLint position_loc = DEFAULT_POSITION_LOC,
        GLint normal_loc = DEFAULT_NORMAL_LOC,
        GLint tex_coord_loc = DEFAULT_TEX_COORD_LOC,
        GLint tangent_loc = DEFAULT_TANGENT_LOC,
        GLint bitangent_loc = DEFAULT_BITANGENT_LOC);

    /**
     * Creates a new @link Geometry object from another geometry performing a deep copy.
     *
     * @param 	other	The other geometry.
     */
    Geometry_3_3(const Geometry_3_3& other);

    /**
     * Move constructor swapping the two geometries.
     *
     * @param 	other	The other geometry.
     */
    Geometry_3_3(Geometry_3_3&& other)
        : Geometry_3_3() { swap_fields(*this, other); };

    /**
     * Destroys this @link Geometry_3_3.
     */
    virtual ~Geometry_3_3();

    // ----------------------------------------------------------------------------
    // Operators
    // ----------------------------------------------------------------------------
public:
    /**
     * The copy assignment using copy-and-swap idiom.
     *
     * @param other The other geometry that will be copied (i.e., swapped into this).
     * @return A shallow copy of the other geometry that was moved into this object.
     */
    Geometry_3_3& operator=(Geometry_3_3 other);;

    // ----------------------------------------------------------------------------
    // Methods
    // ----------------------------------------------------------------------------
public:
    void init_vao();
};
