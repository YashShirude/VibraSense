package com.SSSDevelopers.VibraSense.service;

import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.http.*;
import org.springframework.stereotype.Service;
import org.springframework.ui.Model;
import org.springframework.web.client.RestTemplate;
import org.springframework.web.util.UriComponentsBuilder;

import java.util.HashMap;
import java.util.Map;

@Service
public class GeminiService {

    @Value("${gemini.api.url}")
    private String geminiApiUrl;

    @Value("${gemini.api.key}")
    private String geminiApiKey;

    public String getAnswer(String question, Model model) {
        // Prepare the URL
        String apiUrl = UriComponentsBuilder
                .fromHttpUrl(geminiApiUrl)
                .queryParam("key", geminiApiKey)
                .toUriString();

        // Create the request body in JSON format
        Map<String, Object> requestBody = new HashMap<>();
        Map<String, Object> contents = new HashMap<>();
        Map<String, Object> parts = new HashMap<>();
        parts.put("text", question); // Add the question to the body
        contents.put("parts", new Map[]{parts});
        requestBody.put("contents", new Map[]{contents});

        // Set up headers
        HttpHeaders headers = new HttpHeaders();
        headers.setContentType(MediaType.APPLICATION_JSON);

        // Create the HTTP request entity
        HttpEntity<Map<String, Object>> entity = new HttpEntity<>(requestBody, headers);

        // Create RestTemplate instance
        RestTemplate restTemplate = new RestTemplate();

        try {
            // Make the POST request
            ResponseEntity<String> response = restTemplate.exchange(apiUrl, HttpMethod.POST, entity, String.class);

            // Parse the response using Jackson ObjectMapper
            ObjectMapper objectMapper = new ObjectMapper();
            JsonNode root = objectMapper.readTree(response.getBody());

            // Extract the actual answer from the JSON response
            String answer = root.path("candidates").get(0).path("content").path("parts").get(0).path("text").asText();

            // Remove asterisks (*) from the answer text
            answer = answer.replace("*", "");

            // Add the question and cleaned-up answer to the model
            model.addAttribute("question", question);
            model.addAttribute("answer", answer);

        } catch (Exception e) {
            // Handle error or parsing failure
            model.addAttribute("question", question);
            model.addAttribute("answer", "Failed to retrieve the answer from the Gemini API.");
        }

        // Return the view name
        return "search";
    }
}
