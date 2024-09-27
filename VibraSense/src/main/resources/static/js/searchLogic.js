console.log("Loaded searchLogic.js Successfully");

let answer = "";
let isHighlighting = false;
let highlightInterval;
let interval;
let index = 0;
let sentenceStartStack = [];
let currentSentenceIndex = 0;
let isEncrypted = false;
let realSearchInput = "";

let encryptedAnswer = "";

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

// Function to perform highlighting of text based on the current index
function highlightText(text, answerDisplay) {
    clearInterval(highlightInterval);

    highlightInterval = setInterval(function() {
        if (index < text.length) {
            const highlightedText = text.substring(0, index + 1);
            const remainingText = text.substring(index + 1);

            // Push sentence start index if full stop is encountered
            if (text[index] === '.') {
                sentenceStartStack.push(index);
                console.log(`Full stop found at index ${index}. Pushing to stack.`);
            }

            answerDisplay.innerHTML = `<span style="background-color: green;">${highlightedText}</span>${remainingText}`;
            index++;
        } else {
            clearInterval(highlightInterval);  // Stop the interval when done
            isHighlighting = false;  // Reset highlighting state
        }
    }, 200);  // Adjust highlighting speed (200ms per character)
}

// Function to start or pause highlighting after pressing Ctrl + Shift + Space
function toggleHighlighting() {
    const answerDisplay = document.getElementById('answerDisplay');
    const text = answerDisplay.getAttribute('data-highlighted');

    if (!text) {
        console.log("No text available for highlighting.");
        return;
    }

    if (isHighlighting) {
        console.log("Pausing highlighting.");
        clearInterval(highlightInterval);
        isHighlighting = false;
    } else {
        console.log("Starting or resuming highlighting.");
        highlightText(text, answerDisplay);
        isHighlighting = true;
    }
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

    if (isHighlighting) {
        highlightText(text, answerDisplay);
    } else {
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

    if (isHighlighting) {
        highlightText(text, answerDisplay);
    } else {
        answerDisplay.innerHTML = `<span style="background-color: green;">${text.substring(0, index)}</span>${text.substring(index)}`;
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
        console.log("Ctrl + Shift + Space detected");
        toggleHighlighting();
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
            const searchInput = document.getElementById('inputQuestion');
            event.preventDefault();
            console.log("Ctrl + Shift + H detected");
            clearInterval(interval);
            if(isEncrypted){
                isEncrypted = false;
                answerDisplay.innerHTML = answer; // Making the typing very fast
                if (searchInput) {
                    searchInput.type = 'text';  // Change input field back to normal text
                    searchInput.value = realSearchInput;  // Display real search input
                }
            }else{
                isEncrypted = true;
                answerDisplay.innerHTML = encryptedAnswer; // Making the typing very fast
                if (searchInput) {
                    searchInput.type = 'password';  // Change input field to password type
                    //searchInput.value = '*'.repeat(realSearchInput.length);  // Mask the search input
                }
            }
            try {
               sessionStorage.setItem('isEncrypted', isEncrypted);
               console.log("Encryption state saved:", isEncrypted);  // Debugging log
            } catch (e) {
                console.error("Failed to set sessionStorage:", e);  // Error log if sessionStorage fails
            }
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


