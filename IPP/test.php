<?php


$longoptions = array("directory::","parse-script::","int-script::","jexamxml::","help","recursive",
"parse-only","int-only");
$options = getopt("",$longoptions);
$dirpath = "./";
$recursive = FALSE;
$parse_script = "parse.php";
$int_script = "interpret.py";
$parse_only = FALSE;
$int_only = FALSE;
$jexamxml = "/pub/courses/ipp/jexamxml/jexamxml.jar";

    if(array_key_exists("help",$options) == TRUE)
    {
        if($argc != 2) exit(10);
        echo "help for test.php\n";
        echo "args: --help, -h [THIS HELP]\n";
        echo "written by Petr Dancak\n";
        exit(0);
    }
    if(array_key_exists("directory",$options) == TRUE)
    {
        $dirpath = $options["directory"];
        if(is_dir($dirpath) == false) exit(11);
    }
    if(array_key_exists("recursive",$options) == TRUE) $recursive = TRUE;
    if(array_key_exists("parse-script",$options) == TRUE)
    {
        $parse_script = $options["parse-script"];
        if(file_exists($parse_script) == FALSE) exit(11);
    }
    if(array_key_exists("int-script",$options) == TRUE)
    {
        $int_script = $options["int-script"];
        if(file_exists($int_script) == FALSE) exit(11);
    }
    if(array_key_exists("parse-only",$options) == TRUE)
    {
        if(strcmp($int_script,"interpret.py"))exit(10);
        $parse_only = TRUE;
    }
    if(array_key_exists("int-only",$options) == TRUE)
    {
        if(strcmp($parse_script,"parse.php") || $parse_only == TRUE)exit(10);
        $int_only = TRUE;
    }
    if(array_key_exists("jexamxml",$options) == TRUE)
    {
        $jexamxml = $options["jexamxml"];
        if(file_exists($jexamxml) == FALSE ) exit(11);
    }

exit(0);
?>
