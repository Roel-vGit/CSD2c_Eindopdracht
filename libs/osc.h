/**********************************************************************
*          Copyright (c) 2013, Hogeschool voor de Kunsten Utrecht
*                      Hilversum, the Netherlands
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
*  File name     : osc.h
*  System name   : osc++ : Open Sound Control
* 
*  Description   : C++ wrapper for liblo
*
*
*  Author        : Marc_G
*  E-mail        : marcg@dinkum.nl
*
**********************************************************************/

#include <iostream>
#include <string>
#include <cstdlib>
#include <unistd.h>

#include <lo/lo.h>

using namespace std;

#pragma once


/*
 * class OSC creates a server listening to a certain port. For every
 *  port another server (thus object) must be created
 */
class OSC
{
public:
  OSC();
  void init(string serverport);
  void set_callback(const char *path,const char *types);
  void start();

  // realcallback is meant to be overridden in a subclass
  virtual int realcallback(const char *path, const char *types,lo_arg **argv,int argc);
private:
  static int _wrap_callback(const char *path,const char *types,
          lo_arg **argv,int argc,void *data,void *user_data);

  lo_server_thread server;
};

//OSC Server that receives messages
//--------------------------------------------------------------------------------

// subclass OSC into a local class so we can provide our own callback
class localOSC : public OSC
{


  int realcallback(const char *path,const char *types,lo_arg **argv,int argc)
  {
  std::string msgpath=path; // convert char* to std::string

    if(!msgpath.compare("/joystick1")){
      joystick1Xpos = argv[0]->f;
      joystick1Ypos = argv[1]->f;
        // std::cout << "Joystick 1: " <<
        // joystick1Xpos << " " <<
        // joystick1Ypos << " " << std::endl;
    } // if
    if(!msgpath.compare("/joystick2")){
      joystick2Xpos = argv[0]->f;
      joystick2Ypos = argv[1]->f;
      // std::cout << "Joystick 2: " <<
      //   joystick2Xpos << " " <<
      //   joystick2Ypos << " " << std::endl;
    }
    if(!msgpath.compare("/touchpad1")){
      touchPad1Xpos = argv[0]->f;
      touchPad1Ypos = argv[1]->f;
//      std::cout << "Touchpad 1: " <<
//        touchPad1Xpos << " " <<
//        touchPad1Ypos << " " << std::endl;
    } // if
    if(!msgpath.compare("/touchpad2")){
      touchPad2Xpos = argv[0]->f;
      touchPad2Ypos = argv[1]->f;
//      std::cout << "Touchpad 2: " <<
//      touchPad2Xpos << " " <<
//      touchPad2Ypos << " " << std::endl;
    }

    return 0;
  } // realcallback()

  public:
    float joystick1Xpos;
    float joystick1Ypos;
    float touchPad1Xpos;
    float touchPad1Ypos;

    float joystick2Xpos;
    float joystick2Ypos;
    float touchPad2Xpos;
    float touchPad2Ypos;

    int counter { 0 };

};