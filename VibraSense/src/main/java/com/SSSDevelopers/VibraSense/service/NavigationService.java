package com.SSSDevelopers.VibraSense.service;

import lombok.Data;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;

@Service
public class NavigationService {

    @Value("${learnUrl}")
    private String learnUrl;

    @Value("${searchUrl}")
    private String searchUrl;
    @Value("${practiceUrl}")
    private String practiceUrl;

    public String navigateTo(String page){
        if(page.equals(learnUrl)){
            return "learn";
        }else if(page.equals(practiceUrl)){
            return "practice";
        }else if(page.equals(searchUrl)){
            return "search";
        }
        return "Error to navigate";
    }
}
