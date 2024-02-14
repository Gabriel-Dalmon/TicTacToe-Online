#pragma once

enum matchStates
{
    waiting_players = 0,
    running,
    player1Win,
    player2Win,
    tie
};

enum requestType
{
    IGNORE_REQ = 0,     // Ignore and never send the request
    setName,            // Contains the new name for a player
    updateOpponentName, // Contains the new name of the opponent (to update on the client's screen)
    makePlay,           // Contains a player's index where they want to play
    updateMatchState    // Contains all the info about the match. Sent after a legal move is played
};