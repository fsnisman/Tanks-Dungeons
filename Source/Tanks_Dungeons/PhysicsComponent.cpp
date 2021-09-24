// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicsComponent.h"

TArray<FVector> UPhysicsComponent::GenerateTrajectory(FVector StartPos, FVector Velocity, float MaxTime, float TimeStep, float MinZValue)
{
    TArray<FVector> trajectory;
    FVector gravityVector(0, 0, Gravity);
    for (float time = 0; time < MaxTime; time = time + TimeStep)
    {
        FVector position = StartPos + Velocity * time + gravityVector * time * time / 2;
        if (position.Z <= MinZValue)
            break;

        trajectory.Add(position);
    }

    return trajectory;
}


FVector UPhysicsComponent::CalculateBezierPoint(float t, FVector p0, FVector p1, FVector p2, FVector p3)
{
    float u = 1 - t;
    float tt = t * t;
    float uu = u * u;
    float uuu = uu * u;
    float ttt = tt * t;

    FVector p = uuu * p0;    //first term
    p += 3 * uu * t * p1;    //second term
    p += 3 * u * tt * p2;    //third term
    p += ttt * p3;           //fourth term

    return p;
}

