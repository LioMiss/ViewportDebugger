#pragma once

#include "CoreMinimal.h"
#include "ViewportDebuggerObject.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"

#include"UObject/GCObject.h"

#include "ViewportDebuggerObject.h"


class SViewportDebuggerWindow : public SCompoundWidget, FGCObject
{
	UViewportInfoObject* EditorViewInfo;


public:
	SLATE_BEGIN_ARGS(SViewportDebuggerWindow) {}
	SLATE_END_ARGS()

	SViewportDebuggerWindow();

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	virtual FString GetReferencerName() const override;
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;

	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

private:
	TSharedPtr<class SVerticalBox> RequiredLayout;
	FString CameraDumpStr;
	FString TeleportCommandStr;
	FEditorViewportClient* ActiveViewport;
	
	FReply LoadCameraDumpFromStr() const;
	FReply SetValueToView() const;
	FReply LoadCameraDumpFromFile();
	FReply RecordCameraDump() const;
	FReply CopyTeleportCommand() const;
	FEditorViewportClient* GetActiveViewport() const; 

};
