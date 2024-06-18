// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizController.h"
#include <Kismet/KismetMathLibrary.h>

AArchVizController::AArchVizController() : bGetLocation{ true }, bFirstRoad{ true }, bEditorMode{ false }
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
		BuildingConstructorWidget->GenerateDoor->OnClicked.AddDynamic(this, &AArchVizController::OnGenerateDoorPressed);
		BuildingConstructorWidget->GenerateFloor->OnClicked.AddDynamic(this, &AArchVizController::OnGenerateFloorPressed);
		BuildingConstructorWidget->GenerateRoof->OnClicked.AddDynamic(this, &AArchVizController::OnGenerateRoofPressed);
		BuildingConstructorWidget->Segments->OnValueChanged.AddDynamic(this, &AArchVizController::OnWallSegmentsValueChanged);
	}
}

void AArchVizController::SetupInputComponent()
{
	Super::SetupInputComponent();

	SetRoadConstructionMapping();
	SetWallConstructionMapping();
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
	BuildingConstructorWidget->SegmentBorder->SetVisibility(ESlateVisibility::Hidden);
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
	}
}

//Wall Generator
void AArchVizController::OnGenerateWallPressed()
{
	SelectedBuildingComponent = EBuildingComponentSelection::Wall;
	AddBuildingComponentsMapping();

	BuildingConstructorWidget->SegmentBorder->SetVisibility(ESlateVisibility::Visible);

	if (WallActorClassRef) {
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		WallActor = GetWorld()->SpawnActor<AWallGenerator>(WallActorClassRef, FVector::ZeroVector, FRotator::ZeroRotator, Params);
		int32 Value;
		if(BuildingConstructorWidget) {
			Value = int32(BuildingConstructorWidget->Segments->GetValue());
		}
		WallActor->GenerateWall(Value);
	}
}

void AArchVizController::WallProjectionOnTick()
{
	FCollisionQueryParams TraceParams(FName(TEXT("LineTrace")), true , WallActor);
	FVector CursorWorldLocation;
	FVector CursorWorldDirection;

	DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection);
	if (GetWorld()->LineTraceSingleByChannel(WallHitResult, CursorWorldLocation, CursorWorldLocation + CursorWorldDirection * 10000, ECC_Visibility, TraceParams))
	{
		FVector ResultedLocation = WallHitResult.Location;
		if(WallActor){
			WallActor->SetActorLocation(ResultedLocation);
			SnapActor(WallActor->WallStaticMesh->GetBounds().GetBox().GetSize().Y/2);
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
		EIC->BindAction(GenerateWallAction, ETriggerEvent::Completed, this, &AArchVizController::GenerateWall);
		EIC->BindAction(RotateWallAction, ETriggerEvent::Completed, this, &AArchVizController::RotateWall);
	}
}

void AArchVizController::OnWallSegmentsValueChanged(float SegmentsNo)
{
	if(WallActor){
		WallActor->GenerateWall(int32(SegmentsNo));
	}
}

void AArchVizController::GenerateWall()
{
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	WallActor = GetWorld()->SpawnActor<AWallGenerator>(WallActorClassRef, FVector::ZeroVector, FRotator::ZeroRotator, Params);
	WallActor->GenerateWall(BuildingConstructorWidget->Segments->GetValue());
}

void AArchVizController::RotateWall()
{
	if (WallActor) {
		WallActor->SetActorRotation(FRotator(0, WallActor->GetActorRotation().Yaw + 90, 0));
	}
}

//Door Generator
void AArchVizController::OnGenerateDoorPressed()
{
	if (WallActor) {
		WallActor->Destroy();
	}
	BuildingConstructorWidget->SegmentBorder->SetVisibility(ESlateVisibility::Hidden);


	SelectedBuildingComponent = EBuildingComponentSelection::Door;
	AddBuildingComponentsMapping();
}

void AArchVizController::SetDoorConstructionMapping()
{
	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent);

	DoorMapping = NewObject<UInputMappingContext>(this);

	/*GenerateWallAction = NewObject<UInputAction>(this);
	GenerateWallAction->ValueType = EInputActionValueType::Boolean;*/

	/*DoorMapping->MapKey(GenerateWallAction, EKeys::LeftMouseButton);*/

	if (EIC) {
		//EIC->BindAction(GenerateWallAction, ETriggerEvent::Completed, this, &AArchVizController::GenerateWall);
	}
}

//Floor Generator
void AArchVizController::OnGenerateFloorPressed()
{
	if (WallActor) {
		WallActor->Destroy();
	}
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
		WallActor->Destroy();
	}
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
	if (WallActor) {
		WallActor->Destroy();
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
