#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tank.generated.h"

/**
 * 
 */
UCLASS()
class CANNONCLASH_API ATank : public ABasePawn
{
	GENERATED_BODY()

public:
	ATank();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void HandleDestruction();

	APlayerController* GetTankPlayerController() const { return TankPlayerController; }

	bool bAlive = true;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float Speed = 700.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float TurnRate = 120.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float DashCooldownDuration = 1.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float DashDuration = .2f;

	void UpDown(float Value);
	void LeftRight(float Value);
	void Fire();
	void BeginDash();
	void EndDash();
	void SetDashReady();
	void RotateBase();
	void UpdateMovementInput();

	float UpDownInput;
	float LeftRightInput;
	FVector2D MovementInput;

	bool DashReady = true;
	FTimerHandle DashTimerHandle;
	FTimerHandle DashCooldownHandle;
	APlayerController* TankPlayerController;
};
