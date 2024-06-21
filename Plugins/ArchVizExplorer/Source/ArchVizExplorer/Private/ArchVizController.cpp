// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizController.h"
#include <Kismet/KismetMathLibrary.h>

AArchVizController::AArchVizController() : bGetLocation{ true }, bFirstRoad{ true }, bEditorMode{ false }, bBuildingEditorMode{ false }
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
	BindWidgetDelegates();
}

void AArchVizController::BindWidgetDelegates() {
	if (HomeWidget) {
		HomeWidget->RoadConstruction->OnClicked.AddDynamic(this, &AArchVizController::OnRoadConstructionPressed);
		HomeWidget->BuildingConstruction->OnClicked.AddDynamic(this, &AArchVizController::OnBuildingConstructionPressed);
	}
	if (RoadWidget) {
		RoadWidget->EditorMode->OnClicked.AddDynamic(this, &AArchVizController::OnEditorModePressed);
		RoadWidget->GenerateNewRoad->OnClicked.AddDynamic(this, &AArchVizController::GenerateNewRoad);
		RoadWidget->WidthValueBox->OnValueChanged.AddDynamic(this, &AArchVizController::OnWidthChanged);
		RoadWidget->Location_x->OnValueChanged.AddDynamic(this, &AArchVizController::OnLocationXChanged);
		RoadWidget->Location_Y->OnValueChanged.AddDynamic(this, &AArchVizController::OnLocationYChanged);
	}
	if (BuildingConstructorWidget) {
		BuildingConstructorWidget->GenerateWall->OnClicked.AddDynamic(this, &AArchVizController::OnGenerateWallPressed);
		BuildingConstructorWidget->Segments->OnValueChanged.AddDynamic(this, &AArchVizController::OnWallSegmentsValueChanged);
		BuildingConstructorWidget->WallScrollBox->AfterWallSelection.BindUFunction(this , "SetWallStaticMesh");

		BuildingConstructorWidget->GenerateDoor->OnClicked.AddDynamic(this, &AArchVizController::OnGenerateDoorPressed);
		BuildingConstructorWidget->DoorScrollBox->AfterDoorSelection.BindUFunction(this , "SetDoor");
		
		BuildingConstructorWidget->GenerateFloor->OnClicked.AddDynamic(this, &AArchVizController::OnGenerateFloorPressed);
		BuildingConstructorWidget->GenerateRoof->OnClicked.AddDynamic(this, &AArchVizController::OnGenerateRoofPressed);

		BuildingConstructorWidget->BuildingEditor->OnClicked.AddDynamic(this, &AArchVizController::OnBuildingEditorModePressed);
	}
}

void AArchVizController::SetupInputComponent()
{
	Super::SetupInputComponent();

	SetRoadConstructionMapping();
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
		switch(SelectedBuildingComponent){
		case EBuildingComponentSelection::Wall: {
			WallProjectionOnTick();
			break;
		}
		}
	}
}

//BuildingConstruction
void AArchVizController::OnBuildingConstructionPressed()
{
	if (WallActor) { WallActor->Destroy(); };
	BuildingConstructorWidget->SegmentBorder->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructorWidget->WallScrollBox->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructorWidget->DoorScrollBox->SetVisibility(ESlateVisibility::Hidden);

	SelectedWidget = EWidgetSelection::BuildingConstructor;

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

	switch(SelectedBuildingComponent) {
	case EBuildingComponentSelection::Wall: {
		if(SubSystem){SubSystem->AddMappingContext(WallMapping, 0);}
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

//Wall Generator
void AArchVizController::OnGenerateWallPressed()
{
	SelectedBuildingComponent = EBuildingComponentSelection::Wall;
	AddBuildingComponentsMapping();

	BuildingConstructorWidget->WallScrollBox->SetVisibility(ESlateVisibility::Visible);
	BuildingConstructorWidget->DoorScrollBox->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructorWidget->SegmentBorder->SetVisibility(ESlateVisibility::Visible);

	if (WallActorClassRef) {
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		WallActor = GetWorld()->SpawnActor<AWallGenerator>(WallActorClassRef, FVector::ZeroVector, FRotator::ZeroRotator, Params);
		int32 Value;
		if(BuildingConstructorWidget) {
			Value = int32(BuildingConstructorWidget->Segments->GetValue());
		}
		if(WallActor){
			WallActor->GenerateWall(Value);
		}
	}
}

void AArchVizController::WallProjectionOnTick()
{
	FCollisionQueryParams TraceParams(FName(TEXT("LineTrace")), true , WallActor);
	FVector CursorWorldLocation;
	FVector CursorWorldDirection;

	DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection);
	if (GetWorld()->LineTraceSingleByChannel(HitResult, CursorWorldLocation, CursorWorldLocation + CursorWorldDirection * 10000, ECC_Visibility, TraceParams))
	{
		FVector ResultedLocation = HitResult.Location;
		if(WallActor){
			WallActor->SetActorLocation(ResultedLocation);
			if(WallActor->WallStaticMesh){SnapActor(WallActor->WallStaticMesh->GetBounds().GetBox().GetSize().Y/2);}
		}
	}
}

void AArchVizController::SnapActor(float SnapValue)
{
	auto CurrentLocation = WallActor->GetActorLocation();
	CurrentLocation.X = FMath::RoundToFloat(CurrentLocation.X / SnapValue) * SnapValue;
	CurrentLocation.Y = FMath::RoundToFloat(CurrentLocation.Y / SnapValue) * SnapValue;
	WallActor->SetActorLocation(CurrentLocation);
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
	if(WallActor){
		WallActor->GenerateWall(int32(SegmentsNo));
	}
}

void AArchVizController::GenerateNewWall()
{
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	WallActor = GetWorld()->SpawnActor<AWallGenerator>(WallActorClassRef, FVector::ZeroVector, FRotator::ZeroRotator, Params); 
	if(TempWallMesh){WallActor->WallStaticMesh = TempWallMesh;}
	WallActor->GenerateWall(BuildingConstructorWidget->Segments->GetValue());
}

void AArchVizController::RotateWall()
{
	if (WallActor) {
		WallActor->SetActorRotation(FRotator(0, WallActor->GetActorRotation().Yaw + 90, 0));
	}
}

void AArchVizController::SetWallStaticMesh(const FWallData& WallData) {
	if (IsValid(WallActor)) {
		TempWallMesh = WallData.Mesh;
		WallActor->WallStaticMesh = TempWallMesh;
		WallActor->GenerateWall(BuildingConstructorWidget->Segments->GetValue());
	}
}

//Door Generator
void AArchVizController::OnGenerateDoorPressed()
{
	if (WallActor) {
		if(!bBuildingEditorMode) {WallActor->Destroy();}
		WallActor = nullptr;
	}
	bBuildingEditorMode = false;

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
	if(!bBuildingEditorMode){GetHitResultUnderCursor(ECC_Visibility, true, HitResult);}

	if (HitResult.bBlockingHit) {
		if(Cast<AWallGenerator>(HitResult.GetActor())){
			AWallGenerator* WallTempActor = Cast<AWallGenerator>(HitResult.GetActor());
			if (Cast<UStaticMeshComponent>(HitResult.GetComponent())) {
				UStaticMeshComponent* DoorComponent = Cast<UStaticMeshComponent>(HitResult.GetComponent());
				if (DoorMesh) {
					UProceduralMeshComponent* CubeComponent = NewObject<UProceduralMeshComponent>(WallTempActor);
					float DoorHeight = DoorMesh->GetBounds().GetBox().GetSize().Z;
					FVector WallDimensions = DoorComponent->GetStaticMesh()->GetBounds().GetBox().GetSize();

					if(WallDimensions.Z == WallTempActor->WallHeight){
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

						int32 index = WallTempActor->ComponentsArray.IndexOfByKey(Cast<UStaticMeshComponent>(HitResult.GetComponent()));
						WallTempActor->WallActorMap.Add(index , {DoorMesh , CubeComponent});
					}
					else {
						DoorComponent->SetRelativeRotation(FRotator(0, 90, 0));
						DoorComponent->SetStaticMesh(DoorMesh);
					}
				}
			}
		}
	}
}

void AArchVizController::SetDoor(const FDoorData& DoorData) {
	DoorMesh = DoorData.Mesh ;
	if (bBuildingEditorMode) {
		GenerateDoor();
	}
}

//Floor Generator
void AArchVizController::OnGenerateFloorPressed()
{
	if (WallActor) {
		if (!bBuildingEditorMode) { WallActor->Destroy(); }
		WallActor = nullptr;
	}
		bBuildingEditorMode = false;
	BuildingConstructorWidget->WallScrollBox->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructorWidget->DoorScrollBox->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructorWidget->SegmentBorder->SetVisibility(ESlateVisibility::Hidden);

	SelectedBuildingComponent = EBuildingComponentSelection::Floor;
	AddBuildingComponentsMapping();
}

void AArchVizController::SetFloorConstructionMapping()
{
	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent);

	FloorMapping = NewObject<UInputMappingContext>(this);

	/*GenerateWallAction = NewObject<UInputAction>(this);
	GenerateWallAction->ValueType = EInputActionValueType::Boolean;*/

	/*FloorMapping->MapKey(GenerateWallAction, EKeys::LeftMouseButton);*/

	if (EIC) {
		//EIC->BindAction(GenerateWallAction, ETriggerEvent::Completed, this, &AArchVizController::GenerateWall);
	}
}

//Roof Generator
void AArchVizController::OnGenerateRoofPressed()
{
	if (WallActor) {
		if (!bBuildingEditorMode) { WallActor->Destroy(); }
		WallActor = nullptr;
	}
		bBuildingEditorMode = false;
	BuildingConstructorWidget->WallScrollBox->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructorWidget->DoorScrollBox->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructorWidget->SegmentBorder->SetVisibility(ESlateVisibility::Hidden);

	SelectedBuildingComponent = EBuildingComponentSelection::Roof;
	AddBuildingComponentsMapping();
}

void AArchVizController::SetRoofConstructionMapping()
{
	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent);

	RoofMapping = NewObject<UInputMappingContext>(this);

	/*GenerateWallAction = NewObject<UInputAction>(this);
	GenerateWallAction->ValueType = EInputActionValueType::Boolean;*/

	/*RoofMapping->MapKey(GenerateWallAction, EKeys::LeftMouseButton);*/

	if (EIC) {
		//EIC->BindAction(GenerateWallAction, ETriggerEvent::Completed, this, &AArchVizController::GenerateWall);
	}
}

//BuildingEditor Mode

void AArchVizController::OnBuildingEditorModePressed()
{
	if (WallActor) {
		if (!bBuildingEditorMode) { WallActor->Destroy(); }
		WallActor = nullptr;
	}
	bBuildingEditorMode = true;

	BuildingConstructorWidget->WallScrollBox->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructorWidget->DoorScrollBox->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructorWidget->SegmentBorder->SetVisibility(ESlateVisibility::Hidden);

	SelectedBuildingComponent = EBuildingComponentSelection::Editor;
	AddBuildingComponentsMapping();
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
	GetHitResultUnderCursor(ECC_Visibility,true,HitResult);

	if (Cast<UProceduralMeshComponent>(HitResult.GetComponent())) {
		if(Cast<AWallGenerator>(HitResult.GetActor())){
			WallActor = Cast<AWallGenerator>(HitResult.GetActor());
		}
		EditWall();
	}
	else if (Cast<AWallGenerator>(HitResult.GetActor())) {
		WallActor = Cast<AWallGenerator>(HitResult.GetActor());
		UStaticMeshComponent* TempComponent = Cast<UStaticMeshComponent>(HitResult.GetComponent());

		float HeightOfMesh = TempComponent->GetStaticMesh()->GetBounds().GetBox().GetSize().Z;

		if (HeightOfMesh != WallActor->WallHeight) {
			EditDoor();
		}
		else {
			EditWall();
		}
	}

}

void AArchVizController::EditWall()
{
	BuildingConstructorWidget->DoorScrollBox->SetVisibility(ESlateVisibility::Hidden); 
	BuildingConstructorWidget->SegmentBorder->SetVisibility(ESlateVisibility::Visible); 
	BuildingConstructorWidget->Segments->SetValue(WallActor->ComponentsArray.Num());
}

void AArchVizController::EditDoor()
{
	BuildingConstructorWidget->SegmentBorder->SetVisibility(ESlateVisibility::Hidden);
	BuildingConstructorWidget->DoorScrollBox->SetVisibility(ESlateVisibility::Visible);
}


//ConstructionMode Handler
void AArchVizController::ConstructionModeHandler()
{
	switch (SelectedWidget) {
		case EWidgetSelection::RoadConstructor: {
			RoadWidget->AddToViewport();
			if (BuildingConstructorWidget->IsInViewport()) {
				BuildingConstructorWidget->RemoveFromParent();
			}
			break;
		}
		case EWidgetSelection::BuildingConstructor: {
			BuildingConstructorWidget->AddToViewport();
			if (RoadWidget->IsInViewport()) {
				RoadWidget->RemoveFromParent();
			}
			break;
		}
	}
}

//Road Constructor
void AArchVizController::GetRoadLocationOnClick()
{
	if (!bEditorMode) {
		FCollisionQueryParams TraceParams(FName(TEXT("LineTrace")), true);
		FVector CursorWorldLocation;
		FVector CursorWorldDirection;

		if (bFirstRoad) {
			if (bGetLocation) {
				DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection);
				if (GetWorld()->LineTraceSingleByChannel(HitResult, CursorWorldLocation, CursorWorldLocation + CursorWorldDirection * 10000, ECC_Visibility, TraceParams))
				{
					StartLocation = HitResult.Location;
				}
				bGetLocation = false;
			}
			else {
				DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection);
				if (GetWorld()->LineTraceSingleByChannel(HitResult, CursorWorldLocation, CursorWorldLocation + CursorWorldDirection * 10000, ECC_Visibility, TraceParams))
				{
					EndLocation = HitResult.Location;
				}
				GenerateRoad();
				bFirstRoad = false;
			}
		}
		else {
			DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection);
			if (GetWorld()->LineTraceSingleByChannel(HitResult, CursorWorldLocation, CursorWorldLocation + CursorWorldDirection * 10000, ECC_Visibility, TraceParams))
			{
				FVector ForwardVectorOfPrevRoad = (EndLocation - StartLocation).GetSafeNormal();
				FVector LeftVectorOfPrevRoad = FVector::CrossProduct(ForwardVectorOfPrevRoad, FVector::UpVector);
				FVector BackVectorOfPrevRoad = (StartLocation - EndLocation).GetSafeNormal();
				FVector RightVectorOfPrevRoad = FVector::CrossProduct(FVector::UpVector, ForwardVectorOfPrevRoad);

				StartLocation = EndLocation;
				EndLocation = HitResult.Location;

				float Distance = FVector::Dist(EndLocation, StartLocation);

				FVector ForwardVectorOfCurrRoad = EndLocation - StartLocation;
				float AngleOfPointWithLeftVecOfPrev = FindAngleBetweenVectors(ForwardVectorOfCurrRoad, LeftVectorOfPrevRoad);
				float AngleOfPointWithBackVecOfPrev = FindAngleBetweenVectors(ForwardVectorOfCurrRoad, BackVectorOfPrevRoad);

				FVector EndPointDir;
				if ((AngleOfPointWithLeftVecOfPrev >= 0 && AngleOfPointWithLeftVecOfPrev < 45) || (AngleOfPointWithBackVecOfPrev <= 90 && AngleOfPointWithLeftVecOfPrev <= 90)) {
					EndPointDir = LeftVectorOfPrevRoad;
				}
				else if ((AngleOfPointWithLeftVecOfPrev > 135 && AngleOfPointWithLeftVecOfPrev <= 180) || (AngleOfPointWithBackVecOfPrev <= 90 && AngleOfPointWithLeftVecOfPrev >= 90)) {
					EndPointDir = RightVectorOfPrevRoad;
				}
				else if (AngleOfPointWithLeftVecOfPrev > 45 && AngleOfPointWithLeftVecOfPrev <= 135) {
					EndPointDir = ForwardVectorOfPrevRoad;
				}
				EndLocation = StartLocation + (EndPointDir.GetSafeNormal() * Distance);
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
	FVector RoadLocation = (StartLocation + EndLocation) / 2;
	FRotator RoadRotation = UKismetMathLibrary::FindLookAtRotation(StartLocation, EndLocation);

	float length = FVector::Dist(StartLocation, EndLocation);
	float width = 100;
	float height = 10;

	RoadDimensions = FVector(length, width, height);

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	RoadConstructor = GetWorld()->SpawnActor<ARoadConstructor>(ARoadConstructor::StaticClass(), RoadLocation, RoadRotation, Params);

	if (RoadConstructor) {
		RoadConstructor->Material = Material;
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
	if (IsValid(WallActor)) {
		if(!bBuildingEditorMode) {WallActor->Destroy();}
		WallActor = nullptr;
		bBuildingEditorMode = false;
	}

	if (bEditorMode) {
		OnEditorModePressed();
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

void AArchVizController::OnLocationXChanged(float Value)
{
	if (RoadConstructor) {
		FVector Location = RoadConstructor->GetActorLocation();
		RoadConstructor->SetActorLocation(FVector(Value, Location.Y, Location.Z));
	}
}

void AArchVizController::OnLocationYChanged(float Value)
{
	if (RoadConstructor) {
		FVector Location = RoadConstructor->GetActorLocation();
		RoadConstructor->SetActorLocation(FVector(Location.X, Value, Location.Z));
	}
}

void AArchVizController::OnEditorModePressed()
{
	if (!bEditorMode) {
		FText Text = FText::FromString("Close Editor Mode");
		RoadWidget->EditorText->SetText(Text);
		RoadWidget->OutlineBorder->SetVisibility(ESlateVisibility::Visible);
		RoadWidget->GenerateNewRoad->SetVisibility(ESlateVisibility::Hidden);
		bEditorMode = true;
	}
	else {
		FText Text = FText::FromString("Editor Mode");
		RoadWidget->EditorText->SetText(Text);
		RoadWidget->OutlineBorder->SetVisibility(ESlateVisibility::Hidden);
		RoadWidget->GenerateNewRoad->SetVisibility(ESlateVisibility::Visible);
		bEditorMode = false;
	}
}

void AArchVizController::OnWidthChanged(float Value)
{
	if (RoadConstructor) { RoadConstructor->SetActorScale3D(FVector(1, Value / RoadDimensions.Y, 1)); }
}