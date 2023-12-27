#include "dataBase.h"

void footballMatchObjectDataBase::add(const playerId& Id, player& newPlayer) noexcept
{
	newPlayer.setId(Id);
	players[Id] = newPlayer;
}

void footballMatchObjectDataBase::add(const teamId& Id, team& newTeam) noexcept
{
	newTeam.setId(Id);
	teams[Id] = newTeam;
}

void footballMatchObjectDataBase::add(const matchId& Id,  match& newMatch) noexcept
{
	newMatch.setId(Id);
	matches[Id] = newMatch;
}