// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Inventory/Interactable.h"
#include "ItemBaseStruct.h"

#include "Pickup.generated.h"

/**
 * 
 */
UCLASS()
class MYSTICALFOREST_API APickup : public AInteractable
{
	GENERATED_BODY()
	
public:

	APickup();
	virtual void BeginPlay() override;


	virtual void Interact_Implementation() override;
	
	UFUNCTION(BlueprintNativeEvent)
	void Use();

	virtual void Use_Implementation();

	UPROPERTY(EditAnywhere, Category = "Pickup Properties")
	FDataTableRowHandle ItemInfo;

	UPROPERTY(EditAnywhere, Category = "Pickup Properties")
	FString ItemName;

	UPROPERTY(EditAnywhere, Category = "Pickup Properties")
	UTexture2D* PickupThumbnail;

	void OnPickedUp();
	void NuhatBebru();
};
