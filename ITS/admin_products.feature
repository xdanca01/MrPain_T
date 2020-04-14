Feature: Správa produktů v nabídce obchodu
    Scenario: Přidání produktu do nabídky
        Given Uživatel má administrátorská práva
        And Vyskytuje se na stránce seznamu produktů
        When Vytvoří nový produkt
        And Vyplní informace o produktu
        Then Produkt se objeví v nabídce

    Scenario: Změna statusu produktu
        Given Uživatel má administrátorská práva
        And Vyskytuje se na stránce seznamu produktů
        And Existuje nějaký produkt
        When Změní status produktu
        Then Změní se status produktu

    Scenario: Změna počtu kusů produktu
        Given Uživatel má administrátorská práva
        And Vyskytuje se na stránce seznamu produktů
        And Existuje nějaký produkt
        When Změní počet kusů produktu
        Then Změní se počet kusů produktu

    Scenario: Změna ceny produktu
        Given Uživatel má administrátorská práva
        And Vyskytuje se na stránce seznamu produktů
        And Existuje produkt
        When Změní cenu produktu
        Then Změní se cena produktu
