// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/Border.h>
#include <Components/VerticalBox.h>
#include <Components/Button.h>
#include <Components/HorizontalBox.h>
#include <Components/EditableText.h>
#include <Components/TextBlock.h>
#include <Components/ScrollBox.h>
#include "HomeWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARCHVIZEXPLORER_API UHomeWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UBorder* OutSideBorder;
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* OutsideBox;
	UPROPERTY(meta = (BindWidget))
	UButton* RoadConstruction;
	UPROPERTY(meta = (BindWidget))
	UButton* BuildingConstruction;
	UPROPERTY(meta = (BindWidget))
	UButton* MaterialManagment;
	UPROPERTY(meta = (BindWidget))
	UButton* InteriorDesign; 
	UPROPERTY(meta = (BindWidget))
	UButton* HomeButton;
	UPROPERTY(meta = (BindWidget))
	UButton* Template;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PopUpText;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* PopUpAnimation;
};
