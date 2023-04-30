// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory/Pickup.h"
#include <Kismet/GameplayStatics.h>
#include <MysticalForest/MysticalForestCharacter.h>

APickup::APickup() 
{
  // Set up mesh for pickup, and item name, help text and item value
	InteractableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pickup mesh"));
	InteractableMesh->SetSimulatePhysics(true);

}

void APickup::BeginPlay()
{
	ItemInfo = ItemInfoData.GetRow<FInventoryItem>(ItemInfoData.RowName.ToString());;
	InteractableHelpText = FString::Printf(TEXT("%s: Press E to pick up"), *(ItemInfo->Name));

	if (AmmountOfItems > ItemInfo->StackSize)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ammount of item is bigger than max stack size in item %s!!"), *(ItemInfo->Name));
		AmmountOfItems = ItemInfo->StackSize;
	}
}


void APickup::Interact_Implementation()
{
	AMysticalForestCharacter* Character = Cast<AMysticalForestCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0)); 

	//if(ItemInfo != nullptr)
    //UE_LOG(LogTemp, Warning, TEXT("You picked up %s"), *(ItemInfo->Name) );
	
	if (Character->AddItemToInventory(this))
	{
		OnPickedUp();
	}
}

void APickup::Use_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Use() from base pickup class : You should not see this"));
}

void APickup::UseLMB_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Use LMb() from base pickup class : You should not see this"));
}

void APickup::OnPickedUp()
{
	InteractableMesh->SetVisibility(false);
	InteractableMesh->SetSimulatePhysics(false);
	InteractableMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void APickup::SetItemInfo(FInventoryItem* NewItemInfo)
{
	ItemInfo = NewItemInfo;
}