#include "geometry_3_3.hpp"

// ----------------------------------------------------------------------------
// Constructors
// ----------------------------------------------------------------------------

Geometry_3_3::Geometry_3_3(GLenum mode, std::vector<float> positions, std::vector<float> colors)
    : Geometry_3_3(mode,
                   positions,
                   std::vector<uint32_t>{}, // no indices
                   std::vector<float>{},    // no normals
                   colors,
                   std::vector<float>{}, // no texture cords
                   std::vector<float>{}, // no tangents,
                   std::vector<float>{}, // no bitangents
                   DEFAULT_POSITION_LOC,
                   -1, // no normals
                   DEFAULT_COLOR_LOC,
                   -1, // no texture cords
                   -1, // no tangents,
                   -1  // no bitangents
        ) {
    if (positions.size() != colors.size() && colors.size() > 0) {
        throw "The number of colors does not correspond to the number of vertices.";
    }
}

Geometry_3_3::Geometry_3_3(GLenum mode, std::vector<float> positions, std::vector<float> normals, std::vector<float> colors)
    : Geometry_3_3(mode,
                   positions,
                   std::vector<uint32_t>{}, // no indices
                   normals,
                   colors,
                   std::vector<float>{}, // no texture cords
                   std::vector<float>{}, // no tangents,
                   std::vector<float>{}, // no bitangents
                   DEFAULT_POSITION_LOC,
                   DEFAULT_NORMAL_LOC,
                   DEFAULT_COLOR_LOC,
                   -1, // no texture cords
                   -1, // no tangents,
                   -1  // no bitangents
        ) {
    if (positions.size() != normals.size() && normals.size() > 0) {
        throw "The number of normals does not correspond to the number of vertices.";
    }
    if (positions.size() != colors.size() && colors.size() > 0) {
        throw "The number of colors does not correspond to the number of vertices.";
    }
}

Geometry_3_3::Geometry_3_3(GLenum mode, std::vector<float> positions, std::vector<float> normals, std::vector<float> colors, std::vector<float> coords)
    : Geometry_3_3(
        mode,
        positions,
        std::vector<uint32_t>{}, // no indices
        normals,
        colors,
        coords,
        std::vector<float>{}, // no tangents,
        std::vector<float>{}, // no bitangents
        DEFAULT_POSITION_LOC,
        DEFAULT_NORMAL_LOC,
        DEFAULT_COLOR_LOC,
        DEFAULT_TEX_COORD_LOC,
        -1, // no tangents,
        -1  // no bitangents
        ) {
    if (positions.size() != normals.size() && normals.size() > 0) {
        throw "The number of normals does not correspond to the number of vertices.";
    }
    if (positions.size() != colors.size() && colors.size() > 0) {
        throw "The number of colors does not correspond to the number of vertices.";
    }
    if ((positions.size() / 3) != (coords.size() / 2) && coords.size() > 0) {
        throw "The number of texture coordinates does not correspond to the number of vertices.";
    }
}

Geometry_3_3::Geometry_3_3(
    GLenum mode,
    std::vector<float> positions,
    std::vector<uint32_t> indices,
    std::vector<float> normals,
    std::vector<float> colors,
    std::vector<float> tex_coords,
    std::vector<float> tangents,
    std::vector<float> bitangents,
    GLint position_loc,
    GLint normal_loc,
    GLint color_loc,
    GLint tex_coord_loc,
    GLint tangent_loc,
    GLint bitangent_loc)
    : Geometry_Base(mode, positions, indices, normals, colors, tex_coords, tangents, bitangents, position_loc, normal_loc,
                    tex_coord_loc, tangent_loc, bitangent_loc, color_loc) {
    // Creates a single buffer for vertex data.
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, vertex_buffer_size, interleaved_vertices.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    init_vao();

    // Creates a buffer for indices if requested.
    if (!indices.empty()) {
        glBindVertexArray(vao); // Binds VAO

        glGenBuffers(1, &index_buffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_DYNAMIC_DRAW);

        glBindVertexArray(0); // Unbinds VAO
    }
}

Geometry_3_3::Geometry_3_3(
    GLenum mode,
    int elements_per_vertex,
    int vertices_count,
    const float* vertices,
    int indices_count,
    const unsigned int* indices,
    GLint position_loc,
    GLint normal_loc,
    GLint tex_coord_loc,
    GLint tangent_loc,
    GLint bitangent_loc)
    : Geometry_Base(mode, elements_per_vertex, vertices_count, indices_count, position_loc, normal_loc, tex_coord_loc,
                    tangent_loc, bitangent_loc) {
    // Creates a single buffer for vertex data.
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, vertex_buffer_size, vertices, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    init_vao();

    if (indices && indices_count > 0) {
        glBindVertexArray(vao); // Binds VAO

        // Creates a buffer for indices.
        glGenBuffers(1, &index_buffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex_buffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_count * sizeof(uint32_t), indices, GL_DYNAMIC_DRAW);

        glBindVertexArray(0); // Unbinds VAO
    }
}

Geometry_3_3::Geometry_3_3(const Geometry_3_3& other)
    : Geometry_Base(other) {
    // Creates a single buffer for vertex data.
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_COPY_READ_BUFFER, other.vertex_buffer);
    glBindBuffer(GL_COPY_WRITE_BUFFER, vertex_buffer);
    glBufferData(GL_COPY_WRITE_BUFFER, vertex_buffer_size, nullptr, GL_STATIC_DRAW);
    glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, vertex_buffer_size);

    glGenBuffers(1, &index_buffer);
    glBindBuffer(GL_COPY_READ_BUFFER, other.index_buffer);
    glBindBuffer(GL_COPY_WRITE_BUFFER, index_buffer);
    glBufferData(GL_COPY_WRITE_BUFFER, draw_elements_count * sizeof(uint32_t), nullptr, GL_STATIC_DRAW);
    glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, draw_elements_count * sizeof(uint32_t));

    init_vao();

    // Sets the mode and the number of vertices to draw.
    if (mode != GL_PATCHES) {
        // TODO not sure this is correct
        glBindVertexArray(vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex_buffer);
        glBindVertexArray(0);
    }
}

Geometry_3_3::~Geometry_3_3() {
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteBuffers(1, &index_buffer);
    glDeleteVertexArrays(1, &vao);
}

// ----------------------------------------------------------------------------
// Operators
// ----------------------------------------------------------------------------
Geometry_3_3& Geometry_3_3::operator=(Geometry_3_3 other) {
    swap_fields(*this, other);
    return *this;
}

// ----------------------------------------------------------------------------
// Methods
// ----------------------------------------------------------------------------
// 
void Geometry_3_3::init_vao() {
    // Creates a new VAO.
    glGenVertexArrays(1, &vao);
    // Binds the VAO.
    bind_vao();
    // Bind the interleaved buffer to the VAO.
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

    // Sets the vertex attributes and their parameters.
    int offset = 0;
    if (position_loc >= 0) {
        glEnableVertexAttribArray(position_loc);
        glVertexAttribPointer(position_loc, 3, GL_FLOAT, GL_FALSE, vertex_buffer_stride, static_cast<void*>(nullptr));
        offset += 3;
    }
    if (normal_loc >= 0) {
        glEnableVertexAttribArray(normal_loc);
        glVertexAttribPointer(normal_loc, 3, GL_FLOAT, GL_FALSE, vertex_buffer_stride, (void*)(sizeof(float) * offset));
        normal_offset = offset;
        offset += 3;
    }
    if (color_loc >= 0) {
        glEnableVertexAttribArray(color_loc);
        glVertexAttribPointer(color_loc, 3, GL_FLOAT, GL_FALSE, vertex_buffer_stride, (void*)(sizeof(float) * offset));
        color_offset = offset;
        offset += 3;
    }
    if (tex_coord_loc >= 0) {
        glEnableVertexAttribArray(tex_coord_loc);
        glVertexAttribPointer(tex_coord_loc, 2, GL_FLOAT, GL_FALSE, vertex_buffer_stride, (void*)(sizeof(float) * offset));
        tex_coord_offset = offset;
        offset += 2;
    }
    if (tangent_loc >= 0) {
        glEnableVertexAttribArray(tangent_loc);
        glVertexAttribPointer(tangent_loc, 3, GL_FLOAT, GL_FALSE, vertex_buffer_stride, (void*)(sizeof(float) * offset));
        offset += 3;
    }
    if (bitangent_loc >= 0) {
        glEnableVertexAttribArray(bitangent_loc);
        glVertexAttribPointer(bitangent_loc, 3, GL_FLOAT, GL_FALSE, vertex_buffer_stride, (void*)(sizeof(float) * offset));
        offset += 3;
    }

    // Unbinds the VAO.
    glBindVertexArray(0);
}
