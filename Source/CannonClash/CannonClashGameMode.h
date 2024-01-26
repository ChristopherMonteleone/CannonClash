#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CannonClashGameMode.generated.h"



UCLASS()
class CANNONCLASH_API ACannonClashGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	void ActorDied(AActor* DeadActor);

protected:

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void StartGame();

	UFUNCTION(BlueprintImplementableEvent)
	void GameOver(bool bWonGame);

private:

	class ATank* Tank;
	class ACannonClashPlayerController* CannonClashPlayerController;

	float StartDelay = 4.f;

	void HandleGameStart();

	int32 TargetTowers = 0;
	int32 GetTargetTowerCount();
};
