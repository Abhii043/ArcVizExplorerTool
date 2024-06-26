// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RoofInteriorDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FRoofInteriorData {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, category = "WallAsset")
	UTexture2D* Image;

	UPROPERTY(EditAnywhere, category = "WallAsset")
	UStaticMesh* InteriorMesh;
};

UCLASS()
class ARCHVIZEXPLORER_API URoofInteriorDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
	public:

	UPROPERTY(EditAnywhere , category = "RoofInteriorAsset")
	TArray<FRoofInteriorData> RoofInteriorArr;
};
