// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/Button.h>
#include <Components/TextBlock.h>
#include "ScrollBoxChild.generated.h"

DECLARE_DELEGATE_OneParam(FOnTemplateSlotPressed ,const FText&)
DECLARE_DELEGATE_OneParam(FOnDeleteTemplateSlotPressed ,const FText&)

UCLASS()
class ARCHVIZEXPLORER_API UScrollBoxChild : public UUserWidget
{
	GENERATED_BODY()
	
	public:
		UPROPERTY(meta = (BindWidget))
		UButton* LoadSlotSlot;
		UPROPERTY(meta = (BindWidget))
		UTextBlock* TemplateName;
		UPROPERTY(meta = (BindWidget))
		UButton* DeleteSlot;

		FOnTemplateSlotPressed OnTemplateSlotPressed;
		FOnDeleteTemplateSlotPressed OnDeleteTemplateSlotPressed;

	protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void AfterSlotSelected();

	UFUNCTION()
	void AfterSlotDeleteButtonPressed();
	
};
