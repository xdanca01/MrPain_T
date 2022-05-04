#pragma once

#include "draw_primitives.hpp"
#include "glm_headers.hpp"
#include <vector>
#include <unordered_map>
#include <string>


struct turtleState{
    public:
        glm::vec3 actualPosition = glm::vec3(0);
        glm::vec3 forwardDirection = glm::vec3(0,1,0);
        glm::vec3 upDirection = glm::vec3(1,0,0);
        glm::vec3 leftDirection = glm::vec3(0,0,1);
        float brushWidth = 1.0f;
};
/// Representation of the turtle (from the turtle geometry)
/// Provided is the basic functionality common to all turtle
/// implementations. It means that concrete implementations
/// are supposed to inherit from this general turtle type.
struct TurtleBase {

    /// To allow for the inheritance.
    virtual ~TurtleBase() {}

    /// Returns the current position of the turtle in 3D space.
    glm::vec3 const& position() const {
        return actualState.actualPosition;
    }
    
    /// Returns the unit "forward" vector of the turtle, i.e.,
    /// the direction where the turtle moves from the current position.
    glm::vec3 const& forward() const {
        return actualState.forwardDirection;
    }

    void computeLeft(){
        actualState.leftDirection = glm::cross(actualState.upDirection, actualState.forwardDirection);
    }
    
    /// Returns the unit "left" vector of the turtle, i.e.,
    /// the direction orthogonal to turtle's "forward" and "up" directions.
    glm::vec3 const& left() const {
        return actualState.leftDirection;
    }
    
    /// Returns the unit "up" vector of the turtle, i.e.,
    /// the direction orthogonal to turtle's "forward" and "left" directions.
    glm::vec3 const& up() const {
        return actualState.upDirection;
    }

    /// Returns the width of brush the turtle uses for drawing (constructing)
    /// geometrical objects. Think of this number as a parameter (multiplier) which
    /// a concrete turtle implementation can use for generation of constructed
    /// geometrical objects (e.g., width of tree branch, size of tree leaf, etc.)
    float brush_width() const {
        return actualState.brushWidth;
    }
    
    /// Move the turtle "distance" meters in the direction of the "forward"
    /// vector of the turtle.
    void move(float const distance) {
        actualState.actualPosition += actualState.forwardDirection * distance;
    }

    /// Rotate the turtle "angle_radians" radians about the axis defined
    /// by the unit vector "unit_axis". The direction of "unit_axis" in
    /// the axis defines the rotation direction.
    /// HINTS: Useful GLM functions: toMat3, angleAxis, cross, normalize
    void rotate(glm::vec3 const& unit_axis, float const angle_radians) {
        glm::quat rotationQuat = glm::normalize(glm::angleAxis(angle_radians, unit_axis));
        glm::mat3 rotationMatrix = glm::toMat3(rotationQuat);
        actualState.upDirection = rotationMatrix * actualState.upDirection;
        actualState.forwardDirection = rotationMatrix * actualState.forwardDirection;
        actualState.leftDirection = glm::cross(actualState.upDirection, actualState.forwardDirection);
    }

    /// Allows to set the width of turtle's brush. The width must be
    /// a positive number. For more info look at the method "brush_width()".
    void set_brush_width(float const width) {
        actualState.brushWidth = width;
    }

    /// Saves the curent turtle's state (i.e., position, all three
    /// direction vectors, and brush width) to a LIFO data structure.
    void push() {
        previousState.push_back(actualState);
    }

    /// Sets the curent turtle's state (i.e., position, all three
    /// direction vectors, and brush width) accorting to the record
    /// lastly inserted to the LIFO data structure. That record is
    /// then removed from the data structure.
    /// NOTE: Does nothing, when the LIFO data structure is empty.
    void pop() {
        if(!previousState.empty()){
            actualState = previousState.back();
            previousState.pop_back();
        }
    }

private:
    /// Initialise the initial state of the turtle such that:
    ///     - The turtle is at the origin of the world space.
    ///     - Turtle's forward direction is aligned with positive
    ///       direction of the world Y axis.
    ///     - Turtle's left direction is aligned with positive
    ///       direction of the world Z axis.
    ///     - Turtle's up direction is aligned with positive
    ///       direction of the world X axis.
    ///     - The width of turtle's brush is 1.
    turtleState actualState;
    std::vector<turtleState> previousState;
};

/// A specialisation of the general turtle type "TurtleBase"
/// commanding the turtle according to passed L-system rules.
/// The turtle generates objects of two kinds:
///     Branch
///     Leaf
/// Both types are defined in the file "draw_primitives.hpp".
/// An instance of LTurtle type executes the passed L-system
/// and based on that it generates and stores instances of
/// Branch and Leaf types to output vectors.
/// The LTurtle understands several symbols of the L-system.
/// They are described in the "process" method.
struct LTurtle : public TurtleBase {

    /// Stores several constants (numbers) representing
    /// the configuration of the LTrutle.
    struct Config {
        float radius;                   /// The radius of a tree branch (used for instances of Branch type)
        float distance;                 /// The distance between bottom and top of a tree branch (used for instances of Branch type)
        float leaf_size;                /// The size of a tree leaf (used for instances of Leaf type)
        float angle_world_y;            /// An angle in radians used for rotating the turtle about world Y axis (positive direction)
        float angle_turtle_left;        /// An angle in radians used for rotating the turtle about the axis given by turtle's "left" vector.
        float brush_decay_coef;         /// A multiplier defining the change of width of turtle's brush.
                                        /// If "w" is current brush width, then new brush width should be "w * brush_decay_coef"
        unsigned int max_depth;         /// Maximal recursion depth for application the rules of the L-system.
                                        /// Example: Let "max_depth == 2" and we have a rule A->AA and A is the axiom. Then:
                                        ///             A           <---- depth 0
                                        ///             AA          <---- depth 1
                                        ///             AAAA        <---- depth 2, this is "max_depth" => stop here.
    };

    /// A type for rules of a L-system.
    /// NOTE: We restrict ourselves to rules with a single symbol on the left.
    using Rules = std::unordered_map<char, std::string>;

    /// Construct a turtle for a passed config and L-system rules.
    /// There are further passed references to output vectors, where
    /// the constructed geometrical objects are stored.
    LTurtle(
        Config const& cfg_,                     /// The cunfiguration of the turtle (See sturct Config for more info)
        Rules const& rules_,                    /// The map of rules of the L-system
        std::vector<Branch>& branches_ref,      /// Reference to the output vector for constructed Branch instances
        std::vector<Leaf>& leaves_ref           /// Reference to the output vector for constructed Leaf instances
        )
        : TurtleBase()
        , cfg(cfg_)
        , rules(rules_)
        , branches(branches_ref)
        , leaves(leaves_ref)
    {}

    /// Getter of the config data.
    Config const& config() const { return cfg; }

    /// Applies L-system's rules to the passed sentence (axiom) and
    /// generates and saves corresponding geometrical objects by
    /// calling the "process" method.
    /// IMPORTANT:
    ///     * The method applies the L-system's rules only
    ///       if depth <= config().max_depth.
    ///     * The method is recursive and does NOT construct
    ///       entire sentence explicitly at any depth > 0.
    void run(std::string const& sentence, unsigned int depth = 0U) {
        if(depth < config().max_depth){
            for(auto i : sentence){
                auto rule = rules.find(i);
                if(rule != rules.end()){
                    run(rule->second, depth+1);
                }
                else{
                    process(i);
                }
            }
        }
        else{
            for(auto i : sentence){
                process(i);
            }
        }
    }

    /// Commands the turtle based on the passed symbol.
    /// The "switch" statement in the method's body says
    /// what symbols are recognised and their interpretations
    /// are described individualy for each case (symbol).
    void process(char const symbol) {
        switch (symbol)
        {
        case 'L':
        {
            /// 1. Creates an instance of the Leaf type:
            ///         - The leaf is placed at the position of the turtle.
            ///         - Leaf's direction is turtle's forward direction.
            ///         - Leaf's up direction is turtle's left direction.
            ///         - Leaf's x-size is "config().leaf_size * brush_width()".
            ///         - Leaf's y-size is twice of x-size.
            /// 2. Stores the Leaf instance into the output vector "leaves".
            /// 3. Moves the turtle by "config().distance * brush_width()"
            ///    meters in turtle's forward direction.
            float xSize = config().leaf_size * this->brush_width();
            Leaf newLeaf = Leaf(this->position(), this->forward(), this->left(), glm::vec2(xSize, xSize*2));
            leaves.push_back(newLeaf);
            this->move(config().distance * this->brush_width());
            break;
        }
        case 'l':
        {
            /// 1. Creates an instance of the Leaf type:
            ///         - The leaf is placed at the position of the turtle.
            ///         - Leaf's direction is turtle's forward direction.
            ///         - Leaf's up direction is turtle's up direction.
            ///         - Leaf's x-size is "config().leaf_size * brush_width()".
            ///         - Leaf's y-size is twice of x-size.
            /// 2. Stores the Leaf instance into the output vector "leaves".
            /// 3. Moves the turtle by "config().distance * brush_width()"
            ///    meters in turtle's forward direction.
            float xSize = config().leaf_size * this->brush_width();
            Leaf newLeaf = Leaf(this->position(), this->forward(), this->up(), glm::vec2(xSize, xSize*2));
            leaves.push_back(newLeaf);
            this->move(config().distance * this->brush_width());
            break;
        }
        case 'B':
        {
            /// 1. Creates an instance of the Branch type:
            ///         - The branch is placed at the position of the turtle.
            ///         - The "r1" radius of the bottom side of the branch is
            ///           "config().radius * brush_width()".
            ///           The bottom side lies at the position of the turtle.
            ///         - The "r2" radius of the top side of the branch is
            ///           "config().brush_decay_coef * r1".
            ///           The top side is shifted from the bottom one by
            ///           "config().distance * brush_width()" meters in
            ///           turtle's forward direction.
            /// 2. Stores the Branch instance into the output vector "branches".
            /// 3. Moves the turtle by "config().distance * brush_width()"
            ///    meters in turtle's forward direction.
            glm::vec3 topSide = this->position() + config().distance * this->brush_width() * this->forward();
            float r1 = config().radius * brush_width();
            Branch newBranch = Branch(this->position(), r1, topSide, config().brush_decay_coef * r1);
            branches.push_back(newBranch);
            this->move(config().distance * this->brush_width());
            break;
        }
        case 'M':
            /// Moves the turtle by "config().distance * brush_width()"
            /// meters in turtle's forward direction. Neither branch
            /// nor leaf is generated during this move.

            this->move(config().distance * this->brush_width());
            break;
        case '+':
        {
            /// Rotates the turtle about world Y axis in the positive direction
            /// by the angle "config().angle_world_y".

            glm::vec3 yAxis = glm::vec3(0, 1, 0);
            this->rotate(yAxis, config().angle_world_y);
            break;
        }
        case '-':
        {
            /// Rotates the turtle about world Y axis in the negative direction
            /// by the angle "config().angle_world_y".

            glm::vec3 yAxis = glm::vec3(0, -1, 0);
            this->rotate(yAxis, config().angle_world_y);
            break;
        }
        case '&':
            /// Rotates the turtle about the axis given by turtle's "left" vector
            /// in the positive direction by the angle "config().angle_turtle_left".

            this->rotate(this->left(), config().angle_turtle_left);
            break;
        case '^':
           /// Rotates the turtle about the axis given by turtle's "left" vector
            /// in the negative direction by the angle "config().angle_turtle_left".

            this->rotate(-this->left(), config().angle_turtle_left);
            break;
        case '*':
            /// Sets turtle's brush width to the "config().brush_decay_coef" multiple
            /// of the current width of the branch.

            this->set_brush_width(config().brush_decay_coef * this->brush_width());
            break;
        case '[':
            /// Stores the current turtle's state into the LIFO data structure.

            this->push();
            break;
        case ']':
            /// Sets current turtle's state according to the record lastly stored
            /// into the LIFO data structure. Then removes that record from the
            /// LIFO data structure.

            this->pop();
            break;
        default:
            // We ignore all other symbols - they do not have any interpretation for the turtle.
            break;
        }
    }

private:
    Config cfg;                         /// Stores the configuration of the turtle. Should not be modified.
    Rules rules;                        /// Stores the rules of the L-system. Should not be modified.
    std::vector<Branch>& branches;      /// Output vector for all created instances of Branch type.
    std::vector<Leaf>& leaves;          /// Output vector for all created instances of Branch type.
};
