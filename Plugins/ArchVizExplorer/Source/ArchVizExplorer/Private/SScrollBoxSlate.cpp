// Fill out your copyright notice in the Description page of Project Settings.


#include "SScrollBoxSlate.h"
#include "SlateOptMacros.h"
#include "Brushes/SlateColorBrush.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SScrollBoxSlate::Construct(const FArguments& InArgs)
{
	DoorAssetPtr = InArgs._InDoorAssetManager;
	WallAssetPtr = InArgs._InWallAssetManager;
	ScrollBoxType = InArgs._InScrollBoxType;
	ThumbnailSize = InArgs._InThumbnailSize;

	RootBorder = SNew(SBorder);
	FSlateColorBrush* ColorBrush = new FSlateColorBrush(FLinearColor(0.12549f, 0.043137f, 0.101961f, 1.0f));
	ColorBrush->DrawAs = ESlateBrushDrawType::RoundedBox;
	FSlateBrushOutlineSettings OutlineSettings{};
	OutlineSettings.CornerRadii = FVector4{ 10.f , 10.f ,10.f , 10.f };
	OutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;
	ColorBrush->OutlineSettings = OutlineSettings;
	RootBorder->SetBorderImage(ColorBrush);

	RootVerticalBox = SNew(SVerticalBox);
	RootBoxName = SNew(STextBlock).Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 25)).ColorAndOpacity(FColor::White);
	ScrollBox = SNew(SScrollBox).Orientation(Orient_Horizontal);
	ScrollBox->SetScrollBarPadding(8);

	ScrollBoxSelection();

	RootVerticalBox->AddSlot().HAlign(HAlign_Left).AutoHeight().Padding(15, 5, 0, 5)
		[
			RootBoxName.ToSharedRef()
		];
	RootVerticalBox->AddSlot().HAlign(HAlign_Center)
		[
			ScrollBox.ToSharedRef()
		];

	RootBorder->SetContent(RootVerticalBox.ToSharedRef());

	ChildSlot
		[
			RootBorder.ToSharedRef()
		];
}
void SScrollBoxSlate::ScrollBoxSelection()
{
    switch (ScrollBoxType)
    {
    case EScrollBoxType::WallData:
        PopulateWallScrollBox();
        break;
    case EScrollBoxType::DoorData:
        PopulateDoorScrollBox();
        break;
	}
}


void SScrollBoxSlate::PopulateWallScrollBox()
{
	RootBoxName->SetText(FText::FromString("Wall"));
	ScrollBox->ClearChildren();

	if(WallAssetPtr.IsValid()){
		for (auto& DataItems : WallAssetPtr->WallTypes) {
			if (DataItems.Image) {
				TSharedPtr<STextBlock> Name = SNew(STextBlock).Text(DataItems.Name).Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 20));

				FSlateBrush* SlateBrush = new FSlateBrush();
				SlateBrush->DrawAs = ESlateBrushDrawType::RoundedBox;
				FSlateBrushOutlineSettings OutlineSettings{};
				OutlineSettings.CornerRadii = FVector4{ 5.f , 5.f ,5.f , 5.f };
				OutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;
				SlateBrush->OutlineSettings = OutlineSettings;

				SlateBrush->SetResourceObject(DataItems.Image);
				SlateBrush->ImageSize = FVector2D(ThumbnailSize);
				TSharedPtr<SImage> ThumbnailImage = SNew(SImage).Image(SlateBrush).Cursor(EMouseCursor::Hand)
					.OnMouseButtonDown_Lambda([this, &DataItems](const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
						{
							if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
								OnWallSelection.ExecuteIfBound(DataItems);
								return FReply::Handled();
							}
							return FReply::Unhandled();
						});
				TSharedPtr<SBorder> ScrollBoxBorder = SNew(SBorder);
				FSlateColorBrush* ColorBrush = new FSlateColorBrush(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f));
				ColorBrush->DrawAs = ESlateBrushDrawType::RoundedBox;
				OutlineSettings.CornerRadii = FVector4{ 5.f ,5.f ,5.f ,5.f };
				OutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;
				ColorBrush->OutlineSettings = OutlineSettings;

				ScrollBoxBorder->SetBorderImage(ColorBrush);
				ScrollBoxBorder->SetPadding(FMargin(15, 15, 15, 15));

				ScrollBoxBorder->SetContent(ThumbnailImage.ToSharedRef());
				TSharedPtr<SVerticalBox> ScrollBoxVerticalBox = SNew(SVerticalBox);

				ScrollBoxVerticalBox->AddSlot().HAlign(HAlign_Center).Padding(5)
					[
						ScrollBoxBorder.ToSharedRef()
					];
				ScrollBoxVerticalBox->AddSlot().HAlign(HAlign_Center).AutoHeight()
					[
						Name.ToSharedRef()
					];
				ScrollBox->AddSlot()
					[
						ScrollBoxVerticalBox.ToSharedRef()
					];
			}
		}
	}
}

void SScrollBoxSlate::PopulateDoorScrollBox()
{
	RootBoxName->SetText(FText::FromString("Door"));
	ScrollBox->ClearChildren();

	if(DoorAssetPtr.IsValid()){
		for (auto& DataItems : DoorAssetPtr->DoorTypes) {
			if (DataItems.Image) {
				TSharedPtr<STextBlock> Name = SNew(STextBlock).Text(DataItems.Name).Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 20));
				FSlateBrush* SlateBrush = new FSlateBrush();
				SlateBrush->DrawAs = ESlateBrushDrawType::RoundedBox;
				FSlateBrushOutlineSettings OutlineSettings{};
				OutlineSettings.CornerRadii = FVector4{ 5.f , 5.f ,5.f , 5.f };
				OutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;
				SlateBrush->OutlineSettings = OutlineSettings;

				SlateBrush->SetResourceObject(DataItems.Image);
				SlateBrush->ImageSize = FVector2D(ThumbnailSize);
				TSharedPtr<SImage> ThumbnailImage = SNew(SImage).Image(SlateBrush).Cursor(EMouseCursor::Hand)
					.OnMouseButtonDown_Lambda([this, &DataItems](const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
						{
							if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
								OnDoorSelection.ExecuteIfBound(DataItems);
								return FReply::Handled();
							}
							return FReply::Unhandled();
						});
				TSharedPtr<SBorder> ScrollBoxBorder = SNew(SBorder);
				FSlateColorBrush* ColorBrush = new FSlateColorBrush(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f));
				ColorBrush->DrawAs = ESlateBrushDrawType::RoundedBox;
				OutlineSettings.CornerRadii = FVector4{ 5.f ,5.f ,5.f ,5.f };
				OutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;
				ColorBrush->OutlineSettings = OutlineSettings;

				ScrollBoxBorder->SetBorderImage(ColorBrush);
				ScrollBoxBorder->SetPadding(FMargin(15, 15, 15, 15));

				ScrollBoxBorder->SetContent(ThumbnailImage.ToSharedRef());
				TSharedPtr<SVerticalBox> ScrollBoxVerticalBox = SNew(SVerticalBox);

				ScrollBoxVerticalBox->AddSlot().HAlign(HAlign_Center).Padding(5)
					[
						ScrollBoxBorder.ToSharedRef()
					];
				ScrollBoxVerticalBox->AddSlot().HAlign(HAlign_Center).AutoHeight()
					[
						Name.ToSharedRef()
					];
				ScrollBox->AddSlot()
					[
						ScrollBoxVerticalBox.ToSharedRef()
					];
			}
		}
	}

}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
