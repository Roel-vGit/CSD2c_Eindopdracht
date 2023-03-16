/**********************************************************************
*          Copyright (c) 2023, Hogeschool voor de Kunsten Utrecht
*                      Utrecht, the Netherlands
*                          All rights reserved
***********************************************************************
*  This program is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program.
*  If not, see <http://www.gnu.org/licenses/>.
***********************************************************************
*
*  File name     : osc_server.cpp
*  System name   : osc++ : Open Sound Control
* 
*  Description   : Demo program using C++ wrapper for liblo
*
*
*  Author        : Marc_G
*  E-mail        : marcg@dinkum.nl
*
**********************************************************************/

#include "../libs/osc.h"


// subclass OSC into a local class so we can provide our own callback
class localOSC : public OSC
{
  int realcallback(const char *path,const char *types,lo_arg **argv,int argc)
  {
  std::string msgpath=path; // convert char* to std::string

    if(!msgpath.compare("/sound")){
      char * user_input = (char *)argv[0];
      int int1 = argv[1]->i;
      int int2 = argv[2]->i;
      int int3 = argv[2]->i;
      std::cout << "Message: " <<
        user_input << " " <<
        int1 << " " <<
        int2 << " " <<
        int3 << " " << std::endl;
    } // if
    if(!msgpath.compare("/tactile")){
      int int1 = argv[0]->i;
      int int2 = argv[1]->i;
      std::cout << "Message: " <<
        int1 << " " <<
        int2 << " " << std::endl;
    }
    if(!msgpath.compare("/x")){
      int x = argv[0]->i;
      std::cout << "Message: " << x << std::endl;
    }if(!msgpath.compare("/y")){
      int y = argv[0]->i;
      std::cout << "Message: " << y << std::endl;
    } // if
    if(!msgpath.compare("/freq")){
      float freq = argv[0]->f;
      std::cout << "Message: " << freq << std::endl;
    } // if

    return 0;
  } // realcallback()
};



int main()
{
int done = 0;
localOSC osc;
std::string serverport="7777";

  osc.init(serverport);
  osc.set_callback("/sound","siii");
  osc.set_callback("/tactile","ii");
  osc.set_callback("/x","i");
  osc.set_callback("/y","i");
  osc.set_callback("/freq","f");

  osc.start();
  std::cout << "Listening on port " << serverport << std::endl;

  while (!done) 
  {
    usleep(1000);
  }

  return 0;
}
