Feature: Nakupování na webu
    Scenario: Přidání do košíku
        Given Uživatel je přihlášen
        And Vyskytuje se na stránce produktu
        When Přidá produkt do košíku
        Then Produkt se objeví v košíku

    Scenario: Vytvoření objednávky
        Given Uživatel je přihlášen
        And Vyskytuje se na stránce košíku
        And Má produkt v košíku
        When Vyplní údaje o objednávce
        Then Vytvoří se objednávka

    Scenario: Přidání produktu na wish list
        Given Uživatel je přihlášen
        And Na stránce produktu
        When Přidá produkt na wishlist
        Then Produkt se přidá na wishlist

    Scenario: Přidání produktu co není na skladě do košíku
        Given Uživatel je přihlášen
        And Vyskytuje se na stránce produktu
        When Přidá produkt do košíku
        Then Produkt se přidá do košíku

    Scenario: Pokus o zakoupení produktu, který není na skladě
        Given Uživatel je přihlášen
        And Má v košíku produkt, který není na skladě
        When Pokusí se zakoupit produkt
        Then Produkt nelze zakoupit

    Scenario: Stav objednávky
        Given Uživatel je přihlášen
        And provedl objednávku
        When Vyskytuje se na stránce o historii objednávek
        Then Zobrazí se provedené objednávky

    Scenario: Nákup s kupónem
        Given Uživatel je přihlášen
        And Má v košíku produkt
        When Zadá kupón na slevu
        Then Zobrazí se zlevněná částka


