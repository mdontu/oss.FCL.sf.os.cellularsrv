// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
// Implements the active object that controls the Read() requests.
// 
//

/**
 @file
*/

#include "Receiver.h"
#include "Constants.h"
#include <es_ini.h>

CReceiver::CReceiver(CBcaIoController& aObserver, CBttLogger* aTheLogger, TInt aMaxPacketSise)
/**
 * Constructor. Performs standard active object initialisation.
 *
 * @param aObserver Reference to the observer of this state machine
 * @param aTheLogger The logging object
 */
	: CActive(EPriorityHigh), 
	  iObserver(aObserver), 
	  iTheLogger(aTheLogger),
	  iMaxPacketSise(aMaxPacketSise)
	{	
	CActiveScheduler::Add(this);
	}

CReceiver* CReceiver::NewL(CBcaIoController& aObserver, CBttLogger* aTheLogger, TInt aMaxPacketSise)
/**
 * Two-phase constructor. Creates a new CBcaIoController object, performs 
 * second-phase construction, then returns it.
 *
 * @param aObserver The observer, to which events will be reported
 * @param aTheLogger The logging object
 * @return A newly constructed CBcaIoController object
 */
	{
	CReceiver* self = new (ELeave) CReceiver(aObserver, aTheLogger, aMaxPacketSise);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

void CReceiver::ConstructL()
/**
 * Second-phase constructor. Creates all the state objects it owns.
 */
	{
	_LOG_L1C1(_L8("CReceiver::ConstructL"));
	iData.CreateL(iMaxPacketSise);
	}

CReceiver::~CReceiver()
/**
 * Destructor.
 */
	{
	iData.Close();
	Cancel();
	}

void CReceiver::RunL()
/**
 *  Method called when read request completes. This will only be once the BCA has a full IP 
 *  packet in its buffer.
 */
	{
	_LOG_L1C2(_L8("CReceiver::RunL [iStatus=%d]"), iStatus.Int());

	if (iStatus!=KErrNone)
		{
		if(iStatus == KErrNoMemory)
			{
			_LOG_L2C1(
				_L8("WARNING! CReceiver: Read failed with KErrNoMemory"));
			// Read operation failed!! Nif will re-issue the read request.
			StartListening();
			}
		else 
			{
			_LOG_L2C1(_L8("WARNING! CReceiver: Read failed"));
			iObserver.Stop(iStatus.Int());
			}
		return;
		}

	_LOG_L1C1(_L8("CReceiver: Data Packet Received"));

    iRMBufPacket.CreateL(iData);
    iRMBufPacket.Pack();

    // Immediately execute new read request.
    StartListening();

#ifdef RAWIP_HEADER_APPENDED_TO_PACKETS
    TUint16 protocolCode = iObserver.RemoveHeader(iRMBufPacket);
#else
    TUint16 protocolCode = 0;
#endif // RAWIP_HEADER_APPENDED_TO_PACKETS

    // Process the packet
    iObserver.GetObserver().Process(iRMBufPacket, protocolCode);
    iRMBufPacket.Free();
	}

void CReceiver::DoCancel()
/**
 *	Cancel active request
 */
	{
	_LOG_L1C1(_L8("CReceiver::DoCancel"));

	(iObserver.Bca())->CancelRead(); 
	}

void CReceiver::StartListening()
/**
 *  This method issues a Read request. This is the API used to receive 
 *  packets from BCA.  
 */
	{
	_LOG_L1C1(_L8("CReceiver::StartListening"));

	// The BCA will complete this read once it has a full IP packet in its buffer.
	(iObserver.Bca())->Read(iStatus, iData);

	SetActive();
	}