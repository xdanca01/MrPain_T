# ITS - Projekt 1
 - login: xdanca01
 - rok: 2020
## ucet.feature
 - Soubor se scénáři pro test správy účtu na webu.
 - Testuje se zde tvorba nového uživatelského účtu, přístup k účtu a také změna informací účtu.
### 1. Registrace nového uživatele
 - Požadavky: Výskyt na stránce registrace nového účtu
 - Testování: Vyplní se potřebné údaje pro registraci a potvrdí se registrace
 - Výsledek: Objeví se potvrzení o úspěšné registraci
### 2. Přihlášení uživatele
 - Požadavky: Uživatel je zaregistrován a vyskytuje se na stránce pro přihlášení
 - Testování: Vyplní se správné přihlašovací údaje a potvrdí se přihlášení
 - Výsledek: Dojde k přihlášení na uživatelský účet
### 3. Změna hesla
 - Požadavky: Uživatel je přihlášen a vyskytuje se na stránce pro změnu hesla
 - Testování: Zadá se nové heslo ve správném formátu a potvrdí se změna hesla
 - Výsledek: Vypíše se hlášení o změně hesla
### 4. Změna osobních informací
 - Požadavky: Uživatel je přihlášen a vyskytuje se na stránce pro změnu osobních údajů
 - Testování: Změní některou z informací v údajích a potvrdí změnu
 - Výsledek: Zobrazí se hlášení o úspěšné změně
## nakup.feature
 - Soubor se scénáři pro test uživatelského nakupování.
 - Testují se zde základní prvky přidávání produktů do košíku a následnému vytvoření objednávky.
### 1. Přidání do košíku
 - Požadavky: Uživatel je přihlášen a vyskytuje se na stránce libovolného produktu, který je na skladě
 - Testování: Danný předmět se přidá do košíku
 - Výsledek: Produkt se objeví v košíku uživatele
### 2. Vytvoření objednávky
 - Požadavky: Uživatel je přihlášen, vyskytuje se na stránce košíku a uživatel má produkt, který je na skladě v košíku
 - Testování: Doplní se údaje o objednávce
 - Výsledek: Vznikne nová objednávka
### 3. Přidání produktu na wish list
 - Požadavky: Uživatel je přihlášen a vyskytuje se na stránce produktu
 - Testování: Produkt se přidá na wish list
 - Výsledek: Produkt se objeví na wish listu uživatele
### 4. Přidání produktu co není na skladě do košíku
 - Požadavky: Uživatel je přihlášen a vyskytuje se na stránce produktu, co není na skladě
 - Testování: Produkt se přidá do košíku
 - Výsledek: Produkt se objeví v košíku uživatele
### 5. Pokus o zakoupení produktu, který není na skladě
 - Požadavky: Uživatel je přihlášen a má v košíku produkt, který není na skladě
 - Testování: Dojde k pokusu zakoupení produktu
 - Výsledek: Produkt nelze zakoupit
### 6. Stav objednávky
 - Požadavky: Uživatel je přihlášen a provedl objednávku
 - Testování: Otevře se stránka s historií objednávek uživatele
 - Výsledek: Zobrazí se provedené objednávky
### 7. Nákup s kupónem
 - Požadavky: Uživatel je přihlášen a má v košíku produkt, který je na skladě
 - Testování: Zadá se existující kupón
 - Výsledek: Cena objednávky se změní
## admin_products.feature
 - Soubor se scénáři pro test správy produktů v nabídce obchodu.
 - Testování z administrátorského prostředí s přesnějším zaměřením na stránku správy produktů.
 - Testuje se základní funkčnost práce s produkty v nabídce, které se buď nově přidají nebo se změní jejich stav.
### 1. Přidání produktu do nabídky
 - Požadavky: Administrátorská práva a výskyt na stránce produktu
 - Testování: Vytvoří se nový produkt a vyplní informace o produktu
 - Výsledek: V seznamu produktů se objeví nový produkt
### 2. Změna statusu produktu
 - Požadavky: Administrátorská práva, výskyt na stránce produktu a existence libovolného produktu
 - Testování: Přejde se na informace o produktu a změní se status produktu
 - Výsledek: V seznamu produktů bude u produktu změněný status
### 3. Změna počtu kusů produktu
 - Požadavky: Administrátorská práva, výskyt na stránce produktu a existence
 libovolného produktu
 - Testování: Přejde se na stránku s informacemi o produktu a změní se informace o počtu kusů produktu
 - Výsledek: Dojde k změně informace o počtu kusů v seznamu produktů
### 4. Změna ceny produktu
 - Požadavky: Administrátorská práva, výskyt na stránce produktu a existence
 libovolného produktu
 - Testování: Přejde se na stránku s informacemi o produktu a změní se informace o ceně produktu
 - Výsledek: Dojde ke změně informace o ceně u danného produktu

## admin_orders.feature
 - Soubor se scénáři pro test administrátorské stránky pro správu objednávek.
 - V testování správy objednávek se pracuje se stavem objednávky, obsahem objednávky a zobrazením objednávky.
### 1. Změna statusu objednávky
 - Požadavky: administrátorská práva, výskyt na stránce s objednávkami a existence objednávky
 - Testování: Změní se status objednávky
 - Výsledek: Status objednávky bude odlišný
### 2. Změna měny objednávky
 - Požadavky: administrátorská práva, výskyt na stránce s objednávkami a existence objednávky
 - Testování: Přejde se na detaily jedné z objednávek a změní se měna objednávky
 - Výsledek: Změní se měna a cena objednávky
### 3. Přidání produktu do objednávky
 - Požadavky: administrátorská práva, výskyt na stránce s objednávkami a exi
stence objednávky
 - Testování: Přejde se na stránku s detaily objednávky a přidá se do seznamu produktů objednávky nějaký z produktů v nabídce
 - Výsledek: Změní se cena objednávky
### 4. Smazání objednávky
 - Požadavky: administrátorská práva, výskyt na stránce s objednávkami a exi
stence objednávky
 - Testování: Vybere se jedna z objednávek a smaže se
 - Výsledek: Objednávka se nadále nevyskytuje mezi objednávkami
