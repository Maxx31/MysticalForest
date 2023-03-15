// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CT_BaseCharacter.generated.h"

class UCameraComponent;

UCLASS()
class CATCHTHECOIN_API ACT_BaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACT_BaseCharacter(const FObjectInitializer& Object);

	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	float GetScore() const;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	bool IsRunning() const;

	void AddCoin();

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UCameraComponent* CameraComponent;

	virtual void BeginPlay() override;


private:
	int CurrScore = 0;

	bool WantsToRun = false;
	bool IsMovingForward = false;

	void MoveForward(float Amount);
	void MoveRight(float Amount);
	void OnStartRunning();
	void OnStopRunning();
};
