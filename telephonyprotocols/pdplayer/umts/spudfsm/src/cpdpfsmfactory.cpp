// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// PDP FSM factory implementation
// 
//

/**
 @file 
 @internalComponent
*/
 
#include "cpdpfsmfactory.h"
#include "spudfsmdebuglogger.h"

// NewL 
CPdpFsmFactory* CPdpFsmFactory::NewL ()
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsmFactory::NewL()");
	
	return new (ELeave) CPdpFsmFactory ();	
    }

#if defined(__VC32__)
#if (_MSC_VER >= 1200)
#pragma warning(disable:4355) 
#endif
#endif
//Cpdpfsmfactory.cpp(22...) : warning C4355: 'this' : used in base member initializer list
// these warnings are benign unless the designed usage of SpudFsm changes such that SpudFsm is used
// as a base class to something else, in which case this code will have to change IFF the this pointer
// passed is used in the states c'tor. Currently, the pointer is just stored.
// In V9, e32def.h turns this warning off.

CPdpFsmFactory::CPdpFsmFactory()
: iStateInitialised (this),
  iStateOpeningPhone (this),
  iStateCreatingPrimary (this),
  iStateActivatingPrimary (this),
  iStateCreatingSecondary (this),
  iStateCreatedSecondary (this),
  iStateSettingQoS (this),
  iStateSettingTFT (this),
  iStateActivatingSecondary (this),
  iStateOpen (this),
  iStateChangingQoS (this),
  iStateChangingTFT (this),
  iStateGettingNegQoS (this),
  iStateModifyingActive (this),
  iStateSuspended (this),
  iStateClosing (this),
  iStateStopping (this),
  iStateCreatingMbms(this),
  iStateActivatingMbms(this),
  iStateCreatedMbms(this)
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsmFactory::CPdpFsmFactory()");
    }


CPdpFsmFactory::~CPdpFsmFactory()
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsmFactory::~CPdpFsmFactory()");

	iContexts.DeleteAll();
    iContexts.Reset();
	
    delete iEtelDriverInput;   
    }


void CPdpFsmFactory::InitL(const TName& aTsyName, CPdpFsmInterface * aPdpFsmInterface)
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsmFactory::InitL()");

	iPdpFsmInterface = aPdpFsmInterface;
	
	iTsyName = aTsyName;

	// lets kick off the ETelDriver first then we only alloc FSMs if it works
	//
	iEtelDriverInput = new (ELeave) REtelDriverInput;

	iEtelDriverInput->OpenL (*iPdpFsmInterface);

	// Only create PDP contexts if specifically requested.
    }


void CPdpFsmFactory::Close (void)
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsmFactory::Close()");
	// in OOM conditions iEtelDriveInput may not have successfully
	// been created, check here for safety.
	if (iEtelDriverInput != NULL)
	    {
        iEtelDriverInput->Close();
	    }
    }


/** 
@return may return NULL if there is no PDP context with that Id
*/
CPdpFsm* CPdpFsmFactory::GetFsmContext (TContextId aPdpId)
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsmFactory::GetContext()");

	return iContexts[aPdpId];
    }


const TName& CPdpFsmFactory::TsyName(void)
    {
	return iTsyName;
    }

#ifndef SYMBIAN_NON_SEAMLESS_NETWORK_BEARER_MOBILITY	
TInt CPdpFsmFactory::NewFsmContext(TContextId aPdpId)
#else
TContextId CPdpFsmFactory::NewFsmContextL(MPdpFsmEventHandler& aPdpFsmEventHandler,SpudMan::TPdpContextType aContextType)
#endif
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsmFactory::NewFsmContext()");

#ifndef SYMBIAN_NON_SEAMLESS_NETWORK_BEARER_MOBILITY
	TInt ret = KErrNone;
	ASSERT(ContextIdIsValid(aPdpId));
    if (!HaveFsmContext(aPdpId))
        {
	    // Only ever called by non-leaving functions. We're going to have to trap at some point in the calling hierarchy...
	    //
	    TRAP(ret,
	         CPdpFsm* p = CPdpFsm::NewL(aPdpId, this, iEtelDriverInput);
	         iContexts[aPdpId] = p;       
	         );
	    }
   return ret;
#else
	TInt i=0;
    while (i < KMaxPdpContexts && iContexts[i] != NULL)
    	{
	  	i++;
    	}
    if (i < KMaxPdpContexts)
        {
        CPdpFsm* p = CPdpFsm::NewL(i, this, iEtelDriverInput, aPdpFsmEventHandler, aContextType);
        iContexts[i] = p;
        iEtelDriverInput->CreatePdpL(i, aContextType);
        }
        return i;
#endif	
    }
   
TInt CPdpFsmFactory::DeleteFsmContext(TContextId aPdpId)
    {
	SPUDFSMVERBOSE_FNLOG("CPdpFsmFactory::DeleteFsmContext()");
	ASSERT(ContextIsValid(aPdpId));

    delete iContexts[aPdpId];
    iContexts[aPdpId] = NULL;
    
	return KErrNone;
    }
    
