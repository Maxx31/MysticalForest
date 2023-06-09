// Copyright Epic Games, Inc. All Rights Reserved.

#include "MysticalForestCharacter.h"
#include "MysticalForestProjectile.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Inventory/Interactable.h"
#include "Inventory/Pickup.h"
#include "MysticalForestGameMode.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include <Actions/PawnAction.h>


//////////////////////////////////////////////////////////////////////////
// AMysticalForestCharacter

AMysticalForestCharacter::AMysticalForestCharacter()
{
	AutoPossessPlayer = EAutoReceiveInput::Player0;


	// Character doesnt have a rifle at start
	bHasRifle = false;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	MovementComponent = GetCharacterMovement();
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));

	Mesh1P->SetOwnerNoSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);

	Mesh1P->SetupAttachment(GetCapsuleComponent());
	Mesh1P->bCastDynamicShadow = true;
	Mesh1P->CastShadow = true;

	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -100.f));

	bUseControllerRotationYaw = false;
}

void AMysticalForestCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	Inventory.SetNum(InventorySize);
	ItemsAmmount.SetNum(InventorySize);

	CurrentInteractable = nullptr;
}

void AMysticalForestCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckForInteractables();
}

//////////////////////////////////////////////////////////////////////////// Input

void AMysticalForestCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping 
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump); 
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMysticalForestCharacter::Move);
		EnhancedInputComponent->BindAction(BoostAction, ETriggerEvent::Started, this, &AMysticalForestCharacter::Boost);
		EnhancedInputComponent->BindAction(BoostAction, ETriggerEvent::Completed, this, &AMysticalForestCharacter::StopBoost);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMysticalForestCharacter::Look);

		//Inventory
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AMysticalForestCharacter::Interact);
		EnhancedInputComponent->BindAction(OpenInventoryAction, ETriggerEvent::Started, this, &AMysticalForestCharacter::ToggleInventory);

		//Choosing active slot
		EnhancedInputComponent->BindAction(ChooseSlot1Action, ETriggerEvent::Started, this, &AMysticalForestCharacter::ChooseActiveSlot, 0);
		EnhancedInputComponent->BindAction(ChooseSlot2Action, ETriggerEvent::Started, this, &AMysticalForestCharacter::ChooseActiveSlot, 1);
		EnhancedInputComponent->BindAction(ChooseSlot3Action, ETriggerEvent::Started, this, &AMysticalForestCharacter::ChooseActiveSlot, 2);
		EnhancedInputComponent->BindAction(ChooseSlot4Action, ETriggerEvent::Started, this, &AMysticalForestCharacter::ChooseActiveSlot, 3);
		EnhancedInputComponent->BindAction(ChooseSlot5Action, ETriggerEvent::Started, this, &AMysticalForestCharacter::ChooseActiveSlot, 4);
		EnhancedInputComponent->BindAction(ChooseSlot6Action, ETriggerEvent::Started, this, &AMysticalForestCharacter::ChooseActiveSlot, 5);
		EnhancedInputComponent->BindAction(ChooseSlot7Action, ETriggerEvent::Started, this, &AMysticalForestCharacter::ChooseActiveSlot, 6);
		EnhancedInputComponent->BindAction(ChooseSlot8Action, ETriggerEvent::Started, this, &AMysticalForestCharacter::ChooseActiveSlot, 7);

	}
}


void AMysticalForestCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y );
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AMysticalForestCharacter::Boost(const FInputActionValue& Value)
{
	if (MovementComponent != nullptr)
	{
		MovementComponent->MaxWalkSpeed = CharacterRunMovementSpeed;
	}
}

void AMysticalForestCharacter::StopBoost(const FInputActionValue& Value)
{
	if (MovementComponent != nullptr)
	{
		MovementComponent->MaxWalkSpeed = CharacterDefaultMovementSpeed;
	}
}

void AMysticalForestCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AMysticalForestCharacter::ToggleInventory()
{
	AMysticalForestGameMode* GameMode = Cast<AMysticalForestGameMode>(GetWorld()->GetAuthGameMode());

	if (GameMode->IsInventoryOpened)
	{
		GameMode->CloseInventoryWidget();
	}
	else
	{
		GameMode->OpenInventoryWidget();
	}
}

void AMysticalForestCharacter::Interact()
{
	if (CurrentInteractable != nullptr)
	{
		CurrentInteractable->Interact_Implementation();
	}
}

void AMysticalForestCharacter::CheckForInteractables()
{
	//Throw a line trace and see, if it hits something
	FVector StartTrace = FirstPersonCameraComponent->GetComponentLocation();
	FVector EndTrace = (FirstPersonCameraComponent->GetForwardVector() * RichOfLaneTraceInteraction) + StartTrace;

	FHitResult HitResult;

	FCollisionQueryParams CQP;
	CQP.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_WorldDynamic, CQP);

	AInteractable* Interactable = Cast<AInteractable>(HitResult.GetActor());

	if (Interactable == NULL)
	{
		HelpText = FString("");
		CurrentInteractable = nullptr;
		return;
	}
	else
	{
		CurrentInteractable = Interactable;
		HelpText = Interactable->InteractableHelpText;
	}
}

void AMysticalForestCharacter::ChooseActiveSlot(int32 slotIndex)
{
	if (ActiveItemSlot == slotIndex) return;

	UE_LOG(LogTemp, Warning, TEXT("New active slot index = %d!!"), slotIndex);
	ActiveItemSlot = slotIndex;
	EquipActiveItemAtActiveSlot();
}



bool AMysticalForestCharacter::AddItemToInventory(APickup* Item)
{
	if (Item != nullptr) 
	{
		int32 AvailableSlot = INDEX_NONE;
		

		if (Item->GetItemInfo()->IsStackable)
		{
			int32 AmmountOfItemsToAdd;

			for (int i = 0; i < InventorySize; i++)
			{
				if (Inventory[i] == nullptr) continue;

				if(Inventory[i]->GetItemInfo() == Item->GetItemInfo())
				{
					AmmountOfItemsToAdd = FMath::Min(Inventory[i]->GetItemInfo()->StackSize - ItemsAmmount[i], Item->AmmountOfItems);

					Item->AmmountOfItems -= AmmountOfItemsToAdd;
					ItemsAmmount[i] += AmmountOfItemsToAdd;
				}
				if (Item->AmmountOfItems <= 0) break;
			}
		}

		if (Item->AmmountOfItems > 0)
		{
			AvailableSlot = Inventory.Find(nullptr);

			if (AvailableSlot != INDEX_NONE)
			{
				Inventory[AvailableSlot] = Item;
				ItemsAmmount[AvailableSlot] = Item->AmmountOfItems;

				UE_LOG(LogTemp, Warning, TEXT("Available slot = %d, ActiveItemSLot = %d"), AvailableSlot, ActiveItemSlot);

				if (AvailableSlot == ActiveItemSlot && Item->GetItemInfo()->Mesh != nullptr) EquipActiveItemAtActiveSlot();
				else Item->InteractableMesh->SetVisibility(false);
				return true;
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("You cant carry any more item!"));
				return false;
			}
		}
		else 
		{
			Item->InteractableMesh->SetVisibility(false);
			return true;
		}
	}
	else return false;
}

UTexture2D* AMysticalForestCharacter::GetThumbnailAtInventorySlot(int32 Slot)
{
	if (Slot < 0 || Slot >= Inventory.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("Slot num is not in Inventory array size bounds!"));
		return nullptr;
	}
	if (Inventory[Slot] != NULL)
	{
		if (Inventory[Slot] == nullptr)
		{
		}
		return Inventory[Slot]->GetItemInfo()->Icon;
	}
	else return nullptr;
}

FString AMysticalForestCharacter::GetItemNameAtInventorySlot(int32 Slot)
{
	if (Inventory[Slot] != NULL)
	{
		return Inventory[Slot]->GetItemInfo()->Name;
	}
	else return FString("None");
}


int AMysticalForestCharacter::GetItemsAmmountAtInventorySlot(int32 Slot)
{
	return ItemsAmmount[Slot];
}


bool AMysticalForestCharacter::SwapItemSlots(int32 BeginSlot, int32 EndSlot, bool IsLeftMouseButton)
{
	if (BeginSlot == EndSlot)return false;

	if (BeginSlot == ActiveItemSlot || EndSlot == ActiveItemSlot) DisEquipActiveItemAtActiveSlot();

	if (Inventory[BeginSlot] != NULL)
	{
		if (Inventory[EndSlot] != NULL) 
		{ // Check to stack items
			if (Inventory[BeginSlot]->GetItemInfo()->IsStackable &&
				Inventory[BeginSlot]->GetItemInfo() == Inventory[EndSlot]->GetItemInfo() &&
				ItemsAmmount[EndSlot] < Inventory[EndSlot]->GetItemInfo()->StackSize)
			{
				int32 AmmountOfItemsToAdd;
				if (IsLeftMouseButton)// Drag with left mouse button
				{
					AmmountOfItemsToAdd = FMath::Min(Inventory[EndSlot]->GetItemInfo()->StackSize - ItemsAmmount[EndSlot], ItemsAmmount[BeginSlot]);
				}//Else, if user drag with right mouse button, then we take only one object
				else {
					AmmountOfItemsToAdd = 1;
				}

				ItemsAmmount[BeginSlot] -= AmmountOfItemsToAdd;
				ItemsAmmount[EndSlot] += AmmountOfItemsToAdd;

				if (ItemsAmmount[BeginSlot] <= 0) Inventory[BeginSlot] = NULL;

				return true;
			}
		}
		else
		{
			if (!IsLeftMouseButton && Inventory[BeginSlot]->GetItemInfo()->IsStackable && ItemsAmmount[BeginSlot] > 1)
			{
				APickup* NewPickup = NewObject<APickup>();
				NewPickup->SetItemInfo(Inventory[BeginSlot]->GetItemInfo());

				Inventory[EndSlot] = NewPickup;
				ItemsAmmount[EndSlot] = 1;

				ItemsAmmount[BeginSlot]--;
				return true;
			}
		}
		Inventory.Swap(BeginSlot, EndSlot);
		ItemsAmmount.Swap(BeginSlot, EndSlot);

		if (EndSlot == ActiveItemSlot || BeginSlot == ActiveItemSlot) EquipActiveItemAtActiveSlot();

		return true;
	}
	else 
	{
		return false;
	}
}



bool AMysticalForestCharacter::UseItemAtInventorySlot(int32 Slot)
{
	if (Inventory[Slot] != NULL)
	{
		Inventory[Slot]->Use_Implementation();
		Inventory[Slot] = NULL;
		return true;
	}
	return false;
}


void AMysticalForestCharacter::EquipActiveItemAtActiveSlot()
{
	DisEquipActiveItemAtActiveSlot();

	if (Inventory[ActiveItemSlot] != NULL)
	{
		if  (Inventory[ActiveItemSlot]->GetItemInfo()->Mesh != nullptr)
		{
			const FTransform socket_transform = GetMesh1P()->GetSocketTransform(TEXT("Item_Socket"), ERelativeTransformSpace::RTS_World);
			
			FTransform newTransform = Inventory[ActiveItemSlot]->GetActorTransform();
			newTransform.SetLocation(socket_transform.GetLocation());
			newTransform.SetRotation(socket_transform.GetRotation());

			Inventory[ActiveItemSlot]->SetActorTransform(newTransform);
			Inventory[ActiveItemSlot]->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("Item_Socket"));
			Inventory[ActiveItemSlot]->InteractableMesh->SetVisibility(true);

			CurrentMeshEquiped = ActiveItemSlot;
		}
	}
}

void AMysticalForestCharacter::DisEquipActiveItemAtActiveSlot()
{
	if (Inventory[CurrentMeshEquiped] != NULL)
	{
		if (Inventory[CurrentMeshEquiped]->GetItemInfo()->Mesh != nullptr)
		{
			Inventory[CurrentMeshEquiped]->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			Inventory[CurrentMeshEquiped]->InteractableMesh->SetVisibility(false);
		}
	}
}

FString AMysticalForestCharacter::GetHelpText()
{
	return HelpText;
}

void AMysticalForestCharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool AMysticalForestCharacter::GetHasRifle()
{
	return bHasRifle;
}

