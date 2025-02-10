#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FViewportDebugEditorModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
    
    TSharedRef<SDockTab> OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs);

private:
    TSharedPtr<class FUICommandList> PluginCommands;
    
    void ShowDebugInfoWindow();
    void AddMenuExtension(FMenuBuilder& Builder);
    void AddToolbarExtension(FToolBarBuilder& Builder);
};
