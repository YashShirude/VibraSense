console.log("Loaded exerciseLogic.js Successfully");

let interval;
let highlightSpeed = 200;  // Default speed set to 200 ms
const speedLabels = {
    600: "Slow Speed",
    350: "Normal Speed",
    150: "Fast Speed"
};
var stringFromRepository = /*[[${exerciseCharacters}]]*/ 'abcdefghij1234567890'; // Default value for fallback
// Variable to hold the user input
let userText = "";
let displayIndex = 0;

// Listen for DOM content to load and initialize typing
document.addEventListener("DOMContentLoaded", function () {
    document.addEventListener("keydown", handleKeyEvents);

    const characterDisplay = document.getElementById('characterDisplay');
    stringFromRepository = characterDisplay.textContent; // Getting the value from the HTML element
    console.log("Fetched exercise characters2:", exerciseCharacters);
    // Retrieve text and highlight speed from session storage
    highlightSpeed = parseInt(sessionStorage.getItem('highlightSpeed')) || 200;
    characterDisplay.textContent = stringFromRepository.charAt(0);
});

function displayNextCharacter(text) {
    const characterDisplay = document.getElementById('characterDisplay');

    // Display the current character
    characterDisplay.textContent = text.charAt(displayIndex);
}

// Function to clear the display content
function clearDisplay(characterDisplay) {
    characterDisplay.textContent = ""; // Clear the display text
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
    clearInterval(interval);
    displayNextCharacter(stringFromRepository);

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
        document.getElementById("exerciseAnswer").focus();
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

    // New logic to check if the user types the correct character
    const typedChar = event.key;  // Get the character the user typed
    const currentChar = stringFromRepository.charAt(displayIndex);  // Get the current character to be matched
    const exerciseAnswer = document.getElementById('exerciseAnswer');

    if (typedChar === currentChar) {
        // If the typed character matches the current character, move to the next one
        displayIndex++;
        if (displayIndex < stringFromRepository.length) {
            displayNextCharacter(stringFromRepository);  // Display the next character
        } else {
            console.log("Exercise complete!");  // All characters have been typed correctly
            const characterDisplay = document.getElementById('characterDisplay');
            submitAnswerToServer();
            clearDisplay(characterDisplay);
            // Optionally, you can reset the index or do something else when the exercise is done
        }
    } else {
        console.log("Incorrect character typed!");  // Notify the user of incorrect input
        showPopup("Incorrect");
    }
    event.target.value = '';
}

function showPopup(message) {
    const popup = document.getElementById('popup');
    popup.textContent = message;
    popup.style.display = 'block';  // Show the popup

    setTimeout(() => {
        popup.style.display = 'none';  // Hide after 1 second
    }, 1000);
}

// Function to submit the answer to the server
function submitAnswerToServer() {
    document.querySelector('form').submit();
}
