using UnrealBuildTool;

public class ViewportDebugEditor : ModuleRules
{
    public ViewportDebugEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore", 
                "ViewportDebugger", 
                "JsonUtilities",
                "Json",
                "UnrealEd",
                "ApplicationCore",
#if UE_5_0_OR_LATER
#else
                "EditorStyle", 
#endif
            }
        );
    }
}