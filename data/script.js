document.addEventListener('DOMContentLoaded', (event) => {
  const yesBtn = document.getElementById('yesBtn');
  const noBtn = document.getElementById('noBtn');
  const questionContainer = document.getElementById('questionContainer');
  const statusMessage = document.getElementById('statusMessage');

  let currentQuestion = 0;
  const questions = [
      "Do you have a fever?",
      "Are you experiencing any pain?",
      "Have you had any recent injuries?",
      "Do you have any allergies?",
      "Are you currently taking any medications?",
      "Do you have any chronic conditions?",
      "Have you experienced any recent symptoms?",
      "Are you experiencing any dizziness?",
      "Do you have a history of heart disease?",
      "Are you experiencing any shortness of breath?"
  ];

  function loadQuestion() {
      if (currentQuestion < questions.length) {
          document.querySelector('label[for="question"]').textContent = questions[currentQuestion];
      } else {
          questionContainer.style.display = 'none';
          statusMessage.textContent = 'Thank you for answering the questions. Your information is being processed...';
          submitAnswers();
      }
  }

  function submitAnswer(answer) {
      fetch('/answer?' + new URLSearchParams({
          answer: answer
      }))
      .then(response => response.text())
      .then(data => {
          currentQuestion++;
          loadQuestion();
      })
      .catch(error => {
          statusMessage.textContent = 'An error occurred.';
      });
  }

  yesBtn.addEventListener('click', () => {
      submitAnswer('Yes');
  });

  noBtn.addEventListener('click', () => {
      submitAnswer('No');
  });

  loadQuestion();
});
