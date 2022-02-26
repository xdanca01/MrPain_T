def encrypt(text, rails):
    res = ""

    for i in range( 1, rails + 1 ):
        j = 0
        res += text[ i - 1 ]
        next_i = False

        while not next_i:
            lines_until_up = None
            lines_until_down = None

            if i % rails != 0: # (==0) last row, no down
                lines_until_down = rails - i
            if i % rails != 1: # (==1) first row, no up
                lines_until_up = i - 1
            for shift in [ lines_until_down, lines_until_up ]:
                if shift is not None:
                    j += shift * 2
                    if i + j - 1 >= len( text ):
                        next_i = True
                        break
                    res += text[ i + j - 1 ]
    return res


def decrypt(text, rails):
    switches, rest = divmod( len( text ) - 1, rails - 1 )
    first_row_len = switches // 2 + 1

    rows = [ text[ 0 : first_row_len ] ]

    i = first_row_len
    while i < len( text ):
        mid_row = ""
        if len( text ) - i < switches: # last row
            rows.append( text[ i : ] )
            break
        for j in range( switches ):
            mid_row += text[ i ]
            i += 1
        if rest > 0:
            mid_row += text[ i ]
            i += 1
            rest -= 1
        rows.append( mid_row )

    res = ""
    while any( rows ):
        for i in list( range( 0, len( rows ) ) ) + \
                 list( range( len( rows ) - 2, 0, -1 ) ):
            if len( rows[ i ] ) == 0:
                break
            res += rows[ i ][ 0 ]
            rows[ i ] = rows[ i ][ 1 : ]
    return res
