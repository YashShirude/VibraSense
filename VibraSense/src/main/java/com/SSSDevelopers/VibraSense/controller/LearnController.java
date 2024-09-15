package com.SSSDevelopers.VibraSense.controller;

import com.SSSDevelopers.VibraSense.service.LearnService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestParam;

@Controller
public class LearnController {

    LearnService learnService;

    @Autowired
    public LearnController(LearnService learnService) {
        this.learnService = learnService;
    }

    @GetMapping("/learn")
    public String learn() {
        return "learn";
    }

    @PostMapping("/learn")
    public String getLearningModule(@RequestParam("module") String module, Model model) {
        return learnService.getLearningModule(module,model);
    }
}
