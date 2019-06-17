// On Leonardo/Micro or others with hardware serial, use those! #0 is green wire, #1 is white
// uncomment this line:
// #define mySerial Serial1

// comment these two lines if using hardware serial
//SoftwareSerial mySerial(2, 3);
//
//Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
//// RX is white and should be connected to TX on board, TX is green
//void setup()
//{
//  Serial.begin(9600);
//  while (!Serial);  // For Yun/Leo/Micro/Zero/...
//  delay(100);
//  Serial.println("\n\nAdafruit finger detect test");
//
//  // set the data rate for the sensor serial port
//  finger.begin(57600);
//
//  if (finger.verifyPassword()) {
//    Serial.println("Found fingerprint sensor!");
//  } else {
//    Serial.println("Did not find fingerprint sensor :(");
//    while (1) { delay(1); }
//  }
//
//  finger.getTemplateCount();
//  Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
//  Serial.println("Waiting for valid finger...");
//}
//
//void loop()                     // run over and over again
//{
//  getFingerprintIDez();
//  delay(50);            //don't ned to run this at full speed.
//}
//
//uint8_t getFingerprintID() {
//  uint8_t p = finger.getImage();
//  switch (p) {
//    case FINGERPRINT_OK:
//      Serial.println("Image taken");
//      break;
//    case FINGERPRINT_NOFINGER:
//      Serial.println("No finger detected");
//      return p;
//    case FINGERPRINT_PACKETRECIEVEERR:
//      Serial.println("Communication error");
//      return p;
//    case FINGERPRINT_IMAGEFAIL:
//      Serial.println("Imaging error");
//      return p;
//    default:
//      Serial.println("Unknown error");
//      return p;
//  }
//
//  // OK success!
//
//  p = finger.image2Tz();
//  switch (p) {
//    case FINGERPRINT_OK:
//      Serial.println("Image converted");
//      break;
//    case FINGERPRINT_IMAGEMESS:
//      Serial.println("Image too messy");
//      return p;
//    case FINGERPRINT_PACKETRECIEVEERR:
//      Serial.println("Communication error");
//      return p;
//    case FINGERPRINT_FEATUREFAIL:
//      Serial.println("Could not find fingerprint features");
//      return p;
//    case FINGERPRINT_INVALIDIMAGE:
//      Serial.println("Could not find fingerprint features");
//      return p;
//    default:
//      Serial.println("Unknown error");
//      return p;
//  }
//
//  // OK converted!
//  p = finger.fingerFastSearch();
//  if (p == FINGERPRINT_OK) {
//    Serial.println("Found a print match!");
//  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
//    Serial.println("Communication error");
//    return p;
//  } else if (p == FINGERPRINT_NOTFOUND) {
//    Serial.println("Did not find a match");
//    return p;
//  } else {
//    Serial.println("Unknown error");
//    return p;
//  }
//
//  // found a match!
//  Serial.print("Found ID #"); Serial.print(finger.fingerID);
//  Serial.print(" with confidence of "); Serial.println(finger.confidence);
//
//  return finger.fingerID;
//}
//
//// returns -1 if failed, otherwise returns ID #
//int getFingerprintIDez() {
//  uint8_t p = finger.getImage();
//  if (p != FINGERPRINT_OK)  return -1;
//
//  p = finger.image2Tz();
//  if (p != FINGERPRINT_OK)  return -1;
//
//  p = finger.fingerFastSearch();
//  if (p != FINGERPRINT_OK)  return -1;
//
//  // found a match!
//  Serial.print("Found ID #"); Serial.print(finger.fingerID);
//  Serial.print(" with confidence of "); Serial.println(finger.confidence);
//  return finger.fingerID;
//}