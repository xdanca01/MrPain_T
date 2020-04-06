<?php

$HTML_template = "./template.html";
$HTML_temp = "./results.html";
$HTML_header = "./test_php_files/HTML_header.html";
$HTML_center = "./test_php_files/HTML_center.html";
$HTML_bottom = "./test_php_files/HTML_bottom.html";
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
$counter = 0;
$FAIL_counter = 0;
$PASS_counter = 0;
$options_counter = 0;

    //set variables of test.php
    if(array_key_exists("help",$options) == TRUE)
    {
        if($argc != 2) exit(10);
        echo "help for test.php\n";
	echo "args: --help, -h [THIS HELP]\n";
	echo "	--directory\n";
	echo "	--recursive\n";
	echo "	--parse-only\n";
	echo "	--int-only\n";
	echo "	--int-script\n";
	echo "	--parse-script\n";
	echo "	--jexamxml\n";
        echo "written by Petr Dancak\n";
        exit(0);
    }
    if(array_key_exists("directory",$options) == TRUE)
    {
        $options_counter += 1;
        $dirpath = $options["directory"];
    }
    if(array_key_exists("recursive",$options) == TRUE)
    {
        $recursive = TRUE;
        $options_counter += 1;
    }
    if(array_key_exists("parse-only",$options) == TRUE)
    {
        $options_counter += 1;
        $parse_only = TRUE;
    }
    if(array_key_exists("int-only",$options) == TRUE)
    {
        if(strcmp($parse_script,"parse.php") || $parse_only == TRUE)exit(10);
        $int_only = TRUE;
        $options_counter += 1;
    }
    if(array_key_exists("int-script",$options) == TRUE)
    {
        $int_script = $options["int-script"];
        if($parse_only == TRUE) exit(10);
        $options_counter += 1;
    }
    if(array_key_exists("parse-script",$options) == TRUE)
    {   
        $parse_script = $options["parse-script"];
        if($int_only == TRUE) exit(10);
        $options_counter += 1;
    }
    if(array_key_exists("jexamxml",$options) == TRUE)
    {
        $jexamxml = $options["jexamxml"];
        $options_counter += 1;
    }

    if($recursive == FALSE)
    {
        exec("find -maxdepth 1 $dirpath | grep \.src", $src_files);
    }
    else
    {
        exec("find $dirpath | grep \.src", $src_files);
    }



    //testing existence of files/directories
    if(is_dir($dirpath) == false) exit(11);
    if($int_only == FALSE) if(file_exists($parse_script) == FALSE) exit(11);
    if($parse_only == FALSE) if(file_exists($int_script) == FALSE) exit(11);
    if(file_exists($jexamxml) == FALSE ) exit(11);

    //HTML header template to STDOUT
    $handle = fopen($HTML_header, "r");
    while($vypis = fgets($handle)) echo $vypis;
    fclose($handle);

    //test all selected src files
    foreach($src_files as $src_file)
    {
        $in_file = preg_replace('/src$/', "in", $src_file);
        $out_file = preg_replace('/src$/', "out", $src_file);
        $rc_file = preg_replace('/src$/', "rc", $src_file);
        $xml_file = preg_replace('/src$/', "xml", $src_file);
        $test_name = preg_replace('/\.src$/', "", $src_file);
        $bool_in = FALSE;
        $bool_out = FALSE;
        $bool_rc = FALSE;



        if(file_exists($in_file) == FALSE) 
        {
            exec("echo > $in_file");
            $bool_in = TRUE;
        }
        if(file_exists($out_file) == FALSE)
        {
            exec("echo > $out_file");
            $bool_out = TRUE;
        }
        if(file_exists($rc_file) == FALSE)
        {
            exec("echo 0 > $rc_file");
            $bool_rc = TRUE;
        }

        $handle = fopen($rc_file, "r");
        $rc_file_zero = fgets($handle);
        $rc_file_zero = (int)$rc_file_zero;
        fclose($handle);
        $counter += 1;


        #both
        if($int_only == FALSE && $parse_only == FALSE)
        {
            exec("php $parse_script <$src_file >$xml_file", $parse_out, $parse_rc);
            if($parse_rc != 0)
            {
                if($parse_rc != $rc_file_zero)
                {
                    $FAIL_counter += 1;
                    gen_HTML("FAIL", $test_name, $parse_rc, "NONE", $rc_file_zero);
                }
                else
                {
                    $PASS_counter += 1;
                    gen_HTML("PASS", $test_name, $parse_rc, "NONE", $rc_file_zero);
                }
            }
            else
            {
                exec("$int_script --input=$in_file --source=$xml_file ", $int_out, $int_rc);
                if($rc_file_zero == $int_rc)
                {
                    if($int_rc == 0)
                    {
                        print($int_out);
                        exec("diff $out_file - <<<$int_out", $diff_out, $diff_rc);
                        if($diff_rc == 0)
                        {
                            $PASS_counter += 1;
                            gen_HTML("PASS", $test_name, $int_rc, "SAME", $rc_file_zero);
                        }
                        elseif($diff_rc == 1)
                        {
                            $FAIL_counter += 1;
                            gen_HTML("FAIL", $test_name, $int_rc, "DIFFERENT", $rc_file_zero);
                        }
                        else gen_HTML("FAIL", $test_name, $int_rc, "ERROR", $rc_file_zero);
                    }
                    else
                    {
                        $PASS_counter += 1;
                        gen_HTML("PASS", $test_name, $int_rc, "NONE", $rc_file_zero);
                    }
                }
                else
                {
                    $FAIL_counter += 1;
                    gen_HTML("FAIL", $test_name, $int_rc, "NONE", $rc_file_zero);
                }
            }
            exec("rm $xml_file",$ignoruju);
        }
        elseif($parse_only == TRUE)
        {
            exec("php7.4 $parse_script <$src_file >$xml_file", $parse_out, $parse_rc);
            $xml_result = is_xml_ok($xml_file,$out_file);
            if($parse_rc == $rc_file_zero)
            {
                if($xml_result == 0 && $parse_rc == 0)
                {
                    $PASS_counter += 1;
                    gen_HTML("PASS", $test_name, $parse_rc, "Same", $rc_file_zero);
                }
                elseif($parse_rc == 0 && $xml_result != 0)
                {
                    $FAIL_counter += 1;
                    gen_HTML("FAIL", $test_name, $parse_rc, "Different", $rc_file_zero);
                }
                elseif($parse_rc != 0)
                {
                    $PASS_counter += 1;
                    gen_HTML("PASS", $test_name, $parse_rc, "NONE", $rc_file_zero);
                }
                else
                {
                    $FAIL_counter += 1;
                    gen_HTML("FAIL", $test_name, $parse_rc, "Different", $rc_file_zero);
                }
            }
            else
            {
                $FAIL_counter += 1;
                gen_HTML("FAIL", $test_name, $parse_rc, "NONE", $rc_file_zero);
            }
            exec("rm $xml_file",$ignoruju);
        }
        elseif($int_only == TRUE)
        {
            $my_out = "my1out.out";
            exec("$int_script --input=$in_file --source=$src_file >$my_out", $int_out, $int_rc);
            if($rc_file_zero == $int_rc)
            {
                exec("diff $my_out $out_file", $ignore, $diff_rc);
                if($diff_rc == 0)
                {
                    gen_HTML("PASS", $test_name, $int_rc, "Same", $rc_file_zero);
                }
                else
                {
                    gen_HTML("Fail", $test_name, $int_rc, "Different", $rc_file_zero);
                }
            }
            else
            {
                gen_HTML("Fail", $test_name, $int_rc, "NONE", $rc_file_zero);
            }
        }
        if ($bool_in == TRUE) exec("rm $in_file",$ignoruju);
        if ($bool_rc == TRUE) exec("rm $rc_file",$ignoruju);
        if ($bool_out == TRUE) exec("rm $out_file",$ignoruju);
    }
function is_xml_ok($file1, $file2)
{
    global $jexamxml;
    exec("java -jar $jexamxml $file1 $file2",$neco,$rc_code);
    return $rc_code;
}
function gen_HTML($status, $jmeno, $return_code, $DIFF, $expected_rc)
{
    echo "<tr>\n";
    echo "<td>$jmeno</td>\n";
    echo "<td>$return_code</td>\n";
    echo "<td>$expected_rc</rd>\n";
    echo "<td>$DIFF</td>\n";
    echo "<td>$status</td>\n";
    echo "</tr>\n";
}
function gen_second_table()
{
    global $HTML_temp;
    global $counter;
    global $FAIL_counter;
    global $PASS_counter;
    echo "<tr>\n";
    echo "<td>$counter</td>\n";
    echo "<td>$PASS_counter</td>\n";
    echo "<td>$FAIL_counter</td>\n";
    echo "</tr>\n";
}


$handle = fopen($HTML_center, "r");
while($vystup = fgets($handle)) echo $vystup;
fclose($handle);

gen_second_table();

$handle = fopen($HTML_bottom, "r");
while($vystup = fgets($handle)) echo $vystup;
fclose($handle);

exit(0);
?>
