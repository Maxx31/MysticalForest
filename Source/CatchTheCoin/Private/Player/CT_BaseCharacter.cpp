// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CT_BaseCharacter.h"
#include "Camera/CameraComponent.h"

// Sets default values
ACT_BaseCharacter::ACT_BaseCharacter()
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
	//PlayerInputComponent->BindAxis("MoveRight", this, &ASTUBaseCharacter::MoveRight);
	//PlayerInputComponent->BindAxis("LookUp", this, &ASTUBaseCharacter::AddControllerPitchInput);
	//PlayerInputComponent->BindAxis("TurnAround", this, &ASTUBaseCharacter::AddControllerYawInput);
	//PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASTUBaseCharacter::Jump);
	//PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ASTUBaseCharacter::OnStartRunning);
	//PlayerInputComponent->BindAction("Run", IE_Released, this, &ASTUBaseCharacter::OnStopRunning);
}


void ACT_BaseCharacter::MoveForward(float Amount)
{
	UE_LOG(LogTemp, Warning, TEXT("MOVING"));

}
void ACT_BaseCharacter::MoveRight(float Amount)
{
	

}

