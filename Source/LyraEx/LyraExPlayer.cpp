#include "LyraExPlayer.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Abilities/Attribute/LyraAttributeSetBase.h"
#include "Abilities/TPAbilitySystemComponent/TPAbilitySystemComponent.h"
#include "Component/ItemInteractionComponent/TPItemInteractionComponent_Player.h"
#include "Component/WeaponManagerComponent/WeaponManagerComponent.h"
#include "Config/WeaponAbilityInputConfig.h"

UE_DISABLE_OPTIMIZATION

ALyraExPlayer::ALyraExPlayer()
{
	TPAbilitySystemComponent=CreateDefaultSubobject<UTPAbilitySystemComponent>(TEXT("TPAbilitySystemComponent"));
	TPAbilitySystemComponent->SetIsReplicated(true);
	TPAbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	TPAbilitySystemComponent->SetMontageRepAnimPositionMethod(ERepAnimPositionMethod::CurrentSectionId);

	WeaponManagerComponent=CreateDefaultSubobject<UWeaponManagerComponent>("WeaponManagerComponent");
	ItemInteractionComponent=CreateDefaultSubobject<UTPItemInteractionComponent_Player>("ItemInteractionComponent");
}

void ALyraExPlayer::BeginPlay()
{
	Super::BeginPlay();
	AttributeSet=NewObject<ULyraAttributeSetBase>(this,ULyraAttributeSetBase::StaticClass());
	if (HasAuthority())
	{
		if (GetAttribute())
		{
			TPAbilitySystemComponent->AddAttributeSetSubobject(GetAttribute());
		}
	}
}

void ALyraExPlayer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ALyraExPlayer::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();
	if (APlayerController* PlayerController=Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(AbilityInputMapping, 101);
		}
	}
}

void ALyraExPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (PlayerInputComponent&&ItemInteractionComponent)
	{
		ItemInteractionComponent->AddPlayerInput(Cast<UEnhancedInputComponent>(PlayerInputComponent));
	}
	if (WeaponInputConfig)
	{
		if (UEnhancedInputComponent* EnhancedInputComponent=Cast<UEnhancedInputComponent>(PlayerInputComponent))
		{
			for (const auto&TmpAction:WeaponInputConfig->AbilityInputActions)
			{
				EnhancedInputComponent->BindAction(TmpAction.InputAction,ETriggerEvent::Triggered,this,&ThisClass::Input_AbilityInputTagPressed,TmpAction.InputTag);
				EnhancedInputComponent->BindAction(TmpAction.InputAction,ETriggerEvent::Completed,this,&ThisClass::Input_AbilityInputTagReleased,TmpAction.InputTag);
			}
		}
	}
}

ULyraAttributeSetBase* ALyraExPlayer::GetAttribute()
{
	return AttributeSet;
}

UTPAbilitySystemComponent* ALyraExPlayer::GetAbilitySystemComponent() const
{
	return TPAbilitySystemComponent;
}

void ALyraExPlayer::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	TPAbilitySystemComponent->AbilityInputTagPressed(InputTag);
}

void ALyraExPlayer::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	TPAbilitySystemComponent->AbilityInputTagReleased(InputTag);
}
UE_ENABLE_OPTIMIZATION