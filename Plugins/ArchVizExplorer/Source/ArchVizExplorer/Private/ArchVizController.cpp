// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizController.h"
#include <Kismet/KismetMathLibrary.h>

AArchVizController::AArchVizController() : bGetLocation{ true }, bFirstRoad{ true }, bRoadEditorMode{ false }, bBuildingEditorMode{ false }, bIsWallProjection{ false }, SelectedBuildingComponent{ EBuildingComponentSelection::None }, bIsFloorProjection{ false }
{

}

void AArchVizController::BeginPlay()
{
	Super::BeginPlay();
	bShowMouseCursor = true;

	if (HomeWidgetClassRef) {
		HomeWidget = CreateWidget<UHomeWidget>(this, HomeWidgetClassRef);
		HomeWidget->AddToViewport();
	}
	if (RoadWidgetClassRef) {
		RoadWidget = CreateWidget<URoadUIWidget>(this, RoadWidgetClassRef);
	}
	if (BuildingConstructorWidgetClassRef) {
		BuildingConstructorWidget = CreateWidget<UBuildingConstructorWidget>(this, BuildingConstructorWidgetClassRef);
	}
	if (MaterialManagmentWidgetClassRef) {
		MaterialManagmentWidget = CreateWidget<UMaterialManagmentWidget>(this, MaterialManagmentWidgetClassRef);
	}
	if (InteriorDesignWidgetClassRef) {
		InteriorDesignWidget = CreateWidget<UInteriorDesignWidget>(this, InteriorDesignWidgetClassRef);
	}
	BindWidgetDelegates();
}

void AArchVizController::BindWidgetDelegates() {
	if (HomeWidget) {
		HomeWidget->RoadConstruction->OnClicked.AddDynamic(this, &AArchVizController::OnRoadConstructionPressed);
		HomeWidget->BuildingConstruction->OnClicked.AddDynamic(this, &AArchVizController::OnBuildingConstructionPressed);
		HomeWidget->MaterialManagment->OnClicked.AddDynamic(this, &AArchVizController::OnMaterialManagmentPressed);
		HomeWidget->InteriorDesign->OnClicked.AddDynamic(this, &AArchVizController::OnInteriorDesignPressed);
	}
	if (RoadWidget) {
		RoadWidget->EditorMode->OnClicked.AddDynamic(this, &AArchVizController::OnRoadEditorModePressed);
		RoadWidget->GenerateNewRoad->OnClicked.AddDynamic(this, &AArchVizController::GenerateNewRoad);
		RoadWidget->WidthValueBox->OnValueChanged.AddDynamic(this, &AArchVizController::OnRoadWidthChanged);
		RoadWidget->Location_x->OnValueChanged.AddDynamic(this, &AArchVizController::OnRoadLocationXChanged);
		RoadWidget->Location_Y->OnValueChanged.AddDynamic(this, &AArchVizController::OnRoadLocationYChanged);
	}
	if (BuildingConstructorWidget) {
		BuildingConstructorWidget->GenerateWall->OnClicked.AddDynamic(this, &AArchVizController::OnGenerateWallPressed);
		BuildingConstructorWidget->Segments->OnValueChanged.AddDynamic(this, &AArchVizController::OnWallSegmentsValueChanged);
		BuildingConstructorWidget->Wall_X->OnValueChanged.AddDynamic(this, &AArchVizController::OnWallLocationXChanged);
		BuildingConstructorWidget->Wall_Y->OnValueChanged.AddDynamic(this, &AArchVizController::OnWallLocationYChanged);
		BuildingConstructorWidget->WallScrollBox->AfterWallSelection.BindUFunction(this, "SetWallStaticMesh");
		BuildingConstructorWidget->WallDestroy->OnClicked.AddDynamic(this, &AArchVizController::OnWallDestroyerPressed);
		BuildingConstructorWidget->WallOnMouseControl->OnClicked.AddDynamic(this, &AArchVizController::ControlWallGeneratorActorOnTick);

		BuildingConstructorWidget->GenerateDoor->OnClicked.AddDynamic(this, &AArchVizController::OnGenerateDoorPressed);
		BuildingConstructorWidget->DoorScrollBox->AfterDoorSelection.BindUFunction(this, "SetDoor");
		BuildingConstructorWidget->DoorDestroy->OnClicked.AddDynamic(this, &AArchVizController::OnDoorDestroyerPressed);

		BuildingConstructorWidget->GenerateFloor->OnClicked.AddDynamic(this, &AArchVizController::OnGenerateFloorPressed);
		BuildingConstructorWidget->FloorLength->OnValueChanged.AddDynamic(this, &AArchVizController::OnFloorLengthChanged);
		BuildingConstructorWidget->FloorWidth->OnValueChanged.AddDynamic(this, &AArchVizController::OnFloorWidthChanged);
		BuildingConstructorWidget->FloorHeight->OnValueChanged.AddDynamic(this, &AArchVizController::OnFloorHeightChanged);
		BuildingConstructorWidget->Floor_X->OnValueChanged.AddDynamic(this, &AArchVizController::OnFloorLocationXChanged);
		BuildingConstructorWidget->Floor_Y->OnValueChanged.AddDynamic(this, &AArchVizController::OnFloorLocationYChanged);
		BuildingConstructorWidget->FloorDestroy->OnClicked.AddDynamic(this, &AArchVizController::OnFloorDestroyPressed);
		BuildingConstructorWidget->FloorOnMouseControl->OnClicked.AddDynamic(this, &AArchVizController::ControlFloorGeneratorActorOnTick);

		BuildingConstructorWidget->GenerateRoof->OnClicked.AddDynamic(this, &AArchVizController::OnGenerateRoofPressed);
		BuildingConstructorWidget->RoofLength->OnValueChanged.AddDynamic(this, &AArchVizController::OnRoofLengthChanged);
		BuildingConstructorWidget->RoofWidth->OnValueChanged.AddDynamic(this, &AArchVizController::OnRoofWidthChanged);
		BuildingConstructorWidget->RoofHeight->OnValueChanged.AddDynamic(this, &AArchVizController::OnRoofHeightChanged);
		BuildingConstructorWidget->Roof_X->OnValueChanged.AddDynamic(this, &AArchVizController::OnRoofLocationXChanged);
		BuildingConstructorWidget->Roof_Y->OnValueChanged.AddDynamic(this, &AArchVizController::OnRoofLocationYChanged);
		BuildingConstructorWidget->Roof_Z->OnValueChanged.AddDynamic(this, &AArchVizController::OnRoofLocationZChanged);
		BuildingConstructorWidget->RoofDestroy->OnClicked.AddDynamic(this, &AArchVizController::OnRoofDestroyPressed);

		BuildingConstructorWidget->BuildingEditor->OnClicked.AddDynamic(this, &AArchVizController::OnBuildingEditorModePressed);
	}

	if (MaterialManagmentWidget) {
		MaterialManagmentWidget->RoadMaterialScrollBox->AfterRoadMaterialSelection.BindUFunction(this, "ApplyRoadMaterial");
		MaterialManagmentWidget->BuildingMaterialScrollBox->AfterBuildingMaterialSelection.BindUFunction(this, "ApplyBuildingMaterial");
	}
	if (InteriorDesignWidget) {
		InteriorDesignWidget->WallInteriorScrollBox->AfterInteriorSelection.BindUFunction(this, "ApplyInterior");
		InteriorDesignWidget->FloorInteriorScrollBox->AfterInteriorSelection.BindUFunction(this, "ApplyInterior");
		InteriorDesignWidget->RoofInteriorScrollBox->AfterInteriorSelection.BindUFunction(this, "ApplyInterior");
	}
}

void AArchVizController::SetupInputComponent()
{
	Super::SetupInputComponent();

	SetRoadConstructionMapping();
	SetMaterialManagmentMapping();
	SetInteriorDesignMapping();

	SetWallConstructionMapping();
	SetDoorConstructionMapping();
	SetFloorConstructionMapping();
	SetRoofConstructionMapping();
	SetEditorModeMapping();
}

void AArchVizController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (SelectedWidget == EWidgetSelection::BuildingConstructor) {
		if ((SelectedBuildingComponent == EBuildingComponentSelection::Wall) || bIsWallProjection) {
			PreviewWallActor();
		}
		else if (SelectedBuildingComponent == EBuildingComponentSelection::Door) {
			PreviewDoorActor();
		}
		else if (bIsFloorProjection) {
			PreviewFloorActor();
		}
	}
	if (SelectedWidget == EWidgetSelection::InteriorDesign) {
		if(InteriorDesignActor)
		{
		if(InteriorDesignActor->IdentityIndex == 0)
			PreviewWallInteriorActor();
		else if(InteriorDesignActor->IdentityIndex == 1)
			PreviewFloorInteriorActor();
		else if(InteriorDesignActor->IdentityIndex == 2)
			PreviewRoofInteriorActor();
		}
	}
}

//BuildingConstruction
void AArchVizController::OnBuildingConstructionPressed()
{
	if (WallGeneratorActor) {
		if(!bBuildingEditorMode){WallGeneratorActor->Destroy(); }
		WallGeneratorActor = nullptr;
	};
	if (IsValid(DoorMeshActor)) {
		DoorMeshActor->Destroy();
		DoorMeshActor = nullptr;
	}
	if (IsValid(InteriorDesignActor)) {
		InteriorDesignActor->Destroy();
		InteriorDesignActor = nullptr;
	}
	if (IsValid(FloorGeneratorActor)) {
		FloorGeneratorActor = nullptr;
	}
	if (IsValid(RoofGeneratorActor)) {
		FloorGeneratorActor = nullptr;
	}

	SelectedWidget = EWidgetSelection::BuildingConstructor;
	SelectedBuildingComponent = EBuildingComponentSelection::None;

	BuildingConstructorWidget->BuildingEditorMode->SetText(FText::FromString("Building Editor Mode"));
	bBuildingEditorMode = false;
	bIsWallProjection = false;
	bIsFloorProjection = false;
	bProjection = false;
	BuildingConstructorWidget->BuildingOutlineBorder->SetVisibility(ESlateVisibility::Visible);
	BuildingConstructorWidget->BuildingEditorBorder->SetVisibility(ESlateVisibility::Visible);

	SetDefaultBuildingMode();

	if (GetLocalPlayer()) {
		UEnhancedInputLocalPlayerSubsystem* SubSystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		if (SubSystem) {
			SubSystem->ClearAllMappings();
		}
	}
	ConstructionModeHandler();
}

void AArchVizController::AddBuildingComponentsMapping()
{
	UEnhancedInputLocalPlayerSubsystem* SubSystem{};
	if (GetLocalPlayer()) {
		SubSystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		if (SubSystem) {
			SubSystem->ClearAllMappings();
		}
	}

	switch (SelectedBuildingComponent) {
	case EBuildingComponentSelection::Wall: {
		if (SubSystem) { SubSystem->AddMappingContext(WallMapping, 0); }
		break;
	}
	case EBuildingComponentSelection::Door: {
		if (SubSystem) { SubSystem->AddMappingContext(DoorMapping, 0); }
		break;
	}
	case EBuildingComponentSelection::Floor: {
		if (SubSystem) { SubSystem->AddMappingContext(FloorMapping, 0); }
		break;
	}
	case EBuildingComponentSelection::Roof: {
		if (SubSystem) { SubSystem->AddMappingContext(RoofMapping, 0); }
		break;;
	}
	case EBuildingComponentSelection::Editor: {
		if (SubSystem) { SubSystem->AddMappingContext(EditorMapping, 0); }
		break;;
	}
	}
}

void AArchVizController::SetDefaultBuildingMode()
{
	if (WallGeneratorActor) {
		WallGeneratorActor = nullptr;
	}
	if (FloorGeneratorActor) {
		FloorGeneratorActor = nullptr;
	}
	if (RoofGeneratorActor) {
		RoofGeneratorActor = nullptr;
	}

	BuildingConstructorWidget->WallScrollBox->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructorWidget->WallLocationBorder->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructorWidget->WallDestroyer->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructorWidget->SegmentBorder->SetVisibility(ESlateVisibility::Hidden);

	BuildingConstructorWidget->DoorScrollBox->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructorWidget->DoorDestroyer->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructorWidget->Pop_Up->SetVisibility(ESlateVisibility::Hidden);

	BuildingConstructorWidget->FloorDestroyer->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructorWidget->FloorDimensionsBorder->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructorWidget->FloorLocationBorder->SetVisibility(ESlateVisibility::Hidden);

	BuildingConstructorWidget->RoofDestroyer->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructorWidget->RoofDimensionsBorder->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructorWidget->RoofLocationBorder->SetVisibility(ESlateVisibility::Hidden);
}

//Wall Generator
void AArchVizController::OnGenerateWallPressed()
{
	if (IsValid(DoorMeshActor)) {
		DoorMeshActor->Destroy();
		DoorMeshActor = nullptr;
	}
	if (IsValid(WallGeneratorActor)) {
		WallGeneratorActor->Destroy();
		WallGeneratorActor = nullptr;
	}
	SelectedBuildingComponent = EBuildingComponentSelection::Wall;
	AddBuildingComponentsMapping();

	BuildingConstructorWidget->WallScrollBox->SetVisibility(ESlateVisibility::Visible);
	BuildingConstructorWidget->SegmentBorder->SetVisibility(ESlateVisibility::Visible);

	BuildingConstructorWidget->DoorScrollBox->SetVisibility(ESlateVisibility::Hidden);

	if (WallGeneratorActorClassRef) {
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		WallGeneratorActor = GetWorld()->SpawnActor<AWallGenerator>(WallGeneratorActorClassRef, FVector::ZeroVector, FRotator::ZeroRotator, Params);
		int32 Value;
		if (BuildingConstructorWidget) {
			Value = int32(BuildingConstructorWidget->Segments->GetValue());
		}
		if (WallGeneratorActor) {
			WallGeneratorActor->GenerateWall(Value);
		}
	}
}

void AArchVizController::PreviewWallActor() 
{
	FCollisionQueryParams TraceParams(FName(TEXT("LineTrace")), true, WallGeneratorActor);
	FVector CursorWorldLocation;
	FVector CursorWorldDirection;

	DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection);
	if (GetWorld()->LineTraceSingleByChannel(HitResult, CursorWorldLocation, CursorWorldLocation + CursorWorldDirection * 10000, ECC_Visibility, TraceParams))
	{
		FVector ResultedLocation = HitResult.Location;

		BuildingConstructorWidget->Wall_X->SetValue(ResultedLocation.X);
		BuildingConstructorWidget->Wall_Y->SetValue(ResultedLocation.Y);

		if (WallGeneratorActor) {
			WallGeneratorActor->SetActorLocation(ResultedLocation);
			if (WallGeneratorActor->WallStaticMesh) { SnapWallActor(WallGeneratorActor->WallStaticMesh->GetBounds().GetBox().GetSize().Y / 2); }
		}
	}
}

void AArchVizController::SnapWallActor(float SnapValue)
{
	if (IsValid(WallGeneratorActor)) {
		auto CurrentLocation = WallGeneratorActor->GetActorLocation();
		CurrentLocation.X = FMath::RoundToFloat(CurrentLocation.X / SnapValue) * SnapValue;
		CurrentLocation.Y = FMath::RoundToFloat(CurrentLocation.Y / SnapValue) * SnapValue;
		WallGeneratorActor->SetActorLocation(CurrentLocation);
	}
}

void AArchVizController::SetWallConstructionMapping()
{
	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent);

	WallMapping = NewObject<UInputMappingContext>(this);

	GenerateWallAction = NewObject<UInputAction>(this);
	GenerateWallAction->ValueType = EInputActionValueType::Boolean;

	RotateWallAction = NewObject<UInputAction>(this);
	RotateWallAction->ValueType = EInputActionValueType::Boolean;

	WallMapping->MapKey(GenerateWallAction, EKeys::LeftMouseButton);
	WallMapping->MapKey(RotateWallAction, EKeys::R);

	if (EIC) {
		EIC->BindAction(GenerateWallAction, ETriggerEvent::Completed, this, &AArchVizController::GenerateNewWall);
		EIC->BindAction(RotateWallAction, ETriggerEvent::Completed, this, &AArchVizController::RotateWall);
	}
}

void AArchVizController::OnWallSegmentsValueChanged(float SegmentsNo)
{
	if (WallGeneratorActor) {
		WallGeneratorActor->GenerateWall(int32(SegmentsNo));
	}
}

void AArchVizController::GenerateNewWall()
{
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	WallGeneratorActor = GetWorld()->SpawnActor<AWallGenerator>(WallGeneratorActorClassRef, FVector::ZeroVector, FRotator::ZeroRotator, Params);
	if (TempWallMesh) { WallGeneratorActor->WallStaticMesh = TempWallMesh; }
	WallGeneratorActor->GenerateWall(BuildingConstructorWidget->Segments->GetValue());
}

void AArchVizController::RotateWall()
{
	if (WallGeneratorActor) {
		WallGeneratorActor->SetActorRotation(FRotator(0, WallGeneratorActor->GetActorRotation().Yaw + 90, 0));
	}
}

void AArchVizController::SetWallStaticMesh(const FWallData& WallData) {
	if (IsValid(WallGeneratorActor)) {
		TempWallMesh = WallData.Mesh;
		WallGeneratorActor->WallStaticMesh = TempWallMesh;
		WallGeneratorActor->GenerateWall(BuildingConstructorWidget->Segments->GetValue());
	}
}

//Door Generator
void AArchVizController::OnGenerateDoorPressed()
{	bBuildingEditorMode = false;
	if (WallGeneratorActor) {
		WallGeneratorActor->Destroy();
		WallGeneratorActor = nullptr;
	}
	if (IsValid(DoorMeshActor)) {
		DoorMeshActor->Destroy();
		DoorMeshActor = nullptr;
	}

	BuildingConstructorWidget->DoorScrollBox->SetVisibility(ESlateVisibility::Visible);

	BuildingConstructorWidget->WallScrollBox->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructorWidget->SegmentBorder->SetVisibility(ESlateVisibility::Hidden);

	SelectedBuildingComponent = EBuildingComponentSelection::Door;
	AddBuildingComponentsMapping();
}

void AArchVizController::SetDoorConstructionMapping()
{
	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent);

	DoorMapping = NewObject<UInputMappingContext>(this);

	GenerateDoorAction = NewObject<UInputAction>(this);
	GenerateDoorAction->ValueType = EInputActionValueType::Boolean;

	DoorMapping->MapKey(GenerateDoorAction, EKeys::LeftMouseButton);

	if (EIC) {
		EIC->BindAction(GenerateDoorAction, ETriggerEvent::Completed, this, &AArchVizController::GenerateDoor);
	}
}

void AArchVizController::GenerateDoor() {
	if (DoorMeshActor) {
		DoorMeshActor->Destroy();
		DoorMeshActor = nullptr;
	}
	if (!bBuildingEditorMode) {
		GetHitResultUnderCursor(ECC_Visibility,true,HitResult);
	}

	if (HitResult.bBlockingHit) {
		if (Cast<AWallGenerator>(HitResult.GetActor())) {
			AWallGenerator* WallTempActor = Cast<AWallGenerator>(HitResult.GetActor());
			if (Cast<UStaticMeshComponent>(HitResult.GetComponent())) {
				UStaticMeshComponent* DoorComponent = Cast<UStaticMeshComponent>(HitResult.GetComponent());
				if (DoorMesh) {
					UProceduralMeshComponent* CubeComponent = NewObject<UProceduralMeshComponent>(WallTempActor);
					float DoorHeight = DoorMesh->GetBounds().GetBox().GetSize().Z;
					FVector WallDimensions = DoorComponent->GetStaticMesh()->GetBounds().GetBox().GetSize();

					if (WallDimensions.Z == WallTempActor->WallHeight) {
						FVector DoorLocation = DoorComponent->GetRelativeLocation();
						DoorLocation.Z -= (WallDimensions.Z / 2);
						FVector CubeDimensions = FVector(WallDimensions.X, WallDimensions.Y, WallDimensions.Z - DoorHeight);
						FVector CubeLocation = FVector(DoorLocation.X, DoorLocation.Y, DoorLocation.Z + DoorHeight + ((WallDimensions.Z - DoorHeight) / 2));

						DoorComponent->SetRelativeRotation(FRotator(0, 90, 0));
						DoorComponent->SetRelativeLocation(DoorLocation);
						DoorComponent->SetStaticMesh(DoorMesh);

						CubeComponent->SetupAttachment(WallTempActor->GetRootComponent());
						CubeComponent->RegisterComponentWithWorld(GetWorld());

						WallTempActor->GenerateCube(CubeDimensions, FVector::ZeroVector, CubeComponent);

						CubeComponent->SetRelativeLocation(CubeLocation);
						CubeComponent->SetVisibility(true);

						int32 Index = WallTempActor->ComponentsArray.IndexOfByKey(Cast<UStaticMeshComponent>(HitResult.GetComponent()));
						WallTempActor->WallGeneratorActorMap.Add(Index, { DoorMesh , CubeComponent });
						WallTempActor->Indexs.Add(Index);
					}
					else {
						DoorComponent->SetRelativeRotation(FRotator(0, 90, 0));
						DoorComponent->SetStaticMesh(DoorMesh);
					}
				}
			}
			else if (Cast<UProceduralMeshComponent>(HitResult.GetComponent())) {
				UProceduralMeshComponent* ProceduralComponent = Cast<UProceduralMeshComponent>(HitResult.GetComponent());
				int key{};
				for (int i = 0; i < WallTempActor->Indexs.Num(); i++) {
					if (WallTempActor->WallGeneratorActorMap[WallTempActor->Indexs[i]].ProceduralMesh == ProceduralComponent)
					{
						key = WallTempActor->Indexs[i];
						break;
					}
				}
				if (WallTempActor->ComponentsArray[key]) {
					WallTempActor->ComponentsArray[key]->SetRelativeRotation(FRotator(0, 90, 0));
					WallTempActor->ComponentsArray[key]->SetStaticMesh(DoorMesh);
				}
			}
		}
	}
}

void AArchVizController::SetDoor(const FDoorData& DoorData) {
	DoorMesh = DoorData.Mesh;
	if (bBuildingEditorMode) {
		GenerateDoor();
	}
}

void AArchVizController::PreviewDoorActor()
{
	FCollisionQueryParams TraceParams(FName(TEXT("LineTrace")), true, DoorMeshActor);
	FVector CursorWorldLocation;
	FVector CursorWorldDirection;

	DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection);
	if (GetWorld()->LineTraceSingleByChannel(HitResult, CursorWorldLocation, CursorWorldLocation + CursorWorldDirection * 10000, ECC_Visibility, TraceParams)) {
		if (AWallGenerator * WallActor = Cast<AWallGenerator>(HitResult.GetActor())) {
			if (DoorMeshActor) {
				FVector WallDirection = WallActor->GetActorForwardVector(); // 1,0,0
				FVector PreviewDoorLocation{};
				if (DoorMesh) {
					if ((FindAngleBetweenVectors({ 1, 0, 0 }, WallDirection) == 0) || (FindAngleBetweenVectors({ -1, 0, 0 }, WallDirection) == 0)) { // X || -X
						if (HitResult.Location.Y > WallActor->GetActorLocation().Y) {
							PreviewDoorLocation = HitResult.Location + FVector{ 0, DoorMesh->GetBounds().GetBox().GetSize().X / 2, 0 };
						}
						else {
							PreviewDoorLocation = HitResult.Location + FVector{ 0, -DoorMesh->GetBounds().GetBox().GetSize().X / 2, 0 };
						}
					}
					else if ((FindAngleBetweenVectors({ 0, 1, 0 }, WallDirection) == 0) || (FindAngleBetweenVectors({ 0, -1, 0 }, WallDirection) == 0)) {
						if (HitResult.Location.X > WallActor->GetActorLocation().X) {
							PreviewDoorLocation = HitResult.Location + FVector{ DoorMesh->GetBounds().GetBox().GetSize().X / 2, 0, 0 };
						}
						else {
							PreviewDoorLocation = HitResult.Location + FVector{ -DoorMesh->GetBounds().GetBox().GetSize().X / 2, 0, 0 };
						}
					}
					PreviewDoorLocation.Z = WallActor->GetActorLocation().Z;
					DoorMeshActor->SetActorLocation(PreviewDoorLocation);
				}
			}
			else {
				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				DoorMeshActor = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), HitResult.Location, WallActor->GetActorRotation() + FRotator(0, 90, 0), SpawnParams);
				DoorMeshActor->SetMobility(EComponentMobility::Movable);
				DoorMeshActor->GetStaticMeshComponent()->SetStaticMesh(DoorMesh);
			}
		}
		else {
			if (DoorMeshActor) {
				DoorMeshActor->Destroy();
				DoorMeshActor = nullptr;
			}
		}
	}
}

//Floor Generator
void AArchVizController::OnGenerateFloorPressed()
{
	if (WallGeneratorActor) {
		WallGeneratorActor->Destroy();
		WallGeneratorActor = nullptr;
	}
	if (IsValid(DoorMeshActor)) {
		DoorMeshActor->Destroy();
		DoorMeshActor = nullptr;
	}

	BuildingConstructorWidget->WallScrollBox->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructorWidget->SegmentBorder->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructorWidget->DoorScrollBox->SetVisibility(ESlateVisibility::Hidden);

	SelectedBuildingComponent = EBuildingComponentSelection::Floor;
	AddBuildingComponentsMapping();
}

void AArchVizController::SetFloorConstructionMapping()
{
	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent);

	FloorMapping = NewObject<UInputMappingContext>(this);

	GenerateFloorAction = NewObject<UInputAction>(this);
	GenerateFloorAction->ValueType = EInputActionValueType::Boolean;

	FloorMapping->MapKey(GenerateFloorAction, EKeys::LeftMouseButton);
	if (EIC) {
		EIC->BindAction(GenerateFloorAction, ETriggerEvent::Triggered, this, &AArchVizController::GenerateFloor);
		EIC->BindAction(GenerateFloorAction, ETriggerEvent::Completed, this, &AArchVizController::CompletedFloorGeneration);
	}
}

void AArchVizController::GenerateFloor() {

	GetHitResultUnderCursor(ECC_Visibility, true, HitResult);

	if (!FloorGeneratorActor) {
		FloorStartLocation = HitResult.Location;

		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		if (FloorGeneratorActorClassRef) {
			FloorGeneratorActor = GetWorld()->SpawnActor<AFloorGenerator>(FloorGeneratorActorClassRef, FloorStartLocation, FRotator::ZeroRotator, Params);
		}
	}
	else {
		FVector CurrentLocation = HitResult.Location;
		if (FloorGeneratorActor) {
			float length = FMath::Abs(CurrentLocation.X - FloorStartLocation.X);
			float width = FMath::Abs(CurrentLocation.Y - FloorStartLocation.Y);
			float height = 5;

			FloorGeneratorActor->FloorDimensions = FVector(length, width, height);

			FloorGeneratorActor->SetActorLocation((FloorStartLocation + CurrentLocation) / 2);
			FloorGeneratorActor->GenerateFloor(FloorGeneratorActor->FloorDimensions);
			SnapFloorActor(12.5);
		}
	}
}

void AArchVizController::PreviewFloorActor()
{
	FCollisionQueryParams TraceParams(FName(TEXT("LineTrace")), true, FloorGeneratorActor);
	FVector CursorWorldLocation;
	FVector CursorWorldDirection;

	DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection);
	if (GetWorld()->LineTraceSingleByChannel(HitResult, CursorWorldLocation, CursorWorldLocation + CursorWorldDirection * 10000, ECC_Visibility, TraceParams))
	{
		FVector ResultedLocation = HitResult.Location;
		ResultedLocation.Z = 0;

		BuildingConstructorWidget->Floor_X->SetValue(ResultedLocation.X);
		BuildingConstructorWidget->Floor_Y->SetValue(ResultedLocation.Y);

		if (FloorGeneratorActor) {
			FloorGeneratorActor->SetActorLocation(ResultedLocation);
			SnapFloorActor(12.5);
		}
	}
}

void AArchVizController::SnapFloorActor(float SnapValue) {
	if (IsValid(FloorGeneratorActor)) {
		auto CurrentLocation = FloorGeneratorActor->GetActorLocation();
		CurrentLocation.X = FMath::RoundToFloat(CurrentLocation.X / SnapValue) * SnapValue;
		CurrentLocation.Y = FMath::RoundToFloat(CurrentLocation.Y / SnapValue) * SnapValue;
		FloorGeneratorActor->SetActorLocation(CurrentLocation);
	}
}

void AArchVizController::CompletedFloorGeneration()
{
	FloorGeneratorActor = nullptr;
}

//Roof Generator
void AArchVizController::OnGenerateRoofPressed()
{
	if (WallGeneratorActor) {
		WallGeneratorActor->Destroy();
		WallGeneratorActor = nullptr;
	}
	if (IsValid(DoorMeshActor)) {
		DoorMeshActor->Destroy();
		DoorMeshActor = nullptr;
	}
	BuildingConstructorWidget->WallScrollBox->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructorWidget->DoorScrollBox->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructorWidget->SegmentBorder->SetVisibility(ESlateVisibility::Hidden);

	SelectedBuildingComponent = EBuildingComponentSelection::Roof;
	AddBuildingComponentsMapping();
}

void AArchVizController::GenerateRoof()
{
	GetHitResultUnderCursor(ECC_Visibility, true, HitResult);

	FVector Location = HitResult.Location;
	Location.Z += 100;
	FCollisionQueryParams TraceParams(FName(TEXT("LineTrace")), true);

	int8 WallCount{};
	FVector WallLocationX{};
	FVector WallLocationY{};
	FVector WallLocation_X{};
	FVector WallLocation_Y{};
	AWallGenerator* TempWallActorX{};
	AWallGenerator* TempWallActor_X{};
	AWallGenerator* TempWallActorY{};
	AWallGenerator* TempWallActor_Y{};

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Location, Location + (FVector(1, 0, 0) * 10000), ECC_Visibility, TraceParams)) {
		if(Cast<AWallGenerator>(HitResult.GetActor())) {
			WallCount++;
			WallLocationX = HitResult.Location;
			TempWallActorX = Cast<AWallGenerator>(HitResult.GetActor());

		}
	}
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Location, Location + (FVector(-1, 0, 0) * 10000), ECC_Visibility, TraceParams)) {
		if(Cast<AWallGenerator>(HitResult.GetActor())) {
			WallCount++;
			WallLocation_X = HitResult.Location;
			TempWallActor_X = Cast<AWallGenerator>(HitResult.GetActor());
		}
	}
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Location, Location + (FVector(0, 1, 0) * 10000), ECC_Visibility, TraceParams)) {
		if(Cast<AWallGenerator>(HitResult.GetActor())) {
			WallCount++;
			WallLocationY = HitResult.Location;
			TempWallActorY = Cast<AWallGenerator>(HitResult.GetActor());
		}
	}
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Location, Location + (FVector(0, -1, 0) * 10000), ECC_Visibility, TraceParams)) {
		if(Cast<AWallGenerator>(HitResult.GetActor())) {
			WallCount++;
			WallLocation_Y = HitResult.Location;
			TempWallActor_Y = Cast<AWallGenerator>(HitResult.GetActor());
		}
	}

	if (WallCount == 4) {
		if ((TempWallActorX->WallHeight == TempWallActor_X->WallHeight) && (TempWallActorX->WallHeight == TempWallActorY->WallHeight) && TempWallActorX->WallHeight == TempWallActor_Y->WallHeight) {
			float RoofLength = FMath::Abs(WallLocationX.X - WallLocation_X.X) + (TempWallActorX->WallStaticMesh->GetBounds().GetBox().GetSize().Y * 2);
			float RoofWidth = FMath::Abs(WallLocationY.Y - WallLocation_Y.Y) + (TempWallActorX->WallStaticMesh->GetBounds().GetBox().GetSize().Y * 2);
			float RoofHeight = 25;

			float RoofLocationZ = TempWallActorX->GetActorLocation().Z + TempWallActorX->WallHeight;
			float TempLocationZ = RoofLocationZ;

			int8 LoopCount = CheckIfMultipleWallActorInZ(RoofLocationZ, TempLocationZ, Location);

			FVector RoofLocation{ (WallLocationX.X + WallLocation_X.X) / 2, (WallLocationY.Y + WallLocation_Y.Y) / 2, RoofLocationZ };

			if (LoopCount != -1) {
				FActorSpawnParameters Params;
				Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				if (RoofGeneratorActorClassRef) {
					RoofGeneratorActor = GetWorld()->SpawnActor<ARoofGenerator>(RoofGeneratorActorClassRef, RoofLocation, FRotator::ZeroRotator, Params);
					RoofGeneratorActor->RoofDimensions = FVector(RoofLength, RoofWidth, RoofHeight);
					RoofGeneratorActor->GenerateRoof({ RoofLength, RoofWidth, RoofHeight });
				}
			}
		}
		else {
			/*BuildingConstructionWidget->RoofErrorMsgTxt->SetText(FText::FromString("The height of all walls in a house must be same to build a roof."));
			BuildingConstructionWidget->PlayAnimation(BuildingConstructionWidget->RoofErrorMsgAnim);*/
		}
	}
	else {
		/*BuildingConstructionWidget->RoofErrorMsgTxt->SetText(FText::FromString("There must be 4 walls in a house to build a roof."));
		BuildingConstructionWidget->PlayAnimation(BuildingConstructionWidget->RoofErrorMsgAnim);*/
	} 
	RoofGeneratorActor = nullptr;
}

void AArchVizController::GenerateRoofOnRightClick()
{
	GetHitResultUnderCursor(ECC_Visibility, true, HitResult);

	if (!RoofGeneratorActor) {
		if (AWallGenerator * WallActor = Cast<AWallGenerator>(HitResult.GetActor())) {
			WallActor = Cast<AWallGenerator>(HitResult.GetActor());
			RoofInZ = WallActor->GetActorLocation().Z + WallActor->WallHeight;

			RoofStartLocation = HitResult.Location;
			RoofStartLocation.Z = RoofInZ;

			FActorSpawnParameters Params;
			Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			if (RoofGeneratorActorClassRef) {
				RoofGeneratorActor = GetWorld()->SpawnActor<ARoofGenerator>(RoofGeneratorActorClassRef, RoofStartLocation, FRotator::ZeroRotator, Params);
			}
		}
	}
	else {
		FVector CurrentLocation = HitResult.Location;
		CurrentLocation.Z = RoofInZ;
		if (RoofGeneratorActor) {
			float length = FMath::Abs(CurrentLocation.X - RoofStartLocation.X);
			float width = FMath::Abs(CurrentLocation.Y - RoofStartLocation.Y);
			float height = 25;

			RoofGeneratorActor->RoofDimensions = FVector(length, width, height);

			RoofGeneratorActor->SetActorLocation((RoofStartLocation + CurrentLocation) / 2);
			RoofGeneratorActor->GenerateRoof(RoofGeneratorActor->RoofDimensions);
			SnapRoofActor(12.5);
		}
	}
}

void AArchVizController::GenerateRoofOnRightClickCompleted()
{
	RoofGeneratorActor = nullptr;
}

void AArchVizController::SetRoofConstructionMapping()
{
	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent);

	RoofMapping = NewObject<UInputMappingContext>(this);

	GenerateRoofAction = NewObject<UInputAction>(this);
	GenerateRoofAction->ValueType = EInputActionValueType::Boolean;

	GenerateRoofWithRightClickAction = NewObject<UInputAction>(this);
	GenerateRoofWithRightClickAction->ValueType = EInputActionValueType::Boolean;

	RoofMapping->MapKey(GenerateRoofAction, EKeys::LeftMouseButton);
	RoofMapping->MapKey(GenerateRoofWithRightClickAction, EKeys::RightMouseButton);

	if (EIC) {
		EIC->BindAction(GenerateRoofAction, ETriggerEvent::Completed, this, &AArchVizController::GenerateRoof);
		EIC->BindAction(GenerateRoofWithRightClickAction, ETriggerEvent::Triggered, this, &AArchVizController::GenerateRoofOnRightClick);
		EIC->BindAction(GenerateRoofWithRightClickAction, ETriggerEvent::Completed, this, &AArchVizController::GenerateRoofOnRightClickCompleted);
	}
}

int8 AArchVizController::CheckIfMultipleWallActorInZ(float& RoofLocationZ, float& TempLocationZ , FVector& Location)
{
	int8 Count{};
	int8 LoopCount{};
	AWallGenerator* TempWallActorX{};
	AWallGenerator* TempWallActor_X{};
	AWallGenerator* TempWallActorY{};
	AWallGenerator* TempWallActor_Y{};
	FCollisionQueryParams TraceParams(FName(TEXT("LineTrace")), true);
	while (true) {
		Location.Z = RoofLocationZ + 50;
		if (GetWorld()->LineTraceSingleByChannel(HitResult, Location, Location + (FVector(0, -1, 0) * 10000), ECC_Visibility, TraceParams)) {
			if (Cast<AWallGenerator>(HitResult.GetActor())) {
				TempWallActor_Y = Cast<AWallGenerator>(HitResult.GetActor());
				Count++;
			}
		}
		if (GetWorld()->LineTraceSingleByChannel(HitResult, Location, Location + (FVector(0, 1, 0) * 10000), ECC_Visibility, TraceParams)) {
			if (Cast<AWallGenerator>(HitResult.GetActor())) {
				TempWallActorY = Cast<AWallGenerator>(HitResult.GetActor());
				Count++;
			}
		}
		if (GetWorld()->LineTraceSingleByChannel(HitResult, Location, Location + (FVector(1, 0, 0) * 10000), ECC_Visibility, TraceParams)) {
			if (Cast<AWallGenerator>(HitResult.GetActor())) {
				TempWallActorX = Cast<AWallGenerator>(HitResult.GetActor());
				Count++;
			}
		}
		if (GetWorld()->LineTraceSingleByChannel(HitResult, Location, Location + (FVector(-1, 0, 0) * 10000), ECC_Visibility, TraceParams)) {
			if (Cast<AWallGenerator>(HitResult.GetActor())) {
				TempWallActor_X = Cast<AWallGenerator>(HitResult.GetActor());
				Count++;
			}
		}
		if (Count >= 3) {
			if ((TempWallActorX->WallHeight == TempWallActor_X->WallHeight) && (TempWallActorX->WallHeight == TempWallActorY->WallHeight) && TempWallActorX->WallHeight == TempWallActor_Y->WallHeight) {
				LoopCount++;
				RoofLocationZ = RoofLocationZ + TempLocationZ;
				Count = 0;
			}
			else{
				/*BuildingConstructionWidget->RoofErrorMsgTxt->SetText(FText::FromString("The height of all walls in a house must be same to build a roof."));
			BuildingConstructionWidget->PlayAnimation(BuildingConstructionWidget->RoofErrorMsgAnim);*/
			}
		}
		else {
			if (Count > 0) {
				LoopCount = -1;
			}
			break;
		}
	}

	return LoopCount;
}

void AArchVizController::SnapRoofActor(float SnapValue) {
	if (IsValid(RoofGeneratorActor)) {
		auto CurrentLocation = RoofGeneratorActor->GetActorLocation();
		CurrentLocation.X = FMath::RoundToFloat(CurrentLocation.X / SnapValue) * SnapValue;
		CurrentLocation.Y = FMath::RoundToFloat(CurrentLocation.Y / SnapValue) * SnapValue;
		RoofGeneratorActor->SetActorLocation(CurrentLocation);
	}
}

//BuildingEditor Mode

void AArchVizController::OnBuildingEditorModePressed()
{
	if (IsValid(WallGeneratorActor)) {
		WallGeneratorActor->Destroy();
		WallGeneratorActor = nullptr;
	}
	if (IsValid(DoorMeshActor)) {
		DoorMeshActor->Destroy();
		DoorMeshActor = nullptr;
	}
	if (!bBuildingEditorMode) {
		BuildingConstructorWidget->BuildingEditorMode->SetText(FText::FromString("Exit Editor Mode"));
		if (WallGeneratorActor) {
			if (!bBuildingEditorMode) { WallGeneratorActor->Destroy(); }
			WallGeneratorActor = nullptr;
		}
		BuildingConstructorWidget->Pop_Up->SetVisibility(ESlateVisibility::Visible);

		BuildingConstructorWidget->BuildingOutlineBorder->SetVisibility(ESlateVisibility::Hidden);
		BuildingConstructorWidget->WallScrollBox->SetVisibility(ESlateVisibility::Hidden);
		BuildingConstructorWidget->DoorScrollBox->SetVisibility(ESlateVisibility::Hidden);
		BuildingConstructorWidget->SegmentBorder->SetVisibility(ESlateVisibility::Hidden);
		BuildingConstructorWidget->FloorDimensionsBorder->SetVisibility(ESlateVisibility::Hidden);

		SelectedBuildingComponent = EBuildingComponentSelection::Editor;
		AddBuildingComponentsMapping();
		bBuildingEditorMode = true;
	}
	else {
		BuildingConstructorWidget->BuildingEditorMode->SetText(FText::FromString("Building Editor Mode"));
		bBuildingEditorMode = false;
		bIsWallProjection = false;
		bIsFloorProjection = false;
		bProjection = false;
		BuildingConstructorWidget->BuildingOutlineBorder->SetVisibility(ESlateVisibility::Visible);

		SetDefaultBuildingMode();

		UEnhancedInputLocalPlayerSubsystem* SubSystem{};
		if (GetLocalPlayer()) {
			SubSystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
			if (SubSystem) {
				SubSystem->ClearAllMappings();
			}
		}
	}
}

void AArchVizController::SetEditorModeMapping()
{
	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent);

	EditorMapping = NewObject<UInputMappingContext>(this);

	SelectAssetAction = NewObject<UInputAction>(this);
	SelectAssetAction->ValueType = EInputActionValueType::Boolean;

	EditorMapping->MapKey(SelectAssetAction, EKeys::LeftMouseButton);

	if (EIC) {
		EIC->BindAction(SelectAssetAction, ETriggerEvent::Completed, this, &AArchVizController::OnAssetSelection);
	}
}

void AArchVizController::OnAssetSelection()
{
	if (!bProjection) {
		GetHitResultUnderCursor(ECC_Visibility, true, HitResult);

		if (Cast<AWallGenerator>(HitResult.GetActor())) {
			WallGeneratorActor = Cast<AWallGenerator>(HitResult.GetActor());
			UStaticMeshComponent* TempComponent{};
			if (Cast<UStaticMeshComponent>(HitResult.GetComponent())) {
				TempComponent = Cast<UStaticMeshComponent>(HitResult.GetComponent());
			}
			float HeightOfMesh{};
			if (TempComponent) {
				HeightOfMesh = TempComponent->GetStaticMesh()->GetBounds().GetBox().GetSize().Z;
			}

			if (Cast<UProceduralMeshComponent>(HitResult.GetComponent())) {
				EditWall();
			}
			else if (HeightOfMesh != WallGeneratorActor->WallHeight) {
				EditDoor();
			}
			else {
				EditWall();
			}

		}
		else if (Cast<AFloorGenerator>(HitResult.GetActor())) {
			FloorGeneratorActor = Cast<AFloorGenerator>(HitResult.GetActor());
			EditFloor();
		}
		else if (Cast<ARoofGenerator>(HitResult.GetActor())) {
			RoofGeneratorActor = Cast<ARoofGenerator>(HitResult.GetActor());
			EditRoof();
		}
		else {
			SetDefaultBuildingMode();
		}
	}
	else {
		bProjection = false;
		bIsWallProjection = false;
		bIsFloorProjection = false;
		SetDefaultBuildingMode();
	}

}

			//Wall Edit Logic
void AArchVizController::EditWall()
{
	BuildingConstructorWidget->SegmentBorder->SetVisibility(ESlateVisibility::Visible);
	BuildingConstructorWidget->WallDestroyer->SetVisibility(ESlateVisibility::Visible);
	BuildingConstructorWidget->WallLocationBorder->SetVisibility(ESlateVisibility::Visible);

	BuildingConstructorWidget->DoorScrollBox->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructorWidget->DoorDestroyer->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructorWidget->FloorDestroyer->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructorWidget->FloorLocationBorder->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructorWidget->RoofDimensionsBorder->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructorWidget->RoofLocationBorder->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructorWidget->RoofDestroyer->SetVisibility(ESlateVisibility::Hidden);


	if (WallGeneratorActor) {
		BuildingConstructorWidget->Segments->SetValue(WallGeneratorActor->ComponentsArray.Num());
		FVector CurrentLocation = WallGeneratorActor->GetActorLocation();
		BuildingConstructorWidget->Wall_X->SetValue(CurrentLocation.X);
		BuildingConstructorWidget->Wall_Y->SetValue(CurrentLocation.Y);
	}
}

void AArchVizController::OnWallLocationXChanged(float Value)
{
	if (WallGeneratorActor) {
		FVector Location = WallGeneratorActor->GetActorLocation();
		WallGeneratorActor->SetActorLocation(FVector(Value, Location.Y, Location.Z));
	}
}

void AArchVizController::OnWallLocationYChanged(float Value)
{
	if (WallGeneratorActor) {
		FVector Location = WallGeneratorActor->GetActorLocation();
		WallGeneratorActor->SetActorLocation(FVector(Location.X, Value, Location.Z));
	}
}

void AArchVizController::OnWallDestroyerPressed()
{
	if (WallGeneratorActor) {
		WallGeneratorActor->Destroy();
		WallGeneratorActor = nullptr;
	}
	bIsWallProjection = false;
	bProjection = false;
	BuildingConstructorWidget->WallDestroyer->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructorWidget->SegmentBorder->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructorWidget->WallLocationBorder->SetVisibility(ESlateVisibility::Hidden);
}

void AArchVizController::ControlWallGeneratorActorOnTick() {
	if (WallGeneratorActor) {
		if (!bIsWallProjection || !bProjection) {
			bIsWallProjection = true;
			bProjection = true;
			BuildingConstructorWidget->PlayAnimation(BuildingConstructorWidget->PopUpAnimation);
		}
	}
}

			//Door Edit Logic
void AArchVizController::EditDoor()
{
	bIsDoorDestroyed = false;
	BuildingConstructorWidget->DoorScrollBox->SetVisibility(ESlateVisibility::Visible);
	BuildingConstructorWidget->DoorDestroyer->SetVisibility(ESlateVisibility::Visible);

	BuildingConstructorWidget->SegmentBorder->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructorWidget->WallDestroyer->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructorWidget->WallLocationBorder->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructorWidget->FloorDestroyer->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructorWidget->FloorLocationBorder->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructorWidget->RoofDimensionsBorder->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructorWidget->RoofLocationBorder->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructorWidget->RoofDestroyer->SetVisibility(ESlateVisibility::Hidden);
}

void AArchVizController::OnDoorDestroyerPressed()
{
	if (!bIsDoorDestroyed) {
		if (WallGeneratorActor) {
			if (Cast<UStaticMeshComponent>(HitResult.GetComponent())) {
				UStaticMeshComponent* Component = Cast<UStaticMeshComponent>(HitResult.GetComponent());
				int32 Key = WallGeneratorActor->ComponentsArray.IndexOfByKey(Component);

				if (WallGeneratorActor->WallGeneratorActorMap.Find(Key)->ProceduralMesh) {
					WallGeneratorActor->WallGeneratorActorMap.Find(Key)->ProceduralMesh->DestroyComponent();
					WallGeneratorActor->WallGeneratorActorMap.Find(Key)->ProceduralMesh = nullptr;
				}

				WallGeneratorActor->WallGeneratorActorMap.FindAndRemoveChecked(Key);
				FRotator FirstRotation = Component->GetRelativeRotation();
				Component->SetRelativeRotation(FRotator(0, 0, 0));
				Component->SetStaticMesh(WallGeneratorActor->WallStaticMesh);


				FVector Location = Component->GetRelativeLocation();
				Component->SetRelativeLocation(FVector(Location.X, Location.Y, Location.Z + (WallGeneratorActor->WallHeight / 2)));
			}
		}
	}
	BuildingConstructorWidget->DoorDestroyer->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructorWidget->DoorScrollBox->SetVisibility(ESlateVisibility::Hidden);
	bIsDoorDestroyed = true;
}

			//Floor Edit Logic
void AArchVizController::EditFloor()
{
	BuildingConstructorWidget->FloorDimensionsBorder->SetVisibility(ESlateVisibility::Visible);
	BuildingConstructorWidget->FloorLocationBorder->SetVisibility(ESlateVisibility::Visible);
	BuildingConstructorWidget->FloorDestroyer->SetVisibility(ESlateVisibility::Visible);

	BuildingConstructorWidget->DoorScrollBox->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructorWidget->SegmentBorder->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructorWidget->WallDestroyer->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructorWidget->DoorDestroyer->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructorWidget->WallLocationBorder->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructorWidget->RoofDimensionsBorder->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructorWidget->RoofLocationBorder->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructorWidget->RoofDestroyer->SetVisibility(ESlateVisibility::Hidden);

	if (FloorGeneratorActor) {
		BuildingConstructorWidget->Floor_X->SetValue(FloorGeneratorActor->GetActorLocation().X);
		BuildingConstructorWidget->Floor_Y->SetValue(FloorGeneratorActor->GetActorLocation().Y);
		BuildingConstructorWidget->FloorLength->SetValue(FloorGeneratorActor->FloorDimensions.X);
		BuildingConstructorWidget->FloorWidth->SetValue(FloorGeneratorActor->FloorDimensions.Y);
		BuildingConstructorWidget->FloorHeight->SetValue(FloorGeneratorActor->FloorDimensions.Z);
	}
}

void AArchVizController::OnFloorLocationXChanged(float Value)
{
	if (FloorGeneratorActor) {
		FVector Location = FloorGeneratorActor->GetActorLocation();
		FloorGeneratorActor->SetActorLocation(FVector(Value, Location.Y, Location.Z));
	}
}

void AArchVizController::OnFloorLocationYChanged(float Value)
{
	if (FloorGeneratorActor) {
		FVector Location = FloorGeneratorActor->GetActorLocation();
		FloorGeneratorActor->SetActorLocation(FVector(Location.X, Value, Location.Z));
	}
}

void AArchVizController::OnFloorDestroyPressed()
{
	if (FloorGeneratorActor) {
		FloorGeneratorActor->Destroy();
		FloorGeneratorActor = nullptr;
	}
	bIsFloorProjection = false;
	bProjection = false;

	BuildingConstructorWidget->FloorDestroyer->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructorWidget->FloorLocationBorder->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructorWidget->FloorDimensionsBorder->SetVisibility(ESlateVisibility::Hidden);
}

void AArchVizController::OnFloorLengthChanged(float FloorLength)
{
	if (FloorGeneratorActor) {
		FloorGeneratorActor->FloorComponent->ClearAllMeshSections();

		FVector FloorDimensions{};
		if (BuildingConstructorWidget) {
			float length = FloorLength;
			float Width = BuildingConstructorWidget->FloorWidth->GetValue();
			float Height = BuildingConstructorWidget->FloorHeight->GetValue();

			FloorDimensions = FVector(length, Width, Height);
		}
		FloorGeneratorActor->FloorDimensions.X = FloorLength;
		FloorGeneratorActor->GenerateFloor(FloorDimensions);
	}
}

void AArchVizController::OnFloorWidthChanged(float FloorWidth)
{
	if (FloorGeneratorActor) {
		FloorGeneratorActor->FloorComponent->ClearAllMeshSections();

		FVector FloorDimensions{};
		if (BuildingConstructorWidget) {
			float length = BuildingConstructorWidget->FloorLength->GetValue();
			float Width = FloorWidth;
			float Height = BuildingConstructorWidget->FloorHeight->GetValue();

			FloorDimensions = FVector(length, Width, Height);
		}
		FloorGeneratorActor->FloorDimensions.Y = FloorWidth;
		FloorGeneratorActor->GenerateFloor(FloorDimensions);
	}
}

void AArchVizController::OnFloorHeightChanged(float FloorHeight)
{
	if (FloorGeneratorActor) {
		FloorGeneratorActor->FloorComponent->ClearAllMeshSections();

		FVector FloorDimensions{};
		if (BuildingConstructorWidget) {
			float length = BuildingConstructorWidget->FloorLength->GetValue();
			float Width = BuildingConstructorWidget->FloorWidth->GetValue();
			float Height = FloorHeight;

			FloorDimensions = FVector(length, Width, Height);
		}
		FloorGeneratorActor->FloorDimensions.Z = FloorHeight;
		FloorGeneratorActor->GenerateFloor(FloorDimensions);
	}
}

void AArchVizController::ControlFloorGeneratorActorOnTick()
{
	if (FloorGeneratorActor) {
		if (!bIsFloorProjection || !bProjection) {
			bIsFloorProjection = true;
			bProjection = true;
			BuildingConstructorWidget->PlayAnimation(BuildingConstructorWidget->PopUpAnimation);
		}
	}
}

				//Roof Edit Logic
void AArchVizController::EditRoof()
{
	BuildingConstructorWidget->RoofDimensionsBorder->SetVisibility(ESlateVisibility::Visible);
	BuildingConstructorWidget->RoofLocationBorder->SetVisibility(ESlateVisibility::Visible);
	BuildingConstructorWidget->RoofDestroyer->SetVisibility(ESlateVisibility::Visible);

	BuildingConstructorWidget->FloorDimensionsBorder->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructorWidget->FloorLocationBorder->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructorWidget->FloorDestroyer->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructorWidget->DoorScrollBox->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructorWidget->SegmentBorder->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructorWidget->WallDestroyer->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructorWidget->DoorDestroyer->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructorWidget->WallLocationBorder->SetVisibility(ESlateVisibility::Hidden);

	if (RoofGeneratorActor) {
		BuildingConstructorWidget->Roof_X->SetValue(RoofGeneratorActor->GetActorLocation().X);
		BuildingConstructorWidget->Roof_Y->SetValue(RoofGeneratorActor->GetActorLocation().Y);
		BuildingConstructorWidget->Roof_Z->SetValue(RoofGeneratorActor->GetActorLocation().Z);
		BuildingConstructorWidget->RoofLength->SetValue(RoofGeneratorActor->RoofDimensions.X);
		BuildingConstructorWidget->RoofWidth->SetValue(RoofGeneratorActor->RoofDimensions.Y);
		BuildingConstructorWidget->RoofHeight->SetValue(RoofGeneratorActor->RoofDimensions.Z);
	}
}

void AArchVizController::OnRoofLocationXChanged(float Value)
{
	if (RoofGeneratorActor) {
		FVector Location = RoofGeneratorActor->GetActorLocation();
		RoofGeneratorActor->SetActorLocation(FVector(Value, Location.Y, Location.Z));
	}
}

void AArchVizController::OnRoofLocationYChanged(float Value)
{
	if (RoofGeneratorActor) {
		FVector Location = RoofGeneratorActor->GetActorLocation();
		RoofGeneratorActor->SetActorLocation(FVector(Location.X, Value, Location.Z));
	}
}

void AArchVizController::OnRoofLocationZChanged(float Value)
{
	if (RoofGeneratorActor) {
		FVector Location = RoofGeneratorActor->GetActorLocation();
		RoofGeneratorActor->SetActorLocation(FVector(Location.X, Location.Y, Value));
	}
}

void AArchVizController::OnRoofDestroyPressed()
{
	if (RoofGeneratorActor) {
		RoofGeneratorActor->Destroy();
		RoofGeneratorActor = nullptr;
	}
	bProjection = false;

	BuildingConstructorWidget->RoofDestroyer->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructorWidget->RoofLocationBorder->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructorWidget->RoofDimensionsBorder->SetVisibility(ESlateVisibility::Hidden);
}

void AArchVizController::OnRoofLengthChanged(float RoofLength)
{
	if (RoofGeneratorActor) {
		RoofGeneratorActor->RoofComponent->ClearAllMeshSections();

		FVector RoofDimensions{};
		if (BuildingConstructorWidget) {
			float length = RoofLength;
			float Width = BuildingConstructorWidget->RoofWidth->GetValue();
			float Height = BuildingConstructorWidget->RoofHeight->GetValue();

			RoofDimensions = FVector(length, Width, Height);
		}
		RoofGeneratorActor->RoofDimensions.X = RoofLength;
		RoofGeneratorActor->GenerateRoof(RoofDimensions);
	}
}

void AArchVizController::OnRoofWidthChanged(float RoofWidth)
{
	if (RoofGeneratorActor) {
		RoofGeneratorActor->RoofComponent->ClearAllMeshSections();

		FVector RoofDimensions{};
		if (BuildingConstructorWidget) {
			float length = BuildingConstructorWidget->RoofLength->GetValue();
			float Width = RoofWidth;
			float Height = BuildingConstructorWidget->RoofHeight->GetValue();

			RoofDimensions = FVector(length, Width, Height);
		}
		RoofGeneratorActor->RoofDimensions.Y = RoofWidth;
		RoofGeneratorActor->GenerateRoof(RoofDimensions);
	}
}

void AArchVizController::OnRoofHeightChanged(float RoofHeight)
{
	if (RoofGeneratorActor) {
		RoofGeneratorActor->RoofComponent->ClearAllMeshSections();

		FVector RoofDimensions{};
		if (BuildingConstructorWidget) {
			float length = BuildingConstructorWidget->RoofLength->GetValue();
			float Width = BuildingConstructorWidget->RoofWidth->GetValue();
			float Height = RoofHeight;

			RoofDimensions = FVector(length, Width, Height);
		}
		RoofGeneratorActor->RoofDimensions.Z = RoofHeight;
		RoofGeneratorActor->GenerateRoof(RoofDimensions);
	}
}


//ConstructionMode Handler
void AArchVizController::ConstructionModeHandler()
{
	switch (SelectedWidget) {
	case EWidgetSelection::RoadConstructor: {
		if(RoadWidgetClassRef) {
			if(RoadWidget){
				RoadWidget->AddToViewport();
			}
		}
		if(BuildingConstructorWidget){
			if (BuildingConstructorWidget->IsInViewport()) {
				BuildingConstructorWidget->RemoveFromParent();
			}
		}
		if(MaterialManagmentWidget){
			if (MaterialManagmentWidget->IsInViewport()) {
				MaterialManagmentWidget->RemoveFromParent();
			}
		}
		if(InteriorDesignWidget){
			if (InteriorDesignWidget->IsInViewport()) {
				InteriorDesignWidget->RemoveFromParent();
			}
		}
		break;
	}
	case EWidgetSelection::BuildingConstructor: {
		if(BuildingConstructorWidgetClassRef) {
			if(BuildingConstructorWidget){
				BuildingConstructorWidget->AddToViewport();
			}
		}
		if (RoadWidget) {
			if (RoadWidget->IsInViewport()) {
				RoadWidget->RemoveFromParent();
			}
		}
		if (MaterialManagmentWidget) {
			if (MaterialManagmentWidget->IsInViewport()) {
				MaterialManagmentWidget->RemoveFromParent();
			}
		}
		if (InteriorDesignWidget) {
			if (InteriorDesignWidget->IsInViewport()) {
				InteriorDesignWidget->RemoveFromParent();
			}
		}
		break;
	}
	case EWidgetSelection::MaterialManagment: {
		if(MaterialManagmentWidgetClassRef) {
				if(MaterialManagmentWidget){	
				MaterialManagmentWidget->AddToViewport();
				}
		}
		if (RoadWidget) {
			if (RoadWidget->IsInViewport()) {
				RoadWidget->RemoveFromParent();
			}
		}
		if (BuildingConstructorWidget) {
			if (BuildingConstructorWidget->IsInViewport()) {
				BuildingConstructorWidget->RemoveFromParent();
			}
		}
		if (InteriorDesignWidget) {
			if (InteriorDesignWidget->IsInViewport()) {
				InteriorDesignWidget->RemoveFromParent();
			}
		}
		break;
	}
	case EWidgetSelection::InteriorDesign: {
		if (InteriorDesignWidgetClassRef) { 
			if(InteriorDesignWidget){
				InteriorDesignWidget->AddToViewport();
			}
		}
		if (RoadWidget) {
			if (RoadWidget->IsInViewport()) {
				RoadWidget->RemoveFromParent();
			}
		}
		if (BuildingConstructorWidget) {
			if (BuildingConstructorWidget->IsInViewport()) {
				BuildingConstructorWidget->RemoveFromParent();
			}
		}
		if (MaterialManagmentWidget) {
			if (MaterialManagmentWidget->IsInViewport()) {
				MaterialManagmentWidget->RemoveFromParent();
			}
		}
		break;
	}
	}
}

//Road Constructor
void AArchVizController::GetRoadLocationOnClick()
{
	if (!bRoadEditorMode) {
		FCollisionQueryParams TraceParams(FName(TEXT("LineTrace")), true);
		FVector CursorWorldLocation;
		FVector CursorWorldDirection;

		if (bFirstRoad) {
			if (bGetLocation) {
				DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection);
				if (GetWorld()->LineTraceSingleByChannel(HitResult, CursorWorldLocation, CursorWorldLocation + CursorWorldDirection * 10000, ECC_Visibility, TraceParams))
				{
					RoadStartLocation = HitResult.Location;
				}
				bGetLocation = false;
			}
			else {
				DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection);
				if (GetWorld()->LineTraceSingleByChannel(HitResult, CursorWorldLocation, CursorWorldLocation + CursorWorldDirection * 10000, ECC_Visibility, TraceParams))
				{
					RoadEndLocation = HitResult.Location;
				}
				GenerateRoad();
				bFirstRoad = false;
			}
		}
		else {
			DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection);
			if (GetWorld()->LineTraceSingleByChannel(HitResult, CursorWorldLocation, CursorWorldLocation + CursorWorldDirection * 10000, ECC_Visibility, TraceParams))
			{
				FVector ForwardVectorOfPrevRoad = (RoadEndLocation - RoadStartLocation).GetSafeNormal();
				FVector LeftVectorOfPrevRoad = FVector::CrossProduct(ForwardVectorOfPrevRoad, FVector::UpVector);
				FVector BackVectorOfPrevRoad = (RoadStartLocation - RoadEndLocation).GetSafeNormal();
				FVector RightVectorOfPrevRoad = FVector::CrossProduct(FVector::UpVector, ForwardVectorOfPrevRoad);

				RoadStartLocation = RoadEndLocation;
				RoadEndLocation = HitResult.Location;

				float Distance = FVector::Dist(RoadEndLocation, RoadStartLocation);

				FVector ForwardVectorOfCurrRoad = RoadEndLocation - RoadStartLocation;
				float AngleOfPointWithLeftVecOfPrev = FindAngleBetweenVectors(ForwardVectorOfCurrRoad, LeftVectorOfPrevRoad);
				float AngleOfPointWithBackVecOfPrev = FindAngleBetweenVectors(ForwardVectorOfCurrRoad, BackVectorOfPrevRoad);

				FVector EndPointDir;
				if ((AngleOfPointWithLeftVecOfPrev >= 0 && AngleOfPointWithLeftVecOfPrev < 45) || (AngleOfPointWithBackVecOfPrev <= 90 && AngleOfPointWithLeftVecOfPrev <= 90)) {
					RoadStartLocation += (ForwardVectorOfPrevRoad * 125) + (RightVectorOfPrevRoad * 125);
					EndPointDir = LeftVectorOfPrevRoad;
				}
				else if ((AngleOfPointWithLeftVecOfPrev > 135 && AngleOfPointWithLeftVecOfPrev <= 180) || (AngleOfPointWithBackVecOfPrev <= 90 && AngleOfPointWithLeftVecOfPrev >= 90)) {
					RoadStartLocation += (ForwardVectorOfPrevRoad * 125) + (LeftVectorOfPrevRoad * 125);
					EndPointDir = RightVectorOfPrevRoad;
				}
				else if (AngleOfPointWithLeftVecOfPrev > 45 && AngleOfPointWithLeftVecOfPrev <= 135) {
					EndPointDir = ForwardVectorOfPrevRoad;
				}
				RoadEndLocation = RoadStartLocation + (EndPointDir.GetSafeNormal() * Distance);
			}
			GenerateRoad();
		}
	}
	else {
		GetHitResultUnderCursor(ECC_Visibility, true, HitResult);
		if (IsValid(HitResult.GetActor())) {
			if (Cast<ARoadConstructor>(HitResult.GetActor())) {
				RoadConstructor = Cast<ARoadConstructor>(HitResult.GetActor());
				RoadWidget->WidthValueBox->SetValue(RoadConstructor->GetActorScale3D().Y * RoadDimensions.Y);
				RoadWidget->Location_x->SetValue(RoadConstructor->GetActorLocation().X);
				RoadWidget->Location_Y->SetValue(RoadConstructor->GetActorLocation().Y);
			}
		}
	}
}

void AArchVizController::GenerateRoad()
{
	FVector RoadLocation = (RoadStartLocation + RoadEndLocation) / 2;
	FRotator RoadRotation = UKismetMathLibrary::FindLookAtRotation(RoadStartLocation, RoadEndLocation);

	float length = FVector::Dist(RoadStartLocation, RoadEndLocation);
	float width = 250;
	float height = 10;

	RoadDimensions = FVector(length, width, height);

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if(RoadConstructorClassRef){
		RoadConstructor = GetWorld()->SpawnActor<ARoadConstructor>(RoadConstructorClassRef, RoadLocation, RoadRotation, Params);
	}

	if (RoadConstructor) {
		RoadConstructor->GenerateRoad(RoadDimensions, FVector(0, 0, RoadDimensions.Z / 2));
	}
}

void AArchVizController::GenerateNewRoad()
{
	bFirstRoad = true;
	bGetLocation = true;
}

float AArchVizController::FindAngleBetweenVectors(const FVector& Vec1, const FVector& Vec2)
{
	FVector NormalizedVec1 = Vec1.GetSafeNormal();
	FVector NormalizedVec2 = Vec2.GetSafeNormal();

	float DotProduct = FVector::DotProduct(NormalizedVec1, NormalizedVec2);

	DotProduct = FMath::Clamp(DotProduct, -1.0f, 1.0f);

	float AngleRadians = FMath::Acos(DotProduct);

	float AngleDegrees = FMath::RadiansToDegrees(AngleRadians);

	return AngleDegrees;
}

void AArchVizController::SetRoadConstructionMapping()
{
	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent);

	RoadMapping = NewObject<UInputMappingContext>(this);

	GenerateRoadAction = NewObject<UInputAction>(this);
	GenerateRoadAction->ValueType = EInputActionValueType::Boolean;

	RoadMapping->MapKey(GenerateRoadAction, EKeys::LeftMouseButton);

	if (EIC) {
		EIC->BindAction(GenerateRoadAction, ETriggerEvent::Completed, this, &AArchVizController::GetRoadLocationOnClick);
	}
}

void AArchVizController::OnRoadConstructionPressed()
{
	if (IsValid(WallGeneratorActor)) {
		if (!bBuildingEditorMode) { WallGeneratorActor->Destroy(); }
		WallGeneratorActor = nullptr;
	}
	if (IsValid(DoorMeshActor)) {
		DoorMeshActor->Destroy();
		DoorMeshActor = nullptr;
	}
	if (IsValid(InteriorDesignActor)) {
		InteriorDesignActor->Destroy();
		InteriorDesignActor = nullptr;
	}
	if (IsValid(FloorGeneratorActor)) {
		FloorGeneratorActor = nullptr;
	}
	if (IsValid(RoofGeneratorActor)) {
		RoofGeneratorActor = nullptr;
	}

	if (bRoadEditorMode) {
		OnRoadEditorModePressed();
	}

	SelectedWidget = EWidgetSelection::RoadConstructor;

	if (GetLocalPlayer()) {
		UEnhancedInputLocalPlayerSubsystem* SubSystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		if (SubSystem) {
			SubSystem->ClearAllMappings();
			SubSystem->AddMappingContext(RoadMapping, 0);
		}
	}
	ConstructionModeHandler();
}

void AArchVizController::OnRoadLocationXChanged(float Value)
{
	if (RoadConstructor) {
		FVector Location = RoadConstructor->GetActorLocation();
		RoadConstructor->SetActorLocation(FVector(Value, Location.Y, Location.Z));
	}
}

void AArchVizController::OnRoadLocationYChanged(float Value)
{
	if (RoadConstructor) {
		FVector Location = RoadConstructor->GetActorLocation();
		RoadConstructor->SetActorLocation(FVector(Location.X, Value, Location.Z));
	}
}

void AArchVizController::OnRoadEditorModePressed()
{
	if (!bRoadEditorMode) {
		FText Text = FText::FromString("Close Editor Mode");
		RoadWidget->EditorText->SetText(Text);
		RoadWidget->OutlineBorder->SetVisibility(ESlateVisibility::Visible);
		RoadWidget->GenerateNewRoad->SetVisibility(ESlateVisibility::Hidden);
		bRoadEditorMode = true;
	}
	else {
		FText Text = FText::FromString("Editor Mode");
		RoadWidget->EditorText->SetText(Text);
		RoadWidget->OutlineBorder->SetVisibility(ESlateVisibility::Hidden);
		RoadWidget->GenerateNewRoad->SetVisibility(ESlateVisibility::Visible);
		bRoadEditorMode = false;
	}
}

void AArchVizController::OnRoadWidthChanged(float Value)
{
	if (RoadConstructor) { RoadConstructor->SetActorScale3D(FVector(1, Value / RoadDimensions.Y, 1)); }
}

//Material Managment

void AArchVizController::OnMaterialManagmentPressed()
{
	if (IsValid(WallGeneratorActor)) {
		if (!bBuildingEditorMode) { WallGeneratorActor->Destroy(); }
		WallGeneratorActor = nullptr;
	}
	if (IsValid(InteriorDesignActor)) {
		InteriorDesignActor->Destroy();
		InteriorDesignActor = nullptr;
	}
	if (IsValid(DoorMeshActor)) {
		DoorMeshActor->Destroy();
		DoorMeshActor = nullptr;
	}
	if (IsValid(FloorGeneratorActor)) {
		FloorGeneratorActor = nullptr;
	}
	if (IsValid(RoofGeneratorActor)) {
		RoofGeneratorActor = nullptr;
	}

	SelectedWidget = EWidgetSelection::MaterialManagment;

	if (GetLocalPlayer()) {
		UEnhancedInputLocalPlayerSubsystem* SubSystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		if (SubSystem) {
			SubSystem->ClearAllMappings();
			SubSystem->AddMappingContext(MaterialManagmentMapping, 0);
		}
	}

	ConstructionModeHandler();
}

void AArchVizController::SetMaterialManagmentMapping()
{
	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent);

	MaterialManagmentMapping = NewObject<UInputMappingContext>(this);

	SelectActorAction = NewObject<UInputAction>(this);
	SelectActorAction->ValueType = EInputActionValueType::Boolean;

	MaterialManagmentMapping->MapKey(SelectActorAction, EKeys::LeftMouseButton);

	if (EIC) {
		EIC->BindAction(SelectActorAction, ETriggerEvent::Completed, this, &AArchVizController::SelectActorOnClick);
	}
}

void AArchVizController::SelectActorOnClick()
{
	RoadConstructor = nullptr;
	WallGeneratorActor = nullptr;
	FloorGeneratorActor = nullptr;
	RoofGeneratorActor = nullptr;

	GetHitResultUnderCursor(ECC_Visibility, true, HitResult);
	if (Cast<ARoadConstructor>(HitResult.GetActor())) {

		MaterialManagmentWidget->RoadMaterialScrollBox->SetVisibility(ESlateVisibility::Visible);
		MaterialManagmentWidget->BuildingMaterialScrollBox->SetVisibility(ESlateVisibility::Hidden);

		RoadConstructor = Cast<ARoadConstructor>(HitResult.GetActor());
	}
	else if (Cast<AWallGenerator>(HitResult.GetActor())) {
		MaterialManagmentWidget->RoadMaterialScrollBox->SetVisibility(ESlateVisibility::Hidden);
		MaterialManagmentWidget->BuildingMaterialScrollBox->SetVisibility(ESlateVisibility::Visible);

		WallGeneratorActor = Cast<AWallGenerator>(HitResult.GetActor());
		bBuildingEditorMode = true;
	}
	else if (Cast<AFloorGenerator>(HitResult.GetActor())) {
		MaterialManagmentWidget->RoadMaterialScrollBox->SetVisibility(ESlateVisibility::Hidden);
		MaterialManagmentWidget->BuildingMaterialScrollBox->SetVisibility(ESlateVisibility::Visible);

		FloorGeneratorActor = Cast<AFloorGenerator>(HitResult.GetActor());

	}
	else if (Cast<ARoofGenerator>(HitResult.GetActor())) {
		MaterialManagmentWidget->RoadMaterialScrollBox->SetVisibility(ESlateVisibility::Hidden);
		MaterialManagmentWidget->BuildingMaterialScrollBox->SetVisibility(ESlateVisibility::Visible);

		RoofGeneratorActor = Cast<ARoofGenerator>(HitResult.GetActor());

	}
	else {
		MaterialManagmentWidget->RoadMaterialScrollBox->SetVisibility(ESlateVisibility::Hidden);
		MaterialManagmentWidget->BuildingMaterialScrollBox->SetVisibility(ESlateVisibility::Hidden);

	}
}

void AArchVizController::ApplyRoadMaterial(const FRoadMaterialData& RoadMaterialData) {
	if(RoadConstructor) {
		UMaterialInstanceDynamic* DynamicRoadMaterial = UMaterialInstanceDynamic::Create(RoadMaterialData.Material, this);
		if (DynamicRoadMaterial) {
			float TileX = (RoadConstructor->RoadDimensions.X) / 200.0f;
			float TileY = 1;
			DynamicRoadMaterial->SetScalarParameterValue("TileX", TileX);
			DynamicRoadMaterial->SetScalarParameterValue("TileY", TileY);
			RoadConstructor->ProceduralMeshComponent->SetMaterial(0, DynamicRoadMaterial);
		}
	}
}

void AArchVizController::ApplyBuildingMaterial(const FBuildingMaterialData& BuildingMaterialData) {
	if (WallGeneratorActor) {
		WallGeneratorActor->ApplyMaterialToWallActor(BuildingMaterialData.Material);
	}
	else if (FloorGeneratorActor) {
		UMaterialInstanceDynamic* DynamicFloorMaterial = UMaterialInstanceDynamic::Create(BuildingMaterialData.Material, this);
		if (DynamicFloorMaterial) {
			float TileX = (FloorGeneratorActor->FloorDimensions.X) / 200.0f;
			float TileY = (FloorGeneratorActor->FloorDimensions.Y) / 200.0f;
			DynamicFloorMaterial->SetScalarParameterValue("TileX", TileX);
			DynamicFloorMaterial->SetScalarParameterValue("TileY", TileY);
			FloorGeneratorActor->FloorComponent->SetMaterial(0, DynamicFloorMaterial);
		}
	}
	else if (RoofGeneratorActor) {
		UMaterialInstanceDynamic* DynamicRoofMaterial = UMaterialInstanceDynamic::Create(BuildingMaterialData.Material, this);
		if (DynamicRoofMaterial) {
			float TileX = (RoofGeneratorActor->RoofDimensions.X) / 200.0f;
			float TileY = (RoofGeneratorActor->RoofDimensions.Y) / 200.0f;
			DynamicRoofMaterial->SetScalarParameterValue("TileX", TileX);
			DynamicRoofMaterial->SetScalarParameterValue("TileY", TileY);
			RoofGeneratorActor->RoofComponent->SetMaterial(0, DynamicRoofMaterial);
		}
	}
}

//Interior Design Mode
void AArchVizController::OnInteriorDesignPressed()
{
	if (IsValid(WallGeneratorActor)) {
		if (!bBuildingEditorMode) { WallGeneratorActor->Destroy(); }
		WallGeneratorActor = nullptr;
	}
	if (IsValid(InteriorDesignActor)) {
		InteriorDesignActor->Destroy();
		InteriorDesignActor = nullptr;
	}
	if (IsValid(DoorMeshActor)) {
		DoorMeshActor->Destroy();
		DoorMeshActor = nullptr;
	}
	if (IsValid(FloorGeneratorActor)) {
		FloorGeneratorActor = nullptr;
	}
	if (IsValid(RoofGeneratorActor)) {
		RoofGeneratorActor = nullptr;
	}

	SelectedWidget = EWidgetSelection::InteriorDesign;

	if (GetLocalPlayer()) {
		UEnhancedInputLocalPlayerSubsystem* SubSystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		if (SubSystem) {
			SubSystem->ClearAllMappings();
			SubSystem->AddMappingContext(InteriorDesignMapping, 0);
		}
	}

	ConstructionModeHandler();

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	InteriorDesignActor = GetWorld()->SpawnActor<AInteriorDesignActor>(AInteriorDesignActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
}

void AArchVizController::SetInteriorDesignMapping()
{
	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent);

	InteriorDesignMapping = NewObject<UInputMappingContext>(this);

	PlaceInteriorAction = NewObject<UInputAction>(this);
	PlaceInteriorAction->ValueType = EInputActionValueType::Boolean;

	RotateInteriorAction = NewObject<UInputAction>(this);
	RotateInteriorAction->ValueType = EInputActionValueType::Boolean;

	InteriorDesignMapping->MapKey(PlaceInteriorAction, EKeys::LeftMouseButton);
	InteriorDesignMapping->MapKey(RotateInteriorAction, EKeys::R);

	if (EIC) {
		EIC->BindAction(PlaceInteriorAction, ETriggerEvent::Completed, this, &AArchVizController::PlaceInteriorOnClick);
		EIC->BindAction(RotateInteriorAction, ETriggerEvent::Completed, this, &AArchVizController::RotateInterior);
	}
}

void AArchVizController::PlaceInteriorOnClick()
{
	FVector LastLocation;
	if (InteriorDesignActor) {
		LastLocation = InteriorDesignActor->GetActorLocation();
	}
	FCollisionQueryParams TraceParams(FName(TEXT("LineTrace")), true, InteriorDesignActor);
	FVector CursorWorldLocation;
	FVector CursorWorldDirection;

	DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection);
	GetWorld()->LineTraceSingleByChannel(HitResult, CursorWorldLocation, CursorWorldLocation + CursorWorldDirection * 10000, ECC_Visibility, TraceParams);

	if (InteriorDesignActor->IdentityIndex == 0) {
		if (Cast<AWallGenerator>(HitResult.GetActor())) {
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			InteriorDesignActor = GetWorld()->SpawnActor<AInteriorDesignActor>(AInteriorDesignActor::StaticClass(), LastLocation, FRotator::ZeroRotator, SpawnParams);
		}
	}
	else if (InteriorDesignActor->IdentityIndex == 1) {
		if (Cast<AFloorGenerator>(HitResult.GetActor())) {
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			InteriorDesignActor = GetWorld()->SpawnActor<AInteriorDesignActor>(AInteriorDesignActor::StaticClass(), LastLocation, FRotator::ZeroRotator, SpawnParams);
		}
	}
	else if (InteriorDesignActor->IdentityIndex == 2) {
		if (Cast<ARoofGenerator>(HitResult.GetActor())) {
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			InteriorDesignActor = GetWorld()->SpawnActor<AInteriorDesignActor>(AInteriorDesignActor::StaticClass(), LastLocation, FRotator::ZeroRotator, SpawnParams);
		}
	}
}

void AArchVizController::RotateInterior()
{
	if (InteriorDesignActor) {
		InteriorDesignActor->SetActorRotation(InteriorDesignActor->GetActorRotation() + FRotator(0,90,0));
	}
}

void AArchVizController::ApplyInterior(const FInteriorData& InteriorData)
{
	if (InteriorDesignActor) {
		InteriorDesignActor->IdentityIndex = InteriorData.IdentityIndex;
		if(InteriorData.InteriorMesh){InteriorDesignActor->SetInteriorMesh(InteriorData.InteriorMesh);}
	}
}

void AArchVizController::PreviewWallInteriorActor()
{
	FCollisionQueryParams TraceParams(FName(TEXT("LineTrace")), true, InteriorDesignActor);
	FVector CursorWorldLocation;
	FVector CursorWorldDirection;

	DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection);
	if (GetWorld()->LineTraceSingleByChannel(HitResult, CursorWorldLocation, CursorWorldLocation + CursorWorldDirection * 10000, ECC_Visibility, TraceParams))
	{
		if (AWallGenerator* WallActor = Cast<AWallGenerator>(HitResult.GetActor())) {
			if (InteriorDesignActor) {
				FVector WallDirection = WallActor->GetActorForwardVector(); // 1,0,0
				FVector PreviewInteriorLocation{};
				if (InteriorDesignActor->InteriorMesh) {
					if ((FindAngleBetweenVectors({ 1, 0, 0 }, WallDirection) == 0) || (FindAngleBetweenVectors({ -1, 0, 0 }, WallDirection) == 0)) { // X || -X
						if (HitResult.Location.Y > WallActor->GetActorLocation().Y) {
							PreviewInteriorLocation = HitResult.Location + FVector{ 0, InteriorDesignActor->InteriorMesh->GetBounds().GetBox().GetSize().Y / 2, 0 };
						}
						else {
							PreviewInteriorLocation = HitResult.Location + FVector{ 0, -InteriorDesignActor->InteriorMesh->GetBounds().GetBox().GetSize().Y / 2, 0 };
						}
					}
					else if ((FindAngleBetweenVectors({ 0, 1, 0 }, WallDirection) == 0) || (FindAngleBetweenVectors({ 0, -1, 0 }, WallDirection) == 0)) {
						if (HitResult.Location.X > WallActor->GetActorLocation().X) {
							PreviewInteriorLocation = HitResult.Location + FVector{ InteriorDesignActor->InteriorMesh->GetBounds().GetBox().GetSize().Y / 2, 0, 0 };
						}
						else {
							PreviewInteriorLocation = HitResult.Location + FVector{ -InteriorDesignActor->InteriorMesh->GetBounds().GetBox().GetSize().Y / 2, 0, 0 };
						}
					}
					InteriorDesignActor->SetActorLocation(PreviewInteriorLocation);
				}
			}
		}
	}
}

void AArchVizController::PreviewFloorInteriorActor()
{
	FCollisionQueryParams TraceParams(FName(TEXT("LineTrace")), true, InteriorDesignActor);
	FVector CursorWorldLocation;
	FVector CursorWorldDirection;

	DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection);
	if (GetWorld()->LineTraceSingleByChannel(HitResult, CursorWorldLocation, CursorWorldLocation + CursorWorldDirection * 10000, ECC_Visibility, TraceParams))
	{
		if (AFloorGenerator* FloorActor = Cast<AFloorGenerator>(HitResult.GetActor())) {
			if (InteriorDesignActor) {
				InteriorDesignActor->SetActorLocation(CheckPivotLocation(InteriorDesignActor , HitResult.Location));
			}
		}
	}
}

void AArchVizController::PreviewRoofInteriorActor()
{
}

FVector AArchVizController::CheckPivotLocation(AInteriorDesignActor* InteriorActor , FVector Location)
{
	if(InteriorActor){
		FVector PivotLocation = InteriorActor->InteriorMeshComponent->GetComponentLocation();

		FBox BoundingBox = InteriorActor->InteriorMesh->GetBoundingBox();
		FVector Center = BoundingBox.GetCenter();

		if (PivotLocation.Equals(Center)) {
			float PreviewInteriorMeshLocationZ = (InteriorDesignActor->InteriorMesh->GetBounds().GetBox().GetSize().Z / 2);
			Location.Z = PreviewInteriorMeshLocationZ;
		}
	}
	return Location;
}
