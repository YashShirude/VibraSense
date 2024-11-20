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
let highlightSpeed = 2000;  // Default speed set to 200 ms
const speedLabels = {
    600: "Slow Speed",
    2000: "Normal Speed",
    150: "Fast Speed"
};
let wordLimit = 75;  // Default word limit
const sizeLabels = {
    50: "50 Words",
    75: "75 Words",
    100: "100 Words"
};
let ws;
let isReconnecting = false;
const MAX_RECONNECT_ATTEMPTS = 5;
let reconnectAttempts = 0;
const WEBSOCKET_URL = 'ws://192.168.19.238:8080/ws';
const RECONNECT_DELAY = 5000;
// Add event listeners when the DOM is fully loaded
document.addEventListener("DOMContentLoaded", function() {

    document.addEventListener("keydown", handleKeyEvents);

    // Fetch the answer stored in the data-answer attribute
    const answerDisplay = document.getElementById('answerDisplay');
    answer = answerDisplay.getAttribute('data-answer');

    console.log("Answer fetched:", answer); // Debugging line to check if answer is fetched

    connectWebSocket();

    let storedIsEncrypted = sessionStorage.getItem('isEncrypted');
    let Question = sessionStorage.getItem('Question');

    // Set isEncrypted based on session Storage or default to false if no stored value
    if (storedIsEncrypted === null) {
        console.log("No stored encryption state found. Defaulting to false.");
        isEncrypted = false;  // Default to false if no value is found in session Storage
    } else {
        isEncrypted = (storedIsEncrypted === 'true');  // Convert string 'true'/'false' to boolean
        console.log("Loaded encryption state from session Storage:", isEncrypted);
    }
    console.log(storedIsEncrypted);
    console.log(Question);

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

    document.addEventListener('visibilitychange', () => {
            if (document.visibilityState === 'visible' && (!ws || ws.readyState !== WebSocket.OPEN)) {
                connectWebSocket();
            }
    });

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

// Reconnect if the page visibility changes (user returns to tab)
document.addEventListener('visibilitychange', () => {
    if (document.visibilityState === 'visible' && (!ws || ws.readyState !== WebSocket.OPEN)) {
        connectWebSocket();
    }
});

function connectWebSocket() {
    if (ws && ws.readyState === WebSocket.OPEN) {
        console.log('WebSocket is already connected');
        return;
    }

    if (isReconnecting) {
        console.log('Already attempting to reconnect');
        return;
    }

    if (reconnectAttempts >= MAX_RECONNECT_ATTEMPTS) {
        console.error('Max reconnection attempts reached');
        updateConnectionStatus('Failed to connect');
        return;
    }

    isReconnecting = true;
    updateConnectionStatus('Connecting...');

    try {
        ws = new WebSocket(WEBSOCKET_URL);
        setupWebSocketHandlers();
    } catch (error) {
        console.error('Error creating WebSocket connection:', error);
        handleConnectionFailure();
    }
}

function setupWebSocketHandlers() {
    ws.onopen = () => {
        console.log('WebSocket connection established');
        isReconnecting = false;
        reconnectAttempts = 0;
        updateConnectionStatus('Connected');
        enableInputs();
    };

    ws.onclose = (event) => {
        console.log('WebSocket connection closed. Code:', event.code, 'Reason:', event.reason);
        updateConnectionStatus('Disconnected');
        disableInputs();

        if (event.code !== 1000) {
            handleReconnection();
        }
    };

    ws.onerror = (error) => {
        console.error('WebSocket error:', error);
        updateConnectionStatus('Error');
    };

    ws.onmessage = handleWebSocketMessage;
}

function handleWebSocketMessage(event) {
    console.log('Received message:', event.data);
    try {
        const response = JSON.parse(event.data);
        if (response.status === "processed") {
            console.log(`Server processed character at index ${response.index}: ${response.char}`);
            // You can add additional handling here if needed
            updateProcessingStatus(response);
        }
    } catch (e) {
        console.error('Error parsing server message:', e);
    }
}

function handleReconnection() {
    reconnectAttempts++;
    setTimeout(() => {
        isReconnecting = false;
        connectWebSocket();
    }, RECONNECT_DELAY);
}

function handleConnectionFailure() {
    isReconnecting = false;
    updateConnectionStatus('Connection Failed');
    console.error('Failed to create WebSocket connection');
}

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

// Update the start or resume highlighting function to use the highlightSpeed
function startOrResumeHighlighting() {
    const answerDisplay = document.getElementById('answerDisplay');
    const text = isEncrypted ? encryptedAnswer : answer;

    if (!text) return;

    if (isPaused) {
        console.log("Resuming highlighting from index:", index);
        isPaused = false;
    } else {
        console.log("Starting highlighting from index:", index);
    }

    if (isHighlighting) {
        clearInterval(highlightInterval);
    }

    isHighlighting = true;

    highlightInterval = setInterval(() => {
        if (index < text.length) {
            if (answer[index] === '.') {
                sentenceStartStack.push(index);
            }

            // Update display
            answerDisplay.innerHTML = `<span style="background-color: green;">${text.substring(0, index + 1)}</span>${text.substring(index + 1)}`;

            // Send character to WebSocket if connected
            sendCharacterToServer(index);

            index++;
        } else {
            clearInterval(highlightInterval);
            isHighlighting = false;
            updateProcessingStatus({ status: 'completed' });
        }
    }, highlightSpeed);
}

function sendCharacterToServer(currentIndex) {
    if (!ws || ws.readyState !== WebSocket.OPEN) {
        console.error('WebSocket is not connected. Cannot send character.');
        return;
    }

    const uppercaseAnswer = answer.toUpperCase();
    try {
        ws.send(JSON.stringify({
            char: uppercaseAnswer[currentIndex],
            index: currentIndex
        }));
        console.log("Sent character to server:", uppercaseAnswer[currentIndex]);
    } catch (error) {
        console.error('Error sending character:', error);
    }
}

// UI Update Functions
function updateConnectionStatus(status) {
    const statusElement = document.getElementById('connectionStatus');
    if (statusElement) {
        statusElement.textContent = `Connection Status: ${status}`;
        statusElement.className = `status-${status.toLowerCase()}`;
    }
}

function updateProcessingStatus(response) {
    const statusElement = document.getElementById('processingStatus');
    if (statusElement) {
        if (response.status === 'processed') {
            statusElement.textContent = `Processing: Character ${response.index}`;
        } else if (response.status === 'completed') {
            statusElement.textContent = 'Processing: Complete';
        }
    }
}

function enableInputs() {
    const inputs = document.querySelectorAll('.websocket-dependent');
    inputs.forEach(input => input.disabled = false);
}

function disableInputs() {
    const inputs = document.querySelectorAll('.websocket-dependent');
    inputs.forEach(input => input.disabled = true);
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

// Function to update speed on the sidebar
function updateSpeedOnSidebar() {
    const sidebar = document.getElementById('settingsSidebar');
    const speedDisplay = document.getElementById('currentSpeed');
    if (speedDisplay) {
        speedDisplay.textContent = speedLabels[highlightSpeed];
    }
    sidebar.style.display = 'block';  // Show the sidebar

    // Hide the sidebar after 5 seconds
    setTimeout(() => {
        sidebar.style.display = 'none';
    }, 5000);
}

// Function to update speed on the sidebar
function updateSizeOnSidebar() {
    const sidebar = document.getElementById('settingsSidebar');
    const sizeDisplay = document.getElementById('responseSize');
    if (sizeDisplay) {
        sizeDisplay.textContent = sizeLabels[wordLimit];
    }
    sidebar.style.display = 'block';  // Show the sidebar

    // Hide the sidebar after 5 seconds
    setTimeout(() => {
        sidebar.style.display = 'none';
    }, 5000);
}

function updateEncryptionOnSidebar() {
    const sidebar = document.getElementById('settingsSidebar');
    const encryptionStatus = document.getElementById('encryptionStatus');
    if(encryptionStatus){
        encryptionStatus.textContent = isEncrypted ? "ON" : "OFF";  // Update encryption status in the sidebar
    }

    sidebar.style.display = 'block';  // Show the sidebar

    // Hide the sidebar after 5 seconds
    setTimeout(() => {
        sidebar.style.display = 'none';
    }, 5000);
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

    // Show the sidebar for 5 seconds
    updateEncryptionOnSidebar();

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
    const sidebar = document.getElementById('settingsSidebar');
    const speedStatus = document.getElementById('currentSpeed');

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

    if (event.ctrlKey && event.key === '1') {
        event.preventDefault();
        highlightSpeed = 600;  // Set speed to 250 ms
        console.log("Highlight speed set to 600 ms");
        updateSpeedOnSidebar();  // Update the sidebar with the current speed
        speedStatus.textContent = "Slow Speed";  // Update encryption status in the sidebar
    }

    if (event.ctrlKey && event.key === '2') {
        event.preventDefault();
        highlightSpeed = 350;  // Set speed to 200 ms
        console.log("Highlight speed set to 350 ms");
        updateSpeedOnSidebar();  // Update the sidebar with the current speed
    }

    if (event.ctrlKey && event.key === '3') {
        event.preventDefault();
        highlightSpeed = 150;  // Set speed to 150 ms
        console.log("Highlight speed set to 150 ms");
        updateSpeedOnSidebar();  // Update the sidebar with the current speed
    }

    if (event.ctrlKey && event.key === '8') {
        event.preventDefault();
        wordLimit = 50;
        console.log("Word limit set to 50 words");
        updateSizeOnSidebar();
    }

    if (event.ctrlKey && event.key === '9') {
        event.preventDefault();
        wordLimit = 75;
        console.log("Word limit set to 75 words");
        updateSizeOnSidebar();
    }

    if (event.ctrlKey && event.key === '0') {
        event.preventDefault();
        wordLimit = 100;
        console.log("Word limit set to 100 words");
        updateSizeOnSidebar();
    }
}

function submitQuestionToServer() {
    // Get the user's input from the search bar
    const inputQuestion = document.getElementById('inputQuestion').value;
    console.log("User's input question:", inputQuestion);

    let modifiedQuestion = inputQuestion;

    // Modify the question if it doesn't match certain predefined commands
    if (inputQuestion !== '/learn' && inputQuestion !== '/search' && inputQuestion !== '/practice') {
        modifiedQuestion = ". Limit the response to " + wordLimit + "words.";
    }

    // Append the word limit string to the input question
    console.log(inputQuestion);
    console.log("Modified question with word limit:", modifiedQuestion);

    // Check if inputQuestion is actually empty
    if (!inputQuestion || inputQuestion.trim() === "") {
        console.error("Input question is empty!");
        return; // Stop the submission if the input is empty
    }

    // Store the modified question in sessionStorage
    try {
        sessionStorage.setItem('Question', modifiedQuestion);
        console.log("Stored in sessionStorage:", sessionStorage.getItem('Question'));
    } catch (e) {
        console.error("Failed to set sessionStorage:", e);
    }

    // Set the hidden input value to the modified question
    document.getElementById('realQuestionInput').value = modifiedQuestion;

    // Submit the form
    document.getElementById('questionForm').submit();
}





