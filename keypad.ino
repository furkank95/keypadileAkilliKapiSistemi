  #include <Keypad.h>
  #include <Servo.h>
  Servo myservomotor;
  int ledPinOpen=11; // yeşil led
  int ledPinClose=12; // kırmızı led
  const byte numRows= 4; // keypad satır sayısı
  const byte numCols= 4; // keypad sütün sayısı
  int control = 0; //# tuşuna basıldığını kontrol etmek için
  int counter = 0; //şifre uzunluğu kadar karakter girildiğini kontrol etmek için
  int yetki = 0; //kapıyı açıp kapamak için yetki 
  int izinsiz_kullanici = 0; //şifreyi tahmin etmek isteyeni bekletmek için sayaç
  int kapi_durum = 0;
  int control2 = 0; //* basıldığında aktif olacak 
  String enter_password; // girilen şifreyi tutar
  String password = "#1234"; //kendimizin belirlediği şifre    
  
  char keymap[numRows][numCols]={
  {'1', '2', '3', 'A'}, 
  {'4', '5', '6', 'B'}, 
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}};
    
  byte rowPins[numRows] = {2,3,4,5}; 
  byte colPins[numCols]= {6,7,8,9}; 
    
  Keypad myKeypad= Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);
    
  void setup(){
    Serial.begin(9600);
    pinMode(ledPinOpen,OUTPUT); //led pinlerini çıkış olarak belitliyoruz
    pinMode(ledPinClose,OUTPUT);
    myservomotor.attach(10); 
    myservomotor.write(0); //ilk etapta açısını 0 olarak set ediyoruz
  }  
  
  void loop(){
    char keypressed = myKeypad.getKey(); //keypadden gelen değer
    if (keypressed != NO_KEY){ //keypad'e basılınca bu if çalışacak
      Serial.print(keypressed);
      if (keypressed == '#') { //# tuşuna basıldı ise control 1 olur password kontrole geçer 
        control = 1;
      }      
      if (keypressed == '*') {  //* tuşuna basıldı ise control 1 olur password kontrole geçer
        control2 = 1;
      }
      if (control == 1){ //# tuşuna basıldı ise gerekli kontrolleri yap
        enter_password = enter_password + keypressed;
        counter ++;
        Serial.println("sifre " + enter_password);
        if (enter_password == password){ //password doğru ise
          Serial.println("giriş başarılı");
          enter_password = "";
          control = 0;
          counter = 0;
          yetki = 1;
          izinsiz_kullanici = 0;          
        }        
        else if ((enter_password != password)&&(counter > 4)){ //password yanlış veya 4 den fazla girildi ise
          Serial.println("hatalı giriş");          
          digitalWrite(ledPinOpen,0); 
          digitalWrite(ledPinClose,1);
          delay(2000);
          digitalWrite(ledPinClose,0);
          enter_password = "";
          control = 0;
          counter = 0;
          izinsiz_kullanici++;
          if(izinsiz_kullanici == 2){ //şifre iki kez hatalı girilince bekletiyor
            delay(60000);
            Serial.println("izinsiz kullanıcı");
          }
        }
      }
      if(yetki == 1){
        if(keypressed == 'A'){ //Kapıyı Aç
          for(int value = 0; value < 90; value++){ 
            myservomotor.write(value);
          }
          Serial.println("kapı açıldı");
          digitalWrite(ledPinOpen,1); 
          digitalWrite(ledPinClose,0);
          delay(2000);  // gecikme
          digitalWrite(ledPinOpen,0); 
          kapi_durum = 1;
        }
        if(keypressed == 'B'){ // Kapıyı Kapat           
          for(int value = 90; value > 1; value--){
            myservomotor.write(value);
          }
          digitalWrite(ledPinOpen,0); 
          digitalWrite(ledPinClose,1); 
          delay(2000);  // gecikme
          digitalWrite(ledPinClose,0); 
          kapi_durum = 0;
        }
        if(control2 == 1){ //çıkış kontrolü
          Serial.println("Çıkış yapmak istediğinizden emin misiniz C evet, D hayır");
          if(keypressed == 'C'){ //çıkış yapıldı
            yetki = 0;
            Serial.println("Çıkış yapıldı.");            
            if(kapi_durum == 1){
              for(int value = 90; value > 1; value--){
                myservomotor.write(value);
              }
              digitalWrite(ledPinOpen,0); 
              digitalWrite(ledPinClose,1); 
              delay(2000);  // gecikme
              digitalWrite(ledPinClose,0); 
              kapi_durum = 0;
              control2 = 0;
            }
          }
          if(keypressed == 'D'){ //işlem iptali
            Serial.println("İptal edildi.");
            control2 = 0;
          }         
        }
      } 
    }
  }
