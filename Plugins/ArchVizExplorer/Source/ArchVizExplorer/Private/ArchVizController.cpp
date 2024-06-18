// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizController.h"
#include <Kismet/KismetMathLibrary.h>

AArchVizController::AArchVizController() : bGetLocation{ true }, bFirstRoad{ true } , bEditorMode{ false } , bBuildingConstructionMode{true}
{
}

void AArchVizController::BeginPlay()
{
	Super::BeginPlay();
	bShowMouseCursor = true;

	if (RoadWidgetClassRef) {
		RoadWidget = CreateWidget<URoadUIWidget>(this, RoadWidgetClassRef);
		if(RoadWidget){
			RoadWidget->EditorMode->OnClicked.AddDynamic(this, &AArchVizController::OnEditorModePressed);
			RoadWidget->GenerateNewRoad->OnClicked.AddDynamic(this, &AArchVizController::GenerateNewRoad);
			RoadWidget->WidthValueBox->OnValueChanged.AddDynamic(this, &AArchVizController::OnWidthChanged);
			RoadWidget->Location_x->OnValueChanged.AddDynamic(this, &AArchVizController::OnLocationXChanged);
			RoadWidget->Location_Y->OnValueChanged.AddDynamic(this, &AArchVizController::OnLocationYChanged);
		}
	}
	if (HomeWidgetClassRef) {
		HomeWidget = CreateWidget<UHomeWidget>(this, HomeWidgetClassRef);
		HomeWidget->AddToViewport();
	}
	if (HomeWidget) {
		HomeWidget->RoadConstruction->OnClicked.AddDynamic(this, &AArchVizController::OnRoadConstructionPressed);
	}

	if (WallActorClassRef) {
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		WallActor = GetWorld()->SpawnActor<AWallGenerator>(WallActorClassRef, FVector::ZeroVector, FRotator::ZeroRotator, Params);
		WallActor->GenerateWall();
	}
}

void AArchVizController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(bBuildingConstructionMode){
		WallProjectionOnTick();
	}
}

void AArchVizController::WallProjectionOnTick()
{
	FCollisionQueryParams TraceParams(FName(TEXT("LineTrace")), true);
	FVector CursorWorldLocation;
	FVector CursorWorldDirection;

	DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection);
	if (GetWorld()->LineTraceSingleByChannel(WallHitResult, CursorWorldLocation, CursorWorldLocation + CursorWorldDirection * 10000, ECC_Visibility, TraceParams))
	{
		FVector ResultedLocation = WallHitResult.Location;
		WallActor->SetActorLocation(ResultedLocation);
		
	}
}

void AArchVizController::SetupInputComponent()
{
	Super::SetupInputComponent();

	SetRoadConstructionMapping();	
}

void AArchVizController::GetRoadLocationOnClick()
{
	if(!bEditorMode){
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
		RoadConstructor->GenerateRoad(RoadDimensions, FVector(0, 0, RoadDimensions.Z/2));
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
	if (GetLocalPlayer()) {
		UEnhancedInputLocalPlayerSubsystem* SubSystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		if (SubSystem) {
			SubSystem->ClearAllMappings();
			SubSystem->AddMappingContext(RoadMapping, 0);
		}
	}
	if (RoadWidget) {
		RoadWidget->AddToViewport(); 
	}
}

void AArchVizController::OnLocationXChanged(float Value)
{
	if(RoadConstructor){
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
	if(!bEditorMode){
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
	if(RoadConstructor){RoadConstructor->SetActorScale3D(FVector(1, Value / RoadDimensions.Y,1));}
}


//if (bEditorMode) {
//	OnEditorModePressed();
//}