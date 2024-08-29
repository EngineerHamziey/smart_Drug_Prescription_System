document.addEventListener("DOMContentLoaded", function() {
  function updateReadings() {
      fetch("/data")
          .then(response => response.json())
          .then(data => {
              document.getElementById("temperature").innerText = data.temperature.toFixed(2);
              document.getElementById("heartrate").innerText = data.heartrate;
          })
          .catch(error => console.error("Error fetching data:", error));
  }

  // Update readings every 2 seconds
  setInterval(updateReadings, 2000);
});
