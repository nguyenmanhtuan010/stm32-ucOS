const express = require('express');
const app = express();

var server = require("http").Server(app);
var io = require("socket.io")(server);
server.listen(3000);

app.use('/static', express.static(__dirname + "/static"));
app.get('/',function(req, res){
    res.sendfile(__dirname + '/static/main.html');
});
var mqtt = require('mqtt');
var Topic1 = 'temp1';
var Topic2 = 'humi1';
var Topic3 = 'temp2';
var Topic4 = 'humi2';
var Topic5 = 'light1'; 
var Topic6 = 'light2';//subscribe to all topics
var Topic7 = 'batden1';
var Topic8 = 'batden2';
var Broker_URL = 'mqtt://192.168.43.132';
var Database_URL = '192.168.43.132';

var options = {
    clientId: 'MyMQTT',
    port: 1883,
    keepalive: 60
};

var client = mqtt.connect(Broker_URL, options);
client.on('connect', mqtt_connect); 
client.on('reconnect', mqtt_reconnect);
client.on('error', mqtt_error);
client.on('message', mqtt_messsageReceived);
client.on('close', mqtt_close);

function mqtt_connect() {
    //console.log("Connecting MQTT"); 
    client.subscribe(Topic1, mqtt_subscribe);
    client.subscribe(Topic2, mqtt_subscribe);
    client.subscribe(Topic3, mqtt_subscribe);
    client.subscribe(Topic4, mqtt_subscribe);
    client.subscribe(Topic5, mqtt_subscribe);
    client.subscribe(Topic6, mqtt_subscribe);
    client.subscribe(Topic7, mqtt_subscribe);
    client.subscribe(Topic8, mqtt_subscribe);

}

function mqtt_subscribe(err, granted) {
    console.log("Subscribed to " + Topic1);
    console.log("Subscribed to " + Topic2);
    console.log("Subscribed to " + Topic3);
    console.log("Subscribed to " + Topic4);
    console.log("Subscribed to " + Topic5);
    console.log("Subscribed to " + Topic6);
    console.log("Subscribed to " + Topic7);
    console.log("Subscribed to " + Topic8);
    if (err) { console.log(err); }
}

function mqtt_reconnect(err) {
    //console.log("Reconnect MQTT");
    //if (err) {console.log(err);}
    client = mqtt.connect(Broker_URL, options);
}

function mqtt_error(err) {
    //console.log("Error!");
    //if (err) {console.log(err);}
}

function after_publish() {
    //do nothing
}

function mqtt_messsageReceived(topic, message, packet) {
    //console.log('Message received = ' + message);
    insert_message(topic, message, packet);
}


function mqtt_close() {
    //console.log("Close MQTT");
}

var mysql = require('mysql');

//Create Connection
var connection = mysql.createConnection({
    host: Database_URL,
    user: "smthome",
    password: "mypassword",
    database: "mydb"
});

connection.connect(function(err) {
    if (err) throw err;
    //console.log("Database Connected!");
});
var TEMP1 = 0;
var HUMI1 = 0;
var TEMP2 = 0;
var HUMI2 = 0;
var LIGHT1 = 0;
var LIGHT2 = 0;

function insert_message(topic, message, packet) {
    // var message_arr = extract_string(message_str); //split a string into an array

    if (topic == Topic1) {
        TEMP1 = message.toString();
    }
    if (topic == Topic2) {
        HUMI1 = message.toString();
    }
    if (topic == Topic3) {
        TEMP2 = message.toString();
    }
    if (topic == Topic4) {
        HUMI2 = message.toString();
    }
    if (topic == Topic5) {
        LIGHT1 = message.toString();
    }
    if (topic == Topic6) {
        LIGHT2 = message.toString();
    }
    console.log(TEMP1);
    console.log(HUMI1);
    console.log(LIGHT1);
    console.log(TEMP2);
    console.log(HUMI2);
    console.log(LIGHT2);

    var sql = "INSERT INTO ?? (??,??,??,??,??,??) VALUES (?,?,?,?,?,?)";
    var params = ['dlcb', 'TEMP1', 'HUMI1', 'LIGHT1', 'TEMP2', 'HUMI2', 'LIGHT2', TEMP1, HUMI1, LIGHT1, TEMP2, HUMI2, LIGHT2];
    sql = mysql.format(sql, params);

    connection.query(sql, function(error, results) {
        if (error) throw error;
        //console.log(message.toString());
    });
    io.sockets.emit(topic, message.toString())
};
io.on("connection", function(socket) {
    console.log("co nguoi ket noi:" + socket.id);
    socket.on("disconnect", function() {
        console.log(socket.id + "disconnect");
    });

    socket.on("den1 bat", function(data) {
        client.publish(Topic7, data);
    })
    socket.on("den1 tat", function(data) {
        client.publish(Topic7, data);
    })

    socket.on("den2 bat", function(data) {
        client.publish(Topic8, data);
    })
    socket.on("den2 tat", function(data) {
        client.publish(Topic8, data);
    })


});