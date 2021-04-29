#ifndef DEFINES_H
#define DEFINES_H

#define INVALID_ID 0

#define SETTINGS_COMPANY_NAME "LEICO"
#define SETTINGS_APP_NAME     "BaitaAssistente"

#define SQL_COLID "_ID"
#define SQL_COLID_NUMBER 0

#define POSTGRE_CONNECTION_NAME "POSTGRE"

#define ADDRESS_SQL_TABLE_NAME "_ADDRESSES"
#define ADDRESS_SQL_COL_FID "_FORMID"
#define ADDRESS_SQL_COL_CEP "_CEP"
#define ADDRESS_SQL_COL_NEI "_NEIGHBORHOOD"
#define ADDRESS_SQL_COL_STR "_STREET"
#define ADDRESS_SQL_COL_NUM "_NUMBER"
#define ADDRESS_SQL_COL_CIT "_CITY"
#define ADDRESS_SQL_COL_STA "_STATE"
#define ADDRESS_SQL_COL_COM "_COMPLEMENT"
#define ADDRESS_SQL_COL_REF "_REFERENCE"
#define ADDRESS_FORM_FILTER ADDRESS_SQL_COL_FID " = "
#define ADDRESS_STARTING_NUMBER 0
#define ADDRESS_MAX_STREET_LENGTH 35
#define ADDRESS_MAX_NUMBER 35
#define ADDRESS_NUMBER_OF_BRAZILIAN_STATES 27
#define ADDRESS_CEP_LENGTH 8
#define ADDRESS_CEP_MASK "99999-999;_"
#define ADDRESS_CEP_LENGTH_WITH_MASK  ADDRESS_CEP_LENGTH + 1

#define CATEGORY_SQL_TABLE_NAME "_CATEGORIES"
#define CATEGORY_SQL_COL_IID "_IMAGEID"
#define CATEGORY_SQL_COL_NAM "_NAME"
#define CATEGORY_MAX_NAME_LENGTH 35

#define IMAGE_SQL_TABLE_NAME "_IMAGES"
#define IMAGE_SQL_COL_NAM "_NAME"
#define IMAGE_SQL_COL_IMA "_IMAGE"
#define IMAGE_MAX_NAME_LENGTH 35
#define IMAGE_MAX_SIZE 1048576 //1MB

#define PURCHASE_SQL_TABLE_NAME "_NOTES"
#define PURCHASE_SQL_COL_NUM "_NUMBER"
#define PURCHASE_SQL_COL_DAT "_DATE"
#define PURCHASE_SQL_COL_SID "_SUPPLIERID"
#define PURCHASE_SQL_COL_OBS "_OBSERVATION"
#define PURCHASE_SQL_COL_DIS "_DISCCOUNT"
#define PURCHASE_SQL_COL_EID "_EMPLOYEEID"
#define PURCHASE_SQL_COL_PAY "_PAYMENT_METHOD"
#define PURCHASE_SQL_COL_TID "_STOREID"

#define PURCHASE_ELEMENTS_SQL_TABLE_NAME "_NOTE_ITEMS"
#define PURCHASE_ELEMENTS_SQL_COL_NID "_NOTEID"
#define PURCHASE_ELEMENTS_SQL_COL_PID "_PRODUCTID"
#define PURCHASE_ELEMENTS_SQL_COL_AMM "_AMMOUNT"
#define PURCHASE_ELEMENTS_SQL_COL_PRI "_PRICE"
#define PURCHASE_ELEMENTS_SQL_COL_ISP "_IS_PACK"
#define PURCHASE_ELEMENTS_SQL_COL_PUN "_PACK_UNITY"
#define PURCHASE_ELEMENTS_SQL_COL_PAM "_PACK_AMMOUNT"

#define PURCHASE_MAX_NUMBER_OF_ITEMS 100
#define PURCHASE_DEFAULT_NUMBER 1000

#define PAYMENT_ELEMENTS_SQL_TABLE_NAME "_NOTE_PAYMENT_ITEMS"
#define PAYMENT_ELEMENTS_SQL_COL_NID "_NOTEID"
#define PAYMENT_ELEMENTS_SQL_COL_DAT "_DATE"
#define PAYMENT_ELEMENTS_SQL_COL_VAL "_VALUE"

#define EMPLOYEE_SQL_TABLE_NAME "_EMPLOYEES"
#define EMPLOYEE_SQL_COL_FID "_FORMID"
#define EMPLOYEE_SQL_COL_PIN "_PINCODE"
#define EMPLOYEE_SQL_COL_NED "_NOTE_EDIT"
#define EMPLOYEE_SQL_COL_NRE "_NOTE_REMOVE"
#define EMPLOYEE_SQL_COL_RED "_REMINDER_EDIT"
#define EMPLOYEE_SQL_COL_RRE "_REMINDER_REMOVE"
#define EMPLOYEE_SQL_COL_CED "_COUPON_EDIT"
#define EMPLOYEE_SQL_COL_CRE "_COUPON_REMOVE"
#define EMPLOYEE_UNIQUE_FORM_FILTER SQL_COLID " NOT IN (SELECT " EMPLOYEE_SQL_COL_FID " FROM " EMPLOYEE_SQL_TABLE_NAME ")"

#define SUPPLIER_SQL_TABLE_NAME "_SUPPLIERS"
#define SUPPLIER_SQL_COL_FID "_FORMID"
#define SUPPLIER_UNIQUE_FORM_FILTER SQL_COLID " NOT IN (SELECT " SUPPLIER_SQL_COL_FID " FROM " SUPPLIER_SQL_TABLE_NAME ")"

#define FORM_SQL_TABLE_NAME "_FORMS"
#define FORM_SQL_COL_IID "_IMAGEID"
#define FORM_SQL_COL_NAM "_NAME"
#define FORM_SQL_COL_ALI "_ALIAS"
#define FORM_SQL_COL_EMA "_EMAIL"
#define FORM_SQL_COL_CPF "_CPF_CNPJ"
#define FORM_SQL_COL_RGI "_RG_IE"
#define FORM_SQL_COL_DET "_DETAILS"
#define FORM_SQL_COL_BIR "_BIRTHDATE"
#define FORM_SQL_COL_CRE "_CREATIONDATE"
#define FORM_SQL_COL_ISC "_IS_COMPANY"
#define FORM_SQL_COL_HBI "_HAS_BIRTHDATE"
#define FORM_MAX_NAME_LENGTH 35
#define FORM_MAX_DETAILS_LENGTH 35
#define FORM_FILTER_COMPANY FORM_SQL_TABLE_NAME "." FORM_SQL_COL_ISC " = TRUE"

#define PHONE_SQL_TABLE_NAME "_PHONES"
#define PHONE_SQL_COL_FID "_FORMID"
#define PHONE_SQL_COL_COU "_COUNTRYCODE"
#define PHONE_SQL_COL_COD "_CODE"
#define PHONE_SQL_COL_NUM "_NUMBER"
#define PHONE_SQL_COL_NAM "_NAME"
#define PHONE_FORM_FILTER PHONE_SQL_COL_FID " = "
#define PHONE_DEFAULT_COUNTRY_CODE_VALUE 55
#define PHONE_DEFAULT_CODE_VALUE 54
#define PHONE_DEFAULT_COUNTRY_CODE_VALUE_STR "55"
#define PHONE_DEFAULT_CODE_VALUE_STR "54"

#define EMPLOYEE_PINCODE_LENGTH 4

#define PRODUCT_SQL_TABLE_NAME "_PRODUCTS"
#define PRODUCT_SQL_COL_NAM "_NAME"
#define PRODUCT_SQL_COL_CID "_CATEGORYID"
#define PRODUCT_SQL_COL_IID "_IMAGEID"
#define PRODUCT_SQL_COL_UNI "_UNITY"
#define PRODUCT_SQL_COL_DET "_DETAILS"
#define PRODUCT_SQL_COL_ATB "_AVAILABLE_TO_BUY"
#define PRODUCT_SQL_COL_ATS "_AVAILABLE_TO_SELL"

#define PRODUCT_MAX_NAME_LENGTH 48
#define PRODUCT_MAX_UNITY_LENGTH 10
#define PRODUCT_MAX_DETAILS_LENGTH 35
#define PRODUCT_FILTER_BUY PRODUCT_SQL_TABLE_NAME "." PRODUCT_SQL_COL_ATB " = TRUE"
#define PRODUCT_FILTER_SELL PRODUCT_SQL_TABLE_NAME "." PRODUCT_SQL_COL_ATS " = TRUE"

#define PRODUCT_CODE_ITEMS_SQL_TABLE_NAME "_PRODUCT_CODE_ITEMS"
#define PRODUCT_CODE_ITEMS_SQL_COL_PID "_PRODUCTID"
#define PRODUCT_CODE_ITEMS_SQL_COL_COD "_CODE"

#define USER_SQL_TABLE_NAME "_USERS"
#define USER_SQL_COL_USE "_USER"
#define USER_SQL_COL_PAS "_PASSWORD"
#define USER_SQL_COL_APU "_ACCESS_NOTE"
#define USER_SQL_COL_ARE "_ACCESS_REMINDER"
#define USER_SQL_COL_ACA "_ACCESS_CALCULATOR"
#define USER_SQL_COL_ASH "_ACCESS_SHOP"
#define USER_SQL_COL_AUS "_ACCESS_USER"
#define USER_SQL_COL_APR "_ACCESS_PRODUCT"
#define USER_SQL_COL_ASE "_ACCESS_SETTINGS"
#define USER_SQL_COL_ACT "_ACCESS_CATEGORY"
#define USER_SQL_COL_AIM "_ACCESS_IMAGE"
#define USER_SQL_COL_ASL "_ACCESS_SHOPPING_LIST"
#define USER_SQL_COL_AEM "_ACCESS_EMPLOYEE"
#define USER_SQL_COL_ASU "_ACCESS_SUPPLIER"
#define USER_SQL_COL_AST "_ACCESS_STORE"
#define USER_SQL_COL_ATI "_ACCESS_TIMECARD"
#define USER_SQL_COL_ACO "_ACCESS_COUPON"
#define USER_SQL_COL_ACR "_ACCESS_COUPON_REDEMPTION"
#define USER_SQL_COL_ART "_ACCESS_REPORTS"
#define USER_SQL_DEFAULT_NAME "ADMIN"
#define USER_SQL_DEFAULT_PASSWORD "admin"
#define USER_MAX_USERNAME_LENGTH 20
#define USER_MAX_PASSWORD_LENGTH 20

#define REMINDER_SQL_TABLE_NAME "_REMINDERS"
#define REMINDER_SQL_COL_TIT "_TITLE"
#define REMINDER_SQL_COL_MES "_MESSAGE"
#define REMINDER_SQL_COL_FAV "_FAVORITE"
#define REMINDER_SQL_COL_CAP "_CAPITALIZATION"
#define REMINDER_SQL_COL_SIZ "_SIZE"
#define REMINDER_SQL_COL_HRI "_BARCODEHRI"
#define REMINDER_SQL_COL_BAR "_BARCODE"
#define REMINDER_SQL_COL_DAT "_DATE"
#define REMINDER_SQL_COL_HDA "_HAS_DATE"
#define REMINDER_SQL_COL_TIM "_TIME"
#define REMINDER_SQL_COL_HTI "_HAS_TIME"
#define REMINDER_SQL_COL_SUB "_SUBJECT"

#define REMINDER_MAX_BARCODE_CODE93_LENGTH 255

#define SHOPPING_LIST_SQL_TABLE_NAME "_SHOPPING_LISTS"
#define SHOPPING_LIST_SQL_COL_SID "_SUPPLIERID"
#define SHOPPING_LIST_SQL_COL_IID "_IMAGEID"
#define SHOPPING_LIST_SQL_COL_TIT "_TITLE"
#define SHOPPING_LIST_SQL_COL_DES "_DESCRIPTION"
#define SHOPPING_LIST_SQL_COL_WEE "_WEEK_DAYS"
#define SHOPPING_LIST_SQL_COL_MON "_MONTH_DAYS"
#define SHOPPING_LIST_SQL_COL_NLI "_NUMBER_LINES"

#define SHOPPING_LIST_ELEMENTS_SQL_TABLE_NAME "_SHOPPING_LIST_ITEMS"
#define SHOPPING_LIST_ELEMENTS_SQL_LID "_SHOPPINGLISTID"
#define SHOPPING_LIST_ELEMENTS_SQL_PID "_PRODUCTID"
#define SHOPPING_LIST_ELEMENTS_SQL_AMM "_AMMOUNT"
#define SHOPPING_LIST_ELEMENTS_SQL_PRI "_PRICE"
#define SHOPPING_LIST_ELEMENTS_SQL_ISP "_IS_PACK"
#define SHOPPING_LIST_ELEMENTS_SQL_PUN "_PACK_UNITY"
#define SHOPPING_LIST_ELEMENTS_SQL_PAM "_PACK_AMMOUNT"
#define SHOPPING_LIST_ELEMENTS_SQL_SID "_SUPPLIERID"

#define SHOPPING_LIST_NUMBER_OF_COLUMNS 5
#define SHOPPING_LIST_MAX_NUMBER_OF_ITEMS 100
#define SHOPPING_LIST_SEPARATOR ";"

#define LOGIN_SQL_TABLE_NAME "_LOGIN"
#define LOGIN_SQL_COL_PID "_POSTGRE_PID"
#define LOGIN_SQL_COL_UID "_USERID"
#define LOGIN_SQL_COL_MAC "_MACHINE"
#define LOGIN_SQL_COL_LOG "_LOGIN"

#define STORE_SQL_TABLE_NAME "_STORES"
#define STORE_SQL_COL_FID "_FORMID"
#define STORE_SQL_COL_DES "_DESCRIPTION"

#define STORE_EMPLOYEES_SQL_TABLE_NAME "_STORE_EMPLOYEES"
#define STORE_EMPLOYEES_SQL_COL_EID "_EMPLOYEEID"
#define STORE_EMPLOYEES_SQL_COL_SID "_STOREID"

#define EMPLOYEE_HOURS_SQL_TABLE_NAME "_EMPLOYEE_HOURS"
#define EMPLOYEE_HOURS_SQL_COL_EID "_EMPLOYEEID"
#define EMPLOYEE_HOURS_SQL_COL_DAY "_DAY"
#define EMPLOYEE_HOURS_SQL_COL_BEG "_BEGIN"
#define EMPLOYEE_HOURS_SQL_COL_END "_END"

#define COUPON_SQL_TABLE_NAME "_COUPONS"
#define COUPON_SQL_COL_TYP "_TYPE"
#define COUPON_SQL_COL_COD "_CODE"
#define COUPON_SQL_COL_CDT "_CREATION_TIMESTAMP"
#define COUPON_SQL_COL_RED "_REDEEMED"
#define COUPON_SQL_COL_RDT "_REDEMPTION_TIMESTAMP"
#define COUPON_SQL_COL_EXP "_EXPIRES"
#define COUPON_SQL_COL_EDT "_EXPIRATION_DATE"
#define COUPON_SQL_COL_PCT "_PERCENTAGE"
#define COUPON_SQL_COL_VAL "_VALUE"
#define COUPON_SQL_COL_SID "_STOREID"

#define COUPON_ELEMENTS_SQL_TABLE_NAME "_COUPON_ELEMENTS"
#define COUPON_ELEMENTS_SQL_COL_CID "_COUPONID"
#define COUPON_ELEMENTS_SQL_COL_PID "_PRODUCTID"
#define COUPON_ELEMENTS_SQL_COL_AMM "_AMMOUNT"

#define PRICE_SQL_TABLE_NAME "_PRICES"
#define PRICE_SQL_COL_PID "_PRODUCTID"
#define PRICE_SQL_COL_VAL "_VALUE"

#endif // DEFINES_H
