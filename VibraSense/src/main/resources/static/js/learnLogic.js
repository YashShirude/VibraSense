console.log("Loaded learnLogic.js Successfully");

let interval;
let highlightSpeed = 200;  // Default speed set to 200 ms
const speedLabels = {
    600: "Slow Speed",
    350: "Normal Speed",
    150: "Fast Speed"
};

// Variable to hold the user input
let userText = "";
let displayIndex = 0;

// Listen for DOM content to load and initialize typing
document.addEventListener("DOMContentLoaded", function () {
    document.addEventListener("keydown", handleKeyEvents);

    const learnDisplay = document.getElementById('learnDisplay');

    // Retrieve text and highlight speed from session storage
    let userText = sessionStorage.getItem('LearnText');
    highlightSpeed = parseInt(sessionStorage.getItem('highlightSpeed')) || 200;
    startTyping(userText);
    // Initially clear the display
    clearDisplay(learnDisplay);
});

// Function to gradually display the user-typed text, appending characters
function startTyping(text) {
    const learnDisplay = document.getElementById('learnDisplay');
    clearInterval(interval); // Stop any previous typing intervals

    // Reset display index for new text
    displayIndex = 0;

    interval = setInterval(function () {
        if (displayIndex < text.length) {
            // Append the character instead of replacing it
            learnDisplay.textContent = text.charAt(displayIndex);
            displayIndex++;
        } else {
            clearInterval(interval);  // Stop when done
        }
    }, highlightSpeed);  // Adjust typing speed based on highlightSpeed
    learnDisplay.textContent = '';
    try {
        sessionStorage.setItem('LearnText', "");
        console.log("Stored in sessionStorage:", sessionStorage.getItem('LearnText'));
    } catch (e) {
        console.error("Failed to set sessionStorage:", e);
    }
    learnDisplay.textContent = "";
}

// Function to clear the display content
function clearDisplay(learnDisplay) {
    learnDisplay.textContent = ""; // Clear the display text
}

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

    // Restart typing with new speed
    startTyping(sessionStorage.getItem('LearnText'));

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
        document.getElementById("inputLearnText").focus();
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

function submitLearnTextToServer() {
    // Get the user's input from the search bar
    let inputLearnText = document.getElementById('inputLearnText').value;
    console.log("User's input learn text:", inputLearnText);

    inputLearnText = inputLearnText.toLowerCase();

    // Check if inputLearnText is actually empty
    if (!inputLearnText || inputLearnText.trim() === "") {
        console.error("Input learn text is empty!");
        return; // Stop the submission if the input is empty
    }

    // Optionally store the question in sessionStorage if needed
    try {
        sessionStorage.setItem('LearnText', inputLearnText);
        console.log("Stored in sessionStorage:", sessionStorage.getItem('LearnText'));
    } catch (e) {
        console.error("Failed to set sessionStorage:", e);
    }

    // Submit the form without modifying the input
    document.querySelector('form').submit();
}
