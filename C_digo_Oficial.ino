 #include <Keypad.h>
#include <LiquidCrystal.h>
#include <Adafruit_Fingerprint.h>
SoftwareSerial mySerial(13, 9);  //verde na 13, azul na 9
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

uint8_t id;
uint8_t verifica = FINGERPRINT_NOFINGER;
LiquidCrystal lcd (12, 11, 5, 4, 3, 2); //criando objeto lcd e fornecendo os parâmetros de conexão
#include <Keypad.h>
//10 rele
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the butt ons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {A5, A4, A3, A2}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {A1, A0, 6, 7}; //connect to the column pinouts of the keypad
int num;
//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
String palavra = "";
void setup()
{
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
  Serial.begin(9600);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("\n\nAdafruit finger detect test");
  // set the data rate for the sensor serial port
  finger.begin(57600);

  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");

  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) {
      delay(1);
    }
  }


  finger.getTemplateCount();
  Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  Serial.println("Waiting for valid finger...");
  
}

uint8_t readnumber(void) {
  while (palavra.length() <= 3) {
    char customKey = customKeypad.getKey();
    if (customKey) {
      palavra = palavra + customKey;
      Serial.print(customKey);
      lcd.clear();
      lcd.print(palavra);
      if (palavra.length() == 3) {
        num = palavra.toInt();
        palavra = "";
        return num;
        break;
      }
    }
  }
}

void loop()                     // run over and over again
{
  uint8_t p;
  finger.fingerID = 0;

  Serial.println("Ola, gostaria de entrar?");
  Serial.println("Coloque sua digital no sensor");

  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Coloque sua");
  lcd.setCursor(0, 1);
  lcd.print ("digital");
  verificaID();

  if (finger.fingerID  == 1 ) {
    int opcao = 0;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Oi Administrador");
    delay(2000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("1-cadastro");
    delay(2000);
    lcd.setCursor(0, 1);
    lcd.print("2-entrar");
    delay(2000);
    lcd.clear();
    lcd.print("3-apagar um ID");
    Serial.println("Ola Gabriel");
    Serial.println("Escolha:");
    Serial.println("1 para cadastrar uma nova digital");
    Serial.println("2 para entrar");
    Serial.println("3 para apagar um ID especifico");
    delay(3000);
    while (opcao == 0) {
      //opcao = Serial.parseInt();
      char customKey = customKeypad.getKey();
      if (customKey) {
        if (customKey == '1') {
          opcao = 1;
        } else if (customKey == '2') {
          opcao = 2;
        } else if (customKey == '3') {
          opcao = 3;
        }
      }
    }

    if (opcao == 1) {
      Serial.println("Pronto para cadastrar sua digital");
      Serial.println("Digite o ID # (de 1 a 127) que você quer cadastrar sua digital...");
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Digite o ID");
      lcd.setCursor(1, 1);
      lcd.print("de 1 a 127");


      id = readnumber();
      if (id == 0) {// ID #0 not allowed, try again!
        return;
      }
      Serial.print("Cadastrando id #");
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Cadastrando...");
      delay(2000);
      Serial.println(id);

      getFingerprintEnroll();


      //while (!);

    } else if (opcao == 2) {
      lcd.clear();
      lcd.print("Seja bem vindo");
      Serial.println("Seja bem vindo");
      digitalWrite(10, LOW);
      delay(5000);
      digitalWrite(10, HIGH);
      finger.fingerID = 0;
    } else if (opcao == 3) {
      // run over and over again
      {
        Serial.println("Please type in the ID # (from 1 to 127) you want to delete...");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Digite o ID");
        lcd.setCursor(0, 1);
        lcd.print("de 1 a 127");
        uint8_t id = readnumber();
        if (id == 0) {// ID #0 not allowed, try again!
          return;
        }

        Serial.print("Deleting ID #");
        Serial.println(id);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Deletando");
        deleteFingerprint(id);
      }
    }
  } else if (finger.fingerID > 1) {
    lcd.clear();
    lcd.print("Seja bem vindo");
    Serial.print("Seja bem-vindo");
    digitalWrite(10, LOW);
    delay(5000);
    digitalWrite(10, HIGH);
  }


  delay(1000);            //don't ned to run this at full speed.
}


uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Erro");
      delay(2000);
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Erro");
      delay(2000);
      return p;
    default:
      Serial.println("Unknown error");
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Erro");
      delay(2000);
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Imagem baguncada");
      delay(2000);
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Erro de comunicação");
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Erro");
      delay(2000);
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Erro");
      delay(2000);
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Erro");
      delay(2000);
      return p;
    default:
      Serial.println("Tente novamente");
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Tente Novamente");
      delay(2000);
      return p;
  }

  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    lcd.clear();
    lcd.print("Seja bem vindo");
    Serial.println("Seja bem vindo");
    digitalWrite(10, LOW);
    delay(5000);
    digitalWrite(10, HIGH);
    return p;
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Erro");
    delay(2000);
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    lcd.clear();
    lcd.print("Digital nao encontrada");
    Serial.println("Digital nao encontrada");
    return p;
  } else {
    Serial.println("Tente novamente");
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Tente novamente");
    delay(2000);
    return p;
  }

  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);

  return finger.fingerID;
}

uint8_t getFingerprintEnroll() {
  uint8_t p = -1;
  int c = 0;
  while (c == 0) {
    Serial.print("Waiting for valid finger to enroll as #");
    Serial.println(id);
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Ponha seu dedo");
    delay(100);
    while (p != FINGERPRINT_OK) {
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Ponha seu dedo");
      p = finger.getImage();
      switch (p) {
        case FINGERPRINT_OK:
          //Serial.println("Image taken");
          break;
        case FINGERPRINT_NOFINGER:
          Serial.println(".");
          break;
        case FINGERPRINT_PACKETRECIEVEERR:
          Serial.println("Communication error");
          lcd.clear();
          lcd.setCursor(1, 0);
          lcd.print("Erro");
          delay(2000);
          break;
        case FINGERPRINT_IMAGEFAIL:
          Serial.println("Imaging error");
          lcd.clear();
          lcd.setCursor(1, 0);
          lcd.print("Erro");
          delay(100);
          break;
        default:
          Serial.println("Unknown error");
          lcd.clear();
          lcd.setCursor(1, 0);
          lcd.print("Erro");
          delay(100);
          break;
      }
    }

    // OK success!

    p = finger.image2Tz(1);
    switch (p) {
      case FINGERPRINT_OK:
        //Serial.println("Image converted");
        break;
      case FINGERPRINT_IMAGEMESS:
        Serial.println("Image too messy");
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("Imagem baguncada");
        delay(2000);
        return p;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("Erro");
        delay(2000);
        return p;
      case FINGERPRINT_FEATUREFAIL:
        Serial.println("Could not find fingerprint features");
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("Erro");
        delay(2000);
        return p;
      case FINGERPRINT_INVALIDIMAGE:
        Serial.println("Could not find fingerprint features");
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("Erro");
        delay(2000);
        return p;
      default:
        Serial.println("Unknown error");
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("Erro");
        delay(2000);
        return p;
    }

    Serial.println("Remove finger");
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Retire o dedo");
    delay(2000);
    p = 0;
    while (p != FINGERPRINT_NOFINGER) {
      p = finger.getImage();
    }
    Serial.print("ID "); Serial.println(id);
    p = -1;
    Serial.println("aaaaaaaaaPlace same finger again");
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Ponha novamente");
    delay(2000);
    while (p != FINGERPRINT_OK) {
      p = finger.getImage();
      switch (p) {
        case FINGERPRINT_OK:
          //Serial.println("Image taken");
          break;
        case FINGERPRINT_NOFINGER:
          Serial.print(".");
          break;
        case FINGERPRINT_PACKETRECIEVEERR:
          Serial.println("Communication error");
          lcd.clear();
          lcd.setCursor(1, 0);
          lcd.print("Erro");
          delay(2000);
          break;
        case FINGERPRINT_IMAGEFAIL:
          Serial.println("Imaging error");
          lcd.clear();
          lcd.setCursor(1, 0);
          lcd.print("Erro");
          delay(2000);
          break;
        default:
          Serial.println("Unknown error");
          lcd.clear();
          lcd.setCursor(1, 0);
          lcd.print("Erro");
          delay(2000);
          break;
      }
    }

    // OK success!

    p = finger.image2Tz(2);
    switch (p) {
      case FINGERPRINT_OK:
        //Serial.println("Image converted");
        break;
      case FINGERPRINT_IMAGEMESS:
        Serial.println("Image too messy");
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("Imagem baguncada");
        delay(2000);
        return p;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("Erro");
        delay(2000);
        return p;
      case FINGERPRINT_FEATUREFAIL:
        Serial.println("Could not find fingerprint features");
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("Erro");
        delay(2000);
        return p;
      case FINGERPRINT_INVALIDIMAGE:
        Serial.println("Could not find fingerprint features");
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("Erro");
        delay(2000);
        return p;
      default:
        Serial.println("Unknown error");
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("Erro");
        delay(2000);
        return p;
    }
    switch (p) {
        // OK converted!
        Serial.print("Creating model for #");  Serial.println(id);
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("Criando modelo...");
        delay(2000);
        break;
        p = finger.fingerID;
        //p = finger.createModel();
        if (p == FINGERPRINT_OK) {
          Serial.println("Prints matched!");
          lcd.clear();
          lcd.setCursor(1, 0);
          lcd.print("PRONTO.");
          delay(2000);
          break;
        } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
          Serial.println("Communication error");
          lcd.clear();
          lcd.setCursor(1, 0);
          lcd.print("Erro");
          delay(2000);
          return p = 0;
        } else if (p == FINGERPRINT_ENROLLMISMATCH) {
          Serial.println("Fingerprints did not match");
          lcd.clear();
          lcd.setCursor(1, 0);
          lcd.print("NAO GRAVEI");
          delay(2000);
          return p = 0;
        } else {
          Serial.println("Unknown error");
          lcd.clear();
          lcd.setCursor(1, 0);
          lcd.print("Erro");
          delay(2000);
          return p = 0;
          //finger.fingerID = 0;
        }
    }
    Serial.print("ID "); Serial.println(id);
    p = finger.storeModel(id);
    switch (p) {
      case FINGERPRINT_OK:
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("Digital guardada");
        delay(2000);
        Serial.println("Stored!");
        return c = 1;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("Erro");
        delay(200);
        return p;
      case FINGERPRINT_BADLOCATION:
        Serial.println("Could not store in that location");
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("Erro");
        delay(200);
        return p;
      case FINGERPRINT_FLASHERR:
        Serial.println("Error writing to flash");
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("Erro");
        delay(200);
        return p = 0 ;
      default:
        Serial.println("Unknown error");
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("Erro");
        delay(200);
        return p;
    }
  }
}
uint8_t verificaID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Erro");
      delay(2000);
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Erro");
      delay(2000);
      return p;
    default:
      Serial.println("Unknown error");
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Erro");
      delay(2000);
      return p;
  }
  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Imagem baguncada");
      delay(2000);
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Erro de comunicação");
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Erro");
      delay(2000);
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Erro");
      delay(2000);
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Erro");
      delay(2000);
      return p;
    default:
      Serial.println("Tente novamente");
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Tente Novamente");
      delay(2000);
      return p;
  }
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    return p;
    return finger.fingerID;
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Erro");
    delay(2000);
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Digital nao");
    lcd.setCursor(0, 1);
    lcd.print("encontrada");
    Serial.println("Digital nao encontrada");
    digitalWrite(6, HIGH);
    delay(2000);
    digitalWrite(6, LOW);
    finger.fingerID = 0;
    return p;
    return finger.fingerID;
  } else {
    Serial.println("Tente novamente");
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Tente novamente");
    delay(2000);
    return p;
  }

}

uint8_t deleteFingerprint(uint8_t id) {
  uint8_t p = -1;

  p = finger.deleteModel(id);

  if (p == FINGERPRINT_OK) {
    Serial.println("Deleted!");
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Apagado");
    delay(2000);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Erro");
    delay(2000);
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not delete in that location");
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Erro");
    delay(2000);
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Erro");
    delay(2000);
    return p;
  } else {
    Serial.print("Unknown error: 0x"); Serial.println(p, HEX);
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Erro");
    delay(2000);
    return p;
  }
}
