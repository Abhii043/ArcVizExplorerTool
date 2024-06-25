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
#include <WallGenerator.h>
#include "DoorDataAsset.h"
#include "FloorGenerator.h"
#include "RoofGenerator.h"
#include "ArchVizController.generated.h"

UENUM()
enum class EWidgetSelection {
	RoadConstructor,
	BuildingConstructor,
	InteriorDesign,
	MaterialManagment
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

	//Common
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,category = "ArchVizController")
	EWidgetSelection SelectedWidget; 

	UFUNCTION(BlueprintCallable , category = "ArchVizController")
	void ConstructionModeHandler(); 
	
	UFUNCTION(BlueprintCallable, category = "ArchVizController")
	void BindWidgetDelegates();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "ArchVizController")
	EBuildingComponentSelection SelectedBuildingComponent;

	UPROPERTY()
	FHitResult HitResult;

	//RoadConstructor Mapping
	UPROPERTY(EditDefaultsOnly,Category = "RoadConstructor")
	UMaterialInstance* Material;
	
	UPROPERTY()
	FVector StartLocation;

	UPROPERTY()
	FVector EndLocation;

	UPROPERTY()
	FVector RoadDimensions;

	UPROPERTY()
	bool bGetLocation;
	
	UPROPERTY()
	bool bFirstRoad; 
	
	UPROPERTY()
	bool bEditorMode;

	UPROPERTY()
	ARoadConstructor* RoadConstructor;

	UPROPERTY()
	URoadUIWidget* RoadWidget; 
	
	UPROPERTY()
	UHomeWidget* HomeWidget;

	UPROPERTY(EditAnyWhere , category = "ArchVizController")
	TSubclassOf<URoadUIWidget> RoadWidgetClassRef; 
	
	UPROPERTY(EditAnyWhere, category = "ArchVizController")
	TSubclassOf<UHomeWidget> HomeWidgetClassRef;

	//BuildingConstruction Logic

	UPROPERTY(EditAnyWhere, category = "ArchVizController")
	TSubclassOf<UBuildingConstructorWidget> BuildingConstructorWidgetClassRef;

	UPROPERTY()
	UBuildingConstructorWidget* BuildingConstructorWidget;

	//Wall Logic

	UPROPERTY(EditAnyWhere , category = "ArchVizController")
	TSubclassOf<AWallGenerator> WallGeneratorActorClassRef;

	UPROPERTY()
	AWallGenerator* WallGeneratorActor;

	//Door Logic

	UPROPERTY(VisibleDefaultsOnly , category = "ArchVizController")
	UStaticMesh* DoorMesh{};

	//Editor Logic
	
protected:
	virtual void BeginPlay();
	virtual void Tick(float DeltaSeconds);
	virtual void SetupInputComponent() override;

private:
	//Road Construction Mapping

	UPROPERTY(VisibleDefaultsOnly , category = "ArchVizController")
	UInputMappingContext* RoadMapping;

	UPROPERTY(VisibleDefaultsOnly, category = "ArchVizController")
	UInputAction* GenerateRoadAction; 

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
	void OnLocationXChanged(float Value);
	
	UFUNCTION()
	void OnLocationYChanged(float Value);

	UFUNCTION()
	void OnEditorModePressed(); 
	
	UFUNCTION()
	void OnWidthChanged(float Value);

	//BuildingConstructor Logic
	UFUNCTION()
	void OnBuildingConstructionPressed(); 

	UFUNCTION()
	void AddBuildingComponentsMapping();

	UFUNCTION()
	void SetDefaultBuildingMode();

	//Wall Logic

	UPROPERTY(VisibleDefaultsOnly, category = "ArchVizController")
	UInputAction* GenerateWallAction;
	UPROPERTY(VisibleDefaultsOnly, category = "ArchVizController")
	UInputAction* RotateWallAction;
	UPROPERTY(VisibleDefaultsOnly , category = "ArchVizController")
	UInputMappingContext* WallMapping;
	UPROPERTY()
	UStaticMesh* TempWallMesh;

	UFUNCTION()
	void WallProjectionOnTick();
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
	UPROPERTY(VisibleDefaultsOnly, category = "ArchVizController")
	UInputAction* GenerateDoorAction;
	UPROPERTY(VisibleDefaultsOnly , category = "ArchVizController")
	UInputMappingContext* DoorMapping;

	UFUNCTION()
	void OnGenerateDoorPressed(); 
	UFUNCTION()
	void SetDoorConstructionMapping();
	UFUNCTION()
	void GenerateDoor();
	UFUNCTION()
	void SetDoor(const FDoorData& DoorData);
	
	//Floor Logic
	UPROPERTY(VisibleDefaultsOnly , category = "ArchVizController")
	UInputMappingContext* FloorMapping;
	UPROPERTY(VisibleDefaultsOnly, category = "ArchVizController")
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
	void FloorProjectionOnTick();
	UFUNCTION()
	void SnapFloorActor(float SnapValue);
	UFUNCTION()
	void CompletedFloorGeneration();

	//Roof Logic
	UPROPERTY(VisibleDefaultsOnly , category = "ArchVizController")
	UInputMappingContext* RoofMapping;
	UPROPERTY(VisibleDefaultsOnly, category = "ArchVizController")
	UInputAction* GenerateRoofAction; 
	UPROPERTY()
	ARoofGenerator* RoofGeneratorActor;
	UPROPERTY(EditAnyWhere, category = "ArchVizController")
	TSubclassOf<ARoofGenerator> RoofGeneratorActorClassRef;

	UFUNCTION()
	void OnGenerateRoofPressed(); 
	UFUNCTION()
	void GenerateRoof();
	UFUNCTION()
	void SetRoofConstructionMapping();
	UFUNCTION()
	int8 CheckIfMultipleWallActorInZ(float& Location1, float& Location2, FVector& Locatiionn);

	//EditorMode Logic
	UPROPERTY(VisibleDefaultsOnly , category = "ArchVizController")
	UInputMappingContext* EditorMapping; 
	UPROPERTY(VisibleDefaultsOnly, category = "ArchVizController")
	UInputAction* SelectAssetAction;

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
	UPROPERTY()
	bool bIsRoofProjection;
};
