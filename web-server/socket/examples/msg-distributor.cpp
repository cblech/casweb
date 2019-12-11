/* 
   msg-distributor.cpp

   Copyright (C) 2002-2017 René Nyffenegger

   This source code is provided 'as-is', without any express or implied
   warranty. In no event will the author be held liable for any damages
   arising from the use of this software.

   Permission is granted to anyone to use this software for any purpose,
   including commercial applications, and to alter it and redistribute it
   freely, subject to the following restrictions:

   1. The origin of this source code must not be misrepresented; you must not
      claim that you wrote the original source code. If you use this source code
      in a product, an acknowledgment in the product documentation would be
      appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
      misrepresented as being the original source code.

   3. This notice may not be removed or altered from any source distribution.

   René Nyffenegger rene.nyffenegger@adp-gmbh.ch
*/

#include "../src/Socket.h"
#include <process.h>
#include <string>
#include <list>

typedef std::list<Socket*> socket_list;

socket_list g_connections;

unsigned __stdcall Connection(void* a) {
  Socket* s = (Socket*) a;

  g_connections.push_back(s);

  s->SendLine("Welcome to the Message Distributor");

  while (1) {
    std::string r = s->ReceiveLine();
    if (r.empty()) break;

    for (socket_list::iterator os =g_connections.begin();
                               os!=g_connections.end(); 
                               os++) {
      if (*os != s) (*os)->SendLine(r);
    }
  }

  g_connections.remove(s);

  delete s;

  return 0;
}

int altmain() {
  SocketServer in(2000,5);
  
  while (1) {
    Socket* s=in.Accept();
  
    unsigned ret;
    _beginthreadex(0,0,Connection,(void*) s,0,&ret);
  }
  
  return 0;
}
