// Fill out your copyright notice in the Description page of Project Settings.

#include "Coin/CoinActor.h"
#include "Player/CT_BaseCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
ACoinActor::ACoinActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");

	Collider = CreateDefaultSubobject<USphereComponent>("SphereColider");
	Collider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	SetRootComponent(Collider); 

}

// Called when the game starts or when spawned
void ACoinActor::BeginPlay()
{
	Super::BeginPlay();

	//UE_LOG(LogTemp, Warning, TEXT("Collision Seted Up"));
	Collider->OnComponentBeginOverlap.AddDynamic(this, &ACoinActor::OnProjectileOverlap);

	check(MovementComponent);
	SetUpMovement();

}



// Called every frame
void ACoinActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACoinActor::OnProjectileOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Collision hit"));

	ACT_BaseCharacter* MainCharacter = Cast<ACT_BaseCharacter>(OtherActor);
	if (MainCharacter)
	{
		MainCharacter->AddCoin();
		Destroy();
	}
}

void ACoinActor::SetUpMovement()
{
	FVector FallDirection = FVector(0, 0, -1);
	MovementComponent->Velocity = FallDirection * MovementComponent->InitialSpeed;
}

