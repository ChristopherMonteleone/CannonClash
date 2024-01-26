#include "CannonClashGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"
#include "Tower.h"
#include "CannonClashPlayerController.h"
#include "TimerManager.h"

void ACannonClashGameMode::ActorDied(AActor *DeadActor)
{
    UE_LOG(LogTemp, Warning, TEXT("ACTOR DIED BEING CALLED."));
    if (DeadActor == Tank)
    {
        Tank->HandleDestruction();
        if (CannonClashPlayerController)
        {
            CannonClashPlayerController->SetPlayerEnabledState(false);
        }
        GameOver(false);
    }
    else if (ATower* DestroyedTower = Cast<ATower>(DeadActor))
    {
        DestroyedTower->HandleDestruction();
        --TargetTowers;
        if (TargetTowers == 0)
        {
            GameOver(true);
        }
    }

    FTimerDelegate TimerDel = FTimerDelegate::CreateUObject(this, &ACannonClashGameMode::BeginPlay);
}

void ACannonClashGameMode::BeginPlay()
{
    Super::BeginPlay();
    HandleGameStart();
}

void ACannonClashGameMode::HandleGameStart()
{
    TargetTowers = GetTargetTowerCount();
    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
    CannonClashPlayerController = Cast<ACannonClashPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

    StartGame();

    if (CannonClashPlayerController)
    {
        CannonClashPlayerController->SetPlayerEnabledState(false);

        FTimerHandle PlayerEnableTimerHandle;
        FTimerDelegate PlayerEnableTimerDelegate = FTimerDelegate::CreateUObject(
            CannonClashPlayerController,
            &ACannonClashPlayerController::SetPlayerEnabledState,
            true
        );
        GetWorldTimerManager().SetTimer(PlayerEnableTimerHandle,
            PlayerEnableTimerDelegate,
            StartDelay,
            false
        );
    }
}

int32 ACannonClashGameMode::GetTargetTowerCount()
{
    TArray<AActor*> Towers;
    UGameplayStatics::GetAllActorsOfClass(this, ATower::StaticClass(), Towers);
    return Towers.Num();
}