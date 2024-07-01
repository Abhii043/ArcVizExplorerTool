// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/EditableText.h>
#include <Components/Button.h>
#include <Components/Border.h>
#include <Components/ScrollBox.h>
#include "SaveLoadWidget.generated.h"

UCLASS()
class ARCHVIZEXPLORER_API USaveLoadWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UButton* SaveTemplate;
	UPROPERTY(meta = (BindWidget))
	UButton* LoadTemplate;
	UPROPERTY(meta = (BindWidget))
	UEditableText* SlotNameTxt;
	UPROPERTY(meta = (BindWidget))
	UButton* SaveBtn;

	UPROPERTY(meta = (BindWidget))
	UButton* Close_1;
	UPROPERTY(meta = (BindWidget))
	UButton* Close_2;
	UPROPERTY(meta = (BindWidget))
	UButton* Close_3;

	UPROPERTY(meta = (BindWidget))
	UBorder* SaveMenu;
	UPROPERTY(meta = (BindWidget))
	UBorder* LoadMenu;
	UPROPERTY(meta = (BindWidget))
	UBorder* SaveLoadMenu;

	UPROPERTY(meta = (BindWidget))
	UScrollBox* LoadAllTemplateList;
};
