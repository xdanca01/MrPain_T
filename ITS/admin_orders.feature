Feature: Administrátoská správa objednávek
    Scenario: Změna statusu objednávky
        Given Uživatel má administrátorská práva
        And Vyskytuje se na stránce s objednávkami
        And Existuje objednávka
        When Změní status objednávky
        Then Změní se status objednávky

    Scenario: Změna měny objednávky
        Given Uživatel má administrátorská práva
        And Vyskytuje se na stránce s objednávkami
        And Existuje objednávka
        When Změní měnu objednávky
        Then Změní se cena objednávky

    Scenario: Přidání produktu do objednávky
        Given Uživatel má administrátorská práva
        And Vyskytuje se na stránce s objednávkami
        And Existuje objednávka
        When Přidá se produkt do objednávky
        Then Změní se cena objednávky

    Scenario: Smazání objednávky
        Given Uživatel má administrátorská práva
        And Vyskytuje se na stránce s objednávkami
        And Existuje objednávka
        When Smaže objednávku
        Then Objednávka se nezobrazí
