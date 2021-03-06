// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
//

#ifndef __TE_GPRS_TESTSERVER_H__
#define __TE_GPRS_TESTSERVER_H__

#include <test/testexecuteserverbase.h>

class CGprsTestServer : public CTestServer
	{
public:
	static CGprsTestServer* NewL();
	virtual CTestStep* CreateTestStep(const TDesC& aStepName);
	};

#endif // __TE_GPRS_TESTSERVER_H__
