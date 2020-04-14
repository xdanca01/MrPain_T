Feature: Správa účtu na webu
    Scenario: Registrace nového uživatele
        Given Výskyt na stránce registrace nového účtu
        When Provede registraci se správnými údaji
        Then Objeví se potvrzení o registraci

    Scenario: Přihlášení uživatele
        Given Uživatel je zaregistrován
        And Vyskytuje se na stránce pro přihlášení
        When Zadá správné údaje
        Then Dojde k přihlášení

    Scenario: Změna hesla
        Given Uživatel je přihlášen
        And Vyskytuje se na stránce pro změnu hesla
        When Zadá správně nové heslo
        Then Potvrdí se změna hesla
        
    Scenario: Změna osobních informací
        Given Uživatel je přihlášen
        And Vyskytuje se na stránce pro změnu osobních informací
        When Změní některou z informací v údajích
        Then Potvrdí se změna informací
