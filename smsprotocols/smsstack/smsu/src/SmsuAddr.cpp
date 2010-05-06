// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Implements the class TSmsAddr
// 
//

/**
 @file
*/

#include "smsumain.h"
#include "smsuaddr.H"
#include "smsstacklog.h"


/**
 *  Constructor.
 *  @capability None
 */
EXPORT_C TSmsAddr::TSmsAddr()
	{
	SetSmsAddrFamily(ESmsAddrUnbound);
	}


/**
 *  Gets the SMS address family.
 *  
 *  @return SMS address family
 *  @capability None
 */
EXPORT_C TSmsAddrFamily TSmsAddr::SmsAddrFamily() const
	{
	LOGSMSU1("TSmsAddr::SmsAddrFamily()");

	return static_cast<TSmsAddrFamily>(Family());
	} // TSmsAddr::SmsAddrFamily


/**
 *  Sets the SMS address family.
 *  
 *  @param aFamily SMS address family
 *  @capability None
 */
EXPORT_C void TSmsAddr::SetSmsAddrFamily(TSmsAddrFamily aFamily)
	{
	LOGSMSU1("TSmsAddr::SetSmsAddrFamily()");

	SetFamily(static_cast<TUint>(aFamily));
	} // TSmsAddr::SetSmsAddrFamily


/**
 *  Gets the identifier match.
 *  
 *  @return Identifier match
 *  @capability None
 */
EXPORT_C TInt TSmsAddr::IdentifierMatch() const
	{
	LOGSMSU1("TSmsAddr::IdentifierMatch()");

	__ASSERT_DEBUG(SmsAddrFamily()==ESmsAddrMatchIEI,SmsuPanic(ESmsuPanicWrongSmsAddressFamily));
	return (TInt) Port();
	} // TSmsAddr::IdentifierMatch


/**
 *  Sets the identifier match.
 *  
 *  @param aIdentifier Identifier match
 *  @capability None
 */
EXPORT_C void TSmsAddr::SetIdentifierMatch(TInt aIdentifier)
	{
	LOGSMSU1("TSmsAddr::SetIdentifierMatch()");

	__ASSERT_DEBUG(SmsAddrFamily()==ESmsAddrMatchIEI,SmsuPanic(ESmsuPanicWrongSmsAddressFamily));
	SetPort((TUint) aIdentifier);
	} // TSmsAddr::SetIdentifierMatch


/**
 *  Gets the matched text.
 *  
 *  @return The match text
 *  @capability None
 */
EXPORT_C TPtrC8 TSmsAddr::TextMatch() const
	{
	LOGSMSU1("TSmsAddr::TextMatch()");

	__ASSERT_DEBUG(SmsAddrFamily()==ESmsAddrMatchText,SmsuPanic(ESmsuPanicWrongSmsAddressFamily));
	return TPtrC8(UserPtr(),const_cast<TSmsAddr*>(this)->GetUserLen());
	} // TSmsAddr::TextMatch


/**
 *  Sets the match text.
 *  
 *  @param aText The match text
 *  @capability None
 */
EXPORT_C void TSmsAddr::SetTextMatch(const TDesC8& aText)
	{
	LOGSMSU1("TSmsAddr::SetTextMatch()");

	__ASSERT_DEBUG(SmsAddrFamily()==ESmsAddrMatchText,SmsuPanic(ESmsuPanicWrongSmsAddressFamily));
	TUint8* target = UserPtr();
    TInt length = aText.Length()<=EMaxTextMatchLength ? aText.Length() : EMaxTextMatchLength;
	Mem::Copy(target,aText.Ptr(),length);
	SetUserLen(length);
	} // TSmsAddr::SetTextMatch


/**
 *  Equality operator.
 *  
 *  @param aAddr Address to compare with
 *  @return ETrue if the addresses match
 *  @capability None
 */
EXPORT_C TBool TSmsAddr::operator==(const TSmsAddr& aAddr) const
	{
	LOGSMSU1("TSmsAddr::operator=()");

	TSmsAddrFamily family=aAddr.SmsAddrFamily();
	TBool same=(SmsAddrFamily()==family);
	if (same)
		switch(family)
			{
		case ESmsAddrUnbound:
		case ESmsAddrSendOnly:
		case ESmsAddrLocalOperation:
			{
			same=EFalse;
			break;
			}
		case ESmsAddrRecvAny:
		case ESmsAddrMessageIndication:
		case ESmsAddrStatusReport:
		case ESmsAddrEmail:
			break;
		case ESmsAddrMatchIEI:
			{
			same=(IdentifierMatch()==aAddr.IdentifierMatch());
			break;
			}
		case ESmsAddrMatchText:
			{
			same=(!TextMatch().CompareF(aAddr.TextMatch()));
			break;
			}
		case ESmsAddrApplication8BitPort:
		case ESmsAddrApplication16BitPort:
			{
			same=(aAddr.Port()==Port());
			break;
			}
		default:
			SmsuPanic(ESmsuPanicAddrFamilyNotAllowed);
		}

	return same;
	} // TSmsAddr::operator=
