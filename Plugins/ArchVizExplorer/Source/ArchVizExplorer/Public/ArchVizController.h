// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include <InputMappingContext.h>
#include <InputAction.h>
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "RoadConstructor.h"
#include "RoadUIWidget.h"
#include "HomeWidget.h"
#include "BuildingConstructorWidget.h"
#include "InteriorDesignWidget.h"
#include "MaterialManagmentWidget.h"
#include <WallGenerator.h>
#include "DoorDataAsset.h"
#include "FloorGenerator.h"
#include "RoofGenerator.h"
#include <Engine/StaticMeshActor.h>
#include <InteriorDesignActor.h>
#include "ScrollBoxChild.h"
#include <SaveLoadWidget.h>
#include "ArchVizController.generated.h"

UENUM()
enum class EWidgetSelection {
	Home,
	RoadConstructor,
	BuildingConstructor,
	InteriorDesign,
	MaterialManagment,
	LoadSaveTemplate
};

UENUM()
enum class EBuildingComponentSelection {
	None,
	Wall,
	Floor,
	Roof,
	Door,
	Editor
};

UCLASS()
class ARCHVIZEXPLORER_API AArchVizController : public APlayerController
{
	GENERATED_BODY()

public:
	AArchVizController();

protected:
	virtual void BeginPlay();
	virtual void Tick(float DeltaSeconds);
	virtual void SetupInputComponent() override;

private:
	//Common

	UPROPERTY(EditAnyWhere, category = "ArchVizController")
	TSubclassOf<UHomeWidget> HomeWidgetClassRef;
	UPROPERTY()
	UHomeWidget* HomeWidget;
	UPROPERTY(EditAnyWhere, category = "ArchVizController")
	TSubclassOf<USaveLoadWidget> SaveLoadWidgetClassRef;
	UPROPERTY()
	USaveLoadWidget* SaveLoadWidget;
	UPROPERTY()
	EWidgetSelection SelectedWidget; 
	UPROPERTY()
	EBuildingComponentSelection SelectedBuildingComponent;
	UPROPERTY()
	FHitResult HitResult;

	UPROPERTY(EditAnyWhere, category = "ArchVizController")
	TSubclassOf<UScrollBoxChild> ScrollBoxChildRef;
	UPROPERTY()
	UScrollBoxChild* ScrollBoxChild;

	UFUNCTION(BlueprintCallable , category = "ArchVizController")
	void ConstructionModeHandler(); 	
	UFUNCTION(BlueprintCallable, category = "ArchVizController")
	void BindWidgetDelegates();
	UFUNCTION()
	void HandleModeChange();
	UFUNCTION()
	void OnHomeButtonPressed();

	//Template Load And Save
	UFUNCTION()
	void ToggleVisibility();
	UFUNCTION()
	void OnTemplatePressed();
	UFUNCTION()
	bool CheckFileExists(const FString& FileName);
	UFUNCTION()
	void SaveTemplate();
	UFUNCTION()
	void LoadTemplate(const FText& Slot);
	UFUNCTION()
	TArray<FString> FindFiles(FString Path, FString Extension);
	UFUNCTION()
	void EmptyViewportBeforeLoad();
	UFUNCTION()
	void HideSaveAndLoadMenu();
	UFUNCTION()
	void HideSaveMenu();
	UFUNCTION()
	void HideLoadMenu();
	UFUNCTION()
	void LoadTemplateList_();
	UFUNCTION()
	void DeleteSavedSlot(const FText& Slot);


	//Road Construction Mapping

	UPROPERTY(EditAnyWhere, category = "ArchVizController")
	TSubclassOf<ARoadConstructor> RoadConstructorClassRef;
	UPROPERTY()
	ARoadConstructor* RoadConstructor;
	UPROPERTY(EditAnyWhere, category = "ArchVizController")
	TSubclassOf<URoadUIWidget> RoadWidgetClassRef;
	UPROPERTY()
	URoadUIWidget* RoadWidget; 	
	UPROPERTY()
	UInputMappingContext* RoadMapping;
	UPROPERTY()
	UInputAction* GenerateRoadAction; 
	UPROPERTY()
	FVector RoadStartLocation;
	UPROPERTY()
	FVector RoadEndLocation;
	UPROPERTY()
	FVector RoadDimensions;
	UPROPERTY()
	bool bGetLocation;
	UPROPERTY()
	bool bFirstRoad; 
	UPROPERTY()
	bool bRoadEditorMode;

	UFUNCTION(BlueprintCallable, category = "ArchVizController", Meta = (AllowPrivateAccess = true))
	void GetRoadLocationOnClick();
	UFUNCTION(BlueprintCallable, category = "ArchVizController", Meta = (AllowPrivateAccess = true))
	void GenerateRoad();	
	UFUNCTION(BlueprintCallable, category = "ArchVizController", Meta = (AllowPrivateAccess = true))
	void GenerateNewRoad();
	UFUNCTION(BlueprintCallable , category = "ArchVizController" , Meta = (AllowPrivateAccess = true))
	float FindAngleBetweenVectors(const FVector& Vec1, const FVector& Vec2);	
	UFUNCTION()
	void SetRoadConstructionMapping();
	UFUNCTION()
	void OnRoadConstructionPressed(); 
	UFUNCTION()
	void OnRoadLocationXChanged(float Value);
	UFUNCTION()
	void OnRoadLocationYChanged(float Value);
	UFUNCTION()
	void OnRoadEditorModePressed(); 
	UFUNCTION()
	void OnRoadWidthChanged(float Value);

	//BuildingConstructor Logic
	UPROPERTY(EditAnyWhere, category = "ArchVizController")
	TSubclassOf<UBuildingConstructorWidget> BuildingConstructorWidgetClassRef;
	UPROPERTY()
	UBuildingConstructorWidget* BuildingConstructorWidget;

	UFUNCTION()
	void OnBuildingConstructionPressed(); 
	UFUNCTION()
	void AddBuildingComponentsMapping();
	UFUNCTION()
	void SetDefaultBuildingMode();

				//Wall Logic

	UPROPERTY(EditAnyWhere , category = "ArchVizController")
	TSubclassOf<AWallGenerator> WallGeneratorActorClassRef;
	UPROPERTY()
	AWallGenerator* WallGeneratorActor;
	UPROPERTY()
	UInputAction* GenerateWallAction;
	UPROPERTY()
	UInputAction* RotateWallAction;
	UPROPERTY()
	UInputMappingContext* WallMapping;
	UPROPERTY()
	UStaticMesh* TempWallMesh;

	UFUNCTION()
	void PreviewWallActor();
	UFUNCTION()
	void SnapWallActor(float SnapValue);
	UFUNCTION()
	void OnGenerateWallPressed();
	UFUNCTION()
	void OnWallSegmentsValueChanged(float SegmentsNo);
	UFUNCTION()
	void GenerateNewWall();
	UFUNCTION()
	void RotateWall();
	UFUNCTION()
	void SetWallConstructionMapping();
	UFUNCTION()
	void SetWallStaticMesh(const FWallData& WallData);
	
				//Door Logic
	UPROPERTY()
	UInputAction* GenerateDoorAction;
	UPROPERTY()
	UInputMappingContext* DoorMapping;
	UPROPERTY()
	AStaticMeshActor* DoorMeshActor;
	UPROPERTY()
	UStaticMesh* DoorMesh{};

	UFUNCTION()
	void OnGenerateDoorPressed(); 
	UFUNCTION()
	void SetDoorConstructionMapping();
	UFUNCTION()
	void GenerateDoor();
	UFUNCTION()
	void SetDoor(const FDoorData& DoorData);
	UFUNCTION()
	void PreviewDoorActor();
	
				//Floor Logic
	UPROPERTY()
	UInputMappingContext* FloorMapping;
	UPROPERTY()
	UInputAction* GenerateFloorAction; 
	UPROPERTY()
	AFloorGenerator* FloorGeneratorActor;
	UPROPERTY(EditAnyWhere, category = "ArchVizController")
	TSubclassOf<AFloorGenerator> FloorGeneratorActorClassRef;
	UPROPERTY()
	FVector FloorStartLocation{};

	UFUNCTION()
	void OnGenerateFloorPressed();
	UFUNCTION()
	void SetFloorConstructionMapping();
	UFUNCTION()
	void GenerateFloor();
	UFUNCTION()
	void PreviewFloorActor();
	UFUNCTION()
	void SnapFloorActor(float SnapValue);
	UFUNCTION()
	void CompletedFloorGeneration();

					//Roof Logic
	UPROPERTY()
	UInputMappingContext* RoofMapping;
	UPROPERTY()
	UInputAction* GenerateRoofAction; 
	UPROPERTY()
	UInputAction* GenerateRoofWithRightClickAction;
	UPROPERTY()
	ARoofGenerator* RoofGeneratorActor;
	UPROPERTY(EditAnyWhere, category = "ArchVizController")
	TSubclassOf<ARoofGenerator> RoofGeneratorActorClassRef;
	UPROPERTY()
	FVector RoofStartLocation; 
	UPROPERTY()
	float RoofInZ{};

	UFUNCTION()
	void OnGenerateRoofPressed(); 
	UFUNCTION()
	void GenerateRoofOnMiddleClick();
	UFUNCTION()
	void GenerateRoofOnLeftClick(); 
	UFUNCTION()
	void GenerateRoofOnLeftClickCompleted();
	UFUNCTION()
	void SetRoofConstructionMapping();
	UFUNCTION()
	int8 CheckIfMultipleWallActorInZ(float& Location1, float& Location2, FVector& Locatiionn);
	UFUNCTION()
	void SnapRoofActor(float SnapValue);

					//EditorMode Logic
	UPROPERTY()
	UInputMappingContext* EditorMapping; 
	UPROPERTY()
	UInputAction* SelectAssetAction;
	UPROPERTY()
	UStaticMeshComponent* DoorHighlightComponent;
	UPROPERTY()
	UInputAction* RotateWallInEditorAction;

	UFUNCTION()
	void OnBuildingEditorModePressed();
	UFUNCTION()
	void SetEditorModeMapping(); 
	UFUNCTION()
	void OnAssetSelection();

	UFUNCTION()
	void EditWall();
	UFUNCTION()
	void OnWallDestroyerPressed();
	UFUNCTION()
	void ControlWallGeneratorActorOnTick();
	UFUNCTION()
	void OnWallLocationXChanged(float Value);	
	UFUNCTION()
	void OnWallLocationYChanged(float Value);

	UFUNCTION()
	void EditDoor();
	UFUNCTION()
	void OnDoorDestroyerPressed();

	UFUNCTION()
	void EditFloor();
	UFUNCTION()
	void OnFloorLocationXChanged(float Value);	
	UFUNCTION()
	void OnFloorLocationYChanged(float Value);
	UFUNCTION()
	void OnFloorDestroyPressed();
	UFUNCTION()
	void OnFloorLengthChanged(float Length); 
	UFUNCTION()
	void OnFloorWidthChanged(float Width);
	UFUNCTION()
	void OnFloorHeightChanged(float Height);
	UFUNCTION()
	void ControlFloorGeneratorActorOnTick();

	UFUNCTION()
	void EditRoof();
	UFUNCTION()
	void OnRoofLocationXChanged(float Value);	
	UFUNCTION()
	void OnRoofLocationYChanged(float Value);
	UFUNCTION()
	void OnRoofLocationZChanged(float Value);
	UFUNCTION()
	void OnRoofDestroyPressed();
	UFUNCTION()
	void OnRoofLengthChanged(float Length); 
	UFUNCTION()
	void OnRoofWidthChanged(float Width);
	UFUNCTION()
	void OnRoofHeightChanged(float Height);

	UPROPERTY()
	bool bBuildingEditorMode; 
	UPROPERTY()
	bool bIsDoorDestroyed;
	UPROPERTY()
	bool bIsWallProjection;
	UPROPERTY()
	bool bProjection;
	UPROPERTY()
	bool bIsFloorProjection;


	//Material Managment Mode

	UPROPERTY(EditAnyWhere , category = "ArchVizController")
	TSubclassOf<UMaterialManagmentWidget> MaterialManagmentWidgetClassRef;
	UPROPERTY()
	UMaterialManagmentWidget* MaterialManagmentWidget;
	UPROPERTY()
	UInputMappingContext* MaterialManagmentMapping;
	UPROPERTY()
	UInputAction* SelectActorAction;

	UFUNCTION()
	void OnMaterialManagmentPressed();
	UFUNCTION()
	void SetMaterialManagmentMapping();
	UFUNCTION()
	void SelectActorOnClick();
	UFUNCTION()
	void ApplyRoadMaterial(const FRoadMaterialData& RoadMaterialData);
	UFUNCTION()
	void ApplyBuildingMaterial(const FBuildingMaterialData& BuildingMaterialData);

	//Interior Design Mode
	UPROPERTY(EditAnyWhere , category = "ArchVizController")
	TSubclassOf<UInteriorDesignWidget> InteriorDesignWidgetClassRef;
	UPROPERTY()
	UInteriorDesignWidget* InteriorDesignWidget;
	UPROPERTY()
	UInputMappingContext* InteriorDesignMapping;
	UPROPERTY()
	UInputAction* PlaceInteriorAction;
	UPROPERTY()
	UInputAction* RotateInteriorAction;
	UPROPERTY()
	UInputAction* DestroyInteriorAction;
	UPROPERTY()
	AInteriorDesignActor* InteriorDesignActor;

	UFUNCTION()
	void OnInteriorDesignPressed();
	UFUNCTION()
	void SetInteriorDesignMapping();
	UFUNCTION()
	void PlaceInteriorOnClick(); 
	UFUNCTION()
	void RotateInterior();
	UFUNCTION()
	void DestroyInterior();
	UFUNCTION()
	void ApplyInterior(const FInteriorData& InteriorData);
	UFUNCTION()
	void PreviewWallInteriorActor();
	UFUNCTION()
	void PreviewFloorInteriorActor();
	UFUNCTION()
	void PreviewRoofInteriorActor();
	UFUNCTION()
	FVector CheckPivotLocation(AInteriorDesignActor* InteriorActor, FVector Location ,const int32& indx);
};
