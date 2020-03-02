<?php


$longoptions = array("directory::","parse-script::","int-script::","jexamxml::","help","recursive",
"parse-only","int-only");
$options = getopt("",$longoptions);

for ($i = 0;$i < $argc;++$i)
{
    if(array_key_exists("help",$options) == TRUE)
    {
        if($argc != 2) exit(10);
        echo "help for test.php\n";
        echo "args: --help, -h [THIS HELP]\n";
        echo "written by Petr Dancak\n";
        exit(0);
    }
}
exit(0);
?>
