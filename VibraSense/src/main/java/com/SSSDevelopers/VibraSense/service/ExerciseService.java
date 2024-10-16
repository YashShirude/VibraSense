package com.SSSDevelopers.VibraSense.service;

import com.SSSDevelopers.VibraSense.model.ExerciseDetails;
import com.SSSDevelopers.VibraSense.repository.ExerciseRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;
import org.springframework.ui.Model;

@Service
public class ExerciseService {
    NavigationService navigationService;
    ExerciseRepository exerciseRepository;
    @Value("${learnUrl}")
    private String learnUrl;
    @Value("${searchUrl}")
    private String searchUrl;
    @Value("${practiceUrl}")
    private String practiceUrl;

    @Autowired
    public ExerciseService(NavigationService navigationService,ExerciseRepository exerciseRepository) {
        this.navigationService = navigationService;
        this.exerciseRepository = exerciseRepository;
    }

    public String getExercise(String exerciseTypeId, Model model) {
        ExerciseDetails ex = exerciseRepository.getRandomExerciseByTypeId(exerciseTypeId);
        String exerciseCharacters = ex.getExerciseCharacters();
        System.out.println("hello");
        System.out.println(exerciseCharacters);
        model.addAttribute("exerciseCharacters", exerciseCharacters);
        return "exercise";
    }
}
