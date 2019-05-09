// Fill out your copyright notice in the Description page of Project Settings.

#include "ApplicationController.h"
//#include "Synopticon.h"


void AApplicationController::BeginPlay()
{
	Super::BeginPlay();
	//ARadiantWebViewHUD* WebViewHUD = StaticCast<ARadiantWebViewHUD*>(GetHUD());
	//WebViewHUD->SetServerAdress(ASynOpticonState::GetServerAdress());
	//WebViewHUD->SetWAMPRouterAdress(ASynOpticonState::GetWAMPRouterAdress());
	//WebViewHUD->SetWAMPRealm(ASynOpticonState::GetWAMPRealm());
	/*UGlobalEventSystem* EventSystem = ASynOpticonState::GetGlobalEventSystem();
	EventSystem->OnEventUpdateServerAdress.AddDynamic(WebViewHUD, &ARadiantWebViewHUD::UpdateServerAdressCallBack);
	EventSystem->OnEventUpdateCurrentSOActor.AddDynamic(WebViewHUD, &ARadiantWebViewHUD::UpdateCurrentActorCallback);*/
}