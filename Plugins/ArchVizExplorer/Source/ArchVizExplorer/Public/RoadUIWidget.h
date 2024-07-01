// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/Border.h>
#include <Components/VerticalBox.h>
#include <Components/Button.h>
#include <Components/TextBlock.h>
#include <Components/SpinBox.h>
#include "RoadUIWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARCHVIZEXPLORER_API URoadUIWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* OutsideBox;

	UPROPERTY(meta = (BindWidget))
	UBorder* OutlineBorder;	
	
	UPROPERTY(meta = (BindWidget))
	UBorder* RoadEditor;

	UPROPERTY(meta = (BindWidget))
	UButton* EditorMode; 

	UPROPERTY(meta = (BindWidget))
	UTextBlock* EditorText;

	UPROPERTY(meta = (BindWidget))
	UButton* GenerateNewRoad;

	UPROPERTY(meta = (BindWidget))
	USpinBox* WidthValueBox; 

	UPROPERTY(meta = (BindWidget))
	USpinBox* Location_x;

	UPROPERTY(meta = (BindWidget))
	USpinBox* Location_Y; 
};
