#include <WiFi.h>
#include <HTTPClient.h>

const char *ssid = "Ihub61";
const char *password = "Ihub2019";

float voltage = 9.23;
float current = 0.23;
float power = 18.42;

// Your Domain name with URL path or IP address with path
const char *serverName = "http://116.68.160.118:9181/updateSensor/index.php";

unsigned long lastTime = 0;
unsigned long timerDelay = 3000;

void setup()
{
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
}

void loop()
{
  // Send an HTTP POST request every 10 minutes
  if ((millis() - lastTime) > timerDelay)
  {
    // Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED)
    {
      WiFiClient client;
      HTTPClient http;

      http.begin(client, serverName);

      // Specify content-type header
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      // Data to send with HTTP POST
      String httpRequestData = "Voltage=" + String(voltage) + "&Current=" + String(current) + "&Power=" + String(power);
      // Send HTTP POST request
      int httpResponseCode = http.POST(httpRequestData);

      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      Serial.print("Current: ");
      Serial.println(current);
      Serial.print("Power: ");
      Serial.println(power);
      Serial.println(" ");

      if (current > 9.00)
      {
        current = current - 0.1;
      }
      else
      {
        current = current + 0.2;
      }

      if (power > 90.00)
      {
        power = power - 1;
      }
      else
      {
        power = power + 1;
      }

      // Free resources
      http.end();
    }
    else
    {
      Serial.println("WiFi Disconnected");
      WiFi.begin(ssid, password);
      Serial.println("Try to Connect Again");
      if (WiFi.status() == WL_CONNECTED)
      {
        Serial.println("");
        Serial.print("Connected to WiFi network with IP Address: ");
        Serial.println(WiFi.localIP());
      }
      else
      {
        Serial.println("WiFi Still Disconnected");
      }
    }
    lastTime = millis();
  }
}