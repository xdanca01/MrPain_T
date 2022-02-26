def radix_sort( strings, idx = 0 ):
    buckets = {}
    result = []
    for s in strings:
        if len( s ) > idx:
            buckets.setdefault( s[ idx ], [] ).append( s )
        else:
            result.append( s )
    for _, b in sorted( buckets.items(), key = lambda x: x[ 0 ] ):
        result.extend( radix_sort( b, idx + 1 ) )
    return result
