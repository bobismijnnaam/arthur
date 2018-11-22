https://github.com/ocornut/imgui/issues/1886


- Daemon
    - Responsible for managing all the other nodes and recompiling locally if
      the sources change.
    - Sends:
        - Kill
        - Pull
        - Compile
        - DaemonPresence
    - Receives:
        - Pull
        - Compile
        - DaemonPresence
- Prober
    - Node that can probe the gamestate and its moves. Produces a win
      distribution over the possible moves. Either over the current game state or
      one game state in the future.
    - Sends:
        - ProbeFinished
        - ProbeUpdate
    - Receives:
        - ProbeState
        - Kill
- Protocol
    - Communicates with the server
    - Sends:
        - Connected
        - Disconnected
        - GameStarted
        - ProduceMove
        - MoveMade
        - LobbyInfo
    - Receives:
        - Move
        - ConnectToServer
        - JoinGame
        - DisconnectFromServer
        - RequestLobbyInfo
        - Kill
- ProbeStatistics
    - Displays statistics of the game state probing
    - Sends:
    - Receives:
        - Kill
        - ProbeUpdate
- ProtocolGUI
    - Instructs Protocol what to do
    - Sends:
        - ConnectToServer
        - JoinGame
        - RequestLobbyInfo
        - DisconnectFromServer
    - Receives:
        - LobbyInfo
        - Connected
        - Disconnected
