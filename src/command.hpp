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

   commands.hpp - ex mode input handling 
   */

#ifndef __UI__COMMAND
#define __UI__COMMAND

#include <string>
#include <map>

#include "inputmode.hpp"
#include "player.hpp"
#include "screen.hpp"

namespace Main
{
   class Settings;
}

namespace Ui
{
   // Handles all input received whilst in command mode
   class Command : public InputMode, public Player
   {

   public:
      Command(Ui::Screen & screen, Mpc::Client & client, Main::Settings & settings);
      ~Command();

   public: // Ui::InputMode
      void InitialiseMode(int input);
      bool Handle(int input);
      void GenerateInputString(int const input);

   public:
      // Checks if there is any aliases defined for a command, recursively calling
      // until a proper command is found then executes that command
      //
      // \param[in] input The command string to execute, including the arguments
      bool ExecuteCommand(std::string const & input);

   private: //Ui::InputMode
      bool InputStringHandler(std::string input);
      char const * const Prompt() const;

   private: //Player wrapper functions
      bool ClearScreen(std::string const & arguments) { return Player::ClearScreen(); }
      bool Connect(std::string const & arguments)     { return Player::Connect(arguments); }
      bool Pause(std::string const & arguments)       { return Player::Pause(); }
      bool Play(std::string const & arguments)        { return Player::Play(atoi(arguments.c_str())); }
      bool Quit(std::string const & arguments)        { return Player::Quit(); }
      bool Random(std::string const & arguments)      { return Player::Random(true); }
      bool Stop(std::string const & arguments)        { return Player::Stop(); }

   private:
      template <Player::Skip SKIP>
      bool SkipSong(std::string const & arguments); 

      template <Ui::Screen::MainWindow MAINWINDOW>
      bool SetActiveWindow(std::string const & arguments); 

   private:
      // Executes \p command using \p arguments
      //
      // \param[in] command   The command to execute
      // \param[in] arguments The arguments to pass to the command
      bool ExecuteCommand(std::string const & command, std::string const & arguments);

      // Splits the input into command and argument parts
      //
      // \param[in]  input     The string to split
      // \param[out] command   The command part of the string
      // \param[out] arguments The arguments from the string
      void SplitCommand(std::string const & input, std::string & command, std::string & arguments);

      // Handle the settings
      bool Set(std::string const & arguments);

      // Call the cli mpc client
      bool Mpc(std::string const & arguments);

      // Alias a command to a given string
      bool Alias(std::string const & arguments);

      // Redraws the current window
      bool Redraw(std::string const & arguments);
   
      //
      void ResetTabCompletion(int input);

      // Complete the current command, by searching through the command table
      // for commands that begin with the currently set command
      std::string TabComplete(std::string const & command);

   private:
      typedef bool (Ui::Command::*CommandFunction)(std::string const &);
      typedef std::map<std::string, std::string>     AliasTable;
      typedef std::map<std::string, CommandFunction> CommandTable;

   private: 
      bool             initTabCompletion_;
      AliasTable       aliasTable_;
      CommandTable     commandTable_;
      Main::Settings & settings_;
      Ui::Screen     & screen_;

      // Tab completion searching class
      class TabCompletionMatch 
      {
      public:
         TabCompletionMatch(std::string const & key) : 
            key_(key) 
         {}

      public:
         bool operator() (std::pair<std::string const &, Ui::Command::CommandFunction> element) 
         {
            std::string input(element.first);
            return (key_.compare(input.substr(0, key_.length())) == 0);
         }

      private:
         std::string key_;
      };
   };

   //Implementation of skipping functions
   template <Ui::Player::Skip SKIP>
   bool Command::SkipSong(std::string const & arguments)
   {
      return Player::SkipSong(SKIP, atoi(arguments.c_str()));
   }

   template <Ui::Screen::MainWindow MAINWINDOW>
   bool Command::SetActiveWindow(std::string const & arguments)
   {
      return Player::SetActiveWindow(MAINWINDOW);
   }

}

#endif