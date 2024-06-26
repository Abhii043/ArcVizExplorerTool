// Fill out your copyright notice in the Description page of Project Settings.


#include "ScrollBoxWidget.h"

TSharedRef<SWidget> UScrollBoxWidget::RebuildWidget()
{
	ScrollBoxAsset = SNew(SScrollBoxSlate).InDoorAssetManager(DoorAsset).InWallAssetManager(WallAsset).InBuildingMaterialAssetManager(BuildingMaterialAsset).InRoadMaterialAssetManager(RoadMaterialAsset).InWallInteriorAssetManager(WallInteriorAsset).InFloorInteriorAssetManager(FloorInteriorAsset).InRoofInteriorAssetManager(RoofInteriorAsset).InScrollBoxType(ScrollBoxType).InThumbnailSize(ThumbnailSize);

	if(ScrollBoxAsset.IsValid()){
		ScrollBoxAsset->OnWallSelection.BindUFunction(this, "PassWallInController");
		ScrollBoxAsset->OnDoorSelection.BindUFunction(this, "PassDoorInController");
		ScrollBoxAsset->OnBuildingMaterialSelection.BindUFunction(this, "PassBuildingMaterialInController");
		ScrollBoxAsset->OnRoadMaterialSelection.BindUFunction(this, "PassRoadMaterialInController");
		ScrollBoxAsset->OnWallInteriorSelection.BindUFunction(this, "PassWallInteriorInController");
		ScrollBoxAsset->OnFloorInteriorSelection.BindUFunction(this, "PassFloorInteriorInController");
		ScrollBoxAsset->OnRoofInteriorSelection.BindUFunction(this, "PassRoofInteriorInController");

		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, "Scroll Box");

	}
	return ScrollBoxAsset.ToSharedRef();
}

void UScrollBoxWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	ScrollBoxAsset.Reset();
}

const FText UScrollBoxWidget::GetPaletteCategory()
{
	return FText::FromString("Panel");
}

void UScrollBoxWidget::PassBuildingMaterialInController(const FBuildingMaterialData& BuildingMaterialData) {
	AfterBuildingMaterialSelection.ExecuteIfBound(BuildingMaterialData);
}

void UScrollBoxWidget::PassRoadMaterialInController(const FRoadMaterialData& RoadMaterialData) {
	AfterRoadMaterialSelection.ExecuteIfBound(RoadMaterialData);
}

void UScrollBoxWidget::PassWallInteriorInController(const FWallInteriorData& WallInteriorData) {
	AfterWallInteriorSelection.ExecuteIfBound(WallInteriorData);
}

void  UScrollBoxWidget::PassFloorInteriorInController(const FFloorInteriorData& FloorInteriorData) {
	AfterFloorInteriorSelection.ExecuteIfBound(FloorInteriorData);
}

void UScrollBoxWidget::PassRoofInteriorInController(const FRoofInteriorData& RoofInteriorData) {
	AfterRoofInteriorSelection.ExecuteIfBound(RoofInteriorData);
}

void UScrollBoxWidget::PassWallInController(const FWallData& WallData)
{
	AfterWallSelection.ExecuteIfBound(WallData);
}

void UScrollBoxWidget::PassDoorInController(const FDoorData& DoorData)
{
	AfterDoorSelection.ExecuteIfBound(DoorData);
}

