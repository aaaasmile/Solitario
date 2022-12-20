////////////////////////////////////////////////////////////////////////////////
// File         : cHightScoreMgr.cpp
// Project      :
// Subsystem    :
// Component    :
// Author       : Invido.it (c) 2004-2022
// Description  : implementation of the cMusicManager class.
////////////////////////////////////////////////////////////////////////////////

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

#include "cHightScoreMgr.h"

#include <stdlib.h>

#include "StdAfx.h"
#ifdef _WINDOWS
#include <io.h>
#include <sys/stat.h>
#include <sys/types.h>
#else
#include <unistd.h>
#endif
#include <fcntl.h>

using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cHightScoreMgr::cHightScoreMgr() {
}

cHightScoreMgr::~cHightScoreMgr() {
}

////////////////////////////////////////
//       Save
/*! Save hight score in file
 */
void cHightScoreMgr::Save() {
#ifdef WIN32
  int f;
  char buffer[16];
  unsigned int score, k, nb;
  string name;

  f = open("scores.bin", O_TRUNC | O_WRONLY | O_CREAT, S_IREAD | S_IWRITE);

  if (f > 0) {
    for (k = 0; k < 10; k++) {
      score = HS_Scores[k];
      memset(buffer, 0, 16);
      memcpy(buffer, HS_Names[k].c_str(), 15);
      nb = write(f, &score, 4);
      nb = write(f, buffer, 15);
    }
    close(f);
  }
#endif
}

////////////////////////////////////////
//       Load
/*! Load hight scores from file
 */
void cHightScoreMgr::Load() {
  int f;
  char buffer[16];
  unsigned int score, k;
  string name;

  f = open("scores.bin", O_RDONLY);
  if (f > 0) {
    for (k = 0; k < 10; k++) {
      if (read(f, &score, 4) == 0)
        score = 0;
      if (read(f, buffer, 15) == 0)
        name = "";
      else
        name = buffer;
      HS_Scores[k] = score;
      HS_Names[k] = name;
    }
    close(f);
  } else {
    for (k = 0; k < 10; k++) {
      HS_Scores[k] = (10 - k) * 1000;
      HS_Names[k] = "Re dal sulitari";
    }
  }
}
