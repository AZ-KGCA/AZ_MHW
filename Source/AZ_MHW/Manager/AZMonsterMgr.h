#pragma once
#include <CoreMinimal.h>
#include <UObject/NoExportTypes.h>
#include "AZ_MHW/CommonSource/AZStruct.h"
#include "AZMonsterMgr.generated.h"

UCLASS()
class UAZMonsterMgr : public UObject 
{
	GENERATED_BODY()
	
public:
	void Init();

private:
	// Asset loader function
	bool LoadBehaviorTreeAssets();
	
	// Monster table loader functions
	bool ConvertMonsterTable();
	bool ConvertBossTable();
	bool ConvertMonsterNonCombatActionTable();
	bool ConvertMonsterCombatActionTable();

public:
	// Monster table getter functions
	FMonsterInfo* GetMonsterInfo(const int32 monster_id);
	FBossInfo GetBossInfo(const int32 monster_id, const EBossRank rank);
	TMap<int32, FMonsterNonCombatActionInfo>* GetMonsterNonCombatActionInfo(const int32 monster_id);
	TMap<int32, FMonsterCombatActionInfo>* GetMonsterCombatActionInfo(const int32 monster_id);
	FAttackInfo* GetAttackInfo(const int32 action_id);
	
	// Behavior tree assets getter functions
	class UBehaviorTree* GetBehaviorTree(FName filename);

	// Utility functions
	FName GetMonsterName(int32 monster_id);
	
private:
	// Table maps
	TMap<int32, FMonsterInfo> monster_info_map_; 
	TMap<TTuple<int32, EBossRank>, FBossInfo> boss_info_map_; 
	TMap<int32, TMap<int32, FMonsterNonCombatActionInfo>> monster_noncombat_action_info_map_; 
	TMap<int32, TMap<int32, FMonsterCombatActionInfo>> monster_combat_action_info_map_;
	TMap<int32, FAttackInfo> monster_attack_info_map_;

	// Asset maps
	UPROPERTY() TMap<FName, UBehaviorTree*> behavior_tree_map_;
};
