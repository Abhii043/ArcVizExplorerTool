// Fill out your copyright notice in the Description page of Project Settings.


#include "ScrollBoxChild.h"

void UScrollBoxChild::NativeConstruct()
{
	Super::NativeConstruct();
	if (LoadTemplateSlot) {
		LoadTemplateSlot->OnClicked.AddDynamic(this, &UScrollBoxChild::AfterSlotSelected);
		DeleteSlot->OnClicked.AddDynamic(this, &UScrollBoxChild::AfterSlotDeleteButtonPressed);
	}
}

void UScrollBoxChild::AfterSlotSelected()
{	
	OnTemplateSlotPressed.ExecuteIfBound((TemplateName->GetText()));
}

void UScrollBoxChild::AfterSlotDeleteButtonPressed()
{
	OnDeleteTemplateSlotPressed.ExecuteIfBound((TemplateName->GetText()));
}
