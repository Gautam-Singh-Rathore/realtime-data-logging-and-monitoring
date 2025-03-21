// const mqtt = require("mqtt");
// const WebSocket = require("ws");

// // MQTT Broker configuration
// const brokerUrl = "mqtt://192.168.7.151:1883"; 
// const topics = ["ESP8266RES", "ESP8266CMD"];

// // WebSocket Server
// const wss = new WebSocket.Server({ port: 9080 });

// // In-memory cache for the latest 50 logs
// const latestLogs = [];

// // MQTT Client Setup
// const mqttClient = mqtt.connect(brokerUrl);

// // Handle MQTT connection
// mqttClient.on("connect", () => {
//   console.log("Connected to MQTT broker");

//   // Subscribe to relevant topics
//   topics.forEach((topic) => mqttClient.subscribe(topic));
// });

// // Handle incoming MQTT messages
// mqttClient.on("message", (topic, message) => {
//   try {
//     const data = JSON.parse(message.toString());
//     data.timestamp = new Date().toISOString(); // Add a timestamp

//     // Maintain only the latest 50 logs
//     latestLogs.push(data);
//     if (latestLogs.length > 50) latestLogs.shift();

//     // Broadcast the data to all connected WebSocket clients
//     wss.clients.forEach((client) => {
//       if (client.readyState === WebSocket.OPEN) {
//         client.send(JSON.stringify(data));
//       }
//     });

//     console.log(`Data received and broadcasted: ${JSON.stringify(data)}`);
//   } catch (err) {
//     console.error("Failed to process MQTT message:", err.message);
//   }
// });

// // WebSocket: Handle client connections
// wss.on("connection", (ws) => {
//   console.log("WebSocket client connected");

//   // Send the latest logs on connection
//   ws.send(JSON.stringify({ type: "latestLogs", data: latestLogs }));
// });

// // HTTP Server to Fetch Logs (Optional)
// const express = require("express");
// const app = express();

// app.get("https://g-backend-t9el.onrender.com/api/send", (req, res) => {
//   res.json(latestLogs);
// });

// app.listen(3000, () => {
//   console.log("HTTP server running on port 3000");
// });