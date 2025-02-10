// Copyright 2019 Lipeng Zha, Inc. All Rights Reserved.

#include "ViewportDebuggerCommands.h"

#define LOCTEXT_NAMESPACE "FDebugMenuEditor"

void FViewportDebuggerCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "ViewportDebugger", "ViewportDebugger", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
