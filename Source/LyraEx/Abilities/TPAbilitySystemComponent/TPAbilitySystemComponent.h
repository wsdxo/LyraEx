// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "TPAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup="Custom",BlueprintType,Blueprintable,meta=(BlueprintSpawnableComponent))
class LYRAEX_API UTPAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	void ClearAbilityInput();

	void ProcessAbilityInput(float DeltaTime);
protected:
	//当前帧按下释放的技能
	TArray<FGameplayAbilitySpecHandle>InputPressedSpecHandles;
	//当前帧松开按键释放的技能
	TArray<FGameplayAbilitySpecHandle>InputReleasedSpecHandles;
	//当前帧长按释放的技能
	TArray<FGameplayAbilitySpecHandle>InputHeldSpecHandles;
};
