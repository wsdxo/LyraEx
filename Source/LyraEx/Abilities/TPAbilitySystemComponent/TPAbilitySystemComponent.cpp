// Fill out your copyright notice in the Description page of Project Settings.


#include "TPAbilitySystemComponent.h"


void UTPAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec:ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability&&AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
			{
				InputPressedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.AddUnique(AbilitySpec.Handle);
			}
		}
	}
}

void UTPAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec:ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability&&AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
			{
				InputReleasedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.Remove(AbilitySpec.Handle);
			}
		}
	}
}

void UTPAbilitySystemComponent::ClearAbilityInput()
{
	InputPressedSpecHandles.Reset();
	InputHeldSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
}

void UTPAbilitySystemComponent::ProcessAbilityInput(float DeltaTime)
{
	static TArray<FGameplayAbilitySpecHandle>AbilitiesToActive;
	AbilitiesToActive.Reset();
	for (const FGameplayAbilitySpecHandle&SpecHandle:InputPressedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec=FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{		
				AbilitySpec->InputPressed=true;
				if (AbilitySpec->IsActive())
				{
					AbilitySpecInputPressed(*AbilitySpec);
				}
				else
				{
					AbilitiesToActive.AddUnique(AbilitySpec->Handle);
				}
			}
		}
	}
	for (const FGameplayAbilitySpecHandle& SpecHandle:AbilitiesToActive)
	{
		TryActivateAbility(SpecHandle);
	}
	for (const FGameplayAbilitySpecHandle&SpecHandle:InputReleasedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec=FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{		
				AbilitySpec->InputPressed=false;
				if (AbilitySpec->IsActive())
				{
					AbilitySpecInputReleased(*AbilitySpec);
				}
			}
		}
	}
	//释放技能缓存
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
}
