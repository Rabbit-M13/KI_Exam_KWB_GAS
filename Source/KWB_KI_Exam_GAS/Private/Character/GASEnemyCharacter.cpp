// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GASEnemyCharacter.h"
#include "Components/WidgetComponent.h"
#include "GameAbilitySystem/AttributeSet/EnemyAttributeSet.h"
#include "UI/BarWidget.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"

AGASEnemyCharacter::AGASEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	EnemyAS = CreateDefaultSubobject<UEnemyAttributeSet>(TEXT("EnemyAttributeSet"));

	HealthWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthWidgetComp"));
	HealthWidgetComp->SetupAttachment(GetRootComponent());
	HealthWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	HealthWidgetComp->SetDrawAtDesiredSize(true);
	HealthWidgetComp->SetRelativeLocation(FVector(0, 0, 110.f));
}

void AGASEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// null인지 확인하고 계속 null이면 추가 조치 필요 => 괜찮았음
	const UEnemyAttributeSet* SetInASC =
		AbilitySystemComponent ? AbilitySystemComponent->GetSet<UEnemyAttributeSet>() : nullptr;

	UE_LOG(LogTemp, Warning, TEXT("[Enemy] EnemyAS=%s  ASCSet=%s"),
		*GetNameSafe(EnemyAS),
		*GetNameSafe(SetInASC));

	ApplyInitAttributes();
	CacheBar();
	BindHealth();
	RefreshHealth();

	// Debug
	UE_LOG(LogTemp, Warning, TEXT("[EnemyAS] %s"), EnemyAS ? TEXT("Valid") : TEXT("NULL"));
}

void AGASEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitializeAbilityActorInfo();

	UE_LOG(LogTemp, Warning, TEXT("[Enemy ASC] Self=%s"), *GetName());
	UE_LOG(LogTemp, Warning, TEXT("[Enemy ASC] Owner=%s Avatar=%s"),
		*GetNameSafe(AbilitySystemComponent->GetOwnerActor()),
		*GetNameSafe(AbilitySystemComponent->GetAvatarActor()));

	if (HasAuthority())
	{
		ApplyInitAttributes();   // GE_Enemy_InitAttributes 같은 것
	}

	CacheBar();
	BindHealth();
	RefreshHealth();
}

void AGASEnemyCharacter::ApplyInitAttributes()
{
	if (!HasAuthority()) return;
	if (!AbilitySystemComponent || !InitAttributesEffect) return;

	FGameplayEffectContextHandle Ctx = AbilitySystemComponent->MakeEffectContext();
	FGameplayEffectSpecHandle Spec = AbilitySystemComponent->MakeOutgoingSpec(InitAttributesEffect, 1.f, Ctx);
	if (Spec.IsValid())
	{
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
	}
}

void AGASEnemyCharacter::CacheBar()
{
	if (!HealthWidgetComp) return;

	HealthWidgetComp->InitWidget();

	HealthBar = Cast<UBarWidget>(HealthWidgetComp->GetUserWidgetObject());
}

void AGASEnemyCharacter::BindHealth()
{
	if (bBound) return;
	if (!AbilitySystemComponent) return;
	if (!EnemyAS) return;

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(EnemyAS->GetHealthAttribute())
		.AddUObject(this, &AGASEnemyCharacter::OnHealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(EnemyAS->GetMaxHealthAttribute())
		.AddUObject(this, &AGASEnemyCharacter::OnMaxHealthChanged);

	bBound = true;
}

void AGASEnemyCharacter::OnHealthChanged(const FOnAttributeChangeData& Data) { RefreshHealth(); }
void AGASEnemyCharacter::OnMaxHealthChanged(const FOnAttributeChangeData& Data) { RefreshHealth(); }

void AGASEnemyCharacter::RefreshHealth()
{
	if (!HealthBar || !EnemyAS) return;

	const int32 Cur = (int32)EnemyAS->GetHealth();
	const int32 Max = (int32)EnemyAS->GetMaxHealth();
	HealthBar->SetBarValues(Cur, Max);
}