

// Next 3 lines are a precaution, you can ignore those, and the example would also work without them
#ifndef ARDUINO_ESP32_DEV
#error "Wrong board selection for this example, please select Inkplate 6 in the boards menu."
#endif


#include <WiFi.h>
#include "time.h"

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
    connectWifi();
    initializeTime(true);
    delay(5000);
}


void formatCurrency(char * output, int currency)
{
    float divided = currency / 1000.0;
    char value[10];
    sprintf(value, "$%.2f", divided);
    sprintf(output, "%s", value);
}

int loopsSinceLastUpdate = 0;


bool updateData()
{

    Category results[SECRET_NUM_CATEGORIES];
    if (getBudgetInfo(results) == 0)
    {
        return false;
    }


    strcpy(text0_content, results[0].name);
    formatCurrency(text1_content, results[0].balance);
    strcpy(text2_content, results[1].name);
    formatCurrency(text3_content, results[1].balance);

    return true;
}

int timeUpdateCounter = 0;
void loop()
{

    loopsSinceLastUpdate++;
    connectWifi();

    if (timeUpdateCounter++ == 30)
    {
        initializeTime(false);
        timeUpdateCounter = 0;
    }

    if (updateData())
    {
        loopsSinceLastUpdate = 0;
    }
    float timeSinceLastUpdate = loopsSinceLastUpdate * delayMillis / 1000 / 60.0;

    char time_buf[16];

    formatTime(time_buf, sizeof(time_buf), "%H:%M");

    double voltage = display.readBattery();
    sprintf(time_content, "Last updated at %.5s (%.2f min ago)       %.2fv", time_buf, timeSinceLastUpdate, voltage);

    Serial.printf("battery at: %f\n", voltage);
    display.clearDisplay();

    display.setCursor(150, 320);

    display.setTextSize(4);

    mainDraw();

    display.display();
    delay(delayMillis);
}
