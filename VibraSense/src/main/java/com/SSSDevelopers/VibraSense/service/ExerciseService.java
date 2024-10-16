package com.SSSDevelopers.VibraSense.service;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;
import org.springframework.ui.Model;

@Service
public class ExerciseService {
    NavigationService navigationService;
    @Value("${learnUrl}")
    private String learnUrl;
    @Value("${searchUrl}")
    private String searchUrl;
    @Value("${practiceUrl}")
    private String practiceUrl;

    @Autowired
    public ExerciseService(NavigationService navigationService) {
        this.navigationService = navigationService;
    }

    public String getExercise(String exercise, Model model) {
        // TODO: fetch exercise from the repository
        // TODO: add the exercise in the model and then return exercise
        return "exercise";
    }
}
