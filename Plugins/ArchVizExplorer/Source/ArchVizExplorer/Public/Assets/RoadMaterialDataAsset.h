// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RoadMaterialDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FRoadMaterialData {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, category = "RoadMaterialAsset")
	UTexture2D* Image;

	UPROPERTY(EditAnywhere, category = "RoadMaterialAsset")
	UMaterialInterface* Material;
};

UCLASS()
class ARCHVIZEXPLORER_API URoadMaterialDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
	public:

	UPROPERTY(EditAnywhere , category="RoadMaterialAsset")
	TArray<FRoadMaterialData> RoadMaterialArr;
};
