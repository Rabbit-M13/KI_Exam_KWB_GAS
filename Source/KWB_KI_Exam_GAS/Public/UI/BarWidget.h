// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BarWidget.generated.h"

class UProgressBar;
class UTextBlock;
/**
 * 
 */
UCLASS()
class KWB_KI_EXAM_GAS_API UBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UBarWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "BarWidget")
	void SetBarValues(int32 InCurrent, int32 InMax);

	UFUNCTION(BlueprintCallable, Category = "BarWidget")
	void SetBarColors(const FLinearColor& InFill, const FLinearColor& InBackground, const FLinearColor& InText);

	UFUNCTION(BlueprintCallable, Category = "BarWidget")
	void SetShowText(bool bInShowText);

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

	void ApplyStyle();
	void Refresh();

	float GetPercent() const;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> PB_Bar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CurrentValue;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MaxValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BarWidget|Style", meta = (ExposeOnSpawn = "true"))
	FLinearColor FillColor = FLinearColor(1.0f, 0.05f, 0.05f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BarWidget|Style", meta = (ExposeOnSpawn = "true"))
	FLinearColor BackgroundColor = FLinearColor(0.2f, 0.2f, 0.2f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BarWidget|Style", meta = (ExposeOnSpawn = "true"))
	FLinearColor TextColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BarWidget|Style", meta = (ExposeOnSpawn = "true"))
	bool bShowText = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BarWidget|Runtime")
	int32 CurrentNumeric = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BarWidget|Runtime")
	int32 MaxNumeric = 100;
};
