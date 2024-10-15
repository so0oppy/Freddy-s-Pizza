// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Animatronics.generated.h"

UCLASS(Abstract)
class PIZZA_API AAnimatronics : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAnimatronics();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	// 포즈를 결정하는 함수, 자식 클래스는 이를 반드시 구현해야 함, 또한 여기서 포즈를 취하게 될 것.
	virtual void SetAnimatronicPose() PURE_VIRTUAL(AAnimatronics::SetAnimatronicPose, );

	// 애니메이션을 받아서 포즈를 취하는 함수
	//virtual void PoseAction(UAnimationAsset* AnimationAsset);

	USkeletalMeshComponent* SkeletalMeshComponent;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	
	// 1. 메시 : 캐릭터라 이미 있음.


	// 4. TMap<asdt ,dasjf> : 
};
