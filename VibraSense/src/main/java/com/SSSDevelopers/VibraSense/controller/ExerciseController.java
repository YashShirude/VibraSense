package com.SSSDevelopers.VibraSense.controller;

import com.SSSDevelopers.VibraSense.service.ExerciseService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestParam;

@Controller
public class ExerciseController {
    @Autowired
    ExerciseService exerciseService;

    public ExerciseController(ExerciseService exerciseService) {
        this.exerciseService = exerciseService;
    }

    @GetMapping("/exercise")
    public String getPractice() {
        return "practice";
    }

    @PostMapping("/exercise")
    public String getExercise(@RequestParam("exercise") String exercise, Model model) {
        return exerciseService.getExercise(exercise,model);
    }
}
