#pragma once

enum matchStates
{
    waiting_players = 0,
    running,
    player1Win,
    player2Win,
    tie
};

enum RequestType
{
    IGNORE_REQ = 0, // Avoid sending request at all
    setName,
    updateOpponentName,
    makePlay,
    updateGrid,
    sendGameState
};