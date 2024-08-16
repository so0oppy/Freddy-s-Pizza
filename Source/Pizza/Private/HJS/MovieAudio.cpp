// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/MovieAudio.h"
#include "MediaSoundComponent.h"
// Sets default values
AMovieAudio::AMovieAudio()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Audio = CreateDefaultSubobject<UMediaSoundComponent>(TEXT("Audio"));

}

// Called when the game starts or when spawned
void AMovieAudio::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMovieAudio::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

