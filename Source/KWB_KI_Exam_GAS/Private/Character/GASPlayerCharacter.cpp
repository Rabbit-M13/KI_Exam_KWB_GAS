// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GASPlayerCharacter.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "GameAbilitySystem/AttributeSet/PlayerAttributeSet.h"


AGASPlayerCharacter::AGASPlayerCharacter()
{
	PlayerAttributeSet = CreateDefaultSubobject<UPlayerAttributeSet>(TEXT("PlayerAttributeSet"));

    //// Debug
    //if (PlayerAttributeSet)
    //{
    //    UE_LOG(LogTemp, Warning, TEXT("[%s] Mana Cur=%.1f Base=%.1f | MaxMana Cur=%.1f Base=%.1f"),
    //        *GetName(),
    //        PlayerAttributeSet->Mana.GetCurrentValue(),
    //        PlayerAttributeSet->Mana.GetBaseValue(),
    //        PlayerAttributeSet->MaxMana.GetCurrentValue(),
    //        PlayerAttributeSet->MaxMana.GetBaseValue());
    //}
}

void AGASPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
    CreateHUD();
}

void AGASPlayerCharacter::InitializeAbilityActorInfo()
{
	Super::InitializeAbilityActorInfo();

    //// Debug
    //UE_LOG(LogTemp, Warning, TEXT("[%s][%s] InitializeAbilityActorInfo called. Role=%d Auth=%d"),
    //    *GetName(), *GetClass()->GetName(), (int32)GetLocalRole(), HasAuthority());

	ApplyInitAttributes();

    GrantAbilities();

    BindMana();
    RefreshMana();
}

void AGASPlayerCharacter::CreateHUD()
{
    if (!IsLocallyControlled() || MainHUD) return;
    if (!MainHUDClass) return;

    MainHUD = CreateWidget<UMainHUDWidget>(GetWorld(), MainHUDClass);
    if (MainHUD) MainHUD->AddToViewport();

    RefreshMana();
}

void AGASPlayerCharacter::BindMana()
{
    if (bManaBound) return;
    if (!IsLocallyControlled()) return;
    if (!AbilitySystemComponent || !PlayerAttributeSet) return;

    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
        PlayerAttributeSet->GetManaAttribute()).AddUObject(this, &AGASPlayerCharacter::OnManaChanged);

    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
        PlayerAttributeSet->GetMaxManaAttribute()).AddUObject(this, &AGASPlayerCharacter::OnMaxManaChanged);

    bManaBound = true;
}

void AGASPlayerCharacter::OnManaChanged(const FOnAttributeChangeData& Data) { RefreshMana(); }
void AGASPlayerCharacter::OnMaxManaChanged(const FOnAttributeChangeData& Data) { RefreshMana(); }

void AGASPlayerCharacter::RefreshMana()
{
    if (!MainHUD || !PlayerAttributeSet) return;

    const int32 Cur = FMath::RoundToInt(PlayerAttributeSet->GetMana());
    const int32 Max = FMath::RoundToInt(PlayerAttributeSet->GetMaxMana());

    MainHUD->SetMana(Cur, Max);
}

void AGASPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAction(TEXT("Fireball"), IE_Pressed, this, &AGASPlayerCharacter::Input_FireballPressed);
}

void AGASPlayerCharacter::Input_FireballPressed()
{
    if (!IsLocallyControlled()) return;
    if (!AbilitySystemComponent) return;
    if (!FireballAbilityClass) return;

    AbilitySystemComponent->TryActivateAbilityByClass(FireballAbilityClass);
}

void AGASPlayerCharacter::GrantAbilities()
{
    if (!HasAuthority()) return;
    if (bAbilitiesGranted) return;
    if (!AbilitySystemComponent) return;
    if (!FireballAbilityClass) return;

    AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(FireballAbilityClass, 1, 0));
    bAbilitiesGranted = true;
}

void AGASPlayerCharacter::ApplyInitAttributes()
{
    if (!AbilitySystemComponent || !InitAttributesEffect) return;
    if (!HasAuthority()) return;

    if (bInitAttributesApplied) return;

    FGameplayEffectContextHandle Context = AbilitySystemComponent->MakeEffectContext();
    Context.AddSourceObject(this);

    const FGameplayEffectSpecHandle SpecHandle =
        AbilitySystemComponent->MakeOutgoingSpec(InitAttributesEffect, 1.f, Context);

    if (SpecHandle.IsValid())
    {
        AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
        bInitAttributesApplied = true;
    }
}