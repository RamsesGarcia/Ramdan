#include "esp_camera.h"
#include <WiFi.h>
#include <ArduinoWebsockets.h>  //permite conectarnos remotamente a una pagina web o servidor web
#include "esp_timer.h"          //detecta cuanto tiempo ha pasado para volver a repetir una accion
#include "img_converters.h"     //convertir la imagen en byte o base64
#include "fb_gfx.h"             //para trabajar con el manejo de voltaje para que no de error de voltaje 
#include "soc/soc.h"           //limpian la memoria de la camar cada cierto tiempo, porque no hay SSD
#include "soc/rtc_cntl_reg.h"  //limpian la memoria de la camar cada cierto tiempo, porque no hay SSD
#include "driver/gpio.h"       //para tener mas compatibilidad con la demas libreria


//Definimos los pines para AI Thinker Camera board
#define PWDN_GPIO_NUM 32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 0
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27
#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 21
#define Y4_GPIO_NUM 19
#define Y3_GPIO_NUM 18
#define Y2_GPIO_NUM 5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22

//constante para definir la clave y red wifi
const char* ssid = "Claro_FAMILIA_LARREA";  // WIFI
const char* password = "GCB16916KAD";       // Password

//definmos una constante del servidor host que va a recibir la imagen 
//y el puerto
const char* websockets_server_host = "64.23.139.69";    //servidor
const uint16_t websockets_server_port = 3001;           //puerto

//definir variables para poder almacenar tamaño de imagen
camera_fb_t* fb = NULL;
size_t _jpg_buf_len = 0;  //tamaño
uint8_t* _jpg_buf = NULL; //el tamaño de memoria de RAM de los datos
uint8_t state = 0;        //estado

//Iniciar la lbreria de Websockets para conectarnos al servidor
using namespace websockets;
WebsocketsClient client;

//Funcion para mandar un mensaje
//con esta funcion enviamos las imagenes en byte
void onMessageCallback(WebsocketsMessage message) {
  Serial.print("Got Message: ");
  Serial.println(message.data());
}


//Iniciar la camara con cada pin
esp_err_t init_camera() {
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  //Parametros de la calidad de la imagen
  config.frame_size = FRAMESIZE_VGA;  //FRAMESIZE_ + QVGA|CIF|VGA|SVGA|XGA|SXGA|UXGA
  //VGA es mas flexible y menos pesado por lo tanto no se ocupa tanta memoria
  config.jpeg_quality = 15;           //10-63 bajo numero significa alta calidad
  config.fb_count = 2;                //cuenta la toma de captura antes de enviarla

  //Inicia la camara
  esp_err_t err = esp_camera_init(&config);
  //Si no se conecta envia un mensaje de error en el serial
  if (err != ESP_OK) {
    Serial.printf("camera init FAIL: 0x%x", err);
    return err;
  }
  //Si se conecta envia por el serial un mensaje de OK
  sensor_t* s = esp_camera_sensor_get();
  s->set_framesize(s, FRAMESIZE_VGA);
  Serial.println("camera init OK");
  return ESP_OK;
};

//Funcion para conectarse a Wifi
esp_err_t init_wifi() {
  WiFi.begin(ssid, password);
  Serial.println("Wifi init ");
  //Si no se conecta envia un punto al serial
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  //Si se logra conectar al wifi manda un mensaje exitoso
  //y envia las imagenes al servidor
  Serial.println("");
  Serial.println("WiFi OK");
  Serial.println("connecting to WS: ");

  //Enviamos la informacion de la imagen 
  client.onMessage(onMessageCallback);
  bool connected = client.connect(websockets_server_host, websockets_server_port, "/");
  //Verifica si el servidor esta en pie
  if (!connected) {
    Serial.println("WS connect failed!");
    Serial.println(WiFi.localIP());
    state = 3;
    return ESP_FAIL;
  }
  if (state == 3) {
    return ESP_FAIL;
  }
  Serial.println("WS OK");
  client.send("hello from ESP32 camera stream!");
  return ESP_OK;
};


void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //Evita que no tenga problema con el voltaje

  Serial.begin(115200);
  Serial.setDebugOutput(true);

  init_camera();
  init_wifi();
}


//En caso de que se detecte correctamente la conexion
void loop() {
  if (client.available()) {
    camera_fb_t* fb = esp_camera_fb_get(); //guardamos los datos binarios de la imagen 
    if (!fb) {
      Serial.println("img capture failed");
      esp_camera_fb_return(fb);
      ESP.restart();
    }
    client.sendBinary((const char*)fb->buf, fb->len); //hace la conversion de imagenes a buf(byte) y de longitud total
    Serial.println("image sent");
    esp_camera_fb_return(fb); //vuelve a retornar el valor de fb
    client.poll(); //se conecta al servidor y asi lo hace constantemente
  }
}