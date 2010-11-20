/*
   Vimpc
   Copyright (C) 2010 Nathan Sweetman

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.

   main.cpp - handle command line options and run program 
   */

#include <getopt.h>
#include <iostream>

#include "project.hpp"
#include "vimpc.hpp"

#ifdef __DEBUG_ASSERT

#include <execinfo.h>

extern void assert_failed(const char * file, const char * function, int line)
{
   int const BufferSize = 128;
   void    * buffer[BufferSize];
   int       nptrs;

   endwin();
   std::cout << "ASSERTION FAILED: " << file << " in " << function << " on line " << line << std::endl << std::endl;

   nptrs = backtrace(buffer, BufferSize);
   backtrace_symbols_fd(buffer, nptrs, STDERR_FILENO);
   exit(1);
}

#endif

int main(int argc, char** argv)
{
   bool runVimpc     = true;
   int  option       = 0;
   int  option_index = 0;

   while (option != -1)
   {
      static struct option long_options[] =
      {
         {"bugreport",  no_argument, 0, 'b'},
         {"url",        no_argument, 0, 'u'},
         {"version",    no_argument, 0, 'v'}
      };

      option = getopt_long (argc, argv, "vub", long_options, &option_index);

      if (option != -1)
      {
         std::string output; 

         runVimpc  = false;

         if (option == 'b') 
         {
            output = Main::Project::BugReport();
         }
         else if (option == 'u')
         {
            output = Main::Project::URL();
         }
         else if (option == 'v')
         {
            output = Main::Project::Version();
         }

         std::cout << output << std::endl;
      }
   }

   if (runVimpc == true)
   {
      Main::Vimpc vimpc;
      vimpc.Run();
   }

   return 0;
}