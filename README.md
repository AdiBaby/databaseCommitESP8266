# MotionSensordatabaseCommitESP8266


UPDATE 12-6-18 : 
Implemented a motion sensor in the circuit, so whenever the PIR sensor senses motion, it will automatically update the data 
to the database. NOTE - The pinout on the ESP8266 (In my case) was screwed up, I used a multimeter to measure where the 
current was coming from when I called current on specific Pins, here are the results : 
 - Calling pin 5 will send current to D1.
 - Calling pin 4 will send current to D2. 
 - Calling pin 3 goes who knows where.
 - Calling pin 2 as OUTPUT will result in current from D4, but calling INPUT will short it (Story of my first ESP8266).
 
Also, the PIR motion sensor I am using (HC - SR501) only takes 5V on VCC. The ESP only gives at max 3.3V. DONT PANIC!
A quick fix is to get another old arduino board, and connect the 5V from there to your PIR, and the GND on the 
old arduino to the ESP. This worked for me.

This project is a simple framework to insert into a mysql table. I used all the connections on the same network, using one linux computer as the database, and my chip accessed it through a user I setup for it specifically. The way you do this is 

GRANT ALL PRIVILEGES ON *.* TO 'user'@'ip_of_chip' IDENTIFIED BY 'your_password'

NOTE : THIS STEP IS VERY IMPORTANT. BE SURE TO ASSIGN IT TO THE IP OF THE ESP. IF YOU HAVE TO CHECK, USE Wifi.localIP() to check.

To create the database : 

CREATE DATABASE test_arduino;

To create the table : 

CREATE TABLE hello_arduino (message varchar(100) not null, time varchar(200) not null);

When you setup the ESP itself, change the change the SSID and password to your wifi network and password. Also, change your
myqsl user and password to the parameters you specified above.

Then, setup a database and table for arduino. I used a database test_arduino and the table hello_arduino. If you name your 
database and table to something different, change the INSERT_SQL line from test_arduino.hello_arduino to whatever you need. 

This program also inserts the time. The time is used by the configTime() API. Another possible way to do this is to update the 
time from mysql itself, but I was lazy and decided to just update it with the ESP because I wanted to save it as a String.


Good Luck with your project. 



