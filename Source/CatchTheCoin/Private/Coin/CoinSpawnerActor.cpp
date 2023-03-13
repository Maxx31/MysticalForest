// Fill out your copyright notice in the Description page of Project Settings.

#include "Coin/CoinSpawnerActor.h"
#include "Coin/CoinActor.h"
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

	FVector SpawnLocation =  FVector(-740, -1980, 420); 
	FVector SpawnRotation =  FVector(0, 0, 0);

	AActor* NewActor = GetWorld()->SpawnActor<AActor>(CoinClass, SpawnLocation, FRotator::ZeroRotator);


}

// Called every frame
void ACoinSpawnerActor::Tick(float DeltaTime)
{

}

