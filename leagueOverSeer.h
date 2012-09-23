/*
League Over Seer Plug-in
    Copyright (C) 2012 Ned Anderson & Vladimir Jimenez

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
*/

#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <fstream>
#include <algorithm>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "bzfsAPI.h"

//Define plugin version numbering
const int MAJOR = 0;
const int MINOR = 9;
const int REV = 1;
const int BUILD = 63;

class leagueOverSeer : public bz_Plugin, public bz_CustomSlashCommandHandler, public bz_BaseURLHandler
{
  virtual const char* Name (){return "League Over Seer 0.7.1 (55)";}
  virtual void Init ( const char* config);  
  virtual void Event( bz_EventData *eventData );
  virtual bool SlashCommand( int playerID, bz_ApiString, bz_ApiString, bz_APIStringList*);
  virtual void Cleanup ();
  virtual void URLDone( const char* URL, void* data, unsigned int size, bool complete );
  virtual void URLTimeout(const char* URL, int errorCode);
  virtual void URLError(const char* URL, int errorCode, const char *errorString);
  virtual void setTeamNameAsMottoFromBZID(std::string bzid, int playerID);
  virtual void setTeamNameAsMottoFromCallsign(std::string callsign, int playerID);
  virtual int loadConfig(const char *cmdLine);
  virtual bool toBool(std::string var);
  
  //All the variables that will be used in the plugin
  bool officialMatch, matchCanceled, funMatch, rotLeague, gameoverReport, mottoReplacer, rejoinPrevention;
  double lastQuery[256];
  int DEBUG, RTW, GTW, BTW, PTW;
  std::string LEAGUE_URL, LEAGUE, map;
  const char* mapchangePath;
  
  struct teamQueries { //Stores all the queries that a player request
    int _playerID;
  };
  std::vector<teamQueries> _playerIDs;
  
  struct urlQueries { //Stores the order of match reports and team queries
    std::string _URL;
  };
  std::vector<urlQueries> _urlQuery;
  
  struct matchRedPlayers { //Maintains the players that started the match on the red team
    bz_ApiString callsign;
    bz_ApiString bzid;
  };
  std::vector<matchRedPlayers> matchRedParticipants;
  
  struct matchGreenPlayers { //Maintains the players that started the match on the green team
    bz_ApiString callsign;
    bz_ApiString bzid;
  };
  std::vector<matchGreenPlayers> matchGreenParticipants;
  
  struct matchBluePlayers { //Maintains the players that started the match on the blue team
    bz_ApiString callsign;
    bz_ApiString bzid;
  };
  std::vector<matchBluePlayers> matchBlueParticipants;
  
  struct matchPurplePlayers { //Maintains the players that started the match on the purple team
    bz_ApiString callsign;
    bz_ApiString bzid;
  };
  std::vector<matchPurplePlayers> matchPurpleParticipants;
};

