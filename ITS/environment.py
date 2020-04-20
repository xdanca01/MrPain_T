from behave import *
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
import re, time, unittest


def before_feature(context, feature):
    #self.driver = webdriver.Remote(command_executor='http://mys01.fit.vutbr.cz:8013/wd/hub', desired_capabilities=DesiredCapabilities.FIREFOX)
    context.driver = webdriver.Firefox()
    context.driver.implicitly_wait(30)
    context.base_url = "http://mys01.fit.vutbr.cz:8013/"
    context.verificationErrors = []
    context.accept_next_alert = True

def after_feature(context, feature):
    context.driver.close()


if __name__ == "__main__":
    unittest.main()
