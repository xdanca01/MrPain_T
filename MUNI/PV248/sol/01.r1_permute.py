from math import log,floor
from itertools import permutations, dropwhile

def int_to_list( number, base ):
    r = []
    while number:
        r.append( number % base )
        number //= base
    return r

def unique( lists ):
    return list( set( lists ) )

def list_to_int( list_, base ):
    res = 0
    for i in range(len(list_)):
        res += list_[i] * ( base ** (len(list_)-i-1))
    return res

def permute_digits( n, b ):
    perms = list( permutations( int_to_list( n, b ) ) )
    return unique( map( lambda x : list_to_int( x, b ), perms ) )
