from __future__ import annotations
from typing import Optional, Protocol, Any, TypeVar, Set, Generic
from math import log2, floor

# Since a search tree must be ordered, we need to be able to compare
# (order) the values stored in the tree. For that, we need a type
# variable that is constrained to support order comparison
# operators:

class SupportsLessThan( Protocol ):
    def __lt__( self, other: Any ) -> bool: ...

T = TypeVar( 'T', bound = SupportsLessThan )

# ----- %< ----- %< -----

# Implement the DSW (Day, Stout and Warren) algorithm for
# rebalancing binary search trees. The algorithm is ‘in place’ –
# implement it as a procedure that modifies the input tree. You will
# find suitable pseudocode on Wikipedia, for instance.
#
# The constructor of ‹Node› should accept a single parameter (the
# value).

def create_label(index : int) -> str:
    label = ''
    while index > 0:
        label += chr(ord('A') - 1 + index % 27) if index % 27 != 0 else '0'
        index //= 27
    return label[::-1]

def bfs_label(root: Node[ T ]) -> None:
    queue = [root]
    index = 1
    while queue:
        node = queue.pop(0)
        node._label = create_label(index)
        index += 1
        if node.left is not None:
            queue.append(node.left)
        if node.right is not None:
            queue.append(node.right)

def to_graphviz(root: Node[ T ]) -> None:
    bfs_label(root)
    print('digraph G {')
    queue = [root]
    while queue:
        node = queue.pop(0)
        if node.left is not None:
            print(4*' ' + f'"{node._label}:{node.value}" -> "{node.left._label}:{node.left.value}"')
            queue.append(node.left)
        if node.right is not None:
            print(4*' ' + f'"{node._label}:{node.value}" -> "{node.right._label}:{node.right.value}"')
            queue.append(node.right)
    print('}')

class Node:
    def __init__(self, value):
        self.left = None
        self.right = None
        self.value = value
    def in_order(self, my_list):
        if self.left is None:
            my_list.append(self)
        else:
            self.left.in_order(my_list)
            my_list.append(self)
        if self.right is not None:
            self.right.in_order(my_list)
        return

class Tree(Generic[T]):
    def __init__(self, root):
        self.root = root
    def backbone(self):
        my_list = []
        self.root.in_order(my_list)
        for i in range(len(my_list) - 1):
            my_list[i].right = my_list[i + 1]
            my_list[i].left = None
        self.root = my_list[0]
        return len(my_list)
    def list_to_tree(self, size):
        leaves = size + 1 - pow(2, floor(log2(size+1)))
        compress(self.root, leaves)
        size = floor(size - leaves)
        while size > 1:
            compress(self.root, floor(size / 2))
            size = floor(size / 2)

def tree_to_vine(root):
    rest = root.right
    size = 1
    while rest is not None:
        if rest.left is None:
            root = rest
            rest = rest.right
        else:
            temp = rest.left
            rest.left = temp.right
            temp.right = rest
            rest = temp
            root.right = temp
        size += 1
    return size

def vine_to_tree(root, size):
    nback = size - 1
    next = root
    l = log2(size+1)
    m = pow(2, l) - 1
    if(size - m != 0):
        next = compress2(next, size - m)
    while m > 1:
        m = floor(m / 2)
        next = compress2(next, m)

def compress2(root, count):
    flag = 0
    next = root
    scanner = root
    i = 0
    while i < count:
        child = scanner.right
        scanner.right = child.right
        if flag == 0:
            next = scanner
            flag += 1
        scanner = scanner.right
        child.right = scanner.left
        scanner.left = child
    return next

def compress(root, size):
    scanner = root
    for i in range(size):
        to_graphviz(root)
        child = scanner.right
        scanner.right = child.right
        scanner = scanner.right
        child.right = scanner.left
        scanner.left = child

def rotate_left(root, grand, parent, child):
    if grand is not None:
        grand.right = child
    else:
        root = child
    parent.right = child.left
    child.left = parent

def dsw( tree ): # add a type signature here
    if tree is not None:
        #size = tree_to_vine(tree.root)
        #to_graphviz(tree.root)
        size = tree.backbone()
        tree.list_to_tree(size)
    return


# ----- >% ----- >% -----

def test_random() -> None:
    for i in range( 200 ):
        t, vals = random_tree( 12, -1000, 1000 )
        dsw( t )
        ok, _, _ = check_ordering( t.root, 0 )
        lb, ub = check_balance( t.root, 0 )
        vals = check_values( t.root, vals )
        assert not vals, vals
        assert ok
        assert ub - lb <= 10


def check_values( node: Optional[ Node[ int ] ],
                  vals: Set[ int ] ) -> Set[ int ]:
    if node is not None:
        assert node.value in vals
        vals = check_values( node.left,  vals - { node.value } )
        return check_values( node.right, vals )
    else:
        return vals


def check_ordering( node: Optional[ Node[ T ] ], bound: T ) \
        -> tuple[ bool, T, T ]:

    if node is None:
        return ( True, bound, bound )

    l_ok, l_min, l_max = check_ordering( node.left,  node.value )
    r_ok, r_min, r_max = check_ordering( node.right, node.value )

    this_ok = l_ok and r_ok and \
            ( node.value == l_max or node.value > l_max ) and \
            ( node.value == r_max or node.value < r_max )

    return ( this_ok, l_min, r_max )


def check_balance( node: Optional[ Node[ T ] ], depth: int ) \
        -> tuple[ int, int ]:

    if node is None:
        return ( depth, depth )

    lb_l, ub_l = check_balance( node.left,  depth + 1 )
    lb_r, ub_r = check_balance( node.right, depth + 1 )

    return ( min( lb_l, lb_r ), max( ub_l, ub_r ) )


def random_subtree( depth: int, lb: int, ub: int ) \
        -> tuple[ Optional[ Node[ int ] ], Set[ int ] ]:

    if not depth or lb >= ub:
        return None, set()

    from random import randint
    val = randint( lb, ub )
    node : Node[ int ] = Node( val )

    skip_l = randint( 1, max( 1, depth // 2 ) )
    skip_r = randint( 1, max( 1, depth // 2 ) )

    node.left,  l_vals = random_subtree( depth - skip_l, lb, val - 1 )
    node.right, r_vals = random_subtree( depth - skip_r, val + 1, ub )
    return node, l_vals | r_vals | { val }


def random_tree( depth: int, lb: int, ub: int ) \
        -> tuple[ Tree[ int ], Set[ int ] ]:
    root, vals = random_subtree( depth, lb, ub )
    return Tree( root ), vals


if __name__ == '__main__':
    test_random()
