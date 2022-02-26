from typing import Optional, TypeVar, Generic, Union
# Implement a linked list with the following operations:
#
#  • ‹append› – add an item at the end
#  • ‹join›   – concatenate 2 lists
#  • ‹shift›  – remove an item from the front and return it
#  • ‹empty›  – is the list empty?
#
# The class should be called ‹Linked› and should have a single type
# parameter (the type of item stored in the list). The ‹join› method
# should re-use nodes of the second list. The second list thus
# becomes empty.

T = TypeVar( 'T')
G = TypeVar('G')

class Node():
    def __init__(self, Data : T) -> None:
        self.data = Data
        self.next : Union[Node, None] = None

    def right(self, item : T) -> None:
        self.next = Node(item)

class Linked(Generic[T]):
    def __init__(self) -> None:
        self.first : Union[Node, None] = None

    def append(self, item : Optional[T]) -> None:
        if self.empty():
            self.first = Node(item)
        else:
            next = self.first
            while next is not None:
                if next.next is None:
                    next.right(item)
                    return
                next = next.next

    def join(self, second : "Linked[T]") -> None:
        while second.empty() == False:
            self.append(second.shift())

    def shift(self) -> Union[T, G]:
        if self.first is None:
            self.first = Node(None)
        my_first : Union[T, G] = self.first.data
        self.first = self.first.next
        return my_first

    def empty(self) -> bool:
        return True if self.first is None else False

def test_elementary() -> None:
    l : Linked[ int ] = Linked()
    assert l.empty()
    l.append( 7 )
    assert not l.empty()
    assert l.shift() == 7
    assert l.empty()

def test_join() -> None:
    l_1 : Linked[ int ] = Linked()
    l_2 : Linked[ int ] = Linked()

    l_1.append( 7 )
    l_2.append( 3 )
    l_2.append( 7 )
    l_1.join( l_2 )

    assert l_2.empty()
    assert l_1.shift() == 7
    assert l_1.shift() == 3
    assert l_1.shift() == 7
    assert l_1.empty()

def test_str() -> None:
    l : Linked[ str ] = Linked()
    assert l.empty()
    l.append( 'hello' )
    l.append( 'world' )
    assert not l.empty()
    assert l.shift() == 'hello'
    assert l.shift() == 'world'
    assert l.empty()

def test_nested() -> None:
    l : Linked[ Linked[ int ] ] = Linked()

    u : Linked[ int ] = Linked()
    u.append( 3 )
    u.append( 4 )

    l.append( u )
    l.append( u )

    u = Linked()
    u.append( 1 )
    l.append( u )

    v = l.shift()
    assert v.shift() == 3
    w = l.shift()
    assert w.shift() == 4
    assert v.empty()

    v = l.shift()
    assert v.shift() == 1
    assert v.empty()
    assert l.empty()

    l.append( v )
    w = l.shift()
    assert w.empty()
    assert l.empty()


if __name__ == '__main__':
    test_elementary()
    test_join()
    test_str()
    test_nested()
