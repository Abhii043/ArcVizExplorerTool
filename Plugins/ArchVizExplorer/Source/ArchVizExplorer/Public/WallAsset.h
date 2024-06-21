// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WallAsset.generated.h"

USTRUCT(BlueprintType)
struct FWallData {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, category = "WallAsset")
	FText Name;

	UPROPERTY(EditAnywhere, category = "WallAsset")
	UTexture2D* Image;

	UPROPERTY(EditAnywhere, category = "WallAsset")
	UStaticMesh* Mesh;
};


UCLASS()
class ARCHVIZEXPLORER_API UWallAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, category = "WallAsset")
	TArray<FWallData> WallTypes;
};
