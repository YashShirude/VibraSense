package com.SSSDevelopers.VibraSense.controller;

import com.SSSDevelopers.VibraSense.service.LearnService;
import com.SSSDevelopers.VibraSense.service.PracticeService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestParam;

@Controller
public class PracticeController {
    PracticeService practiceService;

    @Autowired
    public PracticeController(PracticeService practiceService) {
        this.practiceService = practiceService;
    }

    @GetMapping("/practice")
    public String practice() {
        return "practice";
    }

    @PostMapping("/practice")
    public String getExercise(@RequestParam("exercise") String exercise, Model model) {
        return practiceService.getExercise(exercise,model);
    }
}
