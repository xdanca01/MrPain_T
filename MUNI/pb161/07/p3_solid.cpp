#include <cassert>
#include <vector>

/* In this exercise, we will focus on building objects that have
 * optional data attached to them. The idea is that if the optional
 * data is sufficiently big and there are enough instances which do
 * not use this data, it makes sense to split the object into two.
 * Of course, logically (in the interface), the object should still
 * act like a single unit. */

/* To make testing easier, we declare a global counter of matrices.
 * It will be adjusted by the constructor and destructor of
 * ‹transform_matrix› below. This is «not» a design pattern that you
 * should normally use (but it is okay in a small demo). */

int matrix_counter = 0;

/* The two pieces will be, in this case, a general description of a
 * 3D object (a solid) and a 3D transformation matrix with 9 entries
 * (3 rows and 3 columns). The matrix is represented by the class
 * declared below. Make the class default-constructible and do not
 * forget to implement the book-keeping for ‹matrix_counter›. The
 * class should store the matrix entries inline (i.e. they should be
 * part of the object, not managed in a separate heap allocation).
 */

struct transform_matrix;

/* We don't know about inheritance yet, but the below class could be
 * considered a «base class» in a simple «inheritance hierarchy»: it
 * will only have properties common to different object types, but
 * will not describe a complete solid in itself. It should have the
 * following methods:
 *
 *  • ‹pos_x›, ‹pos_y› and ‹pos_z› to give the position of the solid
 *  • ‹transform_entry( int r, int c )› gives the entry in the
 *    transformation matrix at row ‹r› and column ‹c›
 *  • ‹transform_set( int r, int c, double v )› sets the
 *    corresponding entry in the transformation matrix
 *  • a constructor which takes 3 arguments of type ‹double› (the x,
 *    y and z position coordinates)
 *
 * The default transformation matrix is the identity matrix (1's on
 * the main diagonal, 0's everywhere else). Memory should only be
 * allocated for the transformation matrix if it changes from the
 * default. */

class solid;

int main()
{
    /* check that the classes do not contain any extraneous data */
    static_assert( sizeof( solid ) <= 3 * sizeof( double ) + sizeof( void * ) );
    static_assert( sizeof( transform_matrix ) == 9 * sizeof( double ) );

    /* check that the book-keeping works as expected */
    {
        transform_matrix x;
        assert( matrix_counter == 1 );
    }
    assert( matrix_counter == 0 );

    auto check_mat = []( const auto &s, std::vector< double > v )
    {
        return s.transform_entry( 0, 0 ) == v[ 0 ] &&
               s.transform_entry( 0, 1 ) == v[ 1 ] &&
               s.transform_entry( 0, 2 ) == v[ 2 ] &&
               s.transform_entry( 1, 0 ) == v[ 3 ] &&
               s.transform_entry( 1, 1 ) == v[ 4 ] &&
               s.transform_entry( 1, 2 ) == v[ 5 ] &&
               s.transform_entry( 2, 0 ) == v[ 6 ] &&
               s.transform_entry( 2, 1 ) == v[ 7 ] &&
               s.transform_entry( 2, 2 ) == v[ 8 ];
    };

    {
        solid s( 0, 1, 2 );
        const solid &cs = s;

        assert( cs.pos_x() == 0 );
        assert( cs.pos_y() == 1 );
        assert( cs.pos_z() == 2 );

        assert( check_mat( s, { 1, 0, 0,
                                0, 1, 0,
                                0, 0, 1 } ) );

        assert( matrix_counter == 0 );

        s.transform_set( 0, 1, 3 );
        s.transform_set( 2, 1, 3 );
        assert( matrix_counter == 1 );

        assert( check_mat( s, { 1, 3, 0,
                                0, 1, 0,
                                0, 3, 1 } ) );

        s.transform_set( 2, 2, 5 );

        assert( check_mat( s, { 1, 3, 0,
                                0, 1, 0,
                                0, 3, 5 } ) );
    }

    assert( matrix_counter == 0 );

    return 0;
}
