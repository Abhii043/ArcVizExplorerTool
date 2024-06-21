// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/SpinBox.h>
#include <Components/Button.h>
#include <Components/Border.h>
#include <Components/ScrollBox.h>
#include <Components/Image.h>
#include <Components/TextBlock.h>
#include "ScrollBoxWidget.h"
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
	UPROPERTY(meta = (BindWidget))
	UButton* BuildingEditor;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* EditorMode;

	UPROPERTY(meta = (BindWidget))
	UScrollBoxWidget* WallScrollBox; 
	UPROPERTY(meta = (BindWidget))
	UScrollBoxWidget* DoorScrollBox;

	UPROPERTY(meta = (BindWidget))
	UBorder* WallDestroyer; 
	UPROPERTY(meta = (BindWidget))
	UBorder* WallLocationBorder; 
	UPROPERTY(meta = (BindWidget))
	UButton* WallOnMouseControl; 
	UPROPERTY(Transient , meta = (BindWidgetAnim))
	UWidgetAnimation* PopUpAnimation;
	UPROPERTY(meta = (BindWidget))
	USpinBox* Wall_X;
	UPROPERTY(meta = (BindWidget))
	USpinBox* Wall_Y;

	UPROPERTY(meta = (BindWidget))
	UBorder* DoorDestroyer;
	UPROPERTY(meta = (BindWidget))
	UButton* WallDestroy; 
	UPROPERTY(meta = (BindWidget))
	UButton* DoorDestroy;
};
