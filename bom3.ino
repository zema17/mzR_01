//Wifi_http
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
//Fingerprint
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
//Database
#include <FirebaseESP8266.h>
#include <ArduinoJson.h>
//Time
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <TimeLib.h>
//LCD
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//const char* ssid = "Love House";
//const char* password = "cuocsongtuoidep";

#define RELAY_PIN 12

//mạng cá nhân
const char* ssid = "pvp";
const char* password = "pppppppp";

#define FIREBASE_HOST "android-app-cd0eb-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "oKpqbbpM9hSJLJAWEjHtx9SmwYtwieSlLudjELHb"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 7 * 3600, 60000); // Múi giờ UTC+7 và cập nhật mỗi phút

FirebaseData firebaseData;
FirebaseConfig config;
FirebaseAuth auth;

SoftwareSerial mySerial(13, 15); // Chân RX, TX của cảm biến vân tay


Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

bool searching = false;

LiquidCrystal_I2C lcd(0x27, 16, 2);

ESP8266WebServer server(80);

uint8_t id;
String name;

void setup() {

    // Thiết lập chân relay là OUTPUT
  pinMode(RELAY_PIN, OUTPUT); 

  Serial.begin(115200);
  mySerial.begin(57600);
  finger.begin(57600);
  

  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
  Serial.println("Connected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();

  server.on("/saveFingerprint", HTTP_GET, handleSaveFingerprintRequest);
  server.on("/search", HTTP_GET, handleSearchRequest);
  server.on("/delete", HTTP_GET, handleDeleteRequest);

  server.on("/startSearch", HTTP_GET, []() {
        searching = true;
        server.send(200, "text/plain", "Search started");
    });

server.on("/stopSearch", HTTP_GET, []() {
        searching = false;
        server.send(200, "text/plain", "Search stopped");
    });

  // Khởi động I2C và thiết lập các chân SDA và SCL
  Wire.begin(D4, D3); // SDA trên D4, SCL trên D3
  
  // Khởi động LCD
  lcd.begin(16, 2);
  lcd.backlight();
  
  // Hiển thị thông báo khởi động
  lcd.setCursor(0, 0);
  lcd.print("Project started!");


  // Khởi tạo Firebase
  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1);
  }
}

void loop() {
  server.handleClient();

  if (searching) {
        searchFingerprint();
        delay(2000);
    }
  }

String readString() {
  String result = "";
  while (Serial.available() > 0) {
    char c = Serial.read();
    if (c == '\n' || c == '\r') {
      break; // Stop reading on newline or carriage return
    }
    result += c;
  }
  return result;
}


uint8_t readnumber(void) {
  uint8_t num = 0;
  while (num == 0) {
    while (!Serial.available());
    num = Serial.parseInt();
  }
  return num;
}


// Xử lý yêu cầu lưu ID vân tay
void handleSaveFingerprintRequest() {
  if (server.hasArg("id") && server.hasArg("name")) {
    id = server.arg("id").toInt(); // Chuyển đổi từ String sang int và gán cho biến toàn cục
    name = server.arg("name");
    Serial.print("ID: ");
    Serial.println(id);
    Serial.print("Name: ");
    Serial.println(name);

    server.send(200, "text/plain", "ID and Name received");

    // Gọi hàm đăng ký vân tay sau khi lưu ID và tên
    enrollFingerprint();
  } else {
    server.send(400, "text/plain", "Missing ID or Name");
  }
}

// Xử lý yêu cầu tìm kiếm vân tay
void handleSearchRequest() {
  Serial.println("Received request to search fingerprint");
  server.send(200, "text/plain", "Search started");
  searchFingerprint(); // Gọi hàm tìm kiếm vân tay
}


void enrollFingerprint() {
  Serial.println("Ready to enroll a fingerprint!");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enroll ID: ");
  lcd.print(id);

  if (id == 0) { // ID #0 không hợp lệ
    Serial.println("ID is 0, cannot enroll.");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Invalid ID");
    return;
  }

  Serial.print("Enrolling ID #");
  Serial.println(id);
  
  while (!getFingerprintEnroll()) {
    // Keep trying until enrollment is successful
  }
}

uint8_t getFingerprintEnroll() {
  int p = -1;
  Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("put finger...");

  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("imaege taken");

        break;
      case FINGERPRINT_NOFINGER:
        Serial.print(".");
        delay(1000);
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging error");
        break;
      default:
        Serial.println("Unknown error");
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  Serial.println("Remove finger");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("remove finger");

  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");
      lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("place again");

  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.print(".");
        delay(1000);
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging error");
        break;
      default:
        Serial.println("Unknown error");
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
  Serial.print("Creating model for #");  Serial.println(id);
  
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("fingerprints did not match");

    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("stored!");


    // Gọi hàm lưu vào Firebase
    saveToFingerprintDatabase(id, name);
    
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  } 
  return true;  
}


void saveToFingerprintDatabase(uint8_t id, String name) {
  String path = "/fingerprint/" + String(id);

  Firebase.setString(firebaseData, path + "/id", String(id));
  if (firebaseData.httpCode() != 200) {
    Serial.print("Failed to set id: ");
    Serial.println(firebaseData.errorReason());
    return;
  }

  Firebase.setString(firebaseData, path + "/status", "enrolled");
  if (firebaseData.httpCode() != 200) {
    Serial.print("Failed to set status: ");
    Serial.println(firebaseData.errorReason());
    return;
  }

  Firebase.setString(firebaseData, path + "/name", String(name));
  if (firebaseData.httpCode() != 200) {
    Serial.print("Failed to set status: ");
    Serial.println(firebaseData.errorReason());
    return;
  }

  // Lấy thời gian hiện tại từ NTP
  timeClient.update();
  unsigned long epochTime = timeClient.getEpochTime();
  String formattedDate = String(day(epochTime)) + "/" + String(month(epochTime)) + "/" + String(year(epochTime));
  String formattedTime = timeClient.getFormattedTime();

  Firebase.setString(firebaseData, path + "/date", formattedDate);
  if (firebaseData.httpCode() != 200) {
    Serial.print("Failed to set date: ");
    Serial.println(firebaseData.errorReason());
    return;
  }

  Firebase.setString(firebaseData, path + "/time", formattedTime);
  if (firebaseData.httpCode() != 200) {
    Serial.print("Failed to set time: ");
    Serial.println(firebaseData.errorReason());
    return;
  }
  Serial.println("Firebase save succeeded!");
}



void searchFingerprint() {
    Serial.println("Place your finger on the sensor...");

    // Hiển thị thông báo trên LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("fingerprint...");

    // Chờ ngón tay được phát hiện
    while (finger.getImage() != FINGERPRINT_OK);

    // Chuyển đổi hình ảnh sang mẫu
    if (finger.image2Tz() != FINGERPRINT_OK) {
        Serial.println("Failed to convert image to template");
        return;
    }

    // Tìm kiếm vân tay trong cơ sở dữ liệu cảm biến
    int fingerID = finger.fingerSearch();
    if (fingerID == FINGERPRINT_NOTFOUND) {
        Serial.println("No match found");

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("No match found");

        return;
    } else if (fingerID == FINGERPRINT_PACKETRECIEVEERR) {
        Serial.println("Communication error");

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Comm error");

        return;
    }

    // Tìm thấy vân tay
    Serial.print("Found ID #");
    Serial.print(finger.fingerID);
    Serial.print(" with confidence of ");
    Serial.println(finger.confidence);

    // Kiểm tra nếu ID tồn tại trong mục block
    String blockPath = "/block/" + String(finger.fingerID);
    Serial.println(blockPath);
    Firebase.getString(firebaseData, blockPath);
    if (firebaseData.httpCode() == 200) {
        Serial.println("ID is blocked");

        // Hiển thị thông báo bị chặn lên LCD
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("ID blocked");

        // Hiển thị tên từ Firebase, nếu có
        String namePath = "/fingerprint/" + String(finger.fingerID) + "/name";
        Firebase.getString(firebaseData, namePath);
        if (firebaseData.httpCode() == 200) {
            String name = firebaseData.stringData();
            Serial.print("Name: ");
            Serial.println(name);

            lcd.setCursor(0, 1);
            lcd.print(name);
        }

        return;
    }

    // Lấy tên từ Firebase
    String namePath = "/fingerprint/" + String(finger.fingerID) + "/name";
    Serial.println(namePath);
    Firebase.getString(firebaseData, namePath);
    if (firebaseData.httpCode() == 200) {
        String name = firebaseData.stringData();
        Serial.print("Name: ");
        Serial.println(name);

        // Hiển thị tên lên LCD
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Name: ");
        lcd.setCursor(0, 1);
        lcd.print(name);

        // Bật relay trong 4 giây
        digitalWrite(RELAY_PIN, HIGH);
        delay(4000);
        digitalWrite(RELAY_PIN, LOW);


        // Lưu vào cơ sở dữ liệu
        saveToDatabase(finger.fingerID, finger.confidence, name);
    } else {
        Serial.print("HTTP code: ");
        Serial.println(firebaseData.httpCode());
        Serial.print("Error reason: ");
        Serial.println(firebaseData.errorReason());
        Serial.print("String data: ");
        Serial.println(firebaseData.stringData());

        Serial.print("Failed to retrieve name: ");
        Serial.println(firebaseData.errorReason());
    }
}



void saveToDatabase(uint8_t id, uint16_t confidence, String name) {
  String path = "/confirm/" + String(id);

  Firebase.setString(firebaseData, path + "/id", String(id));
  if (firebaseData.httpCode() != 200) {
    Serial.print("Failed to set id: ");
    Serial.println(firebaseData.errorReason());
    return;
  }

  Firebase.setInt(firebaseData, path + "/confidence", confidence);
  if (firebaseData.httpCode() != 200) {
    Serial.print("Failed to set confidence: ");
    Serial.println(firebaseData.errorReason());
    return;
  }

  Firebase.setString(firebaseData, path + "/name", name);
  if (firebaseData.httpCode() != 200) {
    Serial.print("Failed to set name: ");
    Serial.println(firebaseData.errorReason());
    return;
  }

  Firebase.setString(firebaseData, path + "/status", "Success!");
  if (firebaseData.httpCode() != 200) {
    Serial.print("Failed to set status: ");
    Serial.println(firebaseData.errorReason());
    return;
  }

  // Lấy thời gian hiện tại từ NTP
  timeClient.update();
  unsigned long epochTime = timeClient.getEpochTime();
  String formattedDate = String(day(epochTime)) + "/" + String(month(epochTime)) + "/" + String(year(epochTime));
  String formattedTime = timeClient.getFormattedTime();

  Firebase.setString(firebaseData, path + "/date", formattedDate);
  if (firebaseData.httpCode() != 200) {
    Serial.print("Failed to set date: ");
    Serial.println(firebaseData.errorReason());
    return;
  }

  Firebase.setString(firebaseData, path + "/time", formattedTime);
  if (firebaseData.httpCode() != 200) {
    Serial.print("Failed to set time: ");
    Serial.println(firebaseData.errorReason());
    return;
  }

  Serial.println("Firebase save succeeded!");
}



void handleDeleteRequest() {
  if (server.hasArg("id")) {
    uint8_t id = server.arg("id").toInt();
    Serial.print("Deleting fingerprint ID: ");
    Serial.println(id);
    deleteFingerprint(id); // Gọi hàm xóa vân tay
    server.send(200, "text/plain", "Delete request received");
  } else {
    server.send(400, "text/plain", "Missing ID");
  }
}


uint8_t deleteFingerprint(uint8_t id) {
  uint8_t p = finger.deleteModel(id);

  if (p == FINGERPRINT_OK) {
    Serial.println("Deleted!");

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Deleted ID: ");
    lcd.print(id);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not delete in that location");
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
  } else {
    Serial.print("Unknown error: 0x");
    Serial.println(p, HEX);
  }

  return p;
}


//I hope everything coming to me is all great.

