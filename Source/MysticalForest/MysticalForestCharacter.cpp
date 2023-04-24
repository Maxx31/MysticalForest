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
	//Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = true;
	Mesh1P->CastShadow = true;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

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

bool AMysticalForestCharacter::AddItemToInventory(APickup* Item)
{
	if (Item != nullptr) 
	{
		const int32 AvailableSlot = Inventory.Find(nullptr);

		if (AvailableSlot != INDEX_NONE)
		{
			Inventory[AvailableSlot] = Item;
			ItemsAmmount[AvailableSlot] = Item->AmmountOfItems;

			return true;
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("You cant carry any more item!"));
			return false;
		}
	}
	else return false;
}

UTexture2D* AMysticalForestCharacter::GetThumbnailAtInventorySlot(int32 Slot)
{
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

bool AMysticalForestCharacter::SwapItemSlots(int32 BeginSlot, int32 EndSlot)
{
	if (Inventory[BeginSlot] != NULL)
	{
		Inventory.Swap(BeginSlot, EndSlot);
		ItemsAmmount.Swap(BeginSlot, EndSlot);

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

void AMysticalForestCharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool AMysticalForestCharacter::GetHasRifle()
{
	return bHasRifle;
}

