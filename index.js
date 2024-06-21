const express = require('express');
const path = require('path');
const bodyParser = require('body-parser');
const addon = require('./build/Release/addon');

const app = express();
const port = 3000;

app.use(express.static(path.join(__dirname, 'public')));
app.use(bodyParser.urlencoded({ extended: true }));

app.get('/', (req, res) => {
  res.sendFile(path.join(__dirname, 'public/index.html'));
});

app.post('/check-privilege', (req, res) => {
  const username = req.body.username;
  const privilege = addon.getUserPrivilege(username);
  if (privilege === "Пользователя нет") {
    res.send(`Пользователь ${username} не найден`);
  } else {
    res.send(`Пользователь ${username} имеет привилегию ${privilege}`);
  }
});

app.listen(port, () => {
  console.log(`Server running at http://localhost:3000`);
});