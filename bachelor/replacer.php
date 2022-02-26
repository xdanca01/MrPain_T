<?php
$file = fopen("lists.txt", "r");
$spec_args;
//loading stdin
$text = fgets(STDIN);
$list = fgets($file);
if( preg_match('/^\[.*\]+/', $text) == 0)
{
    fprintf(STDERR, "WARNING:On first line must be [] with user defined replacables or just empty brackets\n");
    fclose($file);
    exit(1);
}
else
{
    $length = strlen($text);
    $text = substr($text, 1, $length-3);
    $text = str_replace("=", ' ', $text);
    //odd args are words to be replaced, even args are words to replace odd args
    $arggs = explode(" ", $text);
}
$counter = count($arggs);
//get next line
while($text = fgets(STDIN))
{
    //go through all args
    for($i = 0; $i < $counter; $i = $i + 2)
    {
        $ar = $arggs[$i+1];
        //if arg is in list
        if(preg_match("/$ar/", $list) == 1)
        {
            $IDK = fgets($file);
            echo "IDK $IDK";
            $IDK = fgets($file);
            while(!preg_match("/$ar/", $IDK))
            {
                if(!$IDK = fgets($file))
                {
                    fprintf(STDERR, "ERROR: replacable: \"$ar\" not found in: \"lists.txt\"\n");
                    exit(2);
                }
            }
            $IDK = fgets($file);
            $close = $file;
            $file = fopen("lists.txt", "r");
            fclose($close);
            $IDK = str_replace(' ', '', $IDK);
            $text = str_replace($arggs[$i], $IDK, $text);
        }
        else $text = str_replace($arggs[$i], $arggs[$i+1], $text);
    }
    echo $text;
}
fclose($file);
?>

