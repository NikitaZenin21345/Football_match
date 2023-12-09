#include "dataBase.h"

void dataBase::add(const playerId& id, const player& newPlayer) noexcept
{
	players[id] = std::make_shared<player>(newPlayer);
}

playerId dataBase::add(const player& newPlayer) noexcept
{
	const auto newId = playerId(getFreeId(players));
	add(newId, newPlayer);
	return newId;
}

void dataBase::add(const teamId& Id, const team& newTeam) noexcept
{
	teams[Id] = std::make_shared<team>(newTeam);
}

teamId dataBase::add(const team& newTeam) noexcept
{
	const auto newId = teamId(getFreeId(teams));
	add(newId, newTeam);
	return newId;
}

void dataBase::add(const matchId& Id, const match& newMatch) noexcept
{
	matches[Id] = std::make_shared<match>(newMatch);
}

matchId dataBase::add(const match& newMatch) noexcept
{
	const auto newId = matchId(getFreeId(matches));
	add(newId, newMatch);
	return newId;
}

std::shared_ptr<player> dataBase::findElement(const playerId& id) const
{
	return findElementInTable(players, id);
}

playerId dataBase::findId(const player& element) const noexcept
{
	return findIdInTable(players, element);
}

std::shared_ptr<team> dataBase::findElement(const teamId& id) const
{
	return findElementInTable(teams, id);
}

teamId dataBase::findId(const team& element) const noexcept
{
	return findIdInTable(teams, element);
}

std::shared_ptr<match> dataBase::findElement(const matchId& id) const noexcept
{
	return findElementInTable(matches, id);
}

matchId dataBase::findId(const match& element) const noexcept
{
	return findIdInTable(matches, element);
}
