#include <WiFi.h>
#include <WebServer.h>

// Configuración del LED
const int ledPin = 2;
bool ledState = LOW;  // Estado inicial del LED

// Configuración del servidor web
WebServer server(80);

// Configuración de la red WiFi
const char* ssid = "IoTB";
const char* password = "inventaronelVAR";

// Función para manejar la solicitud de la página principal
void handleRoot() {
  String buttonLabel = ledState ? "Apagar LED" : "Encender LED";  // Cambiar la etiqueta del botón según el estado del LED
  
  String html = "<html>\
                   <body>\
                     <h1>Control del LED</h1>\
                     <button onclick=\"location.href='/toggle_led'\" style=\"padding:20px; font-size:20px\">" + buttonLabel + "</button>\
                   </body>\
                 </html>";
  server.send(200, "text/html", html);
}

// Función para manejar la solicitud de alternar el estado del LED
void handleToggleLed() {
  ledState = !ledState;  // Alternar el estado del LED
  digitalWrite(ledPin, ledState ? HIGH : LOW);  // Actualizar el estado del LED

  // Redirigir al usuario de vuelta a la página principal
  server.sendHeader("Location", "/");
  server.send(303);  // Código 303 para redirigir
}

void setup() {
  // Inicializar el LED
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, ledState);

  // Inicializar la conexión WiFi
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }

  Serial.println("Conectado a la red WiFi");

  // Iniciar el servidor web
  server.on("/", handleRoot);
  server.on("/toggle_led", handleToggleLed);

  server.begin();
  Serial.println("Servidor HTTP iniciado");

  Serial.println("Conectado a la red WiFi");
  Serial.println(WiFi.localIP());  // Mostrar la IP en el monitor serie
}

void loop() {
  // Manejar las solicitudes del cliente
  server.handleClient();
}
