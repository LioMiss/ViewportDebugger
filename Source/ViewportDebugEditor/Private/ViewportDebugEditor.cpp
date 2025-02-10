#include "ViewportDebugEditor.h"

#include "LevelEditor.h"
#include "SViewportDebuggerWindow.h"
#include "ViewportDebuggerCommands.h"
#include "ViewportDebuggerStyle.h"

static const FName EditorTabName("ViewportDebugger");
#define LOCTEXT_NAMESPACE "FViewportDebugEditorModule"

void FViewportDebugEditorModule::StartupModule()
{
	FViewportDebuggerStyle::Initialize();
	FViewportDebuggerStyle::ReloadTextures();

	FViewportDebuggerCommands::Register();
	PluginCommands = MakeShareable(new FUICommandList);
	PluginCommands->MapAction(
		FViewportDebuggerCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FViewportDebugEditorModule::ShowDebugInfoWindow),
		FCanExecuteAction());
	
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(EditorTabName,
		 FOnSpawnTab::CreateRaw(this, &FViewportDebugEditorModule::OnSpawnPluginTab)).SetMenuType(ETabSpawnerMenuType::Hidden);

	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands,
									   FMenuExtensionDelegate::CreateRaw(
										   this, &FViewportDebugEditorModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}
}

void FViewportDebugEditorModule::ShutdownModule()
{
    
}

TSharedRef<SDockTab> FViewportDebugEditorModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SViewportDebuggerWindow)
		];
}

void FViewportDebugEditorModule::ShowDebugInfoWindow()
{
#ifdef UE_5_0_OR_LATER
	FGlobalTabmanager::Get()->TryInvokeTab(EditorTabName);
#else
	FGlobalTabmanager::Get()->TryInvokeTab(EditorTabName);
#endif
}

void FViewportDebugEditorModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FViewportDebuggerCommands::Get().PluginAction);
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FViewportDebugEditorModule, ViewportDebugEditor)