/**********************************************************************
** Copyright (C) 2004 MurphyTalk
**
** This file is part of the MurphyTalk PinYin Chinese input method,
** which is for the Qtopia Environment.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
**
** murphytalk@gmail.com
** $Date: 2004/07/20 11:28:52 $
**
**********************************************************************/
#include "public.h"

std::string trim(std::string &str, char ch)
{
	if(str.length() == 0){
		return str;
	}

	int sPos = str.find_first_not_of(ch);
	int ePos = str.find_last_not_of(ch);
	if(sPos == -1){
		return str.erase();
	}

	return str.substr(sPos, ePos - sPos + 1);
}
/*
 * Revision history
 * 
 * $Log: public.cpp,v $
 * Revision 1.1  2004/07/20 11:28:52  Lu Mu
 * created
 *
 */
