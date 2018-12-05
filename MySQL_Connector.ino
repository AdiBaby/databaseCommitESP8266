 //All libraries used
 #include <ESP8266WiFi.h>
 #include <WiFiClient.h>
 #include <ESP8266Ping.h>
 #include <MySQL_Connection.h>
 #include <MySQL_Cursor.h>
 #include <time.h>

 //Global variables
 // My MySQL server IP. 
 const IPAddress remote_ip(10,0,1,157);
 // All users and passwords of the database and your wifi.
 char* DatabaseUser = "------";         
 char* DatabasePassword = "-----"; 
 char* WifiSsid = "-------";
 char* WifiPass = "------";
// Time zone. In California, it is 4. 
// Change yours to whatever you need.
 int zone = 4 * 3600;
// Always 0.
 int dst = 0;
// Init the char * that will be inserted into mysql.
 char* exe_SQL = "";

//Define function names and signatures.
void executeQuery(char*, MySQL_Connection);
void updateTime(int, int);
 
// Define WifiClient and the MYSQL connection itself.
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
 void setup() {
   Serial.begin(115200);
   connectToWifi(WifiSsid, WifiPass);
   pingDatabase(remote_ip);
   configTime(zone, dst, "pool.ntp.org", "time.nist.gov");
   connectToDatabase(DatabaseUser, DatabasePassword, conn);
 }
 // Loop, runs forever. All this does is call updateTime, which updates
 // the time and also calls the executeQuery() function to update the database
 // itself. It updates every ten seconds, as seen by the delay
void loop() {
  updateTime(zone, dst);
  delay(10000);
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

 void connectToDatabase(char* DatabaseUser, char* DatabasePassword, MySQL_Connection conn) {   
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

void updateTime(int zone, int dst) {
  char INSERT_SQL[1024];
  //configTime(zone, dst, "pool.ntp.org", "time.nist.gov");
  time_t now = time(nullptr);
  sprintf(INSERT_SQL, "INSERT INTO test_arduino.hello_arduino (message) VALUES (' " " %s " " , " "%s" "')", "Hey ddududude!", ctime(&now));
  // TD  : PUT TIME AS ONE OF THE PARAMS OF THE INSERT, ON DB.
  Serial.printf("%s; time:%s", INSERT_SQL, ctime(&now));
  executeQuery(INSERT_SQL, conn);
 }



