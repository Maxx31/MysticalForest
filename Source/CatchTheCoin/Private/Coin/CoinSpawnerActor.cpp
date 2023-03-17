// Fill out your copyright notice in the Description page of Project Settings.

#include "Coin/CoinSpawnerActor.h"
#include "Coin/CoinActor.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
ACoinSpawnerActor::ACoinSpawnerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACoinSpawnerActor::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("Spawning actor"));

	StartSpawning();
}

void ACoinSpawnerActor::Tick(float DeltaTime)
{

}

void ACoinSpawnerActor::StartSpawning()
{
	SpawnCoin();
	GetWorldTimerManager().SetTimer(SpawnTimerHandler, this, &ACoinSpawnerActor::SpawnCoin, TimeBetweenSpawn, true);
}

void ACoinSpawnerActor::SpawnCoin() 
{
	FVector SpawnLocation = FVector(FMath::FRandRange(xMin, xMax), FMath::FRandRange(yMin, yMax), FMath::FRandRange(zMin, zMax));
	//FVector SpawnRotation = FVector(0, 0, FMath::FRandRange(0, 360));

	AActor* NewActor = GetWorld()->SpawnActor<AActor>(CoinClass, SpawnLocation, FRotator::ZeroRotator);
}


