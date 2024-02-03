#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tower.h"
#include "EnemySpawner.generated.h"

UCLASS()
class CANNONCLASH_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

	UFUNCTION(BlueprintCallable)
    void StartSpawningEnemies();

    UFUNCTION(BlueprintCallable)
    void StopSpawningEnemies();

protected:
	UPROPERTY(EditAnywhere)
    TSubclassOf<ATower> EnemyClass;

    UPROPERTY(EditAnywhere)
    float MinSpawnDistance = 500.0f;

    UPROPERTY(EditAnywhere)
    float MaxSpawnDistance = 1000.0f;

    UPROPERTY(EditAnywhere)
    float SpawnRate = 2.5f;

    UPROPERTY(EditAnywhere)
    int32 MaxEnemies = 10;

private:
    TArray<ATower*> SpawnedEnemies;

    FTimerHandle SpawnRateTimerHandle;

    class ATank* Tank;

    void BeginPlay();
};