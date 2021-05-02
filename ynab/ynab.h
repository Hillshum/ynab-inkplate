


#include "HTTPClient.h"
#include <ArduinoJson.h>
#include "arduino_secrets.h"

#define CATEGORY_NAME_LEN 30

struct Category {
    int balance;
    char name[CATEGORY_NAME_LEN];
    bool success = false;
};

bool getTime()
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
    time_t unixTime = response["unixtime"];
    timeval time {unixTime, 0};
    settimeofday(&time, nullptr);
    // strcpy(result, datetime);
    return true;

}


int extractCategory(Category &category, const char * providedId, const JsonArray &categories)
{
    for (JsonObject cat : categories)
    {
        const char * id = cat["id"];
        if (strcmp(id, providedId))
        {
            continue;
        }

        category.balance = cat["balance"];
        strcpy(category.name, cat["name"]);
        return 1;
    }
    return 0;
}

void getTime(char * result, int result_len, char * strfmt)
{

    time_t now;
    struct tm timeinfo;

    time(&now);
    setenv("TZ", "CDT+5", 1);
    tzset();

    localtime_r(&now, &timeinfo);
    strftime(result, result_len, strfmt, &timeinfo);
    Serial.printf("Time is %s\n", result);

}

int getBudgetInfo(Category results[])
{



    HTTPClient http;
    http.useHTTP10(true);

    char url[70];

    char strftime_buf[64];
    getTime(strftime_buf, sizeof(strftime_buf), "%Y-%m-%d");
    sprintf(url, "https://api.youneedabudget.com/v1/budgets/last-used/months/%.7s-01", strftime_buf);

    Serial.println(url);
    http.begin(url);

    Serial.println("began requet");
    char authHeader[80];
    sprintf(authHeader, "Bearer %s", SECRET_YNAB_KEY);
    http.addHeader("Authorization", authHeader);
    Serial.println("added header");

    int httpCode = http.GET();

    Serial.println("got response");
    Serial.printf("budget request returned %d\n", httpCode);
    if (httpCode > 399)
    {
        return 0;
    }

    DynamicJsonDocument response(49152);
    DeserializationError err = deserializeJson(response, http.getStream());
    if (err)
    {
        Serial.print("err: ");
        Serial.println(err.c_str());
        return 0;
    }
    Serial.println("decoded");
    JsonObject data_month = response["data"]["month"];
    

    int found_categories = 0;
    JsonArray categories = data_month["categories"].as<JsonArray>();

    for (int i = 0; i < SECRET_NUM_CATEGORIES; i++)
    {
        found_categories += extractCategory(results[i], CATEGORY_IDS[i], categories);
    }

    return found_categories;

}

