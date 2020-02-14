<?php




$spec_args;
//loading stdin
while($text = fgets(STDIN))
{
    //at least 3 same symbols
    echo $text;
    if(preg_match('/^(.)\1\1.+/', $text) == 1)
    {
        $spec_args = get_args($text);
        if(count($spec_args) > 1) preg_replace("/$spec_args[0]/", $spec_args[1], $text);
        echo $text;
    }   
}

function replace_replac($replace, $replacement, $text)
{
    preg_replace("/$spec_args[0]/", $spec_args[1], $text);
}

function get_args($par)
{
    if(preg_match('/\[.+\]/', $par, $string) == 1)
    {
        preg_match('/.+=/', $string[0], $string2);
        preg_match('/=.+/', $string[0], $string3);
        $string2_len = mb_strlen( $string2[0]);
        $string3_len = mb_strlen( $string3[0]);
        //what we should substitute
        $string2 = substr($string2[0], 1, $string2_len-2);
        //by what we should substitute
        $string3 = substr($string3[0], 1, $string3_len-2);
        $returner = array( 0 => $string2, 1 => $string3);
        return $returner;
    }
    $returner = array( 0 => "nothing");
    return $returner;
}


?>
