Feature: Nakupování na webu
    Scenario: Registrace uživatele
        Given Výskyt na stránce "index.php?route=account/register"
        When Provede registraci se správnými údaji "test2@mail.com" "heslo"
        Then Objeví se potvrzení o registraci

    Scenario: Přidání do košíku
        Given Uživatel "test2@mail.com" "heslo" je přihlášen
        And Vyskytuje se na stránce "index.php?route=product/product&product_id=40"
        When Přidá produkt do košíku
        Then "iPhone" se objeví v košíku

    Scenario: Vytvoření objednávky
        Given Uživatel "test2@mail.com" "heslo" je přihlášen
        And Vyskytuje se na stránce "index.php?route=checkout/cart"
        And Má "iPhone" v košíku
        When Vyplní údaje o objednávce
        Then Vytvoří se objednávka

    Scenario: Přidání produktu na wish list
        Given Uživatel "test2@mail.com" "heslo" je přihlášen
        And Vyskytuje se na stránce "index.php?route=product/product&product_id=43"
        When Přidá produkt na wishlist
        Then Produkt se přidá na wishlist

    Scenario: Přidání produktu co není na skladě do košíku
        Given Uživatel "test2@mail.com" "heslo" je přihlášen
        And Vyskytuje se na stránce "index.php?route=product/product&path=57&product_id=49"
        When Přidá produkt do košíku
        Then "Samsung Galaxy Tab 10.1" se objeví v košíku

    Scenario: Pokus o zakoupení produktu, který není na skladě
        Given Uživatel "test2@mail.com" "heslo" je přihlášen
        And Má "Samsung Galaxy Tab 10.1" v košíku
        When Pokusí se zakoupit produkt
        Then Produkt nelze zakoupit

