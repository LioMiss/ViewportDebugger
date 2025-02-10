// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "UObject/Object.h"
#include "ViewportDebuggerWidget.generated.h"

/**
 * 
 */
UCLASS()
class VIEWPORTDEBUGGER_API UViewportDebuggerWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void NativeOnInitialized() override;

protected:

	UUserWidget* GetActiveWidgetInterface() const;

	UUserWidget* GetInteractionWidget() const;

	void UpdateTexts();

	void MoveToPreviousScreen();

	void MoveToNextScreen();

	void SetSelectedIndex(int NewIndex);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	int m_SelectedScreenIndex = 0;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CameraInfo_Text = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* Button_Record = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* Button_Load = nullptr;

	UFUNCTION()
	void OnRecordClick();
	UFUNCTION()
	void OnLoadClick();
};
