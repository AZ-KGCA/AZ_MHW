// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AZ_MHW : ModuleRules
{
	public AZ_MHW(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;//미리 컴파일된 헤더사용
		ShadowVariableWarningLevel = WarningLevel.Error;//그림자 변수 경고 에러처리
		bEnforceIWYU = true;//강제 IWYU옵션
		
		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",			//표준라이브러리모듈	//모듈 매니저, IO, 컨테이너, 쓰레드, 네트워크, 수학라이브러리, RHI, 블루프린트, 기본클래스(UObject)
			"CoreUObject",	//객체지향모듈		//상속, 리플렉션, 직렬화, 메모리관리, 참조, 인터페이스
			"Engine",		//엔진모듈			//오디오(UAudioComponent), 물리(UPhysicsAsset), 애니메이션(UAnimInstance), 렌더(UWolrd), UI(UUserWidget), 레벨(ULevel, AActor)
			//
			"InputCore",	//입력모듈			//입력이벤트, 이벤트호출, 바인딩, 입력모드(GameOnly, UIOnly...)
			"EnhancedInput",//향상입력모듈		//모듈간의 입력, 장비간의 입력을 통합 개선 입력
			//"HeadMountedDisplay",//VR,AR모듈	//HoloLens용
			
			"Json",
			"JsonUtilities",

			"NavigationSystem",//네비게이션모듈	//
			"AIModule",//BVTree, BlackBoard		//
			"GameplayTasks",//AI				//
			"RenderCore",//Graphic, Option?		//
			
			"Networking",//TCP,UDP				//
			"Sockets",//Socket					//
			"AssetRegistry"//AssetManager		//
			
		});
		PrivateDependencyModuleNames.AddRange(new string[] {
			"Slate",
			"SlateCore",
			"UMG",
		});
		DynamicallyLoadedModuleNames.AddRange(new string[] {
		});

		//PublicIncludePaths.AddRange(new string[] {}); //이 설정은 이제 사용되지 않는다.

		PrivateIncludePaths.AddRange(new string[] {
			"AZ_MHW",
		});//이 모듈의 cpp파일에 해당 경로를 추가한다.

		// 온라인 기능을 사용하는 경우 주석 해제
		// (Uncomment if you are using online features)
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// Online Subsystem Steam을 포함하려면 Enabled 속성을 true로 설정하여 uproject 파일의 플러그인 섹션에 추가하십시오.
		// (To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true)
	}
}