#pragma once
#include "LyraExCharacter.h"
#include "LyraExPlayer.generated.h"


class UTPItemInteractionComponent_Player;
class UWeaponManagerComponent;
class ULyraAttributeSetBase;
class UTPAbilitySystemComponent;
struct FGameplayTag;
class UWeaponAbilityInputConfig;

UCLASS()
class ALyraExPlayer:public ALyraExCharacter
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=TPAbility,meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTPAbilitySystemComponent>TPAbilitySystemComponent;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=TPAbility,meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ULyraAttributeSetBase>AttributeSet;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* AbilityInputMapping;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = TPWeapon, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWeaponManagerComponent> WeaponManagerComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = TPItem, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTPItemInteractionComponent_Player>ItemInteractionComponent;
protected:
	UPROPERTY(EditDefaultsOnly,Category="Config|TPAbility")
	TObjectPtr<UWeaponAbilityInputConfig>WeaponInputConfig;

public:
	ALyraExPlayer();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaSeconds) override;

	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	ULyraAttributeSetBase* GetAttribute();

	virtual UTPAbilitySystemComponent* GetAbilitySystemComponent()const;

protected:
	void Input_AbilityInputTagPressed( FGameplayTag InputTag);
	void Input_AbilityInputTagReleased( FGameplayTag InputTag);
	
};
