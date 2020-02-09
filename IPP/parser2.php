<?php



$radek;
$prvni_radek = 1;
$pole;
//nacitani ze STDIN
while($radek = fgets(STDIN))
{
    $pole = preg_split ("/ /", $radek);
    echo "pred";
    echo $pole[0];
    echo "po";
    $rtrn_code = muj_regex();
    if($rtrn_code != 0) return $rtrn_code;
    $prvni_radek = 0;
}

//bezchybný konec programu
exit(0);


//kontrola radku na regex
function muj_regex()
{
    global $prvni_radek;
    global $radek;
    global $pole;


    if(count($pole) == 0) exit(22);
    //prazdny string (nemel by se sem dostat, ale jistota je jistota)
    if($prvni_radek == 0)
    {
        //move var symb
        if(preg_match('/[mM][oO][vV][eE]/i', $pole[0]) == 1)
        {
            //chybný var
            if(is_var($pole[1]) == 1) exit(22);
            elseif(is_symb($pole[2]) == 1) exit(22);
            echo $radek;
            return 0;
        }
        //createframe
        elseif(preg_match('/[cC][rR][eE][aA][tT][eE][fF][rR][aA][mM][eE]/i', $pole[0]) == 1) echo $radek;
        //pushframe
        elseif(preg_match('/[pP][uU][sS][hH][fF][rR][aA][mM][eE]/i', $pole[0]) == 1) echo $radek;
        //popframe
        elseif(preg_match('/[pP][oO][pP][fF][rR][aA][mM][eE]/i', $pole[0]) == 1) echo $radek;
        //defvar var
        elseif(preg_match('/[dD][eE][fF][vV][aA][rR]/i', $pole[0]) == 1)
        {
            if(count($pole) != 2) exit 22;
            if(is_var($pole[1]) == 1) exit(22);
            return 0;
        }
        //call label
        elseif(preg_match('/[cC][aA][lL][lL]/i', $pole[0]) == 1)
        {
            if(count($pole) != 2) exit 22;
            if(is_label($pole[1]) == 1) exit(22);
            return 0;
        }
        //return
        elseif(preg_match('/[rR][eE][tT][uU][rR][nN]/i', $pole[0]) == 1) echo $radek;
        //pushs symb
        elseif(preg_match('/[pP][uU][sS][hH][sS]/i', $pole[0]) == 1)
        {
            if(count($pole) != 2) exit 22;
            if(is_symb($pole[1]) == 1) exit(22);
            return 0;
        }
        //pops var
        elseif(preg_match('/[pP][oO][pP][sS]/i', $pole[0]) == 1)
        {
            if(count($pole) != 2) exit 22;
            if(is_var($pole[1]) == 1) exit(22);
            return 0;
        }
        //add var symb symb
        elseif(preg_match('/[aA][dD][dD]/i', $pole[0]) == 1)
        {
            if(count($pole) != 4) exit 22;
            if(is_var($pole[1]) == 1) exit(22);
            elseif(is_symb($pole[2]) == 1) exit(22);
            elseif(is_symb($pole[3]) == 1) exit(22);
            return 0;
        }
        //sub var symb symb
        elseif(preg_match('/[sS][uU][bB]/i', $pole[0]) == 1)
        {
            if(count($pole) != 4) exit 22;
            if(is_var($pole[1]) == 1) exit(22);
            elseif(is_symb($pole[2]) == 1) exit(22);
            elseif(is_symb($pole[3]) == 1) exit(22);
            return 0;
        }
        //mul var symb symb
        elseif(preg_match('/[mM][uU][lL]/i', $pole[0]) == 1)
        {
            if(count($pole) != 4) exit 22;
            if(is_var($pole[1]) == 1) exit(22);
            elseif(is_symb($pole[2]) == 1) exit(22);
            elseif(is_symb($pole[3]) == 1) exit(22);
            return 0;
        }
        //idiv var symb symb
        elseif(preg_match('/[iI][dD][iI][vV]/i', $pole[0]) == 1)
        {
            if(count($pole) != 4) exit 22;
            if(is_var($pole[1]) == 1) exit(22);
            elseif(is_symb($pole[2]) == 1) exit(22);
            elseif(is_symb($pole[3]) == 1) exit(22);
            return 0;
        }
        //lt var symb symb
        elseif(preg_match('/[lL][tT]/i', $pole[0]) == 1)
        {
            if(count($pole) != 4) exit 22;
            if(is_var($pole[1]) == 1) exit(22);
            elseif(is_symb($pole[2]) == 1) exit(22);
            elseif(is_symb($pole[3]) == 1) exit(22);
            return 0;
        }
        //gt var symb symb
        elseif(preg_match('/[gG][tT]/i', $pole[0]) == 1)
        {
            if(count($pole) != 4) exit 22;
            if(is_var($pole[1]) == 1) exit(22);
            elseif(is_symb($pole[2]) == 1) exit(22);
            elseif(is_symb($pole[3]) == 1) exit(22);
            return 0;
        }
        //eq var symb symb
        elseif(preg_match('/[eE][qQ]/i', $pole[0]) == 1)
        {
            if(count($pole) != 4) exit 22;
            if(is_var($pole[1]) == 1) exit(22);
            elseif(is_symb($pole[2]) == 1) exit(22);
            elseif(is_symb($pole[3]) == 1) exit(22);
            return 0;
        }
        //and var symb symb
        elseif(preg_match('/[aA][nN][dD]/i', $pole[0]) == 1)
        {
            if(count($pole) != 4) exit 22;
            if(is_var($pole[1]) == 1) exit(22);
            elseif(is_symb($pole[2]) == 1) exit(22);
            elseif(is_symb($pole[3]) == 1) exit(22);
            return 0;
        }
        //or var symb symb
        elseif(preg_match('/[oO][rR]/i', $pole[0]) == 1)
        {
            if(count($pole) != 4) exit 22;
            if(is_var($pole[1]) == 1) exit(22);
            elseif(is_symb($pole[2]) == 1) exit(22);
            elseif(is_symb($pole[3]) == 1) exit(22);
            return 0;
        }
        //not var symb symb
        elseif(preg_match('/[nN][oO][tT]/i', $pole[0]) == 1)
        {
            if(count($pole) != 4) exit 22;
            if(is_var($pole[1]) == 1) exit(22);
            elseif(is_symb($pole[2]) == 1) exit(22);
            elseif(is_symb($pole[3]) == 1) exit(22);
            return 0;
        }
        //int2char var symb
        elseif(preg_match('/[iI][nN][tT]2[cC][hH][aA][rR]/i', $pole[0]) == 1)
        {
            if(count($pole) != 3) exit 22;
            if(is_var($pole[1]) == 1) exit(22);
            elseif(is_symb($pole[2]) == 1) exit(22);
            return 0;
        }
        //stri2int var symb symb
        elseif(preg_match('/[sS][tT][rR][iI]2[iI][nN][tT]/i', $pole[0]) == 1)
        {
            if(count($pole) != 4) exit 22;
            if(is_var($pole[1]) == 1) exit(22);
            elseif(is_symb($pole[2]) == 1) exit(22);
            elseif(is_symb($pole[3]) == 1) exit(22);
            return 0;
        }
        //read var type
        elseif(preg_match('/[rR][eE][aA][dD]/i', $pole[0]) == 1)
        {
            if(count($pole) != 3) exit 22;
            if(is_var($pole[1]) == 1) exit(22);
            elseif(is_type($pole[2]) == 1) exit(22);
            return 0;
        }
        //write symb
        elseif(preg_match('/[wW][rR][iI][tT][eE]/i', $pole[0]) == 1)
        {
            if(count($pole) != 2) exit 22;
            if(is_symb($pole[1]) == 1) exit(22);
            return 0;
        }
        //concat var symb symb
        elseif(preg_match('/[cC][oO][nN][cC][aA][tT]/i', $pole[0]) == 1)
        {
            if(count($pole) != 4) exit 22;
            if(is_var($pole[1]) == 1) exit(22);
            elseif(is_symb($pole[2]) == 1) exit(22);
            elseif(is_symb($pole[3]) == 1) exit(22);
            return 0;
        }
        //strlen var symb
        elseif(preg_match('/[sS][tT][rR][lL][eE][nN]/i', $pole[0]) == 1)
        {
            if(count($pole) != 3) exit 22;
            if(is_var($pole[1]) == 1) exit(22);
            elseif(is_symb($pole[2]) == 1) exit(22);
            return 0;
        }
        //getchar var symb symb
        elseif(preg_match('/[gG][eE][tT][cC][hH][aA][rR]/i', $pole[0]) == 1)
        {
            if(count($pole) != 4) exit 22;
            if(is_var($pole[1]) == 1) exit(22);
            elseif(is_symb($pole[2]) == 1) exit(22);
            elseif(is_symb($pole[3]) == 1) exit(22);
            return 0;
        }
        //setchar var symb symb
        elseif(preg_match('/[sS][eE][tT][cC][hH][aA][rR]/i', $pole[0]) == 1)
        {
            if(count($pole) != 4) exit 22;
            if(is_var($pole[1]) == 1) exit(22);
            elseif(is_symb($pole[2]) == 1) exit(22);
            elseif(is_symb($pole[3]) == 1) exit(22);
            return 0;
        }
        //type var symb
        elseif(preg_match('/[tT][yY][pP][eE]/i', $pole[0]) == 1)
        {
            if(count($pole) != 3) exit 22;
            if(is_var($pole[1]) == 1) exit(22);
            elseif(is_symb($pole[2]) == 1) exit(22);
            return 0;
        }
        //label label
        elseif(preg_match('/[lL][aA][bB][eE][lL]/i', $pole[0]) == 1)
        {
            if(count($pole) != 2) exit 22;
            if(is_label($pole[1]) == 1) exit(22);
            return 0;
        }
        //jump label
        elseif(preg_match('/[jJ][uU][mM][pP]/i', $pole[0]) == 1)
        {
            if(count($pole) != 2) exit 22;
            if(is_label($pole[1]) == 1) exit(22);
            return 0;
        }
        //jumpifeq label symb symb
        elseif(preg_match('/[jJ][uU][mM][pP][iI][fF][eE][qQ]/i', $pole[0]) == 1)
        {
            if(count($pole) != 4) exit 22;
            if(is_label($pole[1]) == 1) exit(22);
            elseif(is_symb($pole[2]) == 1) exit(22);
            elseif(is_symb($pole[3]) == 1) exit(22);
            return 0;
        }
        //jumpifneq label symb symb
        elseif(preg_match('/[jJ][uU][mM][pP][iI][fF][nN][eE][qQ]/i', $pole[0]) == 1)
        {
            if(count($pole) != 4) exit 22;
            if(is_label($pole[1]) == 1) exit(22);
            elseif(is_symb($pole[2]) == 1) exit(22);
            elseif(is_symb($pole[3]) == 1) exit(22);
            return 0;
        }
        //exit symb
        elseif(preg_match('/[eE][xX][iI][tT]/i', $pole[0]) == 1)
        {
            if(count($pole) != 2) exit 22;
            if(is_symb($pole[1]) == 1) exit(22);
            return 0;
        }
        //dprint symb
        elseif(preg_match('/[dD][pP][rR][iI][nN][tT]/i', $pole[0]) == 1)
        {
            if(count($pole) != 2) exit 22;
            if(is_symb($pole[1]) == 1) exit(22);
            return 0;
        }
        //break
        elseif(preg_match('/[bB][rR][eE][aA][kK]/i', $pole[0]) == 1) return 0;
        //neznamy chyba 22
        else
        {
            exit(22);
        } 
    }
    //na prvnim radku musi byt .ippcode2O (na velikosti pismen nezalezi)
    else
    {
        if(preg_match('/\.[iI][pP][pP][cC][oO][dD][eE]20/i', $radek) == 1) echo $radek;
        else
        {
            exit(21);
        }
    }
}

function is_var($par)
{
    if(preg_match('/[lLtTgG][fF]@[A-z0-9\_\-\&\&\%\*\!\?]*/', $par) == 1) return 0;
    elseif(preg_match('/([sS][tT][rR][iI][nN][gG]|[iI][nN][tT]|[bB][oO][oO][lL]|[nN][iI][lL])@[A-z0-9\_\-\&\&\%\*\!\?][A-z0-9\_\-\&\&\%\*\!\?]*/', $par) == 1) return 0;
    else return 1;
}

function is_symb($par)
{
    if(is_var() == 0) return 0;
    elseif(preg_match('/[0-9][0-9]*/', $par) == 1) return 0;
    else return 1;
}

function is_label($par)
{
    if(preg_match('/[A-z0-9\_\-\&\&\%\*\!\?][A-z0-9\_\-\&\&\%\*\!\?]*/', $par) == 1) return 0;
    else return 1;
}

function is_type($par)
{
    if(preg_match('/([i][n][t]|[b][o][o][l]|[s][t][r][i][n][g])/i', $par) == 1) return 0;
    else return 1;
}

?>
