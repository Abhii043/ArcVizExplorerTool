// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScrollBoxWidget.h"
#include "InteriorDesignWidget.generated.h"

UCLASS()
class ARCHVIZEXPLORER_API UInteriorDesignWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UScrollBoxWidget* WallInteriorScrollBox;
	UPROPERTY(meta = (BindWidget))
	UScrollBoxWidget* FloorInteriorScrollBox;
	UPROPERTY(meta = (BindWidget))
	UScrollBoxWidget* RoofInteriorScrollBox;
};
