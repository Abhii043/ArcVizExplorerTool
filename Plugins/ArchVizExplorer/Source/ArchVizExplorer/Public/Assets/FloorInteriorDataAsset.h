// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FloorInteriorDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FFloorInteriorData {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, category = "WallAsset")
	UTexture2D* Image;

	UPROPERTY(EditAnywhere, category = "WallAsset")
	UStaticMesh* InteriorMesh;
};

UCLASS()
class ARCHVIZEXPLORER_API UFloorInteriorDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
	public:

	UPROPERTY(EditAnywhere , category = "FloorInteriorAsset")
	TArray<FFloorInteriorData> FloorInteriorArr;
};
