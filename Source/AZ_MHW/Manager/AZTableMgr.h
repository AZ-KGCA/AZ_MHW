// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <typeinfo>
#include "AZ_MHW/GameSingleton/AZGameSingleton.h"
#include "AZTableMgr.generated.h"

#define GetTableByIndex(TableType, index) GetGameSingleton()->table_mgr->GetData<TableType>(index)
#define GetTable(TableType) GetGameSingleton()->table_mgr->GetData<TableType>();



UCLASS(Blueprintable, BlueprintType)
class AZ_MHW_API UAZTableMgr : public UObject
{
	GENERATED_BODY()
	
private:
	UPROPERTY() TArray<const UObject*> map_array_;
	TMap<uint32, TMap<int32, const UObject*>> map_table_;

public:
	template<class Table>
	bool _Load(FString file_name)
	{
		uint32 hash_code = typeid(Table).hash_code();
		if (map_table_.Contains(hash_code) == true)
		{
			return false;
		}

		//map_table_.Add(file_name, TMap<int32, const UObject*>());
		FString table_path = FString::Printf(TEXT("%sRawContent/Csv/%s"), *FPaths::ProjectDir(), *file_name);

		TMap<int32, const UObject*> data_container;
		if (!FPaths::FileExists(table_path))
		{
			return false;
		}

		TArray<FString> lines;
		TArray<FString> fields;
		TArray<int> except_index;
		int row = 0;
		FFileHelper::LoadFileToStringArrayWithPredicate(lines, *table_path, [](const FString& Line) { return Line.Len() > 0; });
		for (FString& line : lines)
		{
			TArray<FString> cols;
			line.ParseIntoArray(cols, TEXT(","), true);
			if (row == 0)
			{
				for (int32 i = 0; i < cols.Num(); ++i)
				{
					if (cols[i].StartsWith("~"))
					{
						except_index.Add(i);
					}
					else
					{
						fields.Add(cols[i]);
					}
				}
			}

			if (row++ < 2)
			{
				continue;
			}

			int32 offset = 0;
			for (const int32 index : except_index)
			{
				cols.RemoveAt(index - offset);
				++offset;
			}

			TMap<FString, FString> col_values;
			for (int32 i = 0; i < fields.Num(); ++i)
			{
				col_values.Add(fields[i], cols[i]);
			}

			Table* table = NewObject<Table>();
			table->Load(col_values);

			int32 key = FCString::Atoi(*cols[0]);
			if (data_container.Contains(key) == true)
			{
				return false;
			}
			map_array_.Add(table);
			data_container.Add(key, table);
		}
		
		map_table_.Add(hash_code, data_container);
		return true;
	}
	template<class Table>
	TArray<const Table*> GetData(TFunctionRef<bool (const Table*)> Match)
	{
		TArray<const Table*> result;
		uint32 hash_code = typeid(Table).hash_code();
		if (map_table_.Contains(hash_code) == false)
		{
			return result;
		}
		auto tables = map_table_.Find(hash_code);
		for (auto& table : *tables)
		{
			if (Match(Cast<Table>(table.Value)))
			{
				result.Add(Cast<Table>(table.Value));
			}
		}
		return result;
	}
	template<class Table>
	const Table* GetData(int32 key)
	{
		TArray<const Table*> result;
		uint32 hash_code = typeid(Table).hash_code();
		if (map_table_.Contains(hash_code) == false)
		{
			return nullptr;
		}
		auto tables = map_table_.Find(hash_code);
		auto table = (*tables).Find(key);
		if (table == nullptr)
		{
			return nullptr;
		}
		return Cast<Table>(*table);
	}

	template<class Table>
	TArray<const Table*> GetData()
	{
		TArray<const Table*> result;
		uint32 hash_code = typeid(Table).hash_code();
		if (map_table_.Contains(hash_code) == false)
		{
			return result;
		}
		auto tables = map_table_.Find(hash_cod);
		for (auto& table : *tables)
		{
			result.Add(Cast<Table>(table.Value));
		}
		return result;
	}
	void LoadAll();
	void LoadComplete();

};