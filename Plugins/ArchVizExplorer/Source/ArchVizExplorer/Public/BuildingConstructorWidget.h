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
	UBorder* BuildingOutlineBorder; 
	UPROPERTY(meta = (BindWidget))
	UBorder* BuildingEditorBorder;
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
	UTextBlock* BuildingEditorMode;

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
	UBorder* Pop_Up;
	UPROPERTY(meta = (BindWidget))
	UButton* WallDestroy; 
	UPROPERTY(meta = (BindWidget))
	UButton* DoorDestroy; 

	UPROPERTY(meta = (BindWidget))
	UBorder* FloorDimensionsBorder;
	UPROPERTY(meta = (BindWidget))
	UBorder* FloorLocationBorder;
	UPROPERTY(meta = (BindWidget))
	UButton* FloorOnMouseControl; 
	UPROPERTY(meta = (BindWidget))
	USpinBox* Floor_X;
	UPROPERTY(meta = (BindWidget))
	USpinBox* Floor_Y;
	UPROPERTY(meta = (BindWidget))
	USpinBox* FloorLength;
	UPROPERTY(meta = (BindWidget))
	USpinBox* FloorWidth;
	UPROPERTY(meta = (BindWidget))
	USpinBox* FloorHeight;
	UPROPERTY(meta = (BindWidget))
	UBorder* FloorDestroyer;
	UPROPERTY(meta = (BindWidget))
	UButton* FloorDestroy;

	UPROPERTY(meta = (BindWidget))
	UBorder* RoofDimensionsBorder;
	UPROPERTY(meta = (BindWidget))
	UBorder* RoofLocationBorder;
	UPROPERTY(meta = (BindWidget))
	USpinBox* Roof_X;
	UPROPERTY(meta = (BindWidget))
	USpinBox* Roof_Y;
	UPROPERTY(meta = (BindWidget))
	USpinBox* Roof_Z;
	UPROPERTY(meta = (BindWidget))
	USpinBox* RoofLength;
	UPROPERTY(meta = (BindWidget))
	USpinBox* RoofWidth;
	UPROPERTY(meta = (BindWidget))
	USpinBox* RoofHeight;
	UPROPERTY(meta = (BindWidget))
	UBorder* RoofDestroyer;
	UPROPERTY(meta = (BindWidget))
	UButton* RoofDestroy;
};
