// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BuildingMaterialDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FBuildingMaterialData {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, category = "BuildingMaterialAsset")
	UTexture2D* Image;

	UPROPERTY(EditAnywhere, category = "BuildingMaterialAsset")
	UMaterialInterface* Material;
};

UCLASS()
class ARCHVIZEXPLORER_API UBuildingMaterialDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
	public:

	UPROPERTY(EditAnywhere , category = "BuildingMaterialAsset")
	TArray<FBuildingMaterialData> BuildingMaterialArr;
};
