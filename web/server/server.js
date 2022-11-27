const express = require("express");
const app = express();
const fs = require("fs");
const net = require("net");
const moment = require("moment");
const ejs = require("ejs");
const bodyParser = require("body-parser");
const { time } = require("console");
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));
app.set("view engine", "ejs");
const months = {
  Jan: '01',
  Feb: '02',
  Mar: '03',
  Apr: '04',
  May: '05',
  Jun: '06',
  Jul: '07',
  Aug: '08',
  Sep: '09',
  Oct: '10',
  Nov: '11',
  Dec: '12',
}
const SERVER_PORT = process.env.SERVER_PORT || 5000;

const tcpServer = net.createServer(function (client) {
  console.log("MSP432 Is Connected")
  client.setEncoding('utf-8')
  client.on('data', function (data) {
    console.log(data)
      try {
        //parse the data into json
        const json = JSON.parse(data);
        //get the current time
        //convert time to utc+10
        const utctime = moment().format("YYYY-MM-DD HH:mm:ss");
        //datatime split into array 
        const datetime = json.time.split(" ");
        // convert month to number
        const date = datetime[2];
        const year = datetime[4];
        const time = datetime[3];
        const month = months[datetime[1]];
        // iso format string
        const iso = `${year}-${month}-${date}T${time}`;
        //convert to moments
        const momentTime = moment(iso).format("YYYY-MM-DD HH:mm:ss");
        //offset by  minus 14h
        const timeNow = moment().subtract(14, 'hours').format("YYYY-MM-DD HH:mm:ss");
        
        //differnece between time now and time received
        var duration = moment.duration(moment(timeNow).diff(moment(momentTime)));
        var secondsPassed = duration.asSeconds();
        //append json data
        json.duration = secondsPassed;       

   
        //transform json to string
        const string = JSON.stringify(json);
        const writeMSPData = fs.writeFileSync("data.json", string);
        if (json.device == "ESP8266") {
        //append to esp 8266 json array
        const esp8266 = fs.readFileSync("ESP8266Logs.json");
        let esp8266Json = JSON.parse(esp8266);
        esp8266Json.push(json);
        fs.writeFileSync("ESP8266Logs.json", JSON.stringify(esp8266Json));
        console.log("data pushed to esp8266.json");

        }else if (json.device == "m5stick"){
        //append to m5stick json array
        const m5stick = fs.readFileSync("M5StickLogs.json");
        let m5stickJson = JSON.parse(m5stick);
        m5stickJson.push(json);
        fs.writeFileSync("M5StickLogs.json", JSON.stringify(m5stickJson));
        console.log("data pushed to m5stick.json");

        }
      } catch (err) {
        console.log(err)
      }
     
  })
  client.on('end', function () {
    console.log("MSP432 Disconnected")
  })

});

tcpServer.listen(SERVER_PORT, () => {
  console.log("TCP Server Listening on Port:5000");
});
  

app.get("/", (req, res) => {
  //read the data from the file
  const data = fs.readFileSync("data.json", "utf8");
  //parse the data into json
  const jsonData = JSON.parse(data);
  const esp8266 = fs.readFileSync("ESP8266Logs.json");
  const m5stick = fs.readFileSync("M5StickLogs.json");

  //get last 10 data from esp8266
  let esp8266Json = JSON.parse(esp8266);
  let m5stickJson = JSON.parse(m5stick);
  esp8266Json = esp8266Json.slice(-10);
  m5stickJson = m5stickJson.slice(-10);
  //retrieve  array json time data
  const esp_duration = esp8266Json.map((item) => item.duration);
  const m5stick_duration = m5stickJson.map((item) => item.duration);
  console.log(esp_duration);

  res.render("dashboard", { data: jsonData , esp8266: esp8266Json,m5stick: m5stickJson, esp_duration: esp_duration, m5stick: m5stickJson, m5stick_duration: m5stick_duration});
});

app.post("/", (req, res) => {
  console.log(req.body);
  console.log("hello from post");
  // Convert json to sting 
  let data = JSON.stringify(req.body);
  //escape json brackets

  fs.writeFile("test.json", data, function(err) {
    if (err) {
      return console.log(err);
    }
    console.log("The file was saved!");
  });
  res.send("Hello World!");
});
app.listen(3001, () => {
  console.log(`Server running on port ${3001}`);
  console.log(`http://localhost:${3001}`);
});
