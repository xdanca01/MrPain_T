# Projekt do předmětu ISA:
## Filtrující DNS resolver


###Login: xdanca01
###Jméno: Petr
###Příjmení: Dančák
###Rok: 2020/21

##Popis:
Program slouží k filtraci a zodpovězení DNS požadavků typu A a to na předem specifikovaném portu předaného parametrem -p 
(pokud není zadán, tak je nastaven na hodnotu 53). Program filtruje překlad nežádoucích domén, které načte ze souboru 
specifikovaného za argumentem -f a posílá vyfiltrované požadavky na server specifikovaný za argumentem -s.

##Příklad spuštění:
###Přeložení:
make

###Spuštění:
./main -s <server> [-p <port>] -f <soubor>

###Spuštění serveru a testů:
make test

###Smazání vytvořených souborů:
make clean