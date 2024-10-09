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
        String subStringLearn = page.substring(0,Math.min(6,page.length()));
        String subStringSearch = page.substring(0,Math.min(7,page.length()));
        String subStringPractice = page.substring(0,Math.min(9,page.length()));

        if(subStringLearn.equals(learnUrl)){
            return "learn";
        }else if(subStringPractice.equals(practiceUrl)){
            return "practice";
        }else if(subStringSearch.equals(searchUrl)){
            return "search";
        }
        return "Error to navigate";
    }
}
