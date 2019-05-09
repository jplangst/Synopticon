// Fill out your copyright notice in the Description page of Project Settings.

#include "GazeInputComponent.h"
//#include "Synopticon.h"



// Sets default values for this component's properties
UGazeInputComponent::UGazeInputComponent(){
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UGazeInputComponent::BeginPlay(){
	Super::BeginPlay();

	//Add an event binding so the input component can process Gaze Gestures as they are performed
	ASynOpticonState::GetGlobalEventSystem()->OnEventGazeGesture.AddDynamic(this, &UGazeInputComponent::ProcessGesture);
}


void UGazeInputComponent::ProcessGesture(GazeGestureEnum gazeGesture){
	//APawn* Owner = Cast<APawn>(GetOwner());
	//if (Owner){

	//	APlayerController* PC = Cast<APlayerController>(Owner->GetController());

	//	if (PC) {
	//		UCustomPlayerInput* CustomPlayerInput = Cast<UCustomPlayerInput>(PC->PlayerInput);
	//		URaycastComponent_Ours* RaycastComponent = Owner->FindComponentByClass<URaycastComponent_Ours>();
	//		URadiantWebViewInputComponent* WebViewInputComponent = nullptr;

	//		if (RaycastComponent){
	//			AGazeActor* GazeActor = RaycastComponent->GetInteractingActor();
	//			if (GazeActor){
	//				URadiantWebBrowserComponent* BrowserComponent = GazeActor->FindComponentByClass<URadiantWebBrowserComponent>();
	//				if (BrowserComponent)
	//				{
	//					WebViewInputComponent = BrowserComponent->WebViewInputComponent;
	//				}
	//				/*WebViewInputComponent = GazeActor->FindComponentByClass<URadiantWebViewInputComponent>();*/
	//			}
	//		}

	//		if (gazeGesture == GazeGestureEnum::GG_DoubleBlink){
	//			if (WebViewInputComponent){
	//				WebViewInputComponent->KeyEventDelegate.Broadcast(EKeys::LeftMouseButton, IE_Pressed);
	//				WebViewInputComponent->KeyEventDelegate.Broadcast(EKeys::LeftMouseButton, IE_Released);
	//			}
	//			if (CustomPlayerInput){
	//				CustomPlayerInput->InputKey(EKeys::LeftMouseButton, IE_Pressed, 100);
	//				CustomPlayerInput->InputKey(EKeys::LeftMouseButton, IE_Released, 100);
	//			}
	//		}		
	//		else{
	//			if (CustomPlayerInput){
	//				CustomPlayerInput->FlushPressedKeys();
	//			}
	//		}
	//	}
	//}
}
