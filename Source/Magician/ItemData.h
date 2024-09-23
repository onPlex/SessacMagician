// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemData.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Consumable,
	Equipment,
	Quest,
	Material
};

USTRUCT(BlueprintType)
struct FItemData :public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Item")
	int32 ItemId;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Item")
	EItemType ItemType;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Item")
	FText ItemName;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Item")
	FText ItemDescription;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Item")
	UTexture2D* ItemThumnail;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Item")
	UStaticMesh* ItemMesh;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Item")
	int32 Price;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Item")
	bool bIsStackable;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Item")
	int32 MaxStackCount;
};

class MAGICIAN_API ItemData
{
public:
	ItemData();
	~ItemData();
};
