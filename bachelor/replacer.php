<?php
$file = fopen("lists.txt", "r");
$spec_args;
//loading stdin
$text = fgets(STDIN);
$list = fgets($file);
if( preg_match('/^\[.*\]+/', $text) == 0)
{
    fprintf(STDERR, "WARNING:Replacables values not found\n");
    fclose($file);
    exit(1);
}
else
{
    $length = strlen($text);
    $text = substr($text, 1, $length-3);
    $text = str_replace("=", ' ', $text);
    $arggs = explode(" ", $text);
}
$counter = count($arggs);
while($text = fgets(STDIN))
{
    for($i = 0; $i < $counter; $i = $i + 2)
    {
        $ar = $arggs[$i+1];
        if(preg_match("/$ar/", $list) == 1)
        {
            $IDK = fgets($file);
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

