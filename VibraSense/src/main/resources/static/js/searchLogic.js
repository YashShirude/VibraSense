console.log("Loaded searchLogic.js Successfully");

let answer = "";
let isHighlighting = false;
let highlightInterval;
let interval;
let index = 0;
let sentenceStartStack = [];
let isEncrypted = false;
let realSearchInput = "";
let isHighlightingPaused = false;  // Track if highlighting is paused
let currentHighlightText = "";
let encryptedAnswer = "";
let isPaused = false;  // Track if highlighting is paused


// Add event listeners when the DOM is fully loaded
document.addEventListener("DOMContentLoaded", function() {
    document.addEventListener("keydown", handleKeyEvents);

    // Fetch the answer stored in the data-answer attribute
    const answerDisplay = document.getElementById('answerDisplay');
    answer = answerDisplay.getAttribute('data-answer');

    console.log("Answer fetched:", answer); // Debugging line to check if answer is fetched

    let storedIsEncrypted = sessionStorage.getItem('isEncrypted');

        // Set isEncrypted based on session Storage or default to false if no stored value
    if (storedIsEncrypted === null) {
        console.log("No stored encryption state found. Defaulting to false.");
        isEncrypted = false;  // Default to false if no value is found in session Storage
    } else {
        isEncrypted = (storedIsEncrypted === 'true');  // Convert string 'true'/'false' to boolean
        console.log("Loaded encryption state from session Storage:", isEncrypted);
    }
    console.log(storedIsEncrypted);

    if (answer && answer.length > 0) {
        for (let i = 0; i < answer.length; i++) {
          encryptedAnswer += '*';
        }
        if(isEncrypted){
            typeAnswer(encryptedAnswer);
        }else{
            typeAnswer(answer);
        }

    }

    // Set up the event listener for search input encryption
    const searchInput = document.getElementById('inputQuestion');
    if (searchInput) {
        searchInput.addEventListener('input', handleSearchInput);  // Handle changes to search input
        // Set the input field type based on the encryption state
        if (isEncrypted) {
            searchInput.type = 'password';  // Change to password field when encrypted
        } else {
            searchInput.type = 'text';  // Change to text field when not encrypted
        }
    }
});


// Function to gradually display the answer
function typeAnswer(answer) {
    const answerDisplay = document.getElementById('answerDisplay');
    let typeIndex = 0;

    console.log("Typing the answer..."); // Debugging line

     answerDisplay.scrollTop = 0;

    // Gradually print the text character by character
    interval = setInterval(function() {
        if (typeIndex < answer.length) {
            answerDisplay.innerHTML += answer[typeIndex];
            typeIndex++;
        } else {
            clearInterval(interval);  // Stop the interval when done
            // Save the answer text for later highlighting
            answerDisplay.setAttribute('data-highlighted', answerDisplay.textContent);
        }
    }, 12);  // Adjust typing speed (12ms per character)
}

// Function to move highlighting to the start of the current sentence
function moveHighlightToStartOfSentence() {
    const answerDisplay = document.getElementById('answerDisplay');
    const text = answerDisplay.getAttribute('data-highlighted');

    if (!text) {
        console.log("No text available for highlighting.");
        return;
    }

    console.log("Moving highlight to start of the sentence.");

    // Pop the last position from the stack
    if (sentenceStartStack.length === 0) {
        console.log("Sentence stack is empty. Restarting from the beginning.");
        index = 0; // Start from the beginning
    } else {
        index = sentenceStartStack.pop();  // Set index to the start of the last sentence
        console.log(`Popped index ${index} from stack. Moving highlight.`);
    }

    if (!isHighlighting) {
        answerDisplay.innerHTML = `<span style="background-color: green;">${text.substring(0, index)}</span>${text.substring(index)}`;
    }
}

// Function to handle search input and encrypt if needed
function handleSearchInput(event) {
    const searchInput = event.target;
    const currentInputValue = searchInput.value;

    if (isEncrypted) {
        // When encrypted, only append new characters to the real input
        if (currentInputValue.length > realSearchInput.length) {
            realSearchInput += currentInputValue.slice(realSearchInput.length);
        } else if (currentInputValue.length < realSearchInput.length) {
            // Handle backspace or deletion
            realSearchInput = realSearchInput.slice(0, currentInputValue.length);
        }

        // Mask the input display with '*' but keep the real value stored
    } else {
        // If not encrypted, allow normal input behavior and store the value
        realSearchInput = currentInputValue;
    }

    console.log(`Real search input: ${realSearchInput}`);
}


// Function to move highlighting to the start of the text
function moveHighlightToStartOfText() {
    const answerDisplay = document.getElementById('answerDisplay');
    const text = answerDisplay.getAttribute('data-highlighted');

    if (!text) {
        console.log("No text available for highlighting.");
        return;
    }

    console.log("Moving highlight to start of the text.");
    index = 0;

    if (!isHighlighting) {
        answerDisplay.innerHTML = `<span style="background-color: green;">${text.substring(0, index)}</span>${text.substring(index)}`;
    }
}

function startOrResumeHighlighting() {
    const answerDisplay = document.getElementById('answerDisplay');
    const text = isEncrypted ? encryptedAnswer : answer;  // Use correct version of text

    if (!text) return;

    if (isPaused) {
        console.log("Resuming highlighting from index:", index);
        isPaused = false;
    } else {
        console.log("Starting highlighting from index:", index);
    }

    if (isHighlighting) {
        clearInterval(highlightInterval);  // Pause the existing highlighting if needed
    }

    isHighlighting = true;

    // Start or resume highlighting from the current index
    highlightInterval = setInterval(() => {
        if (index < text.length) {
            // Check if the current character is a full stop
            if (text[index] === '.') {
                sentenceStartStack.push(index);  // Push the full stop index into the stack
            }

            // Highlight text up to the current index
            answerDisplay.innerHTML = `<span style="background-color: green;">${text.substring(0, index + 1)}</span>${text.substring(index + 1)}`;
            index++;
        } else {
            clearInterval(highlightInterval);  // Stop when highlighting completes
            isHighlighting = false;
        }
    }, 200);  // Adjust the highlighting speed
}

// Function to pause highlighting
function pauseHighlighting() {
    if (isHighlighting) {
        clearInterval(highlightInterval);  // Stop the interval when paused
        isHighlighting = false;
        isPaused = true;
        console.log("Highlighting paused at index:", index);
    }
}

// Function to toggle encryption
function toggleEncryption() {
    const answerDisplay = document.getElementById('answerDisplay');
    isEncrypted = !isEncrypted;  // Toggle encryption state

    // Update the appropriate text (encrypted or decrypted) based on the current index
    const text = isEncrypted ? encryptedAnswer : answer;

    // Display the text up to the current highlighted index
    answerDisplay.innerHTML = `<span style="background-color: green;">${text.substring(0, index)}</span>${text.substring(index)}`;

    console.log("Toggled encryption. Current state:", isEncrypted ? "Encrypted" : "Decrypted");

    const searchInput = document.getElementById('inputQuestion');
    searchInput.type = isEncrypted ? 'password' : 'text';  // Toggle between password and text type

    // If highlighting is active, continue from the current index with the new text
    if (isHighlighting) {
        startOrResumeHighlighting();  // Restart highlighting with the new text state
    }

    // Save the encryption state in session storage
    try {
        sessionStorage.setItem('isEncrypted', isEncrypted);
        console.log("Encryption state saved:", isEncrypted);
    } catch (e) {
        console.error("Failed to set sessionStorage:", e);
    }
}

// Function to handle the key events
function handleKeyEvents(event) {
    if (event.ctrlKey && event.shiftKey && event.key === 'S') {
        event.preventDefault();
        console.log("Ctrl + Shift + S detected");
        document.getElementById("inputQuestion").focus();
    }

    if (event.ctrlKey && event.shiftKey && event.key === ' ') {
        event.preventDefault();
        console.log("Ctrl + Shift + Space detected: Toggle Start/Pause Highlighting");

        // Toggle between pause and resume highlighting
        if (isHighlighting) {
            pauseHighlighting();  // If highlighting is active, pause it
        } else {
            startOrResumeHighlighting();  // If not active or paused, start or resume it
        }
    }

    if (event.ctrlKey && event.shiftKey && event.key === 'ArrowLeft') {
        event.preventDefault();
        console.log("Ctrl + Shift + ArrowLeft detected");
        moveHighlightToStartOfSentence();
    }

    if (event.ctrlKey && event.shiftKey && event.key === 'R') {
        event.preventDefault();
        console.log("Ctrl + Shift + R detected");
        moveHighlightToStartOfText();
    }

    if (event.ctrlKey && event.shiftKey && event.key === 'H') {
        event.preventDefault();
        toggleEncryption();  // Toggle encryption and manage highlighting
    }
}

function submitQuestionToServer() {
    const realQuestionInput = document.getElementById('realQuestionInput');

    // Set the hidden input's value to the real search input value
    realQuestionInput.value = realSearchInput;

    console.log("Submitting real question to server:", realSearchInput);

    // Now let the form submit with the real question
    const form = document.getElementById('questionForm');
    form.submit();  // Submit the form
}


