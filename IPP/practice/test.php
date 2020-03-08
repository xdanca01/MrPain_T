<?php

$HTML_template = "./template.html";
$HTML_temp = "./results.html";
$longoptions = array("directory::","parse-script::","int-script::","jexamxml::","help","recursive",
"parse-only","int-only");
$options = getopt("",$longoptions);
$dirpath = "./";
$recursive = FALSE;
$parse_script = "./parse.php";
$int_script = "./interpret.py";
$parse_only = FALSE;
$int_only = FALSE;
$jexamxml = "/pub/courses/ipp/jexamxml/jexamxml.jar";

    //set variables of test.php
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
    }
    if(array_key_exists("recursive",$options) == TRUE) $recursive = TRUE;
    if(array_key_exists("parse-only",$options) == TRUE)
    {
        $parse_only = TRUE;
    }
    if(array_key_exists("int-only",$options) == TRUE)
    {
        if(strcmp($parse_script,"parse.php") || $parse_only == TRUE)exit(10);
        $int_only = TRUE;
    }
    if(array_key_exists("int-script",$options) == TRUE)
    {
        $int_script = $options["int-script"];
        if($parse_only == TRUE) exit(10);
    }
    if(array_key_exists("parse-script",$options) == TRUE)
    {   
        $parse_script = $options["parse-script"];
        if($int_only == TRUE) exit(10);
    }
    if(array_key_exists("jexamxml",$options) == TRUE)
    {
        $jexamxml = $options["jexamxml"];
    }

    if($recursive == FALSE)
    {
        exec("find -maxdepth 1 | grep \.src", $src_files);
    }
    else
    {
        exec("find | grep \.src", $src_files);
    }

    //testing existence of files/directories
    if(is_dir($dirpath) == false) exit(11);
    if($int_only == FALSE) if(file_exists($parse_script) == FALSE) exit(11);
    if($parse_only == FALSE) if(file_exists($int_script) == FALSE) exit(11);
    if(file_exists($jexamxml) == FALSE ) exit(11);
    //HTML template check
    if(file_exists($HTML_template) == FALSE) exit(99);
    exec("cp $HTML_template $HTML_temp", $ignore);

    //test all selected src files
    foreach($src_files as $src_file)
    {
        $in_file = preg_replace('/src$/', "in", $src_file);
        $out_file = preg_replace('/src$/', "out", $src_file);
        $rc_file = preg_replace('/src$/', "rc", $src_file);
        $xml_file = preg_replace('/src$/', "xml", $src_file);
        $test_name = preg_replace('/\.src$/', "", $src_file);
        
        if(file_exists($in_file) == FALSE) exec("echo > $in_file");
        if(file_exists($out_file) == FALSE) exec("echo > $out_file");
        if(file_exists($rc_file) == FALSE) exec("echo 0 > $rc_file");
        
        
        if($int_only == FALSE && $parse_only == FALSE)
        {
            exec("php $parse_script <$src_file >$xml_file", $parse_out, $parse_rc);
            if($parse_rc != 0)
            {
                //TODO fail generovani
            }
            else
            {
                exec("$int_script --input=$in_file --source=$xml_file", $int_out, $int_rc);
                exec("grep -e '^0$' <$rc_file", $return_code_rc, $rc_file_zero);
                if($rc_file_zero == $parse_rc)
                {
                    //TODO gen pass
                }
                else
                {
                    //TODO fail gen
                }
            }
            exec("rm $xml_file",$ignoruju);
        }
        elseif($parse_only == TRUE)
        {
            exec("php $parse_script <$src_file >$xml_file", $parse_out, $parse_rc);
            $xml_result = is_xml_ok($xml_file,$out_file);
            if($xml_result == 1)
            {
                gen_HTML("FAIL", $test_name, $parse_rc, "Different");
            }
            elseif($xml_result == 0)
            {
                gen_HTML("PASS", $test_name, $parse_rc, "Same");
            }
            else exit(99);
            exec("rm $xml_file",$ignoruju);
        }
        elseif($int_only == TRUE)
        {
            exec("$int_script --input=$in_file --source=$src_file", $int_out, $int_rc);
        }
    }
function is_xml_ok($file1, $file2)
{
    global $jexamxml;
    exec("java -jar $jexamxml $file1 $file2",$neco,$rc_code);
    return $rc_code;
}
function gen_HTML($status, $jmeno, $return_code, $DIFF)
{
    echo "here";
    global $HTML_temp;
    exec("sed -i 's/#replace_me/<tr>\n#replace_me/' $HTML_temp");
    exec("sed -i 's/#replace_me/<td>$jmeno</td>\n#replace_me/' $HTML_temp");
    exec("sed -i 's/#replace_me/<td>$return_code</td>\n#replace_me/' $HTML_temp");
    exec("sed -i 's/#replace_me/<td>$DIFF</td>\n#replace_me/' $HTML_temp");
    exec("sed -i 's/#replace_me/<td>$status</td>\n#replace_me/' $HTML_temp");
    exec("sed -i 's/#replace_me/</tr>\n#replace_me/' $HTML_temp");
}

$out = fopen($HTML_temp, "r");
while($vystup = fgets($out))
{
    echo $vystup;
}
fclose($out);
exec("rm $HTML_temp", $ignore);
exit(0);
?>
