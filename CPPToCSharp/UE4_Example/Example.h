// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Example.generated.h"

/**
 * 
 */
UCLASS()
class UE4_WEBSITETEST_API UExample : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "My DLL Library")
	static bool importDLL(FString folder, FString name);


	UFUNCTION(BlueprintCallable, Category = "My DLL Library")
	static int importGetCharSize();

	UFUNCTION(BlueprintCallable, Category = "My DLL Library")
	static FString importGetWebsiteContent(int index);


	UFUNCTION(BlueprintCallable, Category = "My DLL Library")
	static void importSetWebsiteLength(int i);

	UFUNCTION(BlueprintCallable, Category = "My DLL Library")
	static FString importPostToWebsite(FString c_fullpath, FString inputChar);

	UFUNCTION(BlueprintCallable, Category = "My DLL Library")
	static void freeDLL();

};
