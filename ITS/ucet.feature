Feature: Správa účtu na webu
    Scenario: Registrace nového uživatele
        Given Výskyt na stránce "index.php?route=account/register"
        When Provede registraci se správnými údaji "MYACCFREE@mail.com" "heslo"
        Then Objeví se potvrzení o registraci

    Scenario: Přihlášení uživatele
        Given Uživatel je odhlášen
        And Vyskytuje se na stránce "index.php?route=account/login"
        When Zadá správné údaje "MYACCFREE@mail.com" "heslo"
        Then Dojde k přihlášení

    Scenario: Změna hesla
        Given Uživatel "MYACCFREE@mail.com" "heslo" je přihlášen
        And Vyskytuje se na stránce "index.php?route=account/password"
        When Zadá správně nové heslo "heslo1"
        And Uloží změny
        Then Potvrdí se změna hesla
        
    Scenario: Změna osobních informací
        Given Uživatel "MYACCFREE@mail.com" "heslo1" je přihlášen
        And Vyskytuje se na stránce "index.php?route=account/edit"
        When Změní informaci "1144"
        And Uloží změny
        Then Potvrdí se změna informací
