// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MysticalForestGameMode.generated.h"


UCLASS(minimalapi)
class AMysticalForestGameMode : public AGameModeBase
{
	GENERATED_BODY()

	virtual void BeginPlay() override;

public:
	AMysticalForestGameMode();
	UUserWidget* AddHUD(TSubclassOf<class UUserWidget>WidgetToApply, bool bShowMouseCursor, bool EnableCLickEvents, bool AddToViewport = true);

	bool IsInventoryOpened = false;

	void OpenInventoryWidget();
	void CloseInventoryWidget();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUD Widgets")
	TSubclassOf<class UUserWidget> IngameHUDClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUD Widgets")
	TSubclassOf<class UUserWidget> InventoryHUDClass;

	UFUNCTION(BlueprintPure, Category = "Inventory")
	bool IsInventoryhWidgetOpened() const { return IsInventoryOpened; }

	UUserWidget* InventoryUserWidget;
	APlayerController* Controller;

};



