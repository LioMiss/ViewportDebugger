#include "SViewportDebuggerWindow.h"
#if ENGINE_MAJOR_VERSION >= 5
#include "DesktopPlatformModule.h"
#else
#include "DesktopPlatform/Public/DesktopPlatformModule.h"
#endif
#include"PropertyEditorModule.h"
#include"Modules/ModuleManager.h"
#include"Widgets/Layout/SBorder.h"
#include "Camera/CameraComponent.h"
#include"Editor.h"
#include"EditorViewportClient.h"
#include "ViewportDebuggerInfo.h"
#include "Camera/PlayerCameraManager.h"
#include "HAL/PlatformApplicationMisc.h"
#include "GameFramework/CheatManager.h"
#include "GameFramework/PlayerController.h"

SViewportDebuggerWindow::SViewportDebuggerWindow()
{
	EditorViewInfo = NewObject<UViewportInfoObject>(GetTransientPackage(), TEXT("ViewportInfoObject"));
	ActiveViewport = nullptr;
}

FString SViewportDebuggerWindow::GetReferencerName() const
{
	return TEXT("SLoadingScreen");
}


void SViewportDebuggerWindow::AddReferencedObjects(FReferenceCollector& Collector)
{
	Collector.AddReferencedObject(EditorViewInfo);
}

void SViewportDebuggerWindow::Construct(const FArguments& InArgs)
{
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	auto EditorViewInfo_Detail = PropertyEditorModule.CreateDetailView(FDetailsViewArgs(false, false, false, FDetailsViewArgs::HideNameArea));
	EditorViewInfo_Detail->SetObject(EditorViewInfo);

	
	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot().HAlign(HAlign_Fill).VAlign(VAlign_Fill).AutoHeight().Padding(FMargin(20.0f, 10.0f))
		[
			SNew(SBorder).HAlign(HAlign_Fill).VAlign(VAlign_Fill)
			[
				SAssignNew(RequiredLayout, SVerticalBox)
				+ SVerticalBox::Slot().HAlign(HAlign_Fill).VAlign(VAlign_Fill).Padding(FMargin(10.0f))
				[
					SNew(STextBlock).Text(FText::FromString("[Teleport Command Str]"))
				]
				+ SVerticalBox::Slot().HAlign(HAlign_Fill).VAlign(VAlign_Fill).AutoHeight()
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot().HAlign(HAlign_Fill).VAlign(VAlign_Fill)
					[
						SNew(SEditableTextBox).Text_Lambda([this](){return FText::FromString(TeleportCommandStr);})
					]
					+ SHorizontalBox::Slot().HAlign(HAlign_Fill).VAlign(VAlign_Fill).AutoWidth()
					[
						SNew(SButton).HAlign(HAlign_Left).VAlign(VAlign_Center)
							.Text(FText::FromString("Copy"))
							.OnClicked(this, &SViewportDebuggerWindow::CopyTeleportCommand)
					]
				]
				+ SVerticalBox::Slot().HAlign(HAlign_Fill).VAlign(VAlign_Fill).Padding(FMargin(10.0f))
				[
					SNew(STextBlock).Text(FText::FromString("[Camera Dump Json]"))
				]
				+ SVerticalBox::Slot().HAlign(HAlign_Fill).VAlign(VAlign_Fill).Padding(FMargin(10.0f))
				[
					SNew(SEditableTextBox).Text(FText::FromString("[Paste Here]"))
					.OnTextCommitted_Lambda([this](const FText& Text, ETextCommit::Type CommitType) {
						CameraDumpStr = Text.ToString();
					})
				]
				+ SVerticalBox::Slot().HAlign(HAlign_Fill).VAlign(VAlign_Fill).AutoHeight()
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot().HAlign(HAlign_Left).VAlign(VAlign_Fill).AutoWidth()
					[
						SNew(SButton).HAlign(HAlign_Center).VAlign(VAlign_Center)
						.Text(FText::FromString("Record"))
						.OnClicked(this, &SViewportDebuggerWindow::RecordCameraDump)
					]
					+ SHorizontalBox::Slot().HAlign(HAlign_Right).VAlign(VAlign_Fill).AutoWidth()
					[
						SNew(SButton).HAlign(HAlign_Center).VAlign(VAlign_Center)
						.Text(FText::FromString("Load"))
						.OnClicked(this, &SViewportDebuggerWindow::LoadCameraDumpFromStr)
					]
					+ SHorizontalBox::Slot().HAlign(HAlign_Right).VAlign(VAlign_Fill).AutoWidth()
					[
						SNew(SButton).HAlign(HAlign_Center).VAlign(VAlign_Center)
						.Text(FText::FromString("LoadFile"))
						.OnClicked(this, &SViewportDebuggerWindow::LoadCameraDumpFromFile)
					]
				]
			]	
		]
		+ SVerticalBox::Slot()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			[
				SNew(SBorder).BorderImage(FCoreStyle::Get().GetBrush("BoxShadow")).Padding(16)
				[
					EditorViewInfo_Detail
				]
			]
		]
		+ SVerticalBox::Slot().HAlign(HAlign_Fill).VAlign(VAlign_Fill).AutoHeight().Padding(FMargin(20.0f, 10.0f))
		[
			SNew(SBorder).HAlign(HAlign_Fill).VAlign(VAlign_Fill)
			[
				SAssignNew(RequiredLayout, SVerticalBox)
				+ SVerticalBox::Slot().HAlign(HAlign_Fill).VAlign(VAlign_Fill).Padding(FMargin(10.0f))
				[
					SNew(STextBlock).Text(FText::FromString("[Go To Set Value]"))
				]
				+ SVerticalBox::Slot().HAlign(HAlign_Fill).VAlign(VAlign_Fill).AutoHeight()
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot().HAlign(HAlign_Left).VAlign(VAlign_Center).AutoWidth()
					[
						SNew(SButton).HAlign(HAlign_Center).VAlign(VAlign_Center)
						.Text(FText::FromString("Go To"))
						.OnClicked(this, &SViewportDebuggerWindow::SetValueToView)
					]
				]
			]	
		]
	];
}


void SViewportDebuggerWindow::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	ActiveViewport = GetActiveViewport();
	if(ActiveViewport)
	{
		EditorViewInfo->ViewPosition = ActiveViewport->GetViewLocation();
		EditorViewInfo->ViewRotation = ActiveViewport->GetViewRotation();
		EditorViewInfo->FOV = ActiveViewport->ViewFOV;
		EditorViewInfo->ActiveMapName = ActiveViewport->GetWorld()->GetName();
	}
	else
	{
		const APlayerController* PlayerController = GEngine->GameViewport->GetWorld()->GetFirstPlayerController();
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
		EditorViewInfo->ViewPosition = CameraManager->GetCameraLocation();
		EditorViewInfo->ViewRotation = CameraManager->GetCameraRotation();
		EditorViewInfo->FOV = CameraManager->GetFOVAngle();
		EditorViewInfo->ActiveMapName = GWorld->GetName();
		EditorViewInfo->PawnPosition = Pawn->GetActorLocation();
		EditorViewInfo->PawnRotation = Pawn->GetActorRotation();
		FViewportInfoDump::GenTeleportCommandStr(EditorViewInfo->PawnPosition, EditorViewInfo->PawnRotation, TeleportCommandStr);
	}
}

FReply SViewportDebuggerWindow::LoadCameraDumpFromStr() const
{
	FViewportInfoDump CameraDumpInfo;
	if(!CameraDumpInfo.Deserialize(CameraDumpStr))
	{
		return FReply::Handled();
	}
	if(ActiveViewport)
	{
		ActiveViewport->SetViewLocation(CameraDumpInfo.Location);
		ActiveViewport->SetViewRotation(CameraDumpInfo.Rotation);
		ActiveViewport->ViewFOV = CameraDumpInfo.FOV;
	}
	else
	{
		APlayerController* PlayerController = GEngine->GameViewport->GetWorld()->GetFirstPlayerController();
		if(PlayerController && PlayerController->CheatManager)
		{
			PlayerController->CheatManager->BugItGo(CameraDumpInfo.PawnLocation.X, CameraDumpInfo.PawnLocation.Y, CameraDumpInfo.PawnLocation.Z,
				CameraDumpInfo.Rotation.Pitch, CameraDumpInfo.Rotation.Yaw, CameraDumpInfo.Rotation.Roll);
		}

		//PlayerController->CheatManager->BugItGo()
	}
	
	return FReply::Handled();
}

FReply SViewportDebuggerWindow::LoadCameraDumpFromFile()
{
	TArray<FString> OutFiles;
	const FString FileTypes = TEXT("All files (*.*)|*.*");

	if (FDesktopPlatformModule::Get()->OpenFileDialog(
		nullptr,
		TEXT("Open File"),
		FPaths::ProjectDir(),
		TEXT(""),
		FileTypes,
		EFileDialogFlags::None,
		OutFiles))
	{
		if (OutFiles.Num() > 0)
		{
			const FString SelectedFile = OutFiles[0];
			FFileHelper::LoadFileToString(CameraDumpStr, *SelectedFile);
			return LoadCameraDumpFromStr();
		}
	}
	return FReply::Handled();
}

FReply SViewportDebuggerWindow::RecordCameraDump() const
{
	FViewportInfoDump CameraDumpInfo;
	CameraDumpInfo.Location = EditorViewInfo->ViewPosition;
	CameraDumpInfo.Rotation = EditorViewInfo->ViewRotation;
	CameraDumpInfo.FOV = EditorViewInfo->FOV;
	CameraDumpInfo.MapName = EditorViewInfo->ActiveMapName;
	CameraDumpInfo.PawnLocation = EditorViewInfo->PawnPosition;
	CameraDumpInfo.PawnRotation = EditorViewInfo->PawnRotation;
	
	// const APlayerController* PlayerController = GEngine->GameViewport->GetWorld()->GetFirstPlayerController();
	// if(PlayerController && PlayerController->CheatManager)
	// {
	// 	PlayerController->CheatManager->BugIt(CameraDumpInfo.ScreenShotDescription);
	// }
	
	FString CameraDumpResult;
	if(CameraDumpInfo.Serialize(CameraDumpResult))
	{
		const FString OutputFile = FString::Printf(TEXT("CameraDump_%s_%s.json"), *CameraDumpInfo.MapName, *FDateTime::Now().ToString());
		FFileHelper::SaveStringToFile(CameraDumpResult, *OutputFile);
		FPlatformApplicationMisc::ClipboardCopy(CameraDumpResult.GetCharArray().GetData());
		UE_LOG(LogTemp, Log, TEXT("Camera Dump Info: %s"), *CameraDumpResult);
	}
	return FReply::Handled();
}

FReply SViewportDebuggerWindow::CopyTeleportCommand() const
{
	FPlatformApplicationMisc::ClipboardCopy(TeleportCommandStr.GetCharArray().GetData());
	return FReply::Handled();
}

FReply SViewportDebuggerWindow::SetValueToView() const
{
	const APlayerController* PlayerController = GEngine->GameViewport->GetWorld()->GetFirstPlayerController();
	if (PlayerController == nullptr)
	{
		return FReply::Handled();
	}
	const APawn* Pawn = PlayerController->GetPawn<APawn>();
	if (!Pawn) return FReply::Handled();
	// const IFirstPersonPawn* FirstPawn = Cast<IFirstPersonPawn>(Pawn);
	// if (FirstPawn)
	// {
	// 	const UCameraComponent* Camera1P = FirstPawn->GetFirstPersonCamera();
	// 	if (Camera1P)
	// 	{
	// 		auto Transform = Camera1P->GetComponentTransform();			
	// 		Transform.SetRotation(FRotator::ZeroRotator.Quaternion());
	// 	}
	// }
	//if(!ActiveViewport) return FReply::Handled();
	//ActiveViewport->SetViewLocation(EditorViewInfo->SetViewPosition);
	//ActiveViewport->SetViewRotation(EditorViewInfo->SetViewRotation);
	//ActiveViewport->ViewFOV = EditorViewInfo->SetFOV;
	return FReply::Handled();
}

FEditorViewportClient* SViewportDebuggerWindow::GetActiveViewport() const
{
	const int ViewportNum = GEditor->GetAllViewportClients().Num();
	int ActiveViewportIndex = -1;

	for (int32 ViewIndex = 0; ViewIndex < ViewportNum; ++ViewIndex)
	{
		const FEditorViewportClient* ViewportClient = GEditor->GetAllViewportClients()[ViewIndex];

		if (ViewportClient == GEditor->GetActiveViewport()->GetClient())
			ActiveViewportIndex = ViewIndex;
	}
	auto AllViewports = GEditor->GetAllViewportClients();
	if(ActiveViewportIndex == -1) return nullptr;
	EditorViewInfo->ActiveViewInfo = FString::FromInt(ActiveViewportIndex) + "(All:" + FString::FromInt(ViewportNum) + ")";
	return GEditor->GetAllViewportClients()[ActiveViewportIndex];
}


