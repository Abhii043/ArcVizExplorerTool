// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DoorDataAsset.h"
#include "WallAsset.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/SCompoundWidget.h"
#include <Widgets/Layout/SScrollBox.h>

DECLARE_DELEGATE_OneParam(FOnWallSelection ,const FWallData&)
DECLARE_DELEGATE_OneParam(FOnDoorSelection ,const FDoorData&)

UENUM()
enum class EScrollBoxType : uint8
{
	WallData,
	DoorData
};

class ARCHVIZEXPLORER_API SScrollBoxSlate : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SScrollBoxSlate) {}
	SLATE_ARGUMENT(TWeakObjectPtr<UDoorDataAsset>, InDoorAssetManager)
	SLATE_ARGUMENT(TWeakObjectPtr<UWallAsset>, InWallAssetManager)
	SLATE_ARGUMENT(EScrollBoxType, InScrollBoxType)
	SLATE_ARGUMENT(float, InThumbnailSize)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	TSharedPtr<SBorder> RootBorder;
	TSharedPtr<STextBlock> RootBoxName;
	TSharedPtr<SScrollBox> ScrollBox;
	TSharedPtr<SVerticalBox> RootVerticalBox;

	FOnWallSelection OnWallSelection;
	FOnDoorSelection OnDoorSelection;

protected:
	void ScrollBoxSelection();

	void PopulateDoorScrollBox();
	void PopulateWallScrollBox();

private:
	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = "SlateScrollBox" , Meta = (UIMin = 1.0f , ClampMin = 1.0f))
	float ThumbnailSize{1.0f};

	TWeakObjectPtr<UWallAsset> WallAssetPtr;
	TWeakObjectPtr<UDoorDataAsset> DoorAssetPtr;

	EScrollBoxType ScrollBoxType;
};
