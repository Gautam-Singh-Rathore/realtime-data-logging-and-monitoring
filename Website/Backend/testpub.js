const mqtt = require("mqtt");

var client = mqtt.connect("mqtt://broker.hivemq.com");

client.on("connect", function () {
    setInterval(function () {
        var random = Math.random() * 50;

        // Only publish if the random number is less than 30
        if (random < 30) {
            // Get current date and time
            const now = new Date();
            const date = now.toLocaleDateString(); // Format: MM/DD/YYYY
            const time = now.toLocaleTimeString(); // Format: HH:mm:ss

            // Construct the message in the desired format
            const message = {
                message: "left-aligned message",
                date: date,
                time: time
            };

            // Publish the message to the "Test" topic
            client.publish("GautamSinghRathore", JSON.stringify(message));
            console.log("Message published:", JSON.stringify(message));
        }
    }, 3000); // Publish every 30 seconds
});
