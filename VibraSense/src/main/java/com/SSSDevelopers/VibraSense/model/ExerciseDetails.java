package com.SSSDevelopers.VibraSense.model;

import jakarta.persistence.*;
import lombok.Data;

@Data
@Entity
@Table(name = "exerciseList")
public class ExerciseDetails {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    @Column(name = "exercise_id")
    private Integer exerciseId;

    @Column(name = "exercise_type_id")
    private String exerciseTypeId;

    @Column(name = "exercise_type")
    private String exerciseType;

    @Column(name = "exercise_characters")
    private String exerciseCharacters;
}
