// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/Button.h>
#include <Components/Border.h>
#include <Components/ComboBoxString.h>
#include "StartMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARCHVIZEXPLORER_API UStartMenuWidget : public UUserWidget
{
	GENERATED_BODY()

	public:

	UPROPERTY(meta = (BindWidget))
	UButton* CreateProject;
	UPROPERTY(meta = (BindWidget))
	UButton* LoadTemplate;
	UPROPERTY(meta = (BindWidget))
	UButton* Load;

	UPROPERTY(meta = (BindWidget))
	UBorder* ListBox;
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* TemplateBoxList;
};
