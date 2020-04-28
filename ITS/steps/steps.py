from behave import given, when, then
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.common.action_chains import ActionChains
from selenium.webdriver.support import expected_conditions
from selenium.webdriver.support.wait import WebDriverWait
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.common.desired_capabilities import DesiredCapabilities
from selenium.webdriver.support.ui import Select
from selenium.common.exceptions import NoSuchElementException
from selenium.common.exceptions import NoAlertPresentException

@given(u'Výskyt na stránce "{registrace}"')
def step_impl(context, registrace):

    #prejde na stranku registace
    context.driver.get(context.base_url + registrace)

@when(u'Provede registraci se správnými údaji "{mail}" "{heslo}"')
def step_impl(context, mail, heslo):
    context.driver.find_element(By.ID, "input-firstname").click()
    context.driver.find_element(By.ID, "input-firstname").send_keys("First")
    context.driver.find_element(By.ID, "input-lastname").click()
    context.driver.find_element(By.ID, "input-lastname").send_keys("Second")
    context.driver.find_element(By.ID, "input-email").click()
    context.driver.find_element(By.ID, "input-email").send_keys(mail)
    context.driver.find_element(By.ID, "input-telephone").click()
    context.driver.find_element(By.ID, "input-telephone").send_keys("1122")
    context.driver.find_element(By.ID, "input-address-1").click()
    context.driver.find_element(By.ID, "input-address-1").send_keys("ssd")
    context.driver.find_element(By.ID, "input-city").click()
    context.driver.find_element(By.ID, "input-city").send_keys("ssd")
    context.driver.find_element(By.ID, "input-postcode").click()
    context.driver.find_element(By.ID, "input-postcode").send_keys("4455")
    context.driver.find_element(By.ID, "input-city").click()
    context.driver.find_element(By.ID, "input-country").click()
    context.driver.find_element(By.ID, "input-country").click()
    context.driver.find_element(By.ID, "input-zone").click()
    dropdown = context.driver.find_element(By.ID, "input-zone")
    dropdown.find_element(By.XPATH, "//option[. = 'Caerphilly']").click()
    context.driver.find_element(By.CSS_SELECTOR, "#input-zone > option:nth-child(13)").click()
    context.driver.find_element(By.ID, "input-password").click()
    context.driver.find_element(By.ID, "input-password").send_keys(heslo)
    context.driver.find_element(By.ID, "input-confirm").click()
    context.driver.find_element(By.ID, "input-confirm").send_keys(heslo)
    context.driver.find_element(By.NAME, "agree").click()
    context.driver.find_element(By.CSS_SELECTOR, ".btn-primary").click()

@then(u'Objeví se potvrzení o registraci')
def step_impl(context):
    WebDriverWait(context.driver, 5).until(expected_conditions.text_to_be_present_in_element((By.CSS_SELECTOR, "h1"), "Your Account Has Been Created!"))
    assert context.driver.find_element(By.CSS_SELECTOR, "h1").text == "Your Account Has Been Created!"

@given(u'Uživatel je odhlášen')
def step_impl(context):
    context.driver.get(context.base_url + "index.php?route=account/logout")

@given(u'Vyskytuje se na stránce "{prihlaseni}"')
def step_impl(context, prihlaseni):
    context.driver.get(context.base_url + prihlaseni)

@when(u'Zadá správné údaje "{mail}" "{heslo}"')
def step_impl(context, mail, heslo):
    WebDriverWait(context.driver, 2).until(expected_conditions.text_to_be_present_in_element((By.CSS_SELECTOR, ".col-sm-6:nth-child(2) h2"), "Returning Customer"))
    context.driver.find_element(By.ID, "input-email").click()
    context.driver.find_element(By.ID, "input-email").send_keys(mail)
    context.driver.find_element(By.ID, "input-password").click()
    context.driver.find_element(By.ID, "input-password").send_keys(heslo)
    context.driver.find_element(By.CSS_SELECTOR, ".btn:nth-child(3)").click()

@then(u'Dojde k přihlášení')
def step_impl(context):
    WebDriverWait(context.driver, 2)
    url = context.driver.current_url
    assert "account/account" in url

@given(u'Uživatel "{mail}" "{heslo}" je přihlášen')
def step_impl(context, mail, heslo):
    context.driver.find_element(By.CSS_SELECTOR, ".list-inline .dropdown-toggle").click()
    elements = context.driver.find_elements(By.LINK_TEXT, "Logout")
    if len(elements) == 0:
        context.driver.get(context.base_url + "index.php?route=account/login")
        WebDriverWait(context.driver, 5).until(expected_conditions.text_to_be_present_in_element((By.CSS_SELECTOR, ".col-sm-6:nth-child(2) h2"), "Returning Customer"))
        context.driver.find_element(By.ID, "input-email").click()
        context.driver.find_element(By.ID, "input-email").send_keys(mail)
        context.driver.find_element(By.ID, "input-password").click()
        context.driver.find_element(By.ID, "input-password").send_keys(heslo)
        context.driver.find_element(By.CSS_SELECTOR, ".btn:nth-child(3)").click()
        context.driver.find_element(By.CSS_SELECTOR, ".list-inline .dropdown-toggle").click()
        elements = context.driver.find_elements(By.LINK_TEXT, "Logout")
    assert len(elements) > 0

@when(u'Zadá správně nové heslo "{heslo}"')
def step_impl(context, heslo):
    context.driver.find_element(By.ID, "input-password").click()
    context.driver.find_element(By.ID, "input-password").send_keys(heslo)
    context.driver.find_element(By.ID, "input-confirm").click()
    context.driver.find_element(By.ID, "input-confirm").send_keys(heslo)

@when(u'Uloží změny')
def step_impl(context):
    context.driver.find_element(By.CSS_SELECTOR, ".btn-primary").click()    

@then(u'Potvrdí se změna hesla')
def step_impl(context):
    WebDriverWait(context.driver, 2)
    assert context.driver.find_element(By.CSS_SELECTOR, ".alert").text == "Success: Your password has been successfully updated."

@when(u'Změní informaci "{phone}"')
def step_impl(context, phone):
    context.driver.find_element(By.ID, "input-telephone").click()
    context.driver.find_element(By.ID, "input-telephone").send_keys(phone)

@then(u'Potvrdí se změna informací')
def step_impl(context):
    WebDriverWait(context.driver, 2)
    assert context.driver.find_element(By.CSS_SELECTOR, ".alert").text == "Success: Your account has been successfully updated."

@when(u'Přidá produkt do košíku')
def step_impl(context):
    context.driver.find_element(By.ID, "button-cart").click()

@then(u'"{produkt}" se objeví v košíku')
def step_impl(context, produkt):
    context.driver.get(context.base_url + "index.php?route=checkout/cart")
    WebDriverWait(context.driver, 2)
    elements = context.driver.find_elements(By.LINK_TEXT, produkt)
    assert len(elements) > 0

@given(u'Má "{produkt}" v košíku')
def step_impl(context, produkt):
    elements = context.driver.find_elements(By.LINK_TEXT, produkt)
    if len(elements) == 0:
        context.driver.get(context.base_url + "index.php?route=product/product&product_id=40")
        WebDriverWait(context.driver, 1)
        context.driver.find_element(By.ID, "button-cart").click()
        context.driver.get(context.base_url + "index.php?route=checkout/cart")
        WebDriverWait(context.driver, 1)
        elements = context.driver.find_elements(By.LINK_TEXT, produkt)
    assert len(elements) > 0

@when(u'Vyplní údaje o objednávce')
def step_impl(context):
    context.driver.find_element(By.LINK_TEXT, "Checkout").click()
    WebDriverWait(context.driver, 8).until(expected_conditions.element_to_be_clickable((By.ID, "button-payment-address")))
    context.driver.find_element(By.ID, "button-payment-address").click()
    element = context.driver.find_element(By.ID, "button-payment-address")
    actions = ActionChains(context.driver)
    actions.move_to_element(element).perform()
    WebDriverWait(context.driver, 8).until(expected_conditions.element_to_be_clickable((By.ID, "button-shipping-address")))
    context.driver.find_element(By.ID, "button-shipping-address").click()
    WebDriverWait(context.driver, 8).until(expected_conditions.element_to_be_clickable((By.ID, "button-shipping-method")))
    context.driver.find_element(By.ID, "button-shipping-method").click()
    WebDriverWait(context.driver, 2).until(expected_conditions.text_to_be_present_in_element((By.CSS_SELECTOR, ".buttons:nth-child(5) > .pull-right"), "I have read and agree to the Terms & Conditions  "))
    context.driver.find_element(By.NAME, "agree").click()
    WebDriverWait(context.driver, 8).until(expected_conditions.element_to_be_clickable((By.ID, "button-payment-method")))
    context.driver.find_element(By.ID, "button-payment-method").click()
    WebDriverWait(context.driver, 8).until(expected_conditions.element_to_be_clickable((By.ID, "button-confirm")))
    context.driver.find_element(By.ID, "button-confirm").click()

@then(u'Vytvoří se objednávka')
def step_impl(context):
    WebDriverWait(context.driver, 6).until(expected_conditions.text_to_be_present_in_element((By.CSS_SELECTOR, "h1"), "Your order has been placed!"))
    assert context.driver.find_element(By.CSS_SELECTOR, "h1").text == "Your order has been placed!"

@when(u'Přidá produkt na wishlist')
def step_impl(context):
    context.driver.find_element(By.CSS_SELECTOR, ".btn > .fa-heart").click()

@then(u'Produkt se přidá na wishlist')
def step_impl(context):
    context.driver.get(context.base_url + "index.php?route=account/wishlist")
    elements = context.driver.find_elements(By.LINK_TEXT, "MacBook")
    assert len(elements) > 0

@when(u'Pokusí se zakoupit produkt')
def step_impl(context):
    context.driver.find_element(By.LINK_TEXT, "Checkout").click()

@then(u'Produkt nelze zakoupit')
def step_impl(context):
    url = context.driver.current_url
    assert "checkout/cart" in url
