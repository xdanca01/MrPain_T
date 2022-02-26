#pragma once

#include "geometry.hpp"
#include "material_ubo.hpp"
#include "model_ubo.hpp"
#include "texture.hpp"

/**
 * The most basic representation of a scene object.
 *
 * @author	<a href="mailto:jan.byska@gmail.com">Jan Byška</a>
 */
class SceneObject {
    // ----------------------------------------------------------------------------
    // Variables
    // ----------------------------------------------------------------------------
  protected:
    /** The geometry representation of the scene object. */
    Geometry* geometry;
    /** The UBO containing the model data. */
    ModelUBO* model_ubo;
    /** The UBO containing the material. */
    PhongMaterialUBO* material;
    /** The texture that should be used when rendering this object. */
    GLuint* texture;

    // ----------------------------------------------------------------------------
    // Constructors
    // ----------------------------------------------------------------------------
  public:
    /**
     * Constructs a new @link SceneObject with custom values using pointers.
     *
     * @param 	geometry 	The geometry representation of the scene object.
     * @param 	model_ubo	The UBO containing the model data.
     * @param 	material 	The UBO containing the material of the scene object.
     */
    SceneObject(Geometry* geometry = nullptr, ModelUBO* model_ubo = nullptr, PhongMaterialUBO* material = nullptr, GLuint* texture = nullptr)
        : geometry(geometry), model_ubo(model_ubo), material(material), texture(texture) {}

    /**
     * Constructs a new @link SceneObject with custom values using references.
     *
     * @param 	geometry 	The geometry representation of the scene object.
     * @param 	model_ubo	The UBO containing the model data.
     * @param 	material 	The UBO containing the material of the scene object.
     */
    SceneObject(Geometry& geometry, ModelUBO& model_ubo, PhongMaterialUBO& material) : SceneObject(&geometry, &model_ubo, &material, nullptr) {}

    /**
     * Constructs a new @link SceneObject with custom values using references.
     *
     * @param 	geometry 	The geometry representation of the scene object.
     * @param 	model_ubo	The UBO containing the model data.
     * @param 	texture  	The texture that should be used when rendering this object.
     */
    SceneObject(Geometry& geometry, ModelUBO& model_ubo, PhongMaterialUBO& material, GLuint& texture) : SceneObject(&geometry, &model_ubo, &material, &texture) {}

    // ----------------------------------------------------------------------------
    // Getters & Setters
    // ----------------------------------------------------------------------------
    /**
     * Returns the geometry representation of the scene object
     *
     * @return	The geometry.
     */
    [[nodiscard]] const Geometry& get_geometry() const { return *this->geometry; }

    /**
     * Returns the UBO containing the model data.
     *
     * @return	The model UBO.
     */
    [[nodiscard]] const ModelUBO& get_model_ubo() const { return *this->model_ubo; }

    /**
     * Returns the UBO containing the material.
     *
     * @return	The material UBO.
     */
    [[nodiscard]] const PhongMaterialUBO& get_material() const { return *this->material; }

    /**
     * Checks if the scene object has a texture defined.
     *
     * @return	@p true if the object has a texture defined, @p false if not.
     */
    [[nodiscard]] bool has_texture() const { return this->texture != nullptr; }

    /**
     * Returns the texture that should be used when rendering this object.
     *
     * @return	The texture.
     */
    [[nodiscard]] const GLuint& get_texture() const { return *this->texture; }
};
