// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InteriorStruct.h"
#include "FloorInteriorDataAsset.generated.h"

UCLASS()
class ARCHVIZEXPLORER_API UFloorInteriorDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
	public:

	UPROPERTY(EditAnywhere , category = "FloorInteriorAsset")
	TArray<FInteriorData> FloorInteriorArr;
};
