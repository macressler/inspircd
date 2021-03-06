/*
 * InspIRCd -- Internet Relay Chat Daemon
 *
 *   Copyright (C) 2009 Daniel De Graaf <danieldg@inspircd.org>
 *   Copyright (C) 2008 Robin Burchell <robin+git@viroteck.net>
 *
 * This file is part of InspIRCd.  InspIRCd is free software: you can
 * redistribute it and/or modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation, version 2.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "inspircd.h"

class ModuleBadChannelExtban : public Module
{
 public:
	Version GetVersion() CXX11_OVERRIDE
	{
		return Version("Extban 'j' - channel status/join ban", VF_OPTCOMMON|VF_VENDOR);
	}

	ModResult OnCheckBan(User *user, Channel *c, const std::string& mask) CXX11_OVERRIDE
	{
		if ((mask.length() > 2) && (mask[0] == 'j') && (mask[1] == ':'))
		{
			std::string rm = mask.substr(2);
			char status = 0;
			ModeHandler* mh = ServerInstance->Modes->FindPrefix(rm[0]);
			if (mh)
			{
				rm = mask.substr(3);
				status = mh->GetModeChar();
			}
			for (UCListIter i = user->chans.begin(); i != user->chans.end(); i++)
			{
				if (InspIRCd::Match((*i)->chan->name, rm))
				{
					if (!status || (*i)->hasMode(status))
						return MOD_RES_DENY;
				}
			}
		}
		return MOD_RES_PASSTHRU;
	}

	void On005Numeric(std::map<std::string, std::string>& tokens) CXX11_OVERRIDE
	{
		tokens["EXTBAN"].push_back('j');
	}
};

MODULE_INIT(ModuleBadChannelExtban)
