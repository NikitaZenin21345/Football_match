#include "dataBase.h"

void dataBase::add(const playerId& id, const player& newPlayer) noexcept
{
	players[id] = newPlayer;
}

void dataBase::add(const teamId& Id, const team& newTeam) noexcept
{
	teams[Id] = newTeam;
}

void dataBase::add(const matchId& Id, const match& newMatch) noexcept
{
	matches[Id] = newMatch;
}



