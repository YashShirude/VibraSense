package com.SSSDevelopers.VibraSense.service;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;
import org.springframework.ui.Model;

@Service
public class LearnService {
    NavigationService navigationService;
    @Value("${learnUrl}")
    private String learnUrl;
    @Value("${searchUrl}")
    private String searchUrl;
    @Value("${practiceUrl}")
    private String practiceUrl;

    @Autowired
    public LearnService(NavigationService navigationService) {
        this.navigationService = navigationService;
    }

    public String getLearningModule(String page, Model model) {
        System.out.println(page);
        if(page.equals(learnUrl) || page.equals(searchUrl) || page.equals(practiceUrl)){
            return navigationService.navigateTo(page);
        }
        // Call the Learning Modules Repository
        return "learn";
    }
}
