/*
 * (C) Aditya Prerepa 12-6-18
 */

// output 5 goes to D1.
// output 4 goes to D2.
// output 3 goes to  ??
// output 2 is builtin and D4 (cant be input)
// Designers of this chip didn't map the pins correctly...

 #include <ESP8266WiFi.h>
 #include <WiFiClient.h>
 #include <ESP8266Ping.h>
 #include <MySQL_Connection.h>
 #include <MySQL_Cursor.h>
 #include <time.h>
 
 const IPAddress remote_ip(10,0,1,157); 
 int inputPirPin = 5;  
 // Connect PIR to D1 in this case.
 // All users and passwords of the database and your wifi.
 char* DatabaseUser = "-----";         
 char* DatabasePassword = "--------"; 
 char* WifiSsid = "-------";
 char* WifiPass = "------";
 int zone = 4 * 3600; // Im in california
 int dst = 0;
 char* exe_SQL = "";
 int val = 0;
 int pirState = LOW;
 int ledPin = 2;
 
//Define function names and signatures.
void executeQuery(char*, MySQL_Connection);
void updateTime(int, int);
 
 WiFiClient client;
 MySQL_Connection conn((Client *)&client);
 
 // setup(), runs once. 
 // Calls connectToWifi() to initialize the connection, and
 // calls pingDatabase() to ping the mysqlDatabase with
 // the parameter of the remote IP we established above.
 // Also calls configTime() (Not mine) to configure the time and the 
 // the website it gets the time from, and also gets the zone
 // you are in as defined above.
 // After the time is configured, setup() calls connectToDatabase() 
 // to establish the connection to the database. Takes in 
 // params the char* of DatabaseUser and DatabasePassword, 
 // as well as the MYSQL_Connection that was established above.
 // Mode Pin inputPirPin to INPUT.
 void setup() {
   Serial.begin(115200);
   connectToWifi(WifiSsid, WifiPass);
   pingDatabase(remote_ip);
   configTime(zone, dst, "pool.ntp.org", "time.nist.gov");
   connectToDatabase(DatabaseUser, DatabasePassword, conn);
   pinMode(inputPirPin, INPUT);
   Serial.println(WiFi.localIP());
 }

// function to updtate the time and commit it to the database. Takes in the 
// parameters of your current zone, and the dst
void updateTime(int zone, int dst) {
  char INSERT_SQL[1024];
  time_t now = time(nullptr);
  sprintf(INSERT_SQL, "INSERT INTO test_arduino.arduino_update (time_of_entry) VALUES (' " "%s" " ')", ctime(&now));
  Serial.printf("%s; time:%s", INSERT_SQL, ctime(&now));
  executeQuery(INSERT_SQL, conn);
 }
 
 // Loop, simple, one function.
 void loop() {
  checkPir();
 }

 // pingDatabase() takes in the mysql address and tries
 // to ping the database, and prints out the result.
 void pingDatabase(IPAddress ip_addr) {
  Serial.print("Pinging DB ");
  Serial.println(ip_addr);
   if(Ping.ping(remote_ip)) {
     Serial.println("Success!!");
   } else {
     Serial.println("Error :(");
   }
 }

 // connectToWifi() takes in the parameters of the SSID of the 
 // wifi and the password. It will print dots until it is 
 // connected. Once it is, It will print the details.
 void connectToWifi(char* WiFiSsid, char* WiFiPass) {
   WiFi.begin(WiFiSsid, WiFiPass);
                         
   while ( WiFi.status() != WL_CONNECTED ) {   
     delay ( 500 );
     Serial.print ( "." );
   }
   Serial.println ( "" );
   Serial.print ( "Connected to " );
   Serial.println ( WiFiSsid );
   Serial.print ( "IP address: " );
   Serial.println ( WiFi.localIP() );
 }

 // Function to connect to the database. Takes in the params of 
 // DatabaseUser, Password for user, and the MYSQL_Connection object. 
 // This will try to connect to the IP on the MySQL port 3306 and will
 // print dots until then, as this is not a blocking function.
 void connectToDatabase(char* DatabaseUser,char* DatabasePassword, MySQL_Connection conn) {   
  Serial.println("DB - Connecting...");
   while (conn.connect(remote_ip, 3306, DatabaseUser, DatabasePassword) != true) {
     delay(500);
     Serial.print ( "." );
   }
 }

 // This function executes the query given to the function, the character 
 // pointer as well as the MYSQL_Connection object. It creates a new MYSQL_Cursor. 
 // It will then insert the code into the table through updateTime();
 void executeQuery(char* INSERT_SQL, MySQL_Connection conn) {
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
  cur_mem->execute(INSERT_SQL);
 }

 // checkPir() will update the value of the motion state. 
 //If a value is 1, it will update the state and will call the updateTime()
 // function, which calls executeQuery. Everything is happenening here.
 void checkPir() {
  val = digitalRead(inputPirPin);  
  
  if (val == HIGH) {            
    digitalWrite(ledPin, HIGH);  
    if (pirState == LOW) { 
      Serial.println("Motion detected!");
      pirState = HIGH;
      // Update Time to databse.
      updateTime(zone,dst);
    }
  } else { 
    digitalWrite(ledPin, LOW); 
    if (pirState == HIGH){
      Serial.println("Motion ended!");
      pirState = LOW;
    }
  }
  
 }


