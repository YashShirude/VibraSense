package com.SSSDevelopers.VibraSense.service;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;
import org.springframework.ui.Model;

@Service
public class PracticeService {
    NavigationService navigationService;
    @Value("${learnUrl}")
    private String learnUrl;
    @Value("${searchUrl}")
    private String searchUrl;
    @Value("${practiceUrl}")
    private String practiceUrl;

    @Autowired
    public PracticeService(NavigationService navigationService) {
        this.navigationService = navigationService;
    }

    public String getExercise(String exercise, Model model) {
        if(exercise.equals(learnUrl) || exercise.equals(searchUrl) || exercise.equals(practiceUrl)){
            return navigationService.navigateTo(exercise);
        }
        // fetch 5 from the database
        for(int i=1;i<=5;i++){
            String temp = "" + i;
            if(exercise.equals(temp)){
                return "exercise";
            }
        }
        return "practice";
    }
}
