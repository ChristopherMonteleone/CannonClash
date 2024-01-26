// Fill out your copyright notice in the Description page of Project Settings.


#include "CannonClashPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "EnemySpawner.h"

void ACannonClashPlayerController::SetPlayerEnabledState(bool bPlayerEnabled)
{
    AEnemySpawner* EnemySpawnerInstance = Cast<AEnemySpawner>(UGameplayStatics::GetActorOfClass(GetWorld(), AEnemySpawner::StaticClass()));
    UE_LOG(LogTemp, Warning, TEXT("In Set player enabled..."));
    if (bPlayerEnabled)
    {
        if (EnemySpawnerInstance) {
            UE_LOG(LogTemp, Warning, TEXT("Spawning..."));
            EnemySpawnerInstance->StartSpawningEnemies();
        }
        GetPawn()->EnableInput(this);
    }
    else
    {
        GetPawn()->DisableInput(this);
        if (EnemySpawnerInstance)
        {
            UE_LOG(LogTemp, Warning, TEXT("Stopping..."));
            EnemySpawnerInstance->StopSpawningEnemies();
        }
    }
    bShowMouseCursor = bPlayerEnabled;
}