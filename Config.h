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
#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <map>

typedef std::map<std::string,std::string> CONFIG_VALUES;

class Config
{
public:
	Config(const char* config_file);
	std::string get_as_string(const char* key_name);
	int get_as_int(const char* key_name);
private:	
	CONFIG_VALUES m_values;
};
#endif
/*
 * Revision history
 * 
 * $Log: Config.h,v $
 * Revision 1.1  2004/07/20 11:28:52  Lu Mu
 * created
 *
 */
