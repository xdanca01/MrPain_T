from p2_ts3norm import *
import re

# ----- %< ----- %< -----

# At this point, we have a structure made of ‹dict›, ‹list›,
# ‹Template›, ‹Document› and ‹int› instances. The lists and maps can
# be arbitrarily nested. Within templates, the substitutions give
# dot-separated paths into this tree-like structure. If the
# top-level object is a map, the first component of a path is a
# string which matches a key of that map. The first component is
# then chopped off, the value corresponding to the matched key is
# picked as a new root and the process is repeated recursively. If
# the current root is a list and the path component is a number, the
# number is used as an index into the list.

# If a ‹dict› meets a number in the path (we will only deal with
# string keys), or a ‹list› meets a string, treat this as a
# precondition violation – fail an ‹assert› – and let someone else
# deal with the problem later.

# The ‹${path}› substitution performs «scalar rendering», while
# ‹#{path}› substitution performs «composite rendering». Scalar
# rendering resolves the path to an object, and depending on its
# type, performs the following:
#
#  • ‹Document› → replace the ‹${…}› with the text of the document;
#    the pasted text is excluded from further processing,
#  • ‹Template› → the ‹${…}› is replaced with the text of the
#    template; occurrences of ‹${…}› and ‹#{…}› within the pasted text
#    are further processed,
#  • ‹int› → it is formatted as a decimal number and the resulting
#    string replaces the ‹${…}›,
#  • ‹list› → the length of the list is formatted as if it was an
#    ‹int›, and finally,
#  • ‹dict› → ‹.default› is appended to the path and the
#    substitution is retried.

# Composite rendering using ‹#{…}› is similar, but:
#
#  • a ‹dict› is rendered as a comma-separated (with a space) list
#    of its values, after the keys are sorted alphabetically, where
#    each value is rendered «as a scalar»,
#  • a ‹list› is likewise rendered as a comma-separated list of its
#    values as scalars,
#  • everything else is an error: again, treat this as a failed
#    precondition, fail an ‹assert›, and leave it to someone else to
#    fix later.

# The top-level entity passed to ‹ts3_render› must always be a
# ‹dict›.  The starting template is expected to be in the key
# ‹$template› of that ‹dict›. Remember that ‹##{…}› and ‹$${…}› must
# remain untouched. If you encounter nested templates while parsing
# the path, e.g. ‹${abc${d}}›, give up (again via a failed
# assertion); however, see also exercise ‹r3›).

def ts3_render( tree: OutputDoc ) -> str:
    my_str = (tree["$template"].text  + '.')[:-1]
    for i in tree:
        if i != "$template":
            if isinstance(i, (type(Document), int, ))

# ----- >% ----- >% -----

def test_scalar_individual() -> None:

    template = Template( "Here is input: ${input}" )

    t : OutputDoc
    t = { '$template': template, 'input': Document( "blahblah" ) }
    assert ts3_render( t ) == "Here is input: blahblah"

    t = { '$template': template, 'input': Document( "blah ${t}" ) }
    assert ts3_render( t ) == "Here is input: blah ${t}"

    t = { '$template': template, 'input': [ 1, 2, 3 ] }
    assert ts3_render( t ) == "Here is input: 3"

    t = { '$template': template,
          'input': { 'a': 7, 'default': Document( "abc}" ) } }
    assert ts3_render( t ) == "Here is input: abc}"

    t = { '$template': template, 'input': -22 }
    assert ts3_render( t ) == "Here is input: -22"

    t = { '$template': template,
          'input': Template( "would need ${more.input}" ),
          'more': { 'input': Document( "hello" ),
                    'output': Document( "bye" ) } }
    assert ts3_render( t ) == "Here is input: would need hello"


def test_composite_individual() -> None:

    template = Template( "List: #{items} and a dog." )
    t : OutputDoc
    t = { '$template': template,
          'items': list( map( Document,
                              [ 'carrot', 'cat', 'potato' ] ) ) }
    assert ts3_render( t ) == "List: carrot, cat, potato and a dog."

    # dict, sort(!)
    t = { '$template': template,
          'items': { 'c': Document( 'foo' ), 'a': 7,
                     'd': Template( "${foo}" ),
          't': -1, }, 'foo': [ Document( '1' ) ] }
    assert ts3_render( t ) == "List: 7, foo, 1, -1 and a dog."


def test_template() -> None:

    template = Template( "Print ${name.idea} and" +
                         " ${name.group.3.people}.." )

    # encountering list in path resolution means an index
    people : OutputDoc = { 'people': [ Document( 'Bernard' ),
                                       Document( 'Ann' ) ] }
    group : OutputDoc = [ 0, 1, 2, people ]
    t : OutputDoc
    t = { '$template': template,
          'name': { 'idea': Document( 'fireflies' ),
                    'group': group } }
    assert ts3_render( t ) == "Print fireflies and 2.."


def test_escape() -> None:

    template = Template( "${header}: show me ${person.name} " +
                         "and ${person.age} of #{persons} " +
                         "but not $${ppl}" )

    t : OutputDoc
    t = { '$template': template,
          'header': Document( "automatic" ),
          'person': { 'name': Document( "Villa" ), 'age': 17 },
          'persons': [ Document( 'Villa' ), Document( 'Serrat' ) ] }
    t_orig = t.copy()

    expect = "automatic: show me Villa and 17 of Villa, " + \
             "Serrat but not $${ppl}"

    assert ts3_render( t ) == expect
    assert t == t_orig


def test_composite() -> None:

    # composite within composite
    template = Template( "Fields: #{fields}!#}" )
    t : OutputDoc
    t = { '$template': template,
          'fields': [ Document( 'CS' ), Document( 'Law' ),
                      Template( 'Others: #{others}' ) ],
          'others': { 'field2': Document( 'Art' ),
                      'field3': Document( 'Archery' ),
                      'field1': Document( 'Plants' ) } }

    assert ts3_render( t ) == "Fields: CS, Law, Others: Plants, Art, Archery!#}"

def test_errors() -> None:
    t : OutputDoc

    # dict meets a number in the path
    t = { '$template': Template( "${path.0}" ),
          'path': { 'foo': 2 } }
    assert_throws( t )

    # list meets a string in the path
    t = { '$template': Template( "${path.a}" ),
          'path': [ Document( 'a' ) ] }
    assert_throws( t )

    # dict end of scalar, no 'default' key
    t = { '$template': Template( "a ${path}" ),
          'path': { 'not-default': 1 } }
    assert_throws( t )

    # composite meets int/Template/Document
    t = { '$template': Template( "#{comp}" ), 'comp': 7 }
    assert_throws( t )

    t = { '$template': Template( "#{comp}" ),
          'comp': Document( "$doc" ) }
    assert_throws( t )

    t = { '$template': Template( "#{comp}" ),
          'comp': Template( "foo" ) }
    assert_throws( t )

    # nested templates
    t = { '$template': Template( "#{ab${t}c}" ),
          'ab': Document( "wrong" ) }
    assert_throws( t )

def assert_throws( t: OutputDoc ) -> None:
    try:
        ts3_render( t )
    except AssertionError:
        return

    raise AssertionError( "expected a failed assertion" )

if __name__ == "__main__":
    test_scalar_individual()
    test_composite_individual()
    test_template()
    test_escape()
    test_composite()
    test_errors()
