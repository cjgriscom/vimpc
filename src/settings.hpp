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

   settings.hpp - handle configuration options via :set command
   */

#ifndef __MAIN__SETTINGS
#define __MAIN__SETTINGS

#include "screen.hpp"

#include <string>
#include <map>

namespace Main
{
   //holds the value of a single setting
   template <typename T>
   class Setting
   {
      public:
         Setting() { }
         Setting(T v) : value_(v) { }
         ~Setting() { }

         T Get() const { return value_; }
         void Set(T v) { value_ = v; }

      private:
         T value_;
   };

   // Manages settings which are set via :set command
   class Settings
   {
      public:
         static Settings & Instance();

      protected:
         Settings();
         ~Settings();

      public:
         //Calls the correct setter function based upon the given input
         void Set(std::string const & input);

         //Gets the default startup window
         Ui::Screen::MainWindow Window() const;

         //Whether to autmatically scroll to playing song
         bool AutoScroll() const;

      private:
         void SetSpecificSetting(std::string setting, std::string arguments);
         void SetSingleSetting(std::string setting);

      private:
         bool Get(std::string setting) const { return toggleTable_.at(setting)->Get(); }

      private:
         //Sets the startup window
         void SetWindow(std::string const & arguments); 

      private:
         Ui::Screen::MainWindow defaultWindow_;

         typedef void (Main::Settings::*ptrToMember)(std::string const &);
         typedef std::map<std::string, ptrToMember> SettingsFunctionTable;
         SettingsFunctionTable settingsTable_;

         typedef std::map<std::string, Setting<bool> * > SettingsToggleTable;
         SettingsToggleTable   toggleTable_;
   };
}

#endif