// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DoorDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FDoorData {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere , category="DoorAsset")
	FText Name;

	UPROPERTY(EditAnywhere, category = "DoorAsset")
	UTexture2D* Image;

	UPROPERTY(EditAnywhere, category = "DoorAsset")
	UStaticMesh* Mesh;
};

UCLASS()
class ARCHVIZEXPLORER_API UDoorDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere , category="DoorAsset")
	TArray<FDoorData> DoorTypes;
};
