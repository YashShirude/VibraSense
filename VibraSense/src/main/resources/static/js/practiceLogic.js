console.log("Loaded practiceLogic.js Successfully");

let interval;
let highlightSpeed = 200;  // Default speed set to 200 ms
const speedLabels = {
    600: "Slow Speed",
    350: "Normal Speed",
    150: "Fast Speed"
};

// Listen for DOM content to load and initialize typing
document.addEventListener("DOMContentLoaded", function () {
    document.addEventListener("keydown", handleKeyEvents);

    // Retrieve text and highlight speed from session storage
    let exercise = sessionStorage.getItem('exercise');
    highlightSpeed = parseInt(sessionStorage.getItem('highlightSpeed')) || 200;
});

// Function to update speed on the sidebar
function updateSpeedOnSidebar() {
    const sidebar = document.getElementById('settingsSidebar');
    const speedDisplay = document.getElementById('currentSpeed');
    if (speedDisplay) {
        speedDisplay.textContent = speedLabels[highlightSpeed];
    }
    sidebar.style.display = 'block';  // Show the sidebar

    try {
        sessionStorage.setItem('highlightSpeed', highlightSpeed);
        console.log("Stored in sessionStorage:", sessionStorage.getItem('highlightSpeed'));
    } catch (e) {
        console.error("Failed to set sessionStorage:", e);
    }

    // Hide the sidebar after 5 seconds
    setTimeout(() => {
        sidebar.style.display = 'none';
    }, 5000);
}

// Function to handle the key events
function handleKeyEvents(event) {
    if (event.ctrlKey && event.shiftKey && event.key === 'S') {
        event.preventDefault();
        console.log("Ctrl + Shift + S detected");
        document.getElementById("inputExercise").focus();
    }

    if (event.ctrlKey && event.key === '1') {
        event.preventDefault();
        highlightSpeed = 600;  // Slow speed
        console.log("Highlight speed set to 600 ms");
        updateSpeedOnSidebar();  // Update the sidebar with the current speed
    }

    if (event.ctrlKey && event.key === '2') {
        event.preventDefault();
        highlightSpeed = 350;  // Normal speed
        console.log("Highlight speed set to 350 ms");
        updateSpeedOnSidebar();  // Update the sidebar with the current speed
    }

    if (event.ctrlKey && event.key === '3') {
        event.preventDefault();
        highlightSpeed = 150;  // Fast speed
        console.log("Highlight speed set to 150 ms");
        updateSpeedOnSidebar();  // Update the sidebar with the current speed
    }
}

function submitExerciseToServer() {
    // Get the user's input from the search bar
    let inputExercise = document.getElementById('inputExercise').value;
    console.log("User's input learn text:", inputExercise);

    // Check if inputLearnText is actually empty
    if (!inputExercise || inputExercise.trim() === "") {
        console.error("Input exercise is empty!");
        return; // Stop the submission if the input is empty
    }

    // Optionally store the question in sessionStorage if needed
    try {
        sessionStorage.setItem('exercise', inputExercise);
        console.log("Stored in sessionStorage:", sessionStorage.getItem('exercise'));
    } catch (e) {
        console.error("Failed to set sessionStorage:", e);
    }

    // Submit the form without modifying the input
    document.querySelector('form').submit();
}
