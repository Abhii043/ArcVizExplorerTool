// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "WallGenerator.h"
#include "ArchVizExplorerSave.generated.h"

USTRUCT()
struct FRoadSaveData {
	GENERATED_BODY()

	UPROPERTY()
	UMaterialInterface* RoadMaterial;
	
	UPROPERTY()
	FTransform RoadTransform;

	UPROPERTY()
	FVector RoadDimensions;
};

USTRUCT()
struct FWallSaveData {
	GENERATED_BODY()

	UPROPERTY()
	FTransform WallTransform;

	UPROPERTY()
	UMaterialInterface* WallMaterial;

	UPROPERTY()
	int32 NoOfSegments;

	UPROPERTY()
	TMap<int32, FPair> WallGeneratorActorMap;
};

USTRUCT()
struct FFloorSaveData {
	GENERATED_BODY()

	UPROPERTY()
	FTransform FloorTransform;

	UPROPERTY()
	UMaterialInterface* FloorMaterial;

	UPROPERTY()
	FVector FloorDimensions;
};

USTRUCT()
struct FRoofSaveData {
	GENERATED_BODY()

	UPROPERTY()
	FTransform RoofTransform;

	UPROPERTY()
	UMaterialInterface* RoofMaterial;

	UPROPERTY()
	FVector RoofDimensions;
};

USTRUCT()
struct FInteriorSaveData {
	GENERATED_BODY()

	UPROPERTY()
	FTransform InteriorTransform;

	UPROPERTY()
	int8 InteriorIndex;

	UPROPERTY()
	UStaticMesh* InteriorMesh;
};

UCLASS()
class ARCHVIZEXPLORER_API UArchVizExplorerSave : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	TArray<FRoadSaveData> RoadActorArray;
	
	UPROPERTY()
	TArray<FWallSaveData> WallActorArray;
	
	UPROPERTY()
	TArray<FFloorSaveData> FloorActorArray;
	
	UPROPERTY()
	TArray<FRoofSaveData> RoofActorArray;
	
	UPROPERTY()
	TArray<FInteriorSaveData> InteriorActorArray;
};
