package com.SSSDevelopers.VibraSense.repository;

import com.SSSDevelopers.VibraSense.model.ExerciseDetails;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.query.Param;
import org.springframework.stereotype.Repository;

@Repository
public interface ExerciseRepository extends JpaRepository<ExerciseDetails, Integer> {

    @Query(value = "SELECT * FROM exerciseList e WHERE e.exercise_type_id = :exerciseTypeId ORDER BY RAND() LIMIT 1", nativeQuery = true)
    ExerciseDetails getRandomExerciseByTypeId(@Param("exerciseTypeId") String exerciseTypeId);
}

