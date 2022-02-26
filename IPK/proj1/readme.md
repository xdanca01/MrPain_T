# IPK - projekt 1

Cílem tohoto projektu je implementace serveru, který bude komunikovat protokolem HTTP, zajišťovat překlad doménových jmen a IP adres za použití knihovny socket.

Operace serveru:
  - GET
  - POST

# GET

  - Pokud server dostane dotaz typu GET, tak očekává správně předaný URL argument, který obsahuje parametry name a type, kde name je hostname nebo IP adresa.
  - Odpověď serveru je jednořádková, obsahuje HTTP hlavičku a případně další odpověď serveru při správně provedené operaci GET.


# POST
  - Pokud server dostane dotaz typu POST, tak očekává správně předaný seznam dotazů, který se skládá z DOTAZU a TYPU, kde DOTAZ je buď IP adresa a nebo hostname a TYP značí typ odpovědi, který je buď A, nebo PTR.

# Makefile
 - Jelikož je server psán v pythonu, tak Makefile obsahuje v části build jen přidělení práv na spuštění.
 - Samotný run je pak zavolán s interpretem python3, cestou ke zdrojovému souboru a navíc ještě očekává argument typu PORT=, který je dále zkontrolován serverem, jestli vyhovuje požadavkům, aby se jednalo o port.
# IPK_proj1.py
 - Samotný server je psán v jazyce python3, ve kterém se používají knihovny na regexy a práci se sockety.
 - Průběh programuje je takový, že se nejdříve zkontroluje argument PORT, pak inicializuje samotný server a nakonec se spustí server, který čeká na příchozí komunikaci. Po zajištění komunikace zjišťuje typ požadavku a následně na něj podle specifikace odpoví. Po odpovědi uzavírá komunikaci s clientem a čeká na další navázání komunikace. Jelikož v zadání není specifikováno, jestli je nutné, aby server odpovídal na vícero požadavků najednou, tak jsem tuto funkcionalitu neimplementoval.
# Spuštění
 - server lze spustit i bez souboru Makefile a to přes cmd ./IPK_proj1.py <PORT>, kde server naslouchá, dokud není ukončen signálem sig int (ctrl + c).
