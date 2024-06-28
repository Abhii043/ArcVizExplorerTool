// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InteriorStruct.h"
#include "WallInteriorDataAsset.generated.h"

UCLASS()
class ARCHVIZEXPLORER_API UWallInteriorDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
	public:

	UPROPERTY(EditAnywhere , category = "WallInteriorAsset")
	TArray<FInteriorData> WallInteriorArr;
};
