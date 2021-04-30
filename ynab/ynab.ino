

// Next 3 lines are a precaution, you can ignore those, and the example would also work without them
#ifndef ARDUINO_ESP32_DEV
#error "Wrong board selection for this example, please select Inkplate 6 in the boards menu."
#endif


#include <WiFi.h>

#include "Inkplate.h"
Inkplate display(INKPLATE_3BIT);

#include "ynab.h"
#include "generatedUI.h"
#include "arduino_secrets.h"

const GFXfont *text_font = &FreeSansBold24pt7b;
#define DELAY_SECS 60

int delayMillis = DELAY_SECS * 1000;


int num = 0;

char month[64] ="";


void connectWifi()
{

    int ConnectCount = 20;

    if (WiFi.status() != WL_CONNECTED)
    {
        while (WiFi.status() != WL_CONNECTED)
        {
            if (ConnectCount++ == 20)
            {
                Serial.println("Connect WiFi");
                WiFi.begin(SECRET_SSID, SECRET_WIFI_PW);
                Serial.print("Connecting.");
                ConnectCount = 0;
            }
            Serial.print(".");
            delay(1000);
        }
        Serial.print("\nConnected to: ");
        Serial.println(SECRET_SSID);
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
        Serial.println("Connected WiFi");
    }
} //======================== END WIFI CONNECT =======================


char categories[SECRET_NUM_CATEGORIES][38] = {
    SECRET_CATEGORY_FUN,
    SECRET_CATEGORY_MEALS
};

void setup()
{

    Serial.begin(115200);
    while (!Serial)
    {
        ;
    }

    Serial.println("serial monitor initalized");

    display.begin();
    display.clearDisplay();
    display.display();

    display.setCursor(150, 320);

    display.setTextSize(4);
    display.setFont(text_font);
    display.print("Welcome to YNAB");
    display.display();
    delay(5000);
}


void formatCurrency(char * output, int currency)
{
    float divided = currency / 1000.0;
    sprintf(output, "$%.2f", divided);
}

int loopsSinceLastUpdate = 0;


bool updateData()
{
    if (!getTime(month)) {
        return false;
    }
    Serial.printf("Month is %s\n", month);

    Category category0 = getCategory(month, categories[0]);
    if (!category0.success) {
        return false;
    }
    

    Category category1 = getCategory(month, categories[1]);
    if (!category1.success) {
        return false;
    }

    // text0_content = category0.name;
    formatCurrency(text1_content, category0.balance);
    formatCurrency(text3_content, category1.balance);

    return true;
}

void loop()
{

    loopsSinceLastUpdate++;
    display.clearDisplay();

    display.setCursor(150, 320);

    display.setTextSize(4);
    connectWifi();

    if (updateData())
    {
        loopsSinceLastUpdate = 0;
    }
    int timeSinceLastUpdate = loopsSinceLastUpdate * delayMillis / 1000;

    sprintf(time_content, "Last updated %d seconds ago", timeSinceLastUpdate);

    mainDraw();

    display.display();
    delay(delayMillis);
}
