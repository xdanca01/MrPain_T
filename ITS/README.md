# ITS - Projekt 1
 - login: xdanca01
 - rok: 2020
## ucet.feature
 - Soubor se scénáři pro test správy účtu na webu.
 - Testuje se zde tvorba nového uživatelského účtu, přístup k účtu a také změna informací účtu.
### 
## nakup.feature
 - Soubor se scénáři pro test uživatelského nakupování.
 - Testují se zde základní prvky přidávání produktů do košíku a následnému vytvoření objednávky.
## admin_products.feature
 - Soubor se scénáři pro test správy produktů v nabídce obchodu.
 - Testování z administrátorského prostředí s přesnějším zaměřením na stránku správy produktů.
 - Testuje se základní funkčnost práce s produkty v nabídce, které se buď nově přidají nebo se změní jejich stav.
## admin_orders.feature
 - Soubor se scénáři pro test administrátorské stránky pro správu objednávek.
 - V testování správy objednávek se pracuje se stavem objednávky, obsahem objednávky a zobrazením objednávky.
### Změna statusu objednávky
 - Požadavky: administrátorská práva, výskyt na stránce s objednávkami a existence objednávky
 - Testování: Změní se status objednávky
 - Výsledek: Status objednávky bude odlišný
### Změna měny objednávky
 - Požadavky: administrátorská práva, výskyt na stránce s objednávkami a existence objednávky
 - Testování: Přejde se na detaily jedné z objednávek a změní se měna objednávky
 - Výsledek: Změní se měna a cena objednávky
### Přidání produktu do objednávky
 - Požadavky: administrátorská práva, výskyt na stránce s objednávkami a exi
stence objednávky
 - Testování: Přejde se na stránku s detaily objednávky a přidá se do seznamu produktů objednávky nějaký z produktů v nabídce
 - Výsledek: Změní se cena objednávky
### Smazání objednávky
 - Požadavky: administrátorská práva, výskyt na stránce s objednávkami a exi
stence objednávky
 - Testování: Vybere se jedna z objednávek a smaže se
 - Výsledek: Objednávka se nadále nevyskytuje mezi objednávkami
