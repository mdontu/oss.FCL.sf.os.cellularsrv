/**
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
*
*/



/**
 @file
*/
#if (!defined __TESTEMSODIIE_STEP_H__)
#define __TESTEMSODIIE_STEP_H__
#include <test/testexecutestepbase.h>
#include "Te_gsmu_emsSuiteStepBase.h"

class CtestemsodiieStep : public CTe_gsmu_emsSuiteStepBase
	{
public:
	virtual TVerdict doTestStepL();
	};


#endif
