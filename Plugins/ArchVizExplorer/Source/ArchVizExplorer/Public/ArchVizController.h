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
	Wall,
	Floor,
	Roof,
	Door
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

	//RoadConstructor Mapping
	UPROPERTY(EditDefaultsOnly,Category = "RoadConstructor")
	UMaterialInstance* Material;
	
	UPROPERTY()
	FVector StartLocation;

	UPROPERTY()
	FVector EndLocation;

	UPROPERTY()
	FHitResult HitResult;

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

	UPROPERTY(EditAnyWhere , category = "ArchVizController")
	TSubclassOf<AWallGenerator> WallActorClassRef;

	UPROPERTY()
	AWallGenerator* WallActor;

	UPROPERTY()
	FHitResult WallHitResult;
	
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

	//Wall Logic
	UFUNCTION()
	void WallProjectionOnTick();
	UFUNCTION()
	void SnapActor(float SnapValue);
	UFUNCTION()
	void OnGenerateWallPressed();
	UFUNCTION()
	void OnWallSegmentsValueChanged(float SegmentsNo);

	UPROPERTY(VisibleDefaultsOnly, category = "ArchVizController")
	UInputAction* GenerateWallAction;
	UPROPERTY(VisibleDefaultsOnly, category = "ArchVizController")
	UInputAction* RotateWallAction;
	UPROPERTY(VisibleDefaultsOnly , category = "ArchVizController")
	UInputMappingContext* WallMapping;
	UFUNCTION()
	void GenerateWall();
	UFUNCTION()
	void RotateWall();

	UFUNCTION()
	void SetWallConstructionMapping();

	//Door Logic
	UPROPERTY(VisibleDefaultsOnly , category = "ArchVizController")
	UInputMappingContext* DoorMapping;
	UFUNCTION()
	void OnGenerateDoorPressed();

	UFUNCTION()
	void SetDoorConstructionMapping();
	
	//Floor Logic
	UPROPERTY(VisibleDefaultsOnly , category = "ArchVizController")
	UInputMappingContext* FloorMapping;
	UFUNCTION()
	void OnGenerateFloorPressed();

	UFUNCTION()
	void SetFloorConstructionMapping();

	//Roof Logic
	UPROPERTY(VisibleDefaultsOnly , category = "ArchVizController")
	UInputMappingContext* RoofMapping;
	UFUNCTION()
	void OnGenerateRoofPressed();

	UFUNCTION()
	void SetRoofConstructionMapping();
};
