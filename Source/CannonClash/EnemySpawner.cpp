// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "Tank.h"
#include "Tower.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemySpawner::BeginPlay()
{
    Super::BeginPlay();

    // Start spawning enemies every one second, and keep doing so until the game ends
    GetWorldTimerManager().SetTimer(SpawnRateTimerHandle, this, &AEnemySpawner::StartSpawningEnemies, SpawnRate, true);
    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
}

void AEnemySpawner::StartSpawningEnemies()
{
    //THIS NOT PRINTING
    UE_LOG(LogTemp, Warning, TEXT("Tank Location: %s"), *Tank->GetActorLocation().ToString());
    // Check if the maximum number of enemies has been reached
    /*if (SpawnedEnemies.Num() >= MaxEnemies)
    {
        return;
    }*/

    if (Tank)
    {
        UE_LOG(LogTemp, Warning, TEXT("Finding place to spawn"));
        FVector PlayerLocation = Tank->GetActorLocation();
        FVector SpawnLocation = FMath::RandPointInBox(FBox(PlayerLocation - FVector(MaxSpawnDistance, MaxSpawnDistance, 0), 
                                                          PlayerLocation + FVector(MaxSpawnDistance, MaxSpawnDistance, 0)));

        // Check if the spawn location is within the minimum distance
        if (FVector::Dist(PlayerLocation, SpawnLocation) >= MinSpawnDistance)
        {
            // Spawn the enemy
            ATower* NewTower = GetWorld()->SpawnActor<ATower>(EnemyClass, SpawnLocation, FRotator::ZeroRotator);
            if (NewTower)
            {
                SpawnedEnemies.Add(NewTower);
            }
        }
    }
}

void AEnemySpawner::StopSpawningEnemies() {
    UE_LOG(LogTemp, Warning, TEXT("Stopping spawning of enemies..."));
    MaxEnemies = 0;
}