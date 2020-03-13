# Implementační dokumentace k 1 úloze do IPP 2019/2020
 - Jméno a přijmení: Petr Dančák
 - Login: xdanca01

## IPP - parse.php
 - První činnost skriptu je inicializace globálních proměnných pro kontrolu prvního řádku, který by měl obsahovat příslušnou hlavičku kódu IPPcode20. Poté následuje kontrola argumentů, tomto případě jen jednoho argumentu a to "--help". Po kontrole argumentů se přejde k vytvoření prostoru pro xml zápis výstupu, do kterého se dále ukládá výstup xmlwriter funkcí. Dále následuje cyklus čtení po řádku, kde se prázdné řádky nebo zakomentované řádky ignorují a komentáře na konci řádků nahrazují prázdným řetezcem.
 - Cely chod skriptu probíhá ve funkci muj_regex, který nemá žádné parametry a pracuje přímo s globálními proměnnými. Průběh je takový, že se zkontroluje, jestli byla nalezena hlavička na prvním řádku a pak dále kontroluje obsah řádků, který je uložen podle rozdělovače do pole, u kterého se kontroluje, o jakou instrukci se jedná, počet prvků pole, který je roven pevně stanovené hodnotě podle instrukce a dále pak typy operandů, s kterými instrukce pracuje. Na kotrolu operandů jsem si vytvořil funkce "is_var", "is_symb", "is_label" a "is_type". U každé funkce z jmenovaných se počítá s parametrem typu string a vrací int hodnotu podle vyhodnocení výsledku.
 - Jakmile je vše u správného zápisu instrukce ověřené, tak dojde k zavolání funkce xml_instrukce, která se postará o správný zápis do mezikódu. Pro přehlednější zápis a zjednodušení problému využívá pomocné uživatelem napsané funkce jako "ascii_to_xml" a "should_cut". Pokud vše proběhne bez chyb, tak dojde k výpisu akumulovaného výstupního zápisu.