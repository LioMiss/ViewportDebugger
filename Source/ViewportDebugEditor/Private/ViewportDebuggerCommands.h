// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"

/**
 * 
 */
class VIEWPORTDEBUGEDITOR_API FViewportDebuggerCommands : public TCommands<FViewportDebuggerCommands>
{
public:

	FViewportDebuggerCommands()
#if ENGINE_MAJOR_VERSION >= 5
		: TCommands<FViewportDebuggerCommands>(TEXT("ViewDebugger"), NSLOCTEXT("Contexts", "ViewDebugger", "ViewDebugger"), NAME_None, FAppStyle::GetAppStyleSetName())
#else
	: TCommands<FViewportDebuggerCommands>(TEXT("ViewDebugger"), NSLOCTEXT("Contexts", "ViewDebugger", "ViewDebugger"), NAME_None, FEditorStyle::GetStyleSetName())
#endif
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
