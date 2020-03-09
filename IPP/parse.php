<?php



$radek;
$prvni_radek = 1;
$pole;
$cislo = 0;

if ($argc == 2 && (strcmp($argv[1],"--help") == 0 || strcmp($argv[1],'-h') == 0))
{
    echo "help for parser.php for IPPcode20\n";
    echo "args: --help, -h [THIS HELP]\n";
    echo "written by Petr Dancak\n";
    exit(0);
}
if($argc != 1)
{
    echo "ERROR: unknown option: $argv[1]\n";
    exit(10);
}
$IDK = xmlwriter_open_memory();
xmlwriter_set_indent($IDK, 1);
$IDK2 = xmlwriter_set_indent_string($IDK, ' ');
if($IDK2 == false) exit(99);
//nacitani ze STDIN
while($radek = fgets(STDIN))
{
    if(preg_match('/^#.*/', $radek) == 1)
    {
        continue;
    }
    //odmazavani komentaru
    if(preg_match('/#.*/', $radek) == 1)
    {   
        $radek = preg_replace('/ *#.*/', '', $radek);
    } 
    $pole = preg_split ("/ +/", $radek);
    $rtrn_code = muj_regex();
    if($rtrn_code != 0) return $rtrn_code;
    $prvni_radek = 0;
    $cislo++;
}
xmlwriter_end_element($IDK);
xmlwriter_end_document($IDK);
echo xmlwriter_output_memory($IDK);
//echo "</program\n>";
//bezchybný konec programu
exit(0);


//kontrola radku na regex
function muj_regex()
{
    global $prvni_radek;
    global $radek;
    global $pole;
    global $cislo;
    global $IDK;

    //prazdny string (nemel by se sem dostat, ale jistota je jistota)
    if(count($pole) == 0) exit(22);
    if($prvni_radek == 0)
    {
        //move var symb
        if(preg_match('/^move$/i', $pole[0]) == 1)
        {
            if(count($pole) != 3) exit(23);
            //chybný var
            if(is_var($pole[1]) == 1) exit(23);
            $arg1 = array(1 => "var", 2 => $pole[1]);
            if(is_var($pole[2]) == 0) $arg2 = array(1 => "var", 2 => $pole[2]);
            elseif(is_symb($pole[2]) == 1) exit(23);
            else
            {
                $type = $pole[2];
                if(which_type($type) != 0) exit(99);
                $arg2 = array(1 => $type, 2 => $pole[2]);
            }
            if(xml_instrukce(2, 'MOVE', $arg1, $arg2, 0) == 1) exit(99);
            return 0;
        }
        //createframe
        elseif(preg_match('/^createframe$/i', $pole[0]) == 1)
        {
            if(count($pole) != 1) exit(23);
            if(xml_instrukce(0, 'CREATEFRAME', 0 , 0 , 0) == 1) exit(99);
            return 0;
        }
        //pushframe
        elseif(preg_match('/^pushframe$/i', $pole[0]) == 1)
        {
            if(count($pole) != 1) exit(23);
            if(xml_instrukce(0, 'PUSHFRAME', 0, 0, 0) == 1) exit(99);
            return 0;
        }
        //popframe
        elseif(preg_match('/^popframe$/i', $pole[0]) == 1)
        {
            if(count($pole) != 1) exit(23);
            if(xml_instrukce(0, 'POPFRAME', 0, 0, 0) == 1) exit(99);
            return 0;
        }
        //defvar var
        elseif(preg_match('/^defvar$/i', $pole[0]) == 1)
        {
            if(count($pole) != 2) exit(23);
            if(is_var($pole[1]) == 1) exit(23);
            $arg1 = array(1 => "var", 2 => $pole[1]);
            if(xml_instrukce(1, 'DEFVAR', $arg1, 0, 0) == 1) exit(99);
            return 0;
        }
        //call label
        elseif(preg_match('/^call$/i', $pole[0]) == 1)
        {
            if(count($pole) != 2) exit(23);
            if(is_label($pole[1]) == 1) exit(23);
            $arg1 = array(1 => "label", 2 => $pole[1]);
            if(xml_instrukce(1, 'CALL', $arg1, 0, 0) == 1) exit(99);
            return 0;
        }
        //return
        elseif(preg_match('/^return$/i', $pole[0]) == 1)
        {
            if(count($pole) != 1) exit(23);
            if(xml_instrukce(0, 'RETURN', 0, 0, 0) == 1) exit(99);
            return 0;
        }
        //pushs symb
        if(preg_match('/^pushs$/i', $pole[0]) == 1)
        {
            if(count($pole) != 2) exit(23);
            if(is_var($pole[1]) == 0) $arg1 = array(1 => "var", 2 => $pole[1]);
            elseif(is_symb($pole[1]) == 1) exit(23);
            else
            {   
                $type = $pole[1];
                if(which_type($type) != 0) exit(99);
                $arg1 = array(1 => $type, 2 => $pole[1]);
            } 
            if(xml_instrukce(1, 'PUSHS', $arg1, 0, 0) == 1) exit(99);
            return 0;
        }
        //pops var
        elseif(preg_match('/^pops$/i', $pole[0]) == 1)
        {
            if(count($pole) != 2) exit(23);
            if(is_var($pole[1]) == 1) exit(23);
            $arg1 = array(1 => "var", 2 => $pole[1]);
            if(xml_instrukce(1, 'POPS', $arg1, 0, 0) == 1) exit(99);
            return 0;
        }
        //add var symb symb
        elseif(preg_match('/^add$/i', $pole[0]) == 1)
        {
            if(count($pole) != 4) exit(23);
            if(is_var($pole[1]) == 1) exit(23);
            $arg1 = array(1 => "var", 2 => $pole[1]);
            if(is_var($pole[2]) == 0) $arg2 = array(1 => "var", 2 => $pole[2]);
            elseif(is_symb($pole[2]) == 1) exit(23);
            else
            {   
                $type = $pole[2];
                if(which_type($type) != 0) exit(99);
                $arg2 = array(1 => $type, 2 => $pole[2]);
            }
            if(is_var($pole[3]) == 0) $arg3 = array(1 => "var", 2 => $pole[3]);
            elseif(is_symb($pole[3]) == 1) exit(23);
            else
            {   
                $type = $pole[3];
                if(which_type($type) != 0) exit(99);
                $arg3 = array(1 => $type, 2 => $pole[3]);
            } 
            if(xml_instrukce(3, 'ADD', $arg1, $arg2, $arg3) == 1) exit(99);
            return 0;
        }
        //sub var symb symb
        elseif(preg_match('/^sub$/i', $pole[0]) == 1)
        {
            if(count($pole) != 4) exit(23);
            if(is_var($pole[1]) == 1) exit(23);
            $arg1 = array(1 => "var", 2 => $pole[1]);
            if(is_var($pole[2]) == 0) $arg2 = array(1 => "var", 2 => $pole[2]);
            elseif(is_symb($pole[2]) == 1) exit(23);
            else
            {   
                $type = $pole[2];
                if(which_type($type) != 0) exit(99);
                $arg2 = array(1 => $type, 2 => $pole[2]);
            }
            if(is_var($pole[3]) == 0) $arg3 = array(1 => "var", 2 => $pole[3]);
            elseif(is_symb($pole[3]) == 1) exit(23);
            else
            {   
                $type = $pole[3];
                if(which_type($type) != 0) exit(99);
                $arg3 = array(1 => $type, 2 => $pole[3]);
            } 
            if(xml_instrukce(3, 'SUB', $arg1, $arg2, $arg3) == 1) exit(99);
            return 0;
        }
        //mul var symb symb
        elseif(preg_match('/^mul$/i', $pole[0]) == 1)
        {
            if(count($pole) != 4) exit(23);
            if(is_var($pole[1]) == 1) exit(23);
            $arg1 = array(1 => "var", 2 => $pole[1]);
            if(is_var($pole[2]) == 0) $arg2 = array(1 => "var", 2 => $pole[2]);
            elseif(is_symb($pole[2]) == 1) exit(23);
            else
            {   
                $type = $pole[2];
                if(which_type($type) != 0) exit(99);
                $arg2 = array(1 => $type, 2 => $pole[2]);
            }
            if(is_var($pole[3]) == 0) $arg3 = array(1 => "var", 2 => $pole[3]);
            elseif(is_symb($pole[3]) == 1) exit(23);
            else
            {   
                $type = $pole[3];
                if(which_type($type) != 0) exit(99);
                $arg3 = array(1 => $type, 2 => $pole[3]);
            }             
            if(xml_instrukce(3, 'MUL', $arg1, $arg2, $arg3) == 1) exit(99);
            return 0;
        }
        //idiv var symb symb
        elseif(preg_match('/^idiv$/i', $pole[0]) == 1)
        {
            if(count($pole) != 4) exit(23);
            if(is_var($pole[1]) == 1) exit(23);
            $arg1 = array(1 => "var", 2 => $pole[1]);
            if(is_var($pole[2]) == 0) $arg2 = array(1 => "var", 2 => $pole[2]);
            elseif(is_symb($pole[2]) == 1) exit(23);
            else
            {   
                $type = $pole[2];
                if(which_type($type) != 0) exit(99);
                $arg2 = array(1 => $type, 2 => $pole[2]);
            }
            if(is_var($pole[3]) == 0) $arg3 = array(1 => "var", 2 => $pole[3]);
            elseif(is_symb($pole[3]) == 1) exit(23);
            else
            {   
                $type = $pole[3];
                if(which_type($type) != 0) exit(99);
                $arg3 = array(1 => $type, 2 => $pole[3]);
            }             
            if(xml_instrukce(3, 'IDIV', $arg1, $arg2, $arg3) == 1) exit(99);
            return 0;
        }
        //lt var symb symb
        elseif(preg_match('/^lt$/i', $pole[0]) == 1)
        {
            if(count($pole) != 4) exit(23);
            if(is_var($pole[1]) == 1) exit(23);
            $arg1 = array(1 => "var", 2 => $pole[1]);
            if(is_var($pole[2]) == 0) $arg2 = array(1 => "var", 2 => $pole[2]);
            elseif(is_symb($pole[2]) == 1) exit(23);
            else
            {   
                $type = $pole[2];
                if(which_type($type) != 0) exit(99);
                $arg2 = array(1 => $type, 2 => $pole[2]);
            } 
            if(is_var($pole[3]) == 0) $arg3 = array(1 => "var", 2 => $pole[3]);
            elseif(is_symb($pole[3]) == 1) exit(23);
            else
            {   
                $type = $pole[3];
                if(which_type($type) != 0) exit(99);
                $arg3 = array(1 => $type, 2 => $pole[3]);
            } 
            if(xml_instrukce(3, 'LT', $arg1, $arg2, $arg3) == 1) exit(99);
            return 0;
        }
        //gt var symb symb
        elseif(preg_match('/^gt$/i', $pole[0]) == 1)
        {
            if(count($pole) != 4) exit(23);
            if(is_var($pole[1]) == 1) exit(23);
            $arg1 = array(1 => "var", 2 => $pole[1]);
            if(is_var($pole[2]) == 0) $arg2 = array(1 => "var", 2 => $pole[2]);
            elseif(is_symb($pole[2]) == 1) exit(23);
            else
            {
                $type = $pole[2];
                if(which_type($type) != 0) exit(99);
                $arg2 = array(1 => $type, 2 => $pole[2]);
            }
            if(is_var($pole[3]) == 0) $arg3 = array(1 => "var", 2 => $pole[3]);
            elseif(is_symb($pole[3]) == 1) exit(23);
            else
            {
                $type = $pole[3];
                if(which_type($type) != 0) exit(99);
                $arg3 = array(1 => $type, 2 => $pole[3]);
            }
            if(xml_instrukce(3, 'GT', $arg1, $arg2, $arg3) == 1) exit(99);
            return 0;
        }
        //eq var symb symb
        elseif(preg_match('/^eq$/i', $pole[0]) == 1)
        {
            if(count($pole) != 4) exit(23);
            if(is_var($pole[1]) == 1) exit(23);
            $arg1 = array(1 => "var", 2 => $pole[1]);
            if(is_var($pole[2]) == 0) $arg2 = array(1 => "var", 2 => $pole[2]);
            elseif(is_symb($pole[2]) == 1) exit(23);
            else
            {
                $type = $pole[2];
                if(which_type($type) != 0) exit(99);
                $arg2 = array(1 => $type, 2 => $pole[2]);
            }
            if(is_var($pole[3]) == 0) $arg3 = array(1 => "var", 2 => $pole[3]);
            elseif(is_symb($pole[3]) == 1) exit(23);
            else
            {
                $type = $pole[3];
                if(which_type($type) != 0) exit(99);
                $arg3 = array(1 => $type, 2 => $pole[3]);
            }            
            if(xml_instrukce(3, 'EQ', $arg1, $arg2, $arg3) == 1) exit(99);
            return 0;
        }
        //and var symb symb
        elseif(preg_match('/^and$/i', $pole[0]) == 1)
        {
            if(count($pole) != 4) exit(23);
            if(is_var($pole[1]) == 1) exit(23);
            $arg1 = array(1 => "var", 2 => $pole[1]);
            if(is_var($pole[2]) == 0) $arg2 = array(1 => "var", 2 => $pole[2]);
            elseif(is_symb($pole[2]) == 1) exit(23);
            else
            {
                $type = $pole[2];
                if(which_type($type) != 0) exit(99);
                $arg2 = array(1 => $type, 2 => $pole[2]);
            }
            if(is_var($pole[3]) == 0) $arg3 = array(1 => "var", 2 => $pole[3]);
            elseif(is_symb($pole[3]) == 1) exit(23);
            else
            {
                $type = $pole[3];
                if(which_type($type) != 0) exit(99);
                $arg3 = array(1 => $type, 2 => $pole[3]);
            }
            if(xml_instrukce(3, 'AND', $arg1, $arg2, $arg3) == 1) exit(99);
            return 0;
        }
        //or var symb symb
        elseif(preg_match('/^or$/i', $pole[0]) == 1)
        {
            if(count($pole) != 4) exit(23);
            if(is_var($pole[1]) == 1) exit(23);
            $arg1 = array(1 => "var", 2 => $pole[1]);
            if(is_var($pole[2]) == 0) $arg2 = array(1 => "var", 2 => $pole[2]);
            elseif(is_symb($pole[2]) == 1) exit(23);
            else
            {
                $type = $pole[2];
                if(which_type($type) != 0) exit(99);
                $arg2 = array(1 => $type, 2 => $pole[2]);
            }
            if(is_var($pole[3]) == 0) $arg3 = array(1 => "var", 2 => $pole[3]);
            elseif(is_symb($pole[3]) == 1) exit(23);
            else
            {
                $type = $pole[3];
                if(which_type($type) != 0) exit(99);
                $arg3 = array(1 => $type, 2 => $pole[3]);
            }
            if(xml_instrukce(3, 'OR', $arg1, $arg2, $arg3) == 1) exit(99);
            return 0;
        }
        //not var symb
        elseif(preg_match('/^not$/i', $pole[0]) == 1)
        {
            if(count($pole) != 3) exit(23);
            if(is_var($pole[1]) == 1) exit(23);
            $arg1 = array(1 => "var", 2 => $pole[1]);
            if(is_var($pole[2]) == 0) $arg2 = array(1 => "var", 2 => $pole[2]);
            elseif(is_symb($pole[2]) == 1) exit(23);
            else
            {
                $type = $pole[2];
                if(which_type($type) != 0) exit(99);
                $arg2 = array(1 => $type, 2 => $pole[2]);
            }            
            if(xml_instrukce(2, 'NOT', $arg1, $arg2, 0) == 1) exit(99);
            return 0;
        }
        //int2char var symb
        elseif(preg_match('/^int2char$/i', $pole[0]) == 1)
        {
            if(count($pole) != 3) exit(23);
            if(is_var($pole[1]) == 1) exit(23);
            $arg1 = array(1 => "var", 2 => $pole[1]);
            if(is_var($pole[2]) == 0) $arg2 = array(1 => "var", 2 => $pole[2]);
            elseif(is_symb($pole[2]) == 1) exit(23);
            else
            {
                $type = $pole[2];
                if(which_type($type) != 0) exit(99);
                $arg2 = array(1 => $type, 2 => $pole[2]);
            }
            if(xml_instrukce(2, 'INT2CHAR', $arg1, $arg2, 0) == 1) exit(99);
            return 0;
        }
        //stri2int var symb symb
        elseif(preg_match('/^stri2int$/i', $pole[0]) == 1)
        {
            if(count($pole) != 4) exit(23);
            if(is_var($pole[1]) == 1) exit(23);
            $arg1 = array(1 => "var", 2 => $pole[1]);
            if(is_var($pole[2]) == 0) $arg2 = array(1 => "var", 2 => $pole[2]);
            elseif(is_symb($pole[2]) == 1) exit(23);
            else
            {
                $type = $pole[2];
                if(which_type($type) != 0) exit(99);
                $arg2 = array(1 => $type, 2 => $pole[2]);
            }
            if(is_var($pole[3]) == 0) $arg3 = array(1 => "var", 2 => $pole[3]);
            elseif(is_symb($pole[3]) == 1) exit(23);
            else
            {
                $type = $pole[3];
                if(which_type($type) != 0) exit(99);
                $arg3 = array(1 => $type, 2 => $pole[3]);
            } 
            if(xml_instrukce(3, 'STRI2INT', $arg1, $arg2, $arg3) == 1) exit(99);
            return 0;
        }
        //read var type
        elseif(preg_match('/^read$/i', $pole[0]) == 1)
        {
            if(count($pole) != 3) exit(23);
            if(is_var($pole[1]) == 1) exit(23);
            $arg1 = array(1 => "var", 2 => $pole[1]);
            if(is_type($pole[2]) == 1) exit(23);
            $arg2 = array(1 => "type", 2 => $pole[2]);
            if(xml_instrukce(2, 'READ', $arg1, $arg2, 0) == 1) exit(99);
            return 0;
        }
        //write symb
        elseif(preg_match('/^write$/i', $pole[0]) == 1)
        {
            if(count($pole) != 2) exit(23);
            if(is_var($pole[1]) == 0) $arg1 = array(1 => "var", 2 => $pole[1]);
            elseif(is_symb($pole[1]) == 1) exit(23);
            else
            {
                $type = $pole[1];
                if(which_type($type) != 0) exit(99);
                $arg1 = array(1 => $type, 2 => $pole[1]);
            }            
            if(xml_instrukce(1, 'WRITE', $arg1, 0, 0) == 1) exit(99);
            return 0;
        }
        //concat var symb symb
        elseif(preg_match('/^concat$/i', $pole[0]) == 1)
        {
            if(count($pole) != 4) exit(23);
            if(is_var($pole[1]) == 1) exit(23);
            $arg1 = array(1 => "var", 2 => $pole[1]);
            if(is_var($pole[2]) == 0) $arg2 = array(1 => "var", 2 => $pole[2]);
            elseif(is_symb($pole[2]) == 1) exit(23);
            else
            {
                $type = $pole[2];
                if(which_type($type) != 0) exit(99);
                $arg2 = array(1 => $type, 2 => $pole[2]);
            }
            if(is_var($pole[3]) == 0) $arg3 = array(1 => "var", 2 => $pole[3]);
            elseif(is_symb($pole[3]) == 1) exit(23);
            else
            {
                $type = $pole[3];
                if(which_type($type) != 0) exit(99);
                $arg3 = array(1 => $type, 2 => $pole[3]);
            }
            if(xml_instrukce(3, 'CONCAT', $arg1, $arg2, $arg3) == 1) exit(99);
            return 0;
        }
        //strlen var symb
        elseif(preg_match('/^strlen$/i', $pole[0]) == 1)
        {
            if(count($pole) != 3) exit(23);
            if(is_var($pole[1]) == 1) exit(23);
            $arg1 = array(1 => "var", 2 => $pole[1]);
            if(is_var($pole[2]) == 0) $arg2 = array(1 => "var", 2 => $pole[2]);
            elseif(is_symb($pole[2]) == 1) exit(23);
            else
            {
                $type = $pole[2];
                if(which_type($type) != 0) exit(99);
                $arg2 = array(1 => $type, 2 => $pole[2]);
            }
            if(xml_instrukce(2, 'STRLEN', $arg1, $arg2, 0) == 1) exit(99);
            return 0;
        }
        //getchar var symb symb
        elseif(preg_match('/^getchar$/i', $pole[0]) == 1)
        {
            if(count($pole) != 4) exit(23);
            if(is_var($pole[1]) == 1) exit(23);
            $arg1 = array(1 => "var", 2 => $pole[1]);
            if(is_var($pole[2]) == 0) $arg2 = array(1 => "var", 2 => $pole[2]);
            elseif(is_symb($pole[2]) == 1) exit(23);
            else
            {
                $type = $pole[2];
                if(which_type($type) != 0) exit(99);
                $arg2 = array(1 => $type, 2 => $pole[2]);
            }
            if(is_var($pole[3]) == 0) $arg3 = array(1 => "var", 2 => $pole[3]);
            elseif(is_symb($pole[3]) == 1) exit(23);
            else
            {
                $type = $pole[3];
                if(which_type($type) != 0) exit(99);
                $arg3 = array(1 => $type, 2 => $pole[3]);
            }
            if(xml_instrukce(3, 'GETCHAR', $arg1, $arg2, $arg3) == 1) exit(99);
            return 0;
        }
        //setchar var symb symb
        elseif(preg_match('/^setchar$/i', $pole[0]) == 1)
        {
            if(count($pole) != 4) exit(23);
            if(is_var($pole[1]) == 1) exit(23);
            $arg1 = array(1 => "var", 2 => $pole[1]);
            if(is_var($pole[2]) == 0) $arg2 = array(1 => "var", 2 => $pole[2]);
            elseif(is_symb($pole[2]) == 1) exit(23);
            else
            {
                $type = $pole[2];
                if(which_type($type) != 0) exit(99);
                $arg2 = array(1 => $type, 2 => $pole[2]);
            }
            if(is_var($pole[3]) == 0) $arg3 = array(1 => "var", 2 => $pole[3]);
            elseif(is_symb($pole[3]) == 1) exit(23);
            else
            {
                $type = $pole[3];
                if(which_type($type) != 0) exit(99);
                $arg3 = array(1 => $type, 2 => $pole[3]);
            }            
            if(xml_instrukce(3, 'SETCHAR', $arg1, $arg2, $arg3) == 1) exit(99);
            return 0;
        }
        //type var symb
        elseif(preg_match('/^type$/i', $pole[0]) == 1)
        {
            if(count($pole) != 3) exit(23);
            if(is_var($pole[1]) == 1) exit(23);
            $arg1 = array(1 => "var", 2 => $pole[1]);
            if(is_var($pole[2]) == 0) $arg2 = array(1 => "var", 2 => $pole[2]);
            elseif(is_symb($pole[2]) == 1) exit(23);
            else
            {
                $type = $pole[2];
                if(which_type($type) != 0) exit(99);
                $arg2 = array(1 => $type, 2 => $pole[2]);
            }
            if(xml_instrukce(2, 'TYPE', $arg1, $arg2, 0) == 1) exit(99);
            return 0;
        }
        //label label
        elseif(preg_match('/^label$/i', $pole[0]) == 1)
        {
            if(count($pole) != 2) exit(23);
            if(is_label($pole[1]) == 1) exit(23);
            $arg1 = array(1 => "label", 2 => $pole[1]);
            if(xml_instrukce(1, 'LABEL', $arg1, 0, 0) == 1) exit(99);
            return 0;
        }
        //jumpifeq label symb symb
        elseif(preg_match('/^jumpifeq$/i', $pole[0]) == 1)
        {
            if(count($pole) != 4) exit(23);
            if(is_label($pole[1]) == 1) exit(23);
            $arg1 = array(1 => "label", 2 => $pole[1]);
            if(is_var($pole[2]) == 0) $arg2 = array(1 => "var", 2 => $pole[2]);
            elseif(is_symb($pole[2]) == 1) exit(23);
            else
            {
                $type = $pole[2];
                if(which_type($type) != 0) exit(99);
                $arg2 = array(1 => $type, 2 => $pole[2]);
            }
            if(is_var($pole[3]) == 0) $arg3 = array(1 => "var", 2 => $pole[3]);
            elseif(is_symb($pole[3]) == 1) exit(23);
            else
            {
                $type = $pole[3];
                if(which_type($type) != 0) exit(99);
                $arg3 = array(1 => $type, 2 => $pole[3]);
            }
            if(xml_instrukce(3, 'JUMPIFEQ', $arg1, $arg2, $arg3) == 1) exit(99);
            return 0;
        }
        //jumpifneq label symb symb
        elseif(preg_match('/^jumpifneq$/i', $pole[0]) == 1)
        {
            if(count($pole) != 4) exit(23);
            if(is_label($pole[1]) == 1) exit(23);
            $arg1 = array(1 => "label", 2 => $pole[1]);
            if(is_var($pole[2]) == 0) $arg2 = array(1 => "var", 2 => $pole[2]);
            elseif(is_symb($pole[2]) == 1) exit(23);
            else
            {
                $type = $pole[2];
                if(which_type($type) != 0) exit(99);
                $arg2 = array(1 => $type, 2 => $pole[2]);
            }
            if(is_var($pole[3]) == 0) $arg3 = array(1 => "var", 2 => $pole[3]);
            elseif(is_symb($pole[3]) == 1) exit(23);
            else
            {
                $type = $pole[3];
                if(which_type($type) != 0) exit(99);
                $arg3 = array(1 => $type, 2 => $pole[3]);
            }
            if(xml_instrukce(3, 'JUMPIFNEQ', $arg1, $arg2, $arg3) == 1) exit(99);
            return 0;
        }
        //jump label
        elseif(preg_match('/^jump$/i', $pole[0]) == 1)
        {
            if(count($pole) != 2) exit(23);
            if(is_label($pole[1]) == 1) exit(23);
            $arg1 = array(1 => "label", 2 => $pole[1]);
            if(xml_instrukce(1, 'JUMP', $arg1, 0, 0) == 1) exit(99);
            return 0;
        }
        //exit symb
        elseif(preg_match('/^exit$/i', $pole[0]) == 1)
        {
            if(count($pole) != 2) exit(23);
            if(is_var($pole[1]) == 0) $arg1 = array(1 => "var", 2 => $pole[1]);
            elseif(is_symb($pole[1]) == 1) exit(23);
            else
            {
                $type = $pole[1];
                if(which_type($type) != 0) exit(99);
                $arg1 = array(1 => $type, 2 => $pole[1]);
            }
            if(xml_instrukce(1, 'EXIT', $arg1, 0, 0) == 1) exit(99);
            return 0;
        }
        //dprint symb
        elseif(preg_match('/^dprint$/i', $pole[0]) == 1)
        {
            if(count($pole) != 2) exit(23);
            if(is_var($pole[1]) == 0) $arg1 = array(1 => "var", 2 => $pole[1]);
            elseif(is_symb($pole[1]) == 1) exit(23);
            else
            {
                $type = $pole[1];
                if(which_type($type) != 0) exit(99);
                $arg1 = array(1 => $type, 2 => $pole[1]);
            }
            if(xml_instrukce(1, 'DPRINT', $arg1, 0, 0) == 1) exit(99);
            return 0;
        }
        //break
        elseif(preg_match('/^break$/i', $pole[0]) == 1)
        {
            if(count($pole) != 1) exit(23);
            if(xml_instrukce(0, 'BREAK', 0, 0, 0) == 1) exit(99);
            return 0;
        }
        //neznamy chyba 22
        else
        {
            printf("neznamy");
            exit(22);
        } 
    }
    //na prvnim radku musi byt .ippcode2O (na velikosti pismen nezalezi)
    else
    {
        if(preg_match('/^\.ippcode20$/i', $pole[0]) == 1)
        {
            if(xmlwriter_start_document($IDK, '1.0', 'UTF-8') == FALSE) exit(99);
            if(xmlwriter_start_element($IDK, 'program') == FALSE) exit(99);
            if(xmlwriter_start_attribute($IDK, 'language') == FALSE) exit(99);
            if(xmlwriter_text($IDK, 'IPPcode20') == FALSE) exit(99);
            if(xmlwriter_end_attribute($IDK) == FALSE) exit(99);
        }
        else
        {
            exit(21);
        }
    }
}

//je $par proměnná ?
//@ret 0 pokud je, else 1
function is_var($par)
{
    if(preg_match('/^[LTG]F@[A-z][A-z0-9\_\-\$\&\%\*\!\?]*/', $par) == 1) return 0;
    else return 1;
}

//je $par konstanta//proměnná ?
//@ret 0 pokud je, else 1
function is_symb($par)
{
    //int
    if(preg_match('/^int@(\-|\+)?[0-9]+/', $par) == 1) return 0;
    //bool
    elseif(preg_match('/^bool@(true|false)$/', $par) === 1) return 0;
    elseif(preg_match('/^string@.*(\\\\[0-9][0-9][0-9])+/i', $par)) return 0;
    elseif(preg_match('/^nil@nil$/', $par)) return 0;
    //string
    elseif(preg_match('/^string@.*(#|\\\\|\")+.*/', $par)) return 1;
    elseif(preg_match('/^string@.*/', $par) == 1) return 0;
    else return 1;
}

//je $par návěští ?
//@ret 0 pokud je, else 1
function is_label($par)
{
    if(preg_match('/^\\\\/', $par) == 1) return 1;
    if(preg_match('/^[A-z0-9\_\-\$\&\%\*\!\?]+$/', $par) == 1) return 0;
    return 1;
}

//je $par datový typ ?
//@ret 0 pokud je, else 1
function is_type($par)
{
    if(preg_match('/^(int|bool|string)$/', $par) == 1) return 0;
    else return 1;
}

//@ret 0 pokud se má cutovat, else 1
function should_cut($par, &$par2)
{
    $tab = array(0 => "string", 1 => "int", 2 => "bool", 3 => "nil");
    $count = count($tab);
    for($i = 0; $i < $count; $i++)
    {
        if(strcmp($tab[$i], $par) == 0)
        {
            $par2 = preg_replace('/.*@/', '', $par2);
            return 0;
        }
    }
    return 1;
}

//vytahne ze stringu $par type a spravne ho zapise
//@ret 0 if OK, 1 if FAIL
function which_type(&$par)
{
    if(preg_match('/int.*/i', $par) == 1) $par = "int";
    elseif(preg_match('/string.*/i', $par) == 1) $par = "string";
    elseif(preg_match('/bool.*/i', $par) == 1) $par = "bool";
    elseif(preg_match('/nil.*/i', $par) == 1) $par = "nil";
    else return 1;
    return 0;
}

function ascii_to_xml(&$vstup)
{
    $vstup = preg_replace('/\\\\060/', '&lt', $vstup);
    $vstup = preg_replace('/\\\\062/', '&gt', $vstup);
    $vstup = preg_replace('/\\\\038/', '&amp', $vstup);
    return 0;
}

function xml_instrukce($count, $op, $ar1, $ar2, $ar3)
{
    global $IDK;
    global $cislo;
    if(xmlwriter_start_element($IDK, 'instruction') == FALSE) return 1;
    if(xmlwriter_start_attribute($IDK, 'order') == FALSE) return 1;
    if(xmlwriter_text($IDK, $cislo) == FALSE) return 1;
    if(xmlwriter_end_attribute($IDK) == FALSE) return 1;
    if(xmlwriter_start_attribute($IDK, 'opcode') == FALSE) return 1;
    if(xmlwriter_text($IDK, $op) == FALSE) return 1;
    if(xmlwriter_end_attribute($IDK) == FALSE) return 1;


    if($count >= 1)
    {
        if(xmlwriter_start_element($IDK, 'arg1') == FALSE) return 1;
        if(xmlwriter_start_attribute($IDK, 'type') == FALSE) return 1;
        if(xmlwriter_text($IDK, $ar1[1]) == FALSE) return 1;
        if(xmlwriter_end_attribute($IDK) == FALSE) return 1;
        should_cut($ar1[1], $ar1[2]);
        $ar1[2] = preg_replace('/\n$/', '', $ar1[2]);
        ascii_to_xml($ar1[2]);
        if(xmlwriter_text($IDK, $ar1[2]) == FALSE) return 1;
        if(xmlwriter_end_element($IDK) == FALSE) return 1;
        
        if($count >= 2)
        {
            if(xmlwriter_start_element($IDK, 'arg2') == FALSE) return 1;
            if(xmlwriter_start_attribute($IDK, 'type') == FALSE) return 1;
            if(xmlwriter_text($IDK, $ar2[1]) == FALSE) return 1;
            if(xmlwriter_end_attribute($IDK) == FALSE) return 1;
            should_cut($ar2[1], $ar2[2]);
            $ar2[2] = preg_replace('/\n$/', '', $ar2[2]);
            ascii_to_xml($ar2[2]);
            if(xmlwriter_text($IDK, $ar2[2]) == FALSE) return 1;
            if(xmlwriter_end_element($IDK) == FALSE) return 1;
            
            if($count == 3)
            {
                if(xmlwriter_start_element($IDK, 'arg3') == FALSE) return 1;
                if(xmlwriter_start_attribute($IDK, 'type') == FALSE) return 1;
                if(xmlwriter_text($IDK, $ar3[1]) == FALSE) return 1;
                if(xmlwriter_end_attribute($IDK) == FALSE) return 1;
                should_cut($ar3[1], $ar3[2]);
                $ar3[2] = preg_replace('/\n$/', '', $ar3[2]);
                ascii_to_xml($ar3[2]);
                if(xmlwriter_text($IDK, $ar3[2]) == FALSE) return 1;
                if(xmlwriter_end_element($IDK) == FALSE) return 1;
            }
            elseif($count > 3) return 1;
        }
    }
    if(xmlwriter_end_element($IDK) == FALSE) return 1;
    return 0;
}

        ?>
