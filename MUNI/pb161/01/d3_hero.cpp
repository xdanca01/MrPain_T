#include <string>
#include <cassert>

/* In many programs, pre-made data types included in the standard
 * library are more than sufficient. However, it is also often the
 * case that a custom data type could be useful – most often to
 * describe a particular concept from the domain which the program
 * models.

 * Let us consider a dungeon crawler, or some other role-playing
 * game set in a fantasy world. In such games, the protagonist will
 * be able to pick up items and make use of them, for instance wield
 * a sword to fight the critters in the dungeon.
 *
 * This would be a rather typical use case for a custom data type:
 * there might be many individual swords in the game, but they all
 * share the same essential set of attributes, like weight, or the
 * amount of damage they deal to the opponent. Of course, we could
 * store these attributes as a tuple, with anonymous fields, and
 * remember that the weight is the first element and the attack
 * strength is the second. While fine in a small program, this
 * approach is not very scalable.¹
 *
 * With ‹struct› (and ‹class›, in a short while), we can create
 * «user-defined data types», with named «attributes» and «methods».
 * The ‹struct› keyword is inherited from C, where it defines an
 * «aggregate» (or record) data type. C++ extends this concept with
 * methods, constructors, destructors, inheritance, and so on.
 * However, at their heart, C++ objects are really just fancy record
 * types. We will start by exploring these. */

/* A record type describes a composite (or aggregate) value, made of
 * a fixed number of attributes (fields), possibly of different
 * types. In this sense, it is very much like a tuple. However, in a
 * record type, the fields have names, and their values are accessed
 * by using those names (instead of their positions as in a tuple).
 * To define a record type, we use the keyword ‹struct›, followed by
 * the name of the type, followed by the definition of the
 * individual fields. Let's start by defining a type which will
 * describe a sword. */

struct sword
{

    /* The most important attribute of a sword is, clearly, a fancy
     * name. Recall that we can use ‹std::string› to conveniently
     * store strings. Let us then declare the attribute ‹name› of
     * type ‹std::string›: */

    std::string name;

    /* Then there are some attributes that deal with game mechanics.
     * Let us just describe them using two integers, ‹weight› and
     * ‹attack›. In actual games, things usually get a bit more
     * complicated. It is possible to give «default values» to
     * attributes – in this case, when a value of type ‹sword› is
     * created, ‹weight› and ‹attack› will be both set zero. How
     * this is achieved or why it is important will be discussed
     * later. */

    int weight = 0;
    int attack = 0;
};

/* That is all. At this point, ‹sword› is a type, like ‹int› or
 * ‹std::string›, and we can declare variables of type ‹sword›,
 * return values of type ‹sword› from functions, or pass values of
 * type ‹sword› as function arguments. For example, let's write a
 * trivial predicate on values of type ‹sword›. Notice the syntax
 * for attribute access: it is the same that we have used for
 * calling methods of ‘built-in’ types like ‹std::string› earlier.
 * This is not a coincidence. */

bool sword_is_heavy( const sword &s )
{
    return s.weight > 50;
}

/* Let us define another record type, ‹shield›, before moving on. */

struct shield
{
    std::string name;
    int weight = 0;
    int defense = 0;
};

/* Swords and shields are usually rather passive. However, programs
 * often also model more dynamic entities; user-defined record types
 * would seem like a good fit to describe their static side (i.e.
 * their attributes). For instance, a ‹hero› would have a health bar
 * (how much damage they can take before dying), and some weapons (a
 * sword and a shield, for instance). And obviously a name.
 *
 * Given a record type which models an entity, it is possible, of
 * course, to write functions which describe the «behaviours» of
 * this entity. For instance ‹hero_walk› or ‹hero_attack› could be
 * functions which take the specific hero to act on as one of their
 * arguments.
 *
 * You perhaps notice the imbalance though: attributes use a nice
 * and concise syntax, ‹value.attribute›, but functions use much
 * clumsier ‹type_method( value, … )›. But we did not have to say
 * ‹string_size( string )› earlier.
 *
 * Indeed, in C++, it is possible to also bundle functions into
 * user-defined data types, in addition to attributes. Such data
 * types are no longer called ‘record types’ – instead, they are
 * known as «classes». In other words, a C++ class is a
 * «user-defined data type» with «attributes» and «methods»
 * (associated functions). Let us define one of those – the syntax
 * is analogous to record types:² */

class hero
{
    /* In classes, attributes are often «private»: only methods of
     * the same class are allowed to directly access them. This is
     * the default: unlike ‹struct›, when we start writing
     * declarations into a class, they will be inaccessible to the
     * outside code. This is okay for our current purposes. It is
     * also common practice to prefix attributes in a class (unless
     * they are public) with an underscore, or some other short
     * string (‹m_›, for member, is also sometimes used), to avoid
     * naming conflicts: it is not allowed to have an attribute and
     * a method with the same name. */

    std::string _name;
    shield _shield;
    sword _sword;

    /* To mark further attributes and methods as accessible to the
     * outside world, we use the label ‹public›, like this: */
public:

    /* Methods are declared just like functions, the only immediate
     * difference being that this is done inside a class. And the
     * odd ‹const› keyword at the end of the signature. This ‹const›
     * tells the compiler that the method does not change the object
     * in any way when it is called (again, this is enforced by the
     * compiler). */

    bool wields_heavy_sword() const
    {
        return sword_is_heavy( _sword );
    }

    /* An example of non-‹const› method would be the following,
     * which causes the hero to wield a sword given by the argument.
     * The method assigns into one of the attributes, which
     * obviously changes the object, and hence cannot be marked
     * ‹const›. */

    void wield( const sword &s )
    {
        _sword = s;
    }

    /* Finally, we will add a «constructor»: a special kind of
     * method which is called automatically by the compiler whenever
     * a value of type ‹hero› is created, e.g. by declaring a local
     * variable. The constructor's name is the name of the class,
     * and it has no return type, bit it can have arguments. Unlike
     * standard functions (and standard methods), constructors have
     * an «initialization section», which can initialize attributes,
     * e.g. by passing arguments to «their» constructors. When the
     * body of the constructor is entered, all the attributes will
     * have been already constructed. The initialization section
     * starts with a colon, and is followed by a list of expressions
     * of the form ‹attribute( argument list )›. */

    hero( std::string name )
        : _name( name )
    {}
};

/* That is quite enough for now. Let us look at a few examples of
 * code using the above types. */

int main() /* demo */
{
    sword katana = { "Katana", 10, 17 };
    hero protagonist( "Hiro Protagonist" );
    protagonist.wield( katana );
    assert( !protagonist.wields_heavy_sword() );
}

/* ¹ After all, we could just use void pointers, remember how big
 *   the data is and which attribute is stored at which offset.
 *   There is a good reason why nobody writes serious programs in
 *   this style.
 *
 * ² In fact, ‹struct› and ‹class› are essentially the same thing,
 *   and only differ in minor syntactic details. Nonetheless, we
 *   will usually write ‹struct› for plain record types (without
 *   methods) and ‹class› for actual classes.  */
