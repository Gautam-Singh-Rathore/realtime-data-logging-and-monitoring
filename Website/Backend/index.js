const mqtt = require("mqtt");
const express = require("express");
const cors = require("cors");

const brokerUrl = "mqtt://172.16.28.174:1883"; // Ensure consistency with ESP
const topics = ["receive_data", "ESP8266RES"];
const mqttClient = mqtt.connect(brokerUrl);

let logs = { "1": [], "2": [], "3": [], "4": [] };

mqttClient.on("connect", () => {
    console.log("Connected to MQTT broker");
    topics.forEach((topic) => mqttClient.subscribe(topic));
});

mqttClient.on("message", (topic, message) => {
    try {
        const data = JSON.parse(message.toString());
        const machineId = data.id ? data.id.toString() : "unknown";

        if (!logs[machineId]) return;

        const logEntry = {
            id: machineId,
            message: data.message || "No message",
            date: new Date().toISOString().split("T")[0],
            time: new Date().toISOString().split("T")[1].split(".")[0]
        };

        logs[machineId].unshift(logEntry);
        if (logs[machineId].length > 10) logs[machineId].pop();

        console.log(`New log added for Machine ${machineId}: ${JSON.stringify(logEntry)}`);
    } catch (err) {
        console.error("Failed to process MQTT message:", err.message);
    }
});

const app = express();
app.use(cors());
app.use(express.json()); // Important for parsing JSON in POST requests

app.get("/latest-log", (req, res) => {
    res.json(logs);
});

app.post("/control-machine", (req, res) => {
    const { machineId, task } = req.body;

    if (!machineId || !["start", "stop"].includes(task)) {
        return res.status(400).json({ error: "Invalid machine ID or task" });
    }

    const commandMessage = JSON.stringify({ id: machineId, task });
    mqttClient.publish("receive_data", commandMessage, (err) => {
        if (err) return res.status(500).json({ error: "Failed to send command" });
        console.log(`Command sent: ${commandMessage}`);
        res.json({ success: true, message: `Machine ${machineId} ${task} command sent` });
    });
});

app.listen(3000, () => {
    console.log("HTTP server running on port 3000");
});
