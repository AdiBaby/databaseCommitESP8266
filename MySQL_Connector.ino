 #include <ESP8266WiFi.h>
 #include <WiFiClient.h>
 #include <ESP8266Ping.h>
 #include <MySQL_Connection.h>
 #include <MySQL_Cursor.h>
 #include <time.h>
 
 const IPAddress remote_ip(10,0,1,157);   
 char* DatabaseUser = "aditya";         
 char* DatabasePassword = "adityapc"; 
 char* WifiSsid = "onhub";
 char* WifiPass = "ekchotaghar";
 int zone = 4 * 3600;
 int dst = 0;
 char* exe_SQL = "";

void executeQuery(char*, MySQL_Connection);
void updateTime(int, int);
 
 WiFiClient client;
 MySQL_Connection conn((Client *)&client);
 
 
 void setup() {
   Serial.begin(115200);
   connectToWifi(WifiSsid, WifiPass);
   pingDatabase(remote_ip);
   configTime(zone, dst, "pool.ntp.org", "time.nist.gov");
   connectToDatabase(DatabaseUser, DatabasePassword, conn);
 }

void updateTime(int zone, int dst) {
  char INSERT_SQL[1024];
  //configTime(zone, dst, "pool.ntp.org", "time.nist.gov");
  time_t now = time(nullptr);
  sprintf(INSERT_SQL, "INSERT INTO test_arduino.hello_arduino (message) VALUES (' " " %s " " , " "%s" "')", "Hey ddududude!", ctime(&now));
  // TD  : PUT TIME AS ONE OF THE PARAMS OF THE INSERT, ON DB.
  Serial.printf("%s; time:%s", INSERT_SQL, ctime(&now));
  executeQuery(INSERT_SQL, conn);
 }
 
 void loop() {
  updateTime(zone, dst);
  delay(10000);
 }

 void pingDatabase(IPAddress ip_addr) {
  Serial.print("Pinging DB ");
  Serial.println(ip_addr);
   if(Ping.ping(remote_ip)) {
     Serial.println("Success!!");
   } else {
     Serial.println("Error :(");
   }
 }

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

 void connectToDatabase(char* DatabaseUser,char* DatabasePassword, MySQL_Connection conn) {   
  Serial.println("DB - Connecting...");
   while (conn.connect(remote_ip, 3306, DatabaseUser, DatabasePassword) != true) {
     delay(500);
     Serial.print ( "." );
   }
 }

 void executeQuery(char* INSERT_SQL, MySQL_Connection conn) {
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
  cur_mem->execute(INSERT_SQL);
 }



