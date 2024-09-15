console.log("Loaded searchLogic.js Successfully");

let isHighlighting = false;
let highlightInterval;
let index = 0;
let sentenceStartStack = [];
let currentSentenceIndex = 0;

// Function to gradually display the answer
function typeAnswer(answer) {
    const answerDisplay = document.getElementById('answerDisplay');
    let typeIndex = 0;

    console.log("Typing the answer..."); // Debugging line

    // Gradually print the text character by character
    const interval = setInterval(function() {
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
}

// Add event listeners when the DOM is fully loaded
document.addEventListener("DOMContentLoaded", function() {
    document.addEventListener("keydown", handleKeyEvents);

    // Fetch the answer stored in the data-answer attribute
    const answerDisplay = document.getElementById('answerDisplay');
    const answer = answerDisplay.getAttribute('data-answer');

    console.log("Answer fetched:", answer); // Debugging line to check if answer is fetched

    if (answer && answer.length > 0) {
        typeAnswer(answer);
    }
});
