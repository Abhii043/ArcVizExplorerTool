// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteriorStruct.generated.h"

USTRUCT(BlueprintType)

struct FInteriorData {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, category = "InteriorAsset")
	int8 IdentityIndex;

	UPROPERTY(EditAnywhere, category = "InteriorAsset")
	UTexture2D* Image;

	UPROPERTY(EditAnywhere, category = "InteriorAsset")
	UStaticMesh* InteriorMesh;
};
