package com.SSSDevelopers.VibraSense.service;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;
import org.springframework.ui.Model;

@Service
public class SearchService {

    NavigationService navigationService;
    GeminiService geminiService;

    @Value("${learnUrl}")
    private String learnUrl;

    @Value("${searchUrl}")
    private String searchUrl;
    @Value("${practiceUrl}")
    private String practiceUrl;

    @Autowired
    public SearchService(NavigationService navigationService,GeminiService geminiService) {
        this.navigationService = navigationService;
        this.geminiService = geminiService;
    }

    public String getAnswer(String question, Model model) {
        // because \ is an escape character
        String subStringLearn = question.substring(0,Math.min(6,question.length()));
        String subStringSearch = question.substring(0,Math.min(7,question.length()));
        String subStringPractice = question.substring(0,Math.min(9,question.length()));

        if(subStringLearn.equals(learnUrl) || subStringSearch.equals(searchUrl) || subStringPractice.equals(practiceUrl)){
            return navigationService.navigateTo(question);
        }
        return geminiService.getAnswer(question,model);
    }
}
