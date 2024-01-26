#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

ATank::ATank()
{
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    SpringArm->SetupAttachment(RootComponent);

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);
}

void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &ATank::UpDown);
    PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &ATank::LeftRight);

    PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATank::Fire);
    PlayerInputComponent->BindAction(TEXT("Dash"), IE_Pressed, this, &ATank::BeginDash);
}

void ATank::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (TankPlayerController)
    {
        FHitResult HitResult;
        TankPlayerController->GetHitResultUnderCursor(
            ECollisionChannel::ECC_Visibility, 
            false, 
            HitResult);

        RotateTurret(HitResult.ImpactPoint);
        RotateBase();
    }
}

void ATank::HandleDestruction()
{
    Super::HandleDestruction();
    SetActorHiddenInGame(true);
    SetActorTickEnabled(false);
    UE_LOG(LogTemp, Warning, TEXT("%s"), IsActorTickEnabled());
    bAlive = false;
}

void ATank::BeginPlay()
{
    Super::BeginPlay();

    TankPlayerController = Cast<APlayerController>(GetController());
}

void ATank::UpDown(float Value)
{
    FVector DeltaLocation = FVector::ZeroVector;
    // X = Value * DeltaTime * Speed
    DeltaLocation.X = Value * Speed * UGameplayStatics::GetWorldDeltaSeconds(this);
    if (bAlive) {
        AddActorLocalOffset(DeltaLocation, true);
    }
    UpDownInput = Value;
    UpdateMovementInput();
}

void ATank::LeftRight(float Value)
{
    FVector DeltaLocation = FVector::ZeroVector;
    // X = Value * DeltaTime * Speed
    DeltaLocation.Y = Value * Speed * UGameplayStatics::GetWorldDeltaSeconds(this);
    if (bAlive) {
        AddActorLocalOffset(DeltaLocation, true);
    }
    LeftRightInput = Value;
    UpdateMovementInput();
}

void ATank::UpdateMovementInput()
{
    MovementInput = FVector2D(LeftRightInput, UpDownInput);
    MovementInput.Normalize(); // Normalize the vector for consistent behavior
}

void ATank::Fire()
{
    /*if (bAlive) {
        Super::Fire();
    }*/
}

void ATank::BeginDash() {
    /*const FVector ForwardDir = this->GetActorRotation().Vector();
    float DashDistance = 5.0f;

    if (bAlive) {
        LaunchCharacter(ForwardDir * DashDistance, true, true);
    }*/
    if (DashReady) {
        DashReady = false;
        Speed = 2500.f;
        
        GetWorldTimerManager().SetTimer(DashTimerHandle, this, &ATank::EndDash, DashDuration, true);
        GetWorldTimerManager().SetTimer(DashCooldownHandle, this, &ATank::SetDashReady, DashCooldownDuration, true);
    }
}

void ATank::EndDash() {
    Speed = 700.f;
}

void ATank::SetDashReady() {
    DashReady = true;
}

void ATank::RotateBase()
{
    // Calculate the movement direction
    FVector MovementDirection = FVector(MovementInput.Y, MovementInput.X, 0.f).GetSafeNormal();

    // Calculate the desired rotation based on the movement direction
    FRotator DesiredRotation = MovementDirection.Rotation();

    // Set the world rotation of the BaseMesh
    //BaseMesh->SetWorldRotation(DesiredRotation);

    BaseMesh->SetWorldRotation(
        FMath::RInterpTo(
			BaseMesh->GetComponentRotation(), 
			DesiredRotation, 
			UGameplayStatics::GetWorldDeltaSeconds(this), 
			15.f));
}