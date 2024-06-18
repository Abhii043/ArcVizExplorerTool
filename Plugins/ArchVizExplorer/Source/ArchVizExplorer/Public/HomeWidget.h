// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/Border.h>
#include <Components/VerticalBox.h>
#include <Components/Button.h>
#include <Components/HorizontalBox.h>
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
};
