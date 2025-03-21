const mqtt = require("mqtt");
const express = require("express");
const cors = require("cors");

// HTTP Server to Fetch the Latest Log
const app = express();

// Enable CORS for all origins (you can limit this to specific origins if needed)
app.use(cors());

var mqttClient = mqtt.connect("mqtt://broker.hivemq.com");

// Variable to store the latest log received via MQTT
let latestLog = null;

// Handle MQTT connection
mqttClient.on("connect", () => {
    console.log("Connected to MQTT broker");
    mqttClient.subscribe("GautamSinghRathore");
});

// Handle incoming MQTT messages
mqttClient.on("message", (topic, message) => {
    try {
        const data = JSON.parse(message.toString());

        // Update the latest log with the received data
        latestLog = data;

        console.log(`Data received : ${JSON.stringify(data)}`);
    } catch (err) {
        console.error("Failed to process MQTT message:", err.message);
    }
});



// Endpoint to serve the latest log
app.get("/latest-log", (req, res) => {
    if (latestLog) {
        res.json(latestLog);
    } else {
        res.status(404).json({ message: "No log available" });
    }
});

app.listen(3000, () => {
    console.log("HTTP server running on port 3000");
});
