#DevLog - ~8:30h
## Multi Grid Tic Tac Toe

<br>

  [![Mega Tic Tac Toe Final](MTT_final.gif)]

  [![Mega Tic Tac Toe WIP](MTT_wip.gif)]

  [![Mega Tic Tac Toe Earlu](MTT_early.gif)]
  
<br>
<br>
<br>

## Building:
Only on Windows.
- go to root folder
- use command "make"
- either run Game.exe from build folder or use "run.bat" file

## Game 3/10 in the 10 games in 10 weeks challenge
////////////////////////////////////
              NOTE:
////////////////////////////////////
#### Goals:
- spend min ~12h per game
- aim for 10 games by week 10 
- have a small scope finished game by the end of the week
- even if not finished - still release it

////////////////////////////////////
              TODO:
////////////////////////////////////

////////////////////////////////////
              DONE:
////////////////////////////////////

// 25.10.2023 - ~0:45h
- indicate visually which board you need to play on
- indicate visually which color has a win on a board

// 24.10.2023 - ~0:45h
- final win condition implementation
- game reset/restart

// 19.10.2023 - ~1:15h
- implement turns
- track individual board winner
- allow only 1 winner per board
- connect the boards (when playing a move on 1 board it moves you to the corresponding board)

// 18.10.2023 - ~4:00h
- board rework
- tile mouse interaction 
- tile states
- win condition check
- single board refactor to support scaling to 9 separate boards
- scale single board into 9 separte boards

// 17.10.2023 - ~1:30h
- basic mockup
- basic 3x3 grid drawing
- basic grid tile interaction with mouse pos
- project structuring WIP

