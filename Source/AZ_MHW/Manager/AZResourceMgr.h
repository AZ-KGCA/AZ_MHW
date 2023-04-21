// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AZResourceMgr.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class AZ_MHW_API UAZResourceMgr : public UObject
{
	GENERATED_BODY()
	
};

namespace AZResourceHelper
{
	static void _VerifyObjectPath(const FString& path, FString& out)
	{
		const FString strCheckDot(".");
		if (path.Contains(strCheckDot) == false)
		{
			int lastSlashindex = 0;
			path.FindLastChar('/', lastSlashindex);
			FString fileName = path.Mid(lastSlashindex + 1);
			out = path + strCheckDot + fileName;
		}
		else
		{
			out = path;
		}
	}

	static void _VerifyClassPath(const FString& path, FString& out)
	{
		const FString strCheckDot(".");
		bool hasDot = path.Contains(strCheckDot);
		const FString strCheckClass("_C");
		_VerifyObjectPath(path, out);
		if (out.EndsWith(strCheckClass) == false || hasDot == false)
		{
			out = out + strCheckClass;
		}
		else
		{
			out = path;
		}
	}

	static UObject* LoadObjectFast(FString path)
	{
		UObject* outObject = FSoftObjectPath(path).TryLoad();
		if (IsValid(outObject) == false)
		{
			return nullptr;
		}

		return outObject;
	}

	static UObject* LoadObject(FString path)
	{
		FString loadPath;
		_VerifyObjectPath(path, loadPath);
		return LoadObjectFast(loadPath);
	}

	static UObject* LoadObject(const FSoftObjectPath& path)
	{
		UObject* outObject = path.TryLoad();
		if (IsValid(outObject) == false)
		{
			outObject = nullptr;
		}

		return outObject;
	}

	template<typename T> 
	static T* LoadObjectFast(FString path) 
	{ 
		return Cast<T>(LoadObjectFast(path)); 
	}
	template<typename T> 
	static T* LoadObject(FString path) 
	{ 
		return Cast<T>(LoadObject(path, cacheType)); 
	}
	template<typename T> 
	static T* LoadObject(const FSoftObjectPath& path) 
	{ 
		return Cast<T>(LoadObject(path, cacheType)); 
	}

	template<typename T>
	static UClass* LoadClassFast(FString path)
	{
		UClass* outClass = FSoftClassPath(path).TryLoadClass<T>();
		if (IsValid(outClass) == false)
		{
			return nullptr;
		}

		return outClass;
	}

	template<typename T>
	static UClass* LoadClass(FString path)
	{
		FString loadPath;
		_VerifyClassPath(path, loadPath);
		return LoadClassFast<T>(loadPath);
	}

	template<typename T>
	static UClass* LoadClass(const FSoftClassPath& path)
	{
		UClass* outClass = path.TryLoadClass<T>();
		if (IsValid(outClass) == false)
		{
			return nullptr;
		}

		return outClass;
	}
}
