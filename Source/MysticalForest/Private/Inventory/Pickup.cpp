// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory/Pickup.h"
#include <Kismet/GameplayStatics.h>
#include <MysticalForest/MysticalForestCharacter.h>


APickup::APickup() 
{
  // Set up mesh for pickup, and item name, help text and item value
	InteractableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pickup mesh"));
	InteractableMesh->SetSimulatePhysics(true);

	ItemName = FString("Enter an item name here...");
	InteractableHelpText = FString("Press E to pick item up.");
}

void APickup::BeginPlay()
{
	InteractableHelpText = FString::Printf(TEXT("%s: Press E to pick up"), *ItemName);
}


void APickup::Interact_Implementation()
{
	AMysticalForestCharacter* Character = Cast<AMysticalForestCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0)); 

	if (Character->AddItemToInventory(this))
	{
		OnPickedUp();
	}

}

void APickup::Use_Implementation()
{
	GLog->Log("Use() from base pickup class : You should not see this");
}

void APickup::OnPickedUp()
{
	InteractableMesh->SetVisibility(false);
	InteractableMesh->SetSimulatePhysics(false);
	InteractableMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
