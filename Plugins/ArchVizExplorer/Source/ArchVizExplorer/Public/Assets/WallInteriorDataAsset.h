// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WallInteriorDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FWallInteriorData {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, category = "WallAsset")
	UTexture2D* Image;

	UPROPERTY(EditAnywhere, category = "WallAsset")
	UStaticMesh* InteriorMesh;
};

UCLASS()
class ARCHVIZEXPLORER_API UWallInteriorDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
	public:

	UPROPERTY(EditAnywhere , category = "WallInteriorAsset")
	TArray<FWallInteriorData> WallInteriorArr;
};
