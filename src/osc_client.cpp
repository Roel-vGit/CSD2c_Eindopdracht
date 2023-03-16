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
*  File name     : osc_client.cpp
*  System name   : osc++ : Open Sound Control
* 
*  Description   : simple OSC client based on liblo
*
*
*  Author        : Marc_G
*  E-mail        : marcg@dinkum.nl
*
**********************************************************************/

#include <iostream>
#include <string>
#include <lo/lo.h>


int main()
{
lo_address target;
std::string user_input;
long fake_timestamp=0;
int x=10;
int y=25;
float freq=301.5;

  target = lo_address_new("localhost","7777");

  while(1)
  {
    std::cin >> user_input;
    if(user_input == "quit" || user_input == "q") break;

    lo_send(target,"/sound","siii",user_input.c_str(),fake_timestamp,1,42);
    lo_send(target,"/tactile","ii",fake_timestamp,42);
    lo_send(target,"/x","i",x);
    lo_send(target,"/y","i",y);
    lo_send(target,"/freq","f",freq);

    fake_timestamp++;
  }

}
