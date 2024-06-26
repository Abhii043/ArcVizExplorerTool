// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/SCompoundWidget.h"
#include <Widgets/Layout/SScrollBox.h>

#include "DoorDataAsset.h"
#include "WallAsset.h"
#include "Assets/RoadMaterialDataAsset.h"
#include "Assets/BuildingMaterialDataAsset.h"
#include "Assets/WallInteriorDataAsset.h"
#include "Assets/FloorInteriorDataAsset.h"
#include "Assets/RoofInteriorDataAsset.h"

DECLARE_DELEGATE_OneParam(FOnWallSelection ,const FWallData&)
DECLARE_DELEGATE_OneParam(FOnDoorSelection ,const FDoorData&)
DECLARE_DELEGATE_OneParam(FOnBuildingMaterialSelection ,const FBuildingMaterialData&)
DECLARE_DELEGATE_OneParam(FOnRoadMaterialSelection ,const FRoadMaterialData&)
DECLARE_DELEGATE_OneParam(FOnWallInteriorSelection ,const FWallInteriorData&)
DECLARE_DELEGATE_OneParam(FOnFloorInteriorSelection ,const FFloorInteriorData&)
DECLARE_DELEGATE_OneParam(FOnRoofInteriorSelection ,const FRoofInteriorData&)

UENUM()
enum class EScrollBoxType : uint8
{
	WallData,
	DoorData,
	WallInterior,
	FloorInterior,
	RoofInterior,
	BuildingMaterialData,
	RoadMaterialData,
};

class ARCHVIZEXPLORER_API SScrollBoxSlate : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SScrollBoxSlate) {}
	SLATE_ARGUMENT(TWeakObjectPtr<UDoorDataAsset>, InDoorAssetManager)
	SLATE_ARGUMENT(TWeakObjectPtr<UWallAsset>, InWallAssetManager)
	SLATE_ARGUMENT(TWeakObjectPtr<UBuildingMaterialDataAsset>, InBuildingMaterialAssetManager)
	SLATE_ARGUMENT(TWeakObjectPtr<URoadMaterialDataAsset>, InRoadMaterialAssetManager)
	SLATE_ARGUMENT(TWeakObjectPtr<UWallInteriorDataAsset>, InWallInteriorAssetManager)
	SLATE_ARGUMENT(TWeakObjectPtr<UFloorInteriorDataAsset>, InFloorInteriorAssetManager)
	SLATE_ARGUMENT(TWeakObjectPtr<URoofInteriorDataAsset>, InRoofInteriorAssetManager)
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
	FOnBuildingMaterialSelection OnBuildingMaterialSelection;
	FOnRoadMaterialSelection OnRoadMaterialSelection;
	FOnWallInteriorSelection OnWallInteriorSelection;
	FOnFloorInteriorSelection OnFloorInteriorSelection;
	FOnRoofInteriorSelection OnRoofInteriorSelection;

protected:
	void ScrollBoxSelection();

	void PopulateDoorScrollBox();
	void PopulateWallScrollBox();
	void PopulateBuildingMaterialScrollBox();
	void PopulateRoadMaterialScrollBox();
	void PopulateWallInteriorScrollBox();
	void PopulateFloorInteriorScrollBox();
	void PopulateRoofInteriorScrollBox();

private:
	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = "SlateScrollBox" , Meta = (UIMin = 1.0f , ClampMin = 1.0f))
	float ThumbnailSize{1.0f};

	TWeakObjectPtr<UWallAsset> WallAssetPtr;
	TWeakObjectPtr<UDoorDataAsset> DoorAssetPtr;
	TWeakObjectPtr<UBuildingMaterialDataAsset> BuildingMaterialAssetPtr;
	TWeakObjectPtr<URoadMaterialDataAsset> RoadMaterialAssetPtr;
	TWeakObjectPtr<UWallInteriorDataAsset> WallInteriorAssetPtr;
	TWeakObjectPtr<UFloorInteriorDataAsset> FloorInteriorAssetPtr;
	TWeakObjectPtr<URoofInteriorDataAsset> RoofInteriorAssetPtr;

	EScrollBoxType ScrollBoxType;
};
