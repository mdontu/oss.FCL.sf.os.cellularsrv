// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef _TE_ETELSATPCMDS1A_H_
#define _TE_ETELSATPCMDS1A_H_

#include <etel.h>

class CTestSatPCmds1a : public CRSatTestStep
	{
public:
	CTestSatPCmds1a() ;
	~CTestSatPCmds1a(){} ;
	virtual enum TVerdict doTestStepL();

private:
	void ExtendedTest1L(TRequestStatus& reqStatus);
	void ExtendedTest2L(TRequestStatus& reqStatus);
	void ExtendedTest3L();
	};

#endif // _TE_ETELSATPCMDS1A_H_
