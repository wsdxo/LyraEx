// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "LyraAttributeSetBase.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	float Last##PropertyName;\
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class LYRAEX_API ULyraAttributeSetBase : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	ULyraAttributeSetBase();
	
public:
	UPROPERTY(BlueprintReadOnly,Category="Attribute",ReplicatedUsing=OnRep_Level)
	FGameplayAttributeData Level;
	ATTRIBUTE_ACCESSORS(ULyraAttributeSetBase,Level)

	UPROPERTY(BlueprintReadOnly,Category="Attribute",ReplicatedUsing=OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(ULyraAttributeSetBase,Health)

	UPROPERTY(BlueprintReadOnly,Category="Attribute",ReplicatedUsing=OnRep_PhysicsAttack)
	FGameplayAttributeData PhysicsAttack;
	ATTRIBUTE_ACCESSORS(ULyraAttributeSetBase,PhysicsAttack)

	UPROPERTY(BlueprintReadOnly,Category="Attribute",ReplicatedUsing=OnRep_Damage)
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(ULyraAttributeSetBase,Damage)

public:
	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
protected:
	UFUNCTION()
	virtual void OnRep_Level(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_PhysicsAttack(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_Damage(const FGameplayAttributeData& OldValue);
};
