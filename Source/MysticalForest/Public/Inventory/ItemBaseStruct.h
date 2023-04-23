#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"

#include "UObject/ObjectMacros.h"
#include "ItemBaseStruct.generated.h"

USTRUCT(BlueprintType)
struct FInventoryItem: public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MultiLine = "true"))
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D* Icon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool IsStuckable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 StackSize;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FDataTableRowHandle>CraftItems;
};