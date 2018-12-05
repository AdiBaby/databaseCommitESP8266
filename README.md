# databaseCommitESP8266


This project is a simple framework to insert into a mysql table. I used all the connections on the same network, using one linux
computer as the database, and my chip accessed it through a user I setup for it specifically. The way you do this is 

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

