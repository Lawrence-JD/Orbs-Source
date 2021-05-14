// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "Queue.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "IntQueue.generated.h"

/**
 *
 */
UCLASS(Blueprintable, BlueprintType)
class GUIDINGLIGHT_API UIntQueue : public UObject
{
	GENERATED_BODY()
private:
	TQueue<int32> _mainQueue;
public:
	int32 size = 0;

	UFUNCTION(BlueprintCallable, Category = "Function")
		bool Dequeue(int32& value);

	UFUNCTION(BlueprintCallable, Category = "Function")
		bool Enqueue(int32 value);

	UFUNCTION(BlueprintCallable, Category = "Function")
		void Empty();

	UFUNCTION(BlueprintPure, Category = "Function")
		bool IsEmpty();

	UFUNCTION(BlueprintPure, Category = "Function")
		int32 Size();

	UFUNCTION(BlueprintPure, Category = "Function")
		bool Peek(int32& value);
};