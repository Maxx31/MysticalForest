// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "ItemBaseStruct.h"
#include "DragDropItemOperation.generated.h"


UCLASS()
class MYSTICALFOREST_API UDragDropItemOperation : public UDragDropOperation
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slots", meta = (ExposeOnSpawn = "true"))
	int32 SlotIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MouseData", meta = (ExposeOnSpawn = "true"))
	bool IsLeftMouseButtonDrag = true;
};
