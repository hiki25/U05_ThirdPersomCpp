// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#define PrintLine() {CLog::Log(__FUNCTION__, __LINE__);}

class THIRDPERSONCPP_API CLog
{
public:
	static void Print(int32 InValue, int32 InKey = -1, float InDuration = 10.f, FColor InColor = FColor :: Black);
	static void Print(float InValue, int32 InKey = -1, float InDuration = 10.f, FColor InColor = FColor :: Black);
	static void Print(const FString& InValue, int32 InKey = -1, float InDuration = 10.f, FColor InColor = FColor :: Black);
	static void Print(const FVector& InValue, int32 InKey = -1, float InDuration = 10.f, FColor InColor = FColor :: Black);
	static void Print(const FRotator& InValue, int32 InKey = -1, float InDuration = 10.f, FColor InColor = FColor :: Black);

	static void Log(int32 Invalue);
	static void Log(float Invalue);
	static void Log(const FString& Invalue);
	static void Log(const FVector& Invalue);
	static void Log(const FRotator& Invalue);
	static void Log(const UObject* InObject);
	static void Log(const FString& InFuncName, int32 InLineNumber);

};
