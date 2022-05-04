#pragma once

#include "glm_headers.hpp"

/** The definition of a rounded cone. */
struct Branch {
    // ----------------------------------------------------------------------------
    // Variables
    // ----------------------------------------------------------------------------
  public:
    /** The position of the bottom point. */
    glm::vec3 p1;
    /** The bottom radius. */
    float r1;
    /** The position of the top point. */
    glm::vec3 p2;
    /** The upper radius. */
    float r2;

    // ----------------------------------------------------------------------------
    // Constructors & Destructors
    // ----------------------------------------------------------------------------
  public:
    /**
     * Creates a new @link Branch with default values.
     */
    Branch() : Branch({0, 0, 0}, 0, {0, 0, 0}, 0) {}
    /**
     * Creates a new @link Branch with specified values.
     *
     * @param 	p1 	The position of the bottom point.
     * @param 	r1  The bottom radius.
     * @param 	p2	The position of the top point.
     * @param 	r2  The upper radius.
     */
    Branch(const glm::vec3& p1, float r1, const glm::vec3& p2, float r2) : p1(p1), r1(r1), p2(p2), r2(r2) {}
};

/** The definition of a leaf. */
struct Leaf {
    // ----------------------------------------------------------------------------
    // Variables
    // ----------------------------------------------------------------------------
  public:
    /** The position of the leaf (.w is used only for padding). */
    glm::vec4 position;
    /** The direction of the leaf (.w is used only for padding). */
    glm::vec4 direction;
    /** The up vector for the leaf (.w is used only for padding). */
    glm::vec4 up;
    /** The size of the leaf (.zw are used only for padding). */
    glm::vec4 size;

    // ----------------------------------------------------------------------------
    // Constructors & Destructors
    // ----------------------------------------------------------------------------
  public:
    /**
     * Creates a new @link Leaf with default values.
     */
    Leaf() : Leaf({0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0,0}) {}
    /**
     * Creates a new @link Leaf with specified values.
     *
     * @param 	position 	The position of the leaf.
     * @param 	direction  The direction of the leaf.
     * @param 	up	The up vector for the leaf.
     * @param 	size    The size of the leaf.
     */
    Leaf(const glm::vec3& position_, const glm::vec3& direction_, const glm::vec3& up_, const glm::vec2& size_)
        : position(glm::vec4(position_,1)), direction(glm::vec4(direction_,0)), up(glm::vec4(up_,0)), size(glm::vec4(size_,0,0))
    {}
};
