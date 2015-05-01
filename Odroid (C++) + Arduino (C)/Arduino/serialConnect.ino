// Strings for testing Serial connection
//String test = "hello";
String test2= "5:hello,";

void setup() {
  Serial.begin(9600);
}

void loop() {
  
   
  
  String payload = "Regine is sooo smarT!";
     
  Serial.println(encodeNetstring(payload));
  Serial.println('\n');
 
   
 // Serial.println(decodeNetstring(test2)); 
  
  //EXAMPLE: 
  // 1) read data from IR and store in payload
  //      int voltReading = analogRead(IR_PIN);
  //      int ir_cm = ((2914 / (voltReading + 5) - 1));
  //      payload = ir_cm;
  
//  if(Serial.available()) {
//  String netstring = Serial.readStringUntil(','); 
//  Serial.println(decodeNetstring(netstring)); 
//  } 
//  
//  if(Serial.available()) {
//  String payload = Serial.readStringUntil('\n'); 
//  Serial.println(encodeNetstring(payload));
//  }
}

// ****  Functions    *****// 

String encodeNetstring(String payload) {
  int len = payload.length();
  if(!len) return "ERROR_EMPTY_STRING"; 
  String netstring = len + String(":" + payload + ",");

  return netstring;
}


String decodeNetstring(String netstring) {
  if (netstring.length() < 3) return "NETSTRING_ERROR_TOO_SHORT";
  
  //if (netstring.length() > ?) return "NETSTRING_ERROR_TOO_LONG"; 
  
  int semicolonIndex = netstring.indexOf(':');
  if (semicolonIndex < 0) return "NETSTRING_ERROR_NO_COLON"; 
  
  String getLength = netstring.substring(0, semicolonIndex); 
  int payloadLength = getLength.toInt();
  if (payloadLength < 1) return "NETSTRING_ERROR_LEADING_ZERO"; 
 
  String payload = netstring.substring(semicolonIndex+1); 
  if (!payload.length()) return "NETSTRING_ERROR_NO_LENGTH"; 
  
  if (payload.substring(payload.length() -1) == ",") payload.remove(payload.length()-1); //remove the comma
  if (payload.length() != payloadLength) return "NETSTRING_ERROR_INVALID_LENGTH";
  
  return payload;
  
}

