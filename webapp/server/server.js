const express = require("express");
const app = express();
fs = require("fs");

const bodyParser = require("body-parser");
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));

const SERVER_PORT = process.env.SERVER_PORT || 5000;

app.get("/", (req, res) => {
  console.log("hello world");
  res.send("Hello World!");
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
app.listen(SERVER_PORT, () => {
  console.log(`Server running on port ${SERVER_PORT}`);
  console.log(`http://localhost:${SERVER_PORT}`);
});
