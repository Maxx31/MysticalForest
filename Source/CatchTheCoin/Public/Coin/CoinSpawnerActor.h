// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoinSpawnerActor.generated.h"

class ACoinActor;
UCLASS()
class CATCHTHECOIN_API ACoinSpawnerActor : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Coin")
	TSubclassOf<ACoinActor> CoinClass;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	ACoinSpawnerActor();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Coins spawn transform")
	float xMin;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Coins spawn transform")
	float xMax;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Coins spawn transform")
	float yMin;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Coins spawn transform")
	float yMax;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Coins spawn transform")
	float zMin;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Coins spawn transform")
	float zMax;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Coins spawn params")
	float TimeBetweenSpawn;

private:
	void SpawnCoin();
	void StartSpawning();

	FTimerHandle SpawnTimerHandler;
};
