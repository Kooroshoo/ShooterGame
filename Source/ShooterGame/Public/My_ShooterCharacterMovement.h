// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/ShooterCharacterMovement.h"
#include "My_ShooterCharacterMovement.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API UMy_ShooterCharacterMovement : public UShooterCharacterMovement
{
	GENERATED_BODY()
	
		/*
	Moves are data that have all the information about some specific movement that the player has done.
	When a movement is performed locally we need also to create a Move, save it in memory and send it to the server to work properly with Replication.
	We perform the movement locally and we send the move to the server (that always command as it has authority) because it can correct the client if the information don't match or if there is delay.
	We save our move in memory so that if a correction from the server happens, we may want to replay all the moves in the pending list to let feel the game more fluid or to obey to the server.
	*/

	// We create our own Move from the standard that Unreal uses (FSavedMove_Character)
	class FSavedMove_My : public FSavedMove_Character
	{
	public:
		//So we can use Super::
		typedef FSavedMove_Character Super;

		// This function let us combine more moves together if we can to send and save less data.
		virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* Character, float MaxDelta) const override;

		// This function resets our move
		virtual void Clear() override;

		/*
		Moves are made of different flags (boolean values of one bit) that have the information on some specific movement from user input.
		We will use a custom one that Unreal gives us:
		enum ASCompressedFlags
		{
			MYFLAG_JumpPressed = FLAG_JumpPressed,
			MYFLAG_WantsCrouch = FLAG_WantsToCrouch,
			MYFLAG_WantsSprint = FLAG_Custom_0,
			MYFLAG_WantsThing1 = FLAG_Custom_1,
			MYFLAG_WantsThing2 = FLAG_Custom_2,
			MYFLAG_WantsThing3 = FLAG_Custom_3
		};
		*/
		// This function returns a byte containing those flags.
		virtual uint8 GetCompressedFlags() const override;

		// This function gets called when a move need to be replayed and puts the saved data from the Move to our Movement Component.
		virtual void PrepMoveFor(class ACharacter* Character) override;

		// This function is the opposite of the last one and saves the data from the Movement Component to the Move.
		virtual void SetMoveFor(ACharacter* Character, float InDeltaTime, FVector const& NewAccel, class FNetworkPredictionData_Client_Character& ClientData) override;

		// The location to where we were teleporting in this Move. The saved version of TeleportLocation.
		FVector SavedTeleportLocation;

		// The flag that tell us when we want to teleport, the Saved version of bWantsToTeleport
		uint8 bSavedWantsToTeleport : 1;
	};

	// We also need our version of the FNetworkPredictionData_Client_Character class to allocate our own Move.
	class FNetworkPredictionData_Client_My : public FNetworkPredictionData_Client_Character
	{
	public:
		// Constructor
		FNetworkPredictionData_Client_My(const UCharacterMovementComponent& ClientMovement);

		// So we can use Super::
		typedef FNetworkPredictionData_Client_Character Super;

		// This function allocates our Move, it basically return our Move that we have created before.
		virtual FSavedMovePtr AllocateNewMove() override;
	};

public:
	// Function called from the input binding in the character
	void Teleport();

private:
	// Function to decompress flags from a saved Move
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;

	// Get prediction data for a client game. Allocates the data on demand and can be overridden to allocate a custom override if desired.
	virtual class FNetworkPredictionData_Client* GetPredictionData_Client() const override;

	// RPC that will execute on the server, sending the location to which the character will teleport. We can validate if we want.
	UFUNCTION(Reliable, Server, WithValidation)
	void Server_SendTeleportLocation(FVector LocationToTeleport);

	// Function native of the standard Movement Component, this function is triggered at the end of a movement update.
	void OnMovementUpdated(float DeltaTime, const FVector& OldLocation, const FVector& OldVelocity) override;

	// Location to which the character will teleport
	FVector TeleportLocation;

	// We want to teleport 10m forward
	float TeleportOffset = 1000;

	// This boolean will trigger the movement in the OnMovementUpdated function native of the standard Movement Component
	// We use a single bit of a byte to fit this boolean in the FSavedMove_Character template we will override
	uint8 bWantsToTeleport : 1;
};


