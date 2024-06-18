// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/SpinBox.h>
#include <Components/Button.h>
#include <Components/Border.h>
#include "BuildingConstructorWidget.generated.h"

UCLASS()
class ARCHVIZEXPLORER_API UBuildingConstructorWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UButton* GenerateWall;
	UPROPERTY(meta = (BindWidget))
	UButton* GenerateDoor;
	UPROPERTY(meta = (BindWidget))
	UButton* GenerateFloor;
	UPROPERTY(meta = (BindWidget))
	UButton* GenerateRoof;
	UPROPERTY(meta = (BindWidget))
	UBorder* SegmentBorder;
	UPROPERTY(meta = (BindWidget))
	USpinBox* Segments;
};
