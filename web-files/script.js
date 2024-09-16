
let ws;
const connectBtn = document.getElementById('connectBtn');
const userFeedback = document.querySelector("#user-feedback");
const connectionStatus = document.getElementById('status');
connectBtn.onclick = connectToSocket; 


let temperature, heartRate;
const add = true, remove = false;

function alertUserLoading(addOrRemove) {
  userFeedback.innerHTML = (addOrRemove) ?`<i>Connecting......</i>` : ``;
}

function connectToSocket() {
  const esp32IpAddress = document.getElementById('esp32IpAddress').value;
  // check if ipAddress was added
  if(!esp32IpAddress) {
    userFeedback.innerHTML = `<i style="color:red;">ESP32 IP Address must be entered to establish connection</i>`
    return;
  }
  alertUserLoading(add);
  // using wss:// is safer as it can work for both http and https unlike ws that can only work for http
  const wsUrl = `wss://${esp32IpAddress}:81/`; // Connect to WebSocket server at ESP32 IP and port 81

  ws = new WebSocket(wsUrl); //

  ws.onopen = function() {//once connection is established
    connectionStatus.textContent = 'Status: Connected';
    connectionStatus.classList.remove("not-connected");

    console.log('Connected to WebSocket server');
    alertUserLoading(remove);
  };

  ws.onmessage = function(event) { //once a new message is received
    const data = JSON.parse(event.data);// the string into object
    temperature = parseFloat(data.temperature);
    heartRate = parseInt(data.heartRate);
    console.log(data);
    document.getElementById('temperature').textContent = 'Temperature ' + data.temperature;
    document.getElementById('heart-rate').textContent = 'Heart Rate: ' + data.heartRate;
  };

  ws.onclose = function() { //once the connection is closed
    connectionStatus.textContent = 'Status: Disconnected';
    connectionStatus.classList.add("not-connected");
    console.log('Disconnected from WebSocket server');
  };

  ws.onerror = function() { //once error is encountered in connection
    document.getElementById('status').textContent = 'Status: Connection Error';
    console.log('Error connecting to WebSocket server');
  };

  if(heartRate >= 0) {
    // disconnect when heart rate is read.
    console.log("heart rate exceeds 0");
    ws.close();
  }
};

const questions = [
  "Are you unable to sleep?",
  "Are you currently taking any medications prescribed by your doctor?",
  "Are you currently pregnant?", 
  "Do you have difficulty breathing?",
  "Do you have a cough?",
  "Do you have a headache?",
  "Are you experiencing fatigue?",
  "Are you experiencing any pain or discomfort in your abdomen?",
  "Is your energy level low these days?",
  "Have you noticed any changes in your appetite?",
  "Are you experiencing nausea or vomiting?"
];

function submitForm() {
  const formData = getFormData();
  const prescription = generatePrescription(formData);
  document.getElementById('prescriptionResult').innerHTML = `<strong>Prescription:</strong><br>${prescription.replace(/\n/g, '<br>')}`;
}

generateQuestions();

function generateQuestions() {
const questionContainer = document.getElementById('healthForm');

questions.forEach((question, index) => {
    const div = document.createElement('div');

    
    div.innerHTML = `
        <label>${question}</label>
        <input type="radio" id="q${index}yes" name="q${index}" value="yes">
        <label class="answer-btn" for="q${index}yes">Yes</label>
        <input type="radio" id="q${index}no" name="q${index}" value="no">
        <label class="answer-btn no-btn" for="q${index}no">No</label>
    `;

    questionContainer.appendChild(div);
});

}

function getFormData() {
    const formData = {};
    questions.forEach((_, index) => {
        if (index >= 11) {
            formData[`q${index}`] = document.querySelector(`input[name="q${index}"]`)?.value || '';
        } else {
            formData[`q${index}`] = document.querySelector(`input[name="q${index}"]:checked`)?.value || 'no';
        }
    });
    formData['age'] = document.getElementById('age').value;
    return formData;
}

function generatePrescription(formData) {
    let prescription = "Based on your responses, we suggest that you take appropriate measures. ";

    // General advice
    if (formData['q0'] === 'yes') {
        prescription += "Consider using a sleep aid such as Doxepin or Temazepam. ";
    }
    if (formData['q1'] === 'yes') {
        prescription += "Continue following your prescribed medication regimen. ";
    }
    if (formData['q2'] === 'yes') {
        prescription += "Consult with your healthcare provider for appropriate prenatal care. ";
    }

    // Specific conditions
    if (formData['q3'] === 'yes') {
        prescription += "You may need medication for breathing difficulties such as Albuterol. ";
    }
    if (formData['q4'] === 'yes') {
        prescription += "Consult a doctor for a possible cough suppressant. ";
    }
    if (formData['q5'] === 'yes') {
        prescription += "Consider over-the-counter pain relievers like Acetaminophen. ";
    }
    if (formData['q6'] === 'yes') {
        prescription += "Try a remedy for fatigue such as Iron supplements or Vitamin B12. ";
    }
    if (formData['q7'] === 'yes') {
        prescription += "Consider consulting a gastroenterologist for abdominal pain. ";
    }
    if (formData['q8'] === 'yes') {
        prescription += "Consider a multivitamin supplement if your energy is low. ";
    }
    if (formData['q9'] === 'yes') {
        prescription += "Monitor your diet for any significant changes in appetite. ";
    }
    if (formData['q10'] === 'yes') {
        prescription += "You may need to see a doctor for nausea or vomiting. ";
    }

    console.log("temperature: " + temperature + "heart rate: " + heartRate);
    if (!(typeof temperature === 'undefined' && typeof heartRate === 'undefined')) {
      if (temperature > 38 || temperature < 36) {
        prescription += "\nYour Temperature is abnormal, You might have a fever. Consider taking antipyretics like Ibuprofen or Paracetamol. ";
      }else {
        prescription += "\nYour Temperature is normal. ";
      }

      if (heartRate > 100 || heartRate < 60) {
          prescription += "Your heart rate is abnormal, Your heart rate is elevated. Consider seeing a cardiologist for further evaluation. ";
      }else {
        prescription += "Your heart rate is normal.";
      }
    }

    return prescription;
}