


#include "HTTPClient.h"
#include <ArduinoJson.h>
#include "arduino_secrets.h"

#define CATEGORY_NAME_LEN 30

struct Category {
    int balance;
    char name[CATEGORY_NAME_LEN];
    bool success = false;
};

bool getTime(char * result)
{

    HTTPClient http;
    http.begin("http://worldtimeapi.org/api/ip");
    int httpCode = http.GET();

    if (httpCode > 399)
    {
        Serial.println(httpCode);
        return false;
    }
    StaticJsonDocument<768> response;
    String r = http.getString();
    Serial.println(r);
    DeserializationError err = deserializeJson(response, r);
    if (err)
    {
        Serial.print("Err in getting time: ");
        Serial.println(err.c_str());
        return false;
    }

    const char* datetime = response["datetime"];
    strcpy(result, datetime);
    return true;

}

Category getCategory(const char * month, const char * categoryName)
{
    HTTPClient http;

    char url[256];

    sprintf(url, "https://api.youneedabudget.com/v1/budgets/last-used/months/%.10s/categories/%s", month, categoryName);

    Serial.println(url);
    http.begin(url);

    char authHeader[80];
    sprintf(authHeader, "Bearer %s", SECRET_YNAB_KEY);
    http.addHeader("Authorization", authHeader);



    int httpCode = http.GET();

    if (httpCode > 399)
    {
        char returnval[3];
        sprintf(returnval, "%d", httpCode);
        return {};
    }

    StaticJsonDocument<768> response;

    Serial.println("allocated response");
    String resp = http.getString();
    Serial.println(resp);
    DeserializationError err = deserializeJson(response, resp);
    if (err)
    {
        Serial.print("err: ");
        Serial.println(err.c_str());
        return {};
    }

    Serial.println("decoded");

    int balance = response["data"]["category"]["balance"];

    Category category;
    category.balance = balance;
    strcpy(category.name, response["data"]["category"]["name"]);
    category.success = true;
    Serial.println("copied label");
    return category;
}