// Copyright (C) 2023 Leap Studio All rights reserved.


#include "ViewportDebuggerWidget.h"

#include "ViewportDebuggerInfo.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CheatManager.h"
#include "GameFramework/Pawn.h"
#include "HAL/PlatformApplicationMisc.h"
#include "Kismet/KismetSystemLibrary.h"

void UViewportDebuggerWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	Button_Record->OnClicked.AddDynamic(this, &UViewportDebuggerWidget::OnRecordClick);
	Button_Load->OnClicked.AddDynamic(this, &UViewportDebuggerWidget::OnLoadClick);
}

void UViewportDebuggerWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	//GEngine->AddOnScreenDebugMessage(-1, -1.0, FColor::Green,FString::Printf(TEXT("[AverageTotalLatencyMs]: %f"), 11111.0));
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	const APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController == nullptr)
	{
		return;
	}
#if ENGINE_MAJOR_VERSION >= 5
	const APlayerCameraManager* CameraManager = PlayerController->PlayerCameraManager.Get();
#else
	const APlayerCameraManager* CameraManager = PlayerController->PlayerCameraManager;
#endif

	if(CameraManager == nullptr) return;
	CameraInfo_Text->SetText(FText::FromString(FString::Printf(TEXT("[Location]: %s\n[Rotation]: %s\n[FOV]: %f"), *CameraManager->GetCameraLocation().ToString(), *CameraManager->GetCameraRotation().ToString(),  CameraManager->GetFOVAngle())));
}

void UViewportDebuggerWidget::OnRecordClick()
{
	const APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController == nullptr)
	{
		return;
	}
	const APawn* Pawn = PlayerController->GetPawn<APawn>();
	if(!Pawn) return;
#if ENGINE_MAJOR_VERSION >= 5
	const APlayerCameraManager* CameraManager = PlayerController->PlayerCameraManager.Get();
#else
	const APlayerCameraManager* CameraManager = PlayerController->PlayerCameraManager;
#endif
	if(CameraManager == nullptr) return;
	FViewportInfoDump CameraDumpInfo;
	CameraDumpInfo.Location = CameraManager->GetCameraLocation();
	CameraDumpInfo.Rotation = CameraManager->GetCameraRotation();
	CameraDumpInfo.FOV = CameraManager->GetFOVAngle();
	CameraDumpInfo.PawnLocation = Pawn->GetActorLocation();
	CameraDumpInfo.PawnRotation = Pawn->GetActorRotation();
	CameraDumpInfo.MapName = GetWorld()->GetName();
	FString CameraString;
	if(CameraDumpInfo.Serialize(CameraString))
	{
		const FString OutputFile = FString::Printf(TEXT("CameraDump_%s_%s.json"), *CameraDumpInfo.MapName, *FDateTime::Now().ToString());
		FFileHelper::SaveStringToFile(CameraString, *OutputFile);
		FPlatformApplicationMisc::ClipboardCopy(CameraString.GetCharArray().GetData());
		UE_LOG(LogTemp, Log, TEXT("Camera Dump Info: %s"), *CameraString);
	}
}

void UViewportDebuggerWidget::OnLoadClick()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController == nullptr)
	{
		return;
	}
	APawn* Pawn = PlayerController->GetPawn<APawn>();
	if(!Pawn) return;
	//const IFirstPersonPawn* FirstPawn = Cast<IFirstPersonPawn>(Pawn);
	//if (!FirstPawn)
	//{
	//	return;
	//}
	//UCameraComponent* Camera1P = FirstPawn->GetFirstPersonCamera();
	//if (Camera1P == nullptr)
	//{
	//	return;
	//}
	FString CameraString;
	FPlatformApplicationMisc::ClipboardPaste(CameraString);
	FViewportInfoDump CameraDumpInfo;
	CameraDumpInfo.Deserialize(CameraString);
	//Camera1P->SetFieldOfView(CameraDumpInfo.FOV);
	FString TeleportCommandStr;
	FViewportInfoDump::GenTeleportCommandStr(CameraDumpInfo.PawnLocation, CameraDumpInfo.PawnRotation, TeleportCommandStr);
	UKismetSystemLibrary::ExecuteConsoleCommand(PlayerController, TeleportCommandStr, PlayerController);
}



