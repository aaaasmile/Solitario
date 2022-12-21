////////////////////////////////////////////////////////////////////////////////
// File         : main.cpp
// Project      : 
// Subsystem    : 
// Component    : 
// Author       : Igor Sarzi Sartori (c) 2004
// Description  : 
////////////////////////////////////////////////////////////////////////////////
// History      : 8.05.2004   Igor Sarzi Sartori    Created


/*
*
*	This program is free software; you can redistribute it and/or
*	modify it under the terms of the GNU General Public License
*	as published by the Free Software Foundation; either version 2
*	of the License, or (at your option) any later version.
*
*	This program is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License
*	along with this program; if not, write to the Free Software
*	Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*
*
**/


#include "StdAfx.h"
#include <stdio.h>
#include <stdlib.h>
#include <SDL_image.h>
#include "EngineApp.h"

cEngineApp TheApp;




////////////////////////////////////////
//       main
/*! 
// \param int argc : 
// \param char *argv[] : 
*/
int main(int argc, char *argv[])
{
    

    // app name and set the directory on installed application
#ifdef _WINDOWS
    std::string strAppName;
    strAppName = argv[0];
    int iPos = strAppName.find_last_of('\\'); 
    int iLen = strAppName.length(); 
    std::string strDirname = strAppName.substr(0, 38); 
    ::SetCurrentDirectory(strDirname.c_str() );
#endif

	TheApp.Setup(argc, argv); 
    TheApp.Init();

    
	TheApp.MainMenu(); 
    
    return EXIT_SUCCESS;
}
