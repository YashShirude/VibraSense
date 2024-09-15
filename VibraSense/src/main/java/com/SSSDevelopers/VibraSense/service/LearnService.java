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

    public String getLearningModule(String module, Model model) {
        if(module.equals(learnUrl) || module.equals(searchUrl) || module.equals(practiceUrl)){
            return navigationService.navigateTo(module);
        }
        // Call the Learning Modules Repository
        return "Code the Learning Modules";
    }
}
