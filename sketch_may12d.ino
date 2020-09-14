#include <ESP8266WiFi.h>
#include <WiFiClient.h>

//ESP Web Server Library to host a web page
#include <ESP8266WebServer.h>

//---------------------------------------------------------------
//Our HTML webpage contents in program memory
const char MAIN_page[] PROGMEM = R"=====(<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
<meta name="author" content="Igor Fonseca Albuquerque">
<style>
* {
  box-sizing: border-box;
}
.column1 {
  float: left;
  width:35%;
  padding: 0px;
  height: 470px; 
}
.column2 {
  float: left;
  width:65%;
  padding:0px;
  height: 470px; 
}
.column3 {
  float: left;
  width:100%;
  padding:0px;
  height: 150px; 
}

.row:after {
  content: "";
  display: table;
  clear: both;
}
h4 {
  background-color: lightgreen;
}
.column1 a{
  background-color: #f44336;
  color: white;
  padding: 14px 14px;
  text-align: center;
  text-decoration: none;
  display: inline-block;
}

.column1 a:hover, .column1 a:active {
  background-color: red;
}
</style>
<script>
function readUrlAV (form) {
    TextVar = form.inputbox.value;
 VideoVar = "http://"+TextVar+":8080/video";
  AudioVar = "http://"+TextVar+":8080/audio.opus";
  document.getElementById("video").setAttribute('data', VideoVar);
  document.getElementById("audio").setAttribute('data', AudioVar);
}
</script>


</head>
<body>

<div class="row">

    <div class="column1" style="border:solid;">
  <h2 align="center">MOVEMENT CONTROL</h2>
    <div style="text-align:center;">
      
      <a href="fore" target="myIframe">FRONT</a><br> <br>
      <a href="left" target="myIframe">LEFT</a> 
      <a href="stop" target="myIframe">STOP</a> 
      <a href="right" target="myIframe">RIGHT </a><br> <br> 
      <a href="reve" target="myIframe">BACK </a>
      
  </div>
<h3>State</h3>: <iframe name="myIframe" width="100" height="25" frameBorder="0"></iframe><br>
  </div>
    

  <div class="column2" style="border:solid;">
 
<h2 align="center"> AUDIO AND VIDEO</h2>
      <form name="myform" action="" method="GET">
        IP Webcam (IP):
        <input type="text" name="inputbox" value="192.168.0.5">
        <input type="button" name="button1" value="Load" onClick="readUrlAV(this.form)">
      </form>

      <object id="video" type="text/html" data="http://192.168.0.5:8080/video" 
        style="position: absolute; width:100%; height:380px;">
      </object>
      
      <object id="audio" type="text/html" data="http://192.168.0.5:8080/audio.opus"
        style="position: absolute; width:250px; height:30px; top:425px;">
      </object>
 
  </div>

  
              
  

  <div class="column3" style="border:solid;" >
 
  <h2 align="center">SENSOR OUTPUT</h2>
    <h4 align="center">Alert: <span id="ADCValue">0</span> P.P.M of Smoke is Detected</h4>
    <h5 align="right">At Normal Room Temperature ~~69P.P.M</h5>
  
   </div>

<script>
 
setInterval(function() {
  
  getData();
}, 2000); 
function getData() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("ADCValue").innerHTML =
      this.responseText;
    }
  };
  xhttp.open("GET", "readADC", true);
  xhttp.send();
}
</script>

</div>

</body>
</html>




)=====";
//---------------------------------------------------------------
//On board LED Connected to GPIO2
#define log1  13
#define log2  5
#define log3  4
#define log4  14  

//SSID and Password of your WiFi router
const char* ssid = "Tenda_0CF358";
const char* password = "12345678";


ESP8266WebServer server(80); //Server on port 80

//===============================================================
// This routine is executed when we open its IP in browser
//===============================================================
void handleRoot() {
 Serial.println("called root page");
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}

void handleForeward() { 
 Serial.println("Foreward");
 digitalWrite(log1,HIGH);
 digitalWrite(log2,LOW);
digitalWrite(log3,HIGH);
digitalWrite(log4,LOW);
 server.send(200, "text/html", "Foreward"); //Send ADC value only to client ajax request
}

void handleReverse() { 
 Serial.println("Reverse");
 digitalWrite(log1,LOW); 
digitalWrite(log2,HIGH);
digitalWrite(log3,LOW);
digitalWrite(log4,HIGH);
 server.send(200, "text/html", "Reverse"); //Send ADC value only to client ajax request
}

void handleLeft() { 
 Serial.println("Left");
 digitalWrite(log1,LOW); 
digitalWrite(log2,LOW);
digitalWrite(log3,HIGH);
digitalWrite(log4,LOW);
 server.send(200, "text/html", "Left"); //Send ADC value only to client ajax request
}

void handleRight() { 
 Serial.println("Right");
 digitalWrite(log1,HIGH); 
digitalWrite(log2,LOW);
digitalWrite(log3,LOW);
digitalWrite(log4,LOW);
 server.send(200, "text/html", "Right"); //Send ADC value only to client ajax request
}

void handleStop() { 
 Serial.println("Stop");
 digitalWrite(log1,LOW); 
digitalWrite(log2,LOW);
digitalWrite(log3,LOW);
digitalWrite(log4,LOW);
 server.send(200, "text/html", "Stop"); //Send ADC value only to client ajax request
}

void handleADC() {
 int a = analogRead(A0);
 String adcValue = String(a);
 
 server.send(200, "text/plane", adcValue); //Send ADC value only to client ajax request
}
//==============================================================
//                  SETUP
//==============================================================
void setup(void){
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);     //Connect to WiFi router
  Serial.println("");

  //Onboard LED port Direction output
  pinMode(log1,OUTPUT); 
pinMode(log2,OUTPUT);
pinMode(log3,OUTPUT);
pinMode(log4,OUTPUT);
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned our ESP
 
  server.on("/", handleRoot);      //Which routine to handle at root location. This is display page
  server.on("/fore", handleForeward); //as Per  , Subroutine to be called
  server.on("/reve", handleReverse);
server.on("/left", handleLeft);
server.on("/right", handleRight);
server.on("/stop", handleStop);
 server.on("/readADC", handleADC);
  server.begin();                  //Start server
  Serial.println("HTTP server started");
}
//==============================================================
//                     LOOP
//==============================================================
void loop(void){
  server.handleClient();          //Handle client requests
}
