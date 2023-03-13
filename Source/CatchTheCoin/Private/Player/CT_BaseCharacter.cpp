// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/CT_BaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/MyCharacterMovementComponent.h"
// Sets default values
ACT_BaseCharacter::ACT_BaseCharacter(const FObjectInitializer& ObjInit)
: Super(ObjInit.SetDefaultSubobjectClass<UMyCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ACT_BaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACT_BaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACT_BaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACT_BaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACT_BaseCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ACT_BaseCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("TurnAround", this, &ACT_BaseCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACT_BaseCharacter::Jump);
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ACT_BaseCharacter::OnStartRunning);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &ACT_BaseCharacter::OnStopRunning);
}


void ACT_BaseCharacter::MoveForward(float Amount)
{
	IsMovingForward = (Amount > 0);
	if (Amount == 0.0f)return;
	AddMovementInput(GetActorForwardVector(), Amount);
}
void ACT_BaseCharacter::MoveRight(float Amount)
{
	if (Amount == 0.0f)return;
	AddMovementInput(GetActorRightVector(), Amount);
}

void ACT_BaseCharacter::OnStartRunning()
{
	WantsToRun = true;
}

void ACT_BaseCharacter::OnStopRunning()
{
	WantsToRun = false;
}

bool ACT_BaseCharacter::IsRunning() const
{
	return WantsToRun && IsMovingForward && !GetVelocity().IsZero();
}
