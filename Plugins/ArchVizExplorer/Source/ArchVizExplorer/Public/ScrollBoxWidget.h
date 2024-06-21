// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "SScrollBoxSlate.h"
#include "ScrollBoxWidget.generated.h"

DECLARE_DELEGATE_OneParam(FAfterWallSelection, const FWallData&)
DECLARE_DELEGATE_OneParam(FAfterDoorSelection, const FDoorData&)


UCLASS()
class ARCHVIZEXPLORER_API UScrollBoxWidget : public UWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ScrollBoxWidget")
	UWallAsset* WallAsset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ScrollBoxWidget")
	UDoorDataAsset* DoorAsset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ScrollBoxWidget")
	EScrollBoxType ScrollBoxType;
	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = "ScrollBox" , Meta = (UIMin = 1.0f , ClampMin = 1.0f , AllowPrivateAccess = "true"))
	float ThumbnailSize{1.0f};

	FAfterWallSelection AfterWallSelection ;
	FAfterDoorSelection AfterDoorSelection ;

	virtual TSharedRef<SWidget> RebuildWidget() override;
protected:

	UFUNCTION()
	void PassWallInController(const FWallData& WallData);

	UFUNCTION()
	void PassDoorInController(const FDoorData& DoorData);

private:
	TSharedPtr<SScrollBoxSlate> ScrollBoxAsset;
	void ReleaseSlateResources(bool bReleaseChildren);
	virtual const FText GetPaletteCategory() override;
};
