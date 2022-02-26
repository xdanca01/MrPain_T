#pragma once

#include "glad.h"

/**
 * The base class for all wrappers of OpenGL objects (such as Textures, Buffers, etc.). The idea is that this
 * construct simplifies the work with these native OpenGL objects and shields students from the pure OpenGL in some
 * courses.
 *
 * @author	<a href="mailto:jan.byska@gmail.com">Jan Byška</a>
 */
class OpenGLObject {

    // ----------------------------------------------------------------------------
    // Variables
    // ----------------------------------------------------------------------------
  protected:
    /** The corresponding OpenGL object. */
    GLuint opengl_object = 0;

    /** The target to which this object can be bound (i.e., the target used by the glBind* methods). */
    GLenum target;

    /** The flag determining if the OpenGL counterparts should be initialized. */
    bool cpu_only = false;

    // ----------------------------------------------------------------------------
    // Constructors
    // ----------------------------------------------------------------------------
  public:
    /**
     * Constructs a new @link OpenGLObject including the OpenGL counterpart.
     *
     * @param 	target  	The target to which this object can be bound.
     * @param 	cpu_only	The flag determining if the object will be CPU only (should be @p true for tests as they do
     * 						not have OpenGL context).
     */
    OpenGLObject(GLenum target, bool cpu_only) : target(target), cpu_only(cpu_only) {
        // Note that the OpenGL objects are created in child classes.
    }

    /**
     * Constructs a new @link OpenGLObject from another object (copy constructor that performs a deep copy).
     * TODO: Note that the @link opengl_object is not copied.
     *
     * @param other The other OpenGL object that will be copied
     */
    OpenGLObject(const OpenGLObject& other) : target(other.target), cpu_only(other.cpu_only) {
        // Note that the OpenGL objects are created in child classes.
    }

    /**
     * Constructor that moves the @link OpenGLObject to the new object. Zeroes out the old @link OpenGLObject so that destructor does nothing.
     *
     * @param other The other object to move
     */
     OpenGLObject(OpenGLObject&& other) : OpenGLObject(other) { swap_fields(*this, other); }

    /**
     * Copy assignment. Performs a deep copy the OpenGL object.
     *
     * @param other The other @link OpenGLObject that will be copied.
     */
    // OpenGLObject& operator=(OpenGLObject other) {
    //    swap_fields(*this, other);
    //    return *this;
    // }
    // ^ Use this form with copy-and-swap idiom!
    // ^ Move assignment is automatic when there is swap idiom + copy construct + move constructor

    /**
     * Destroys this {@link OpenGLObject}.
     */
    virtual ~OpenGLObject() {}

  public:
    // ----------------------------------------------------------------------------
    // Methods
    // ----------------------------------------------------------------------------

    /**
     * The custom swap method that exchanges the values of fields of two objects.
     *
     * @param 	first 	The first buffer to swap.
     * @param 	second	The second buffer to swap.
     */
    void swap_fields(OpenGLObject& first, OpenGLObject& second) noexcept {
        std::swap(first.opengl_object, second.opengl_object);
        std::swap(first.target, second.target);
        std::swap(first.cpu_only, second.cpu_only);
    }

    /** Uploads the data from CPU to GPU. */
    void virtual update_opengl_data() = 0;

    // ----------------------------------------------------------------------------
    // Getters & Setters
    // ----------------------------------------------------------------------------
  public:
    /**
     * Returns the OpenGL counterpart of this object.
     *
     * @return	The OpenGL object.
     */
    GLuint get_opengl_object() const { return opengl_object; }
};
