// Fill out your copyright notice in the Description page of Project Settings.


#include "LyraAttributeSetBase.h"

#include "Net/UnrealNetwork.h"

#define CHECK_OWNING_ASC_GAMEPLAYATTRIBUTE_NOTIFY(ClassName,PropertyName,OldValue,String)\
if(GetOwningAbilitySystemComponent())\
{\
	GAMEPLAYATTRIBUTE_REPNOTIFY(ClassName,PropertyName,OldValue);}\
else\
{\
	GEngine->AddOnScreenDebugMessage(-1,10.f,FColor::Red,String);\
}\

ULyraAttributeSetBase::ULyraAttributeSetBase():
	Super(),
	Level(1.f),
	Health(100.f),
	PhysicsAttack(10.f),
	Damage(0.f)
{
}

bool ULyraAttributeSetBase::PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data)
{
	return Super::PreGameplayEffectExecute(Data);
}

void ULyraAttributeSetBase::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}

void ULyraAttributeSetBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ULyraAttributeSetBase,Level);
	DOREPLIFETIME(ULyraAttributeSetBase,Health);
	DOREPLIFETIME(ULyraAttributeSetBase,PhysicsAttack);
}

void ULyraAttributeSetBase::OnRep_Level(const FGameplayAttributeData& OldValue)
{
	CHECK_OWNING_ASC_GAMEPLAYATTRIBUTE_NOTIFY(ULyraAttributeSetBase,Level,OldValue,TEXT("OnRep_Level Error"));
}

void ULyraAttributeSetBase::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	CHECK_OWNING_ASC_GAMEPLAYATTRIBUTE_NOTIFY(ULyraAttributeSetBase,Health,OldValue,TEXT("OnRep_Health Error"));
}

void ULyraAttributeSetBase::OnRep_PhysicsAttack(const FGameplayAttributeData& OldValue)
{
	CHECK_OWNING_ASC_GAMEPLAYATTRIBUTE_NOTIFY(ULyraAttributeSetBase,PhysicsAttack,OldValue,TEXT("OnRep_PhysicsAttack Error"));
}

void ULyraAttributeSetBase::OnRep_Damage(const FGameplayAttributeData& OldValue)
{
	CHECK_OWNING_ASC_GAMEPLAYATTRIBUTE_NOTIFY(ULyraAttributeSetBase,Damage,OldValue,TEXT("OnRep_Damage Error"));
}
