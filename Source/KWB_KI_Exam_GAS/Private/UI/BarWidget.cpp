// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

UBarWidget::UBarWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//UE_LOG(LogTemp, Error, TEXT("UBarWidget!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
}

void UBarWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	ApplyStyle();
	Refresh();
}

void UBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ApplyStyle();
	Refresh();
}

void UBarWidget::SetBarValues(int32 InCurrent, int32 InMax)
{
	MaxNumeric = FMath::Max(0, InMax);
	CurrentNumeric = FMath::Clamp(InCurrent, 0, MaxNumeric);

	Refresh();

	UE_LOG(LogTemp, Warning, TEXT("SetBarValues: %d/%d, PB=%s CurTxt=%s MaxTxt=%s"),
		InCurrent, InMax,
		PB_Bar ? TEXT("Valid") : TEXT("NULL"),
		CurrentValue ? TEXT("Valid") : TEXT("NULL"),
		MaxValue ? TEXT("Valid") : TEXT("NULL"));
}

void UBarWidget::SetBarColors(const FLinearColor& InFill, const FLinearColor& InBackground, const FLinearColor& InText)
{
	FillColor = InFill;
	BackgroundColor = InBackground;
	TextColor = InText;

	ApplyStyle();
}

void UBarWidget::SetShowText(bool bInShowText)
{
	bShowText = bInShowText;
	Refresh();
}

float UBarWidget::GetPercent() const
{
	if (MaxNumeric <= 0) return 0.f;
	return FMath::Clamp(static_cast<float>(CurrentNumeric) / static_cast<float>(MaxNumeric), 0.f, 1.f);
}

void UBarWidget::ApplyStyle()
{
	if (PB_Bar)
	{
		PB_Bar->SetFillColorAndOpacity(FillColor);
		PB_Bar->WidgetStyle.BackgroundImage.TintColor = FSlateColor(BackgroundColor);
		PB_Bar->SynchronizeProperties();
	}

	const FSlateColor SlateText(TextColor);

	if (CurrentValue)
	{
		CurrentValue->SetColorAndOpacity(SlateText);
	}
	if (MaxValue)
	{
		MaxValue->SetColorAndOpacity(SlateText);
	}
}

void UBarWidget::Refresh()
{
	if (PB_Bar)
	{
		PB_Bar->SetPercent(GetPercent());
	}

	const ESlateVisibility Vis = bShowText ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed;

	if (CurrentValue)
	{
		CurrentValue->SetVisibility(Vis);
		if (bShowText)
		{
			CurrentValue->SetText(FText::AsNumber(CurrentNumeric));
		}
	}

	if (MaxValue)
	{
		MaxValue->SetVisibility(Vis);
		if (bShowText)
		{
			MaxValue->SetText(FText::AsNumber(MaxNumeric));
		}
	}
}