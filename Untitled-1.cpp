
#include <utility.h> #include "toolbox.h" #include <cvirte.h> #include <userint.h> #include "Connect 4.h"
//Global variables
const int col = 7;
const int row = 6;
int board[7][8]; //stores player choices int score[4][8];
int rowindicator[8]; //bottom row available in each column
int columnpick;
int player; //computer is player 3
int computer; //is there a computer player? 0 = false, 1 = true int player1color = VAL_BLUE;
int player2color = VAL_RED;
int computerplayercolor = VAL_WHITE; int difficulty; //1-easy, 2-medium, 3-hard
static int panel3;//start
static int panel1;//game
static int panel2;//play again?
static int panel4;//difficulty for computer static int panel5;//difficulty for 2 player
int timerwanted; //0-false, 1-true int timeelapsed; //timer
void InitGlobal(void);
void pickbestplay (void);
void horizscore (int scoreplayer); void vertscore (int scoreplayer);
void leftdiagscore (int scoreplayer); void rightdiagscore (int scoreplayer);
/************************************************************ InitGlobal()
Initialize global variables that are not constant ************************************************************/ void InitGlobal(void)
{
int i,j,k;
//Initialize board to all 0s for(i = 1; i <= row; i++) {
for(j = 1; j <= col; j++) {
board[i][j] = 0; }
}
//Initialize the current row in each column to 1 for(k = 1; k <= col; k++)
{
rowindicator[k] = 1; }
//computer = 0; //default no computer player player = 1;
timerwanted = 0;
timeelapsed = 0;
}
/************************************************************ main()
Start of program ************************************************************/ int main (int argc, char *argv[])
{
6
InitGlobal();
if (InitCVIRTE (0, argv, 0) == 0) return -1; /* out of memory */
if ((panel3 = LoadPanel (0, "Connect 4.uir", PANEL_3)) < 0) return -1;
SetPanelAttribute (panel3, ATTR_TITLE, "Start Connect 4"); DisplayPanel (panel3);
RunUserInterface ();
return 0;
}
/************************************************************ pickbestplay()
AI for computer player, allows for the program to pick
column for different difficulty levels ************************************************************/ void pickbestplay (void)
{
int bestchoice;
int realplayer = 1;
int computerplayer = 3; int i,j;
int found = 0; //false
Delay (0.5);
//Initialize score to all 0s for(i = 1; i <= 3; i++)
{
for(j = 1; j <= col; j++) {
score[i][j] = 0; }
}
//offensive play to win
//else if comp has any 3 pick that spot horizscore(computerplayer); vertscore(computerplayer); leftdiagscore(computerplayer); rightdiagscore(computerplayer);
//all difficulty levels
for(j = 1; j <= col; j++) {
if (score[computerplayer][j] >= 3) {
if (rowindicator[j] <= row) {
bestchoice = j;
found = 1; }
} }
if (found == 0) {
horizscore(realplayer); vertscore(realplayer); leftdiagscore(realplayer); rightdiagscore(realplayer);
//defensive play to block player //if p1 has any 3, pick that spot for(j = 1; j <= col; j++)
{
if (score[realplayer][j] >= 3) {
if (rowindicator[j] <= row) {
bestchoice = j;
found = 1; }
} }
}
if ((found == 0) && (difficulty >= 2)) {
//offensive play to create win
//if computer has any 2, pick that spot for(j = 1; j <= col; j++)
{
if (score[computerplayer][j] == 2) {
7

if (rowindicator[j] <= row) {
bestchoice = j;
found = 1; }
} }
}
if ((found == 0) && (difficulty >= 2)) {
//defensive play to create win
//if player has any 2, pick that spot for(j = 1; j <= col; j++)
{
if (score[realplayer][j] == 2) {
if (rowindicator[j] <= row) {
bestchoice = j;
found = 1; }
} }
}
//else pick comp = 2,1,0 but closer to center better choice if (found == 0)
{
SetRandomSeed (0); do
{
bestchoice = Random(0,8);
}while (rowindicator[bestchoice] > row);
}
//now that have the column the computer picks, select it if (bestchoice == 1)
{
Column1 (panel1,2,EVENT_COMMIT, 0, 0, 0); }
else if (bestchoice == 2) {
Column2 (panel1,2,EVENT_COMMIT, 0, 0, 0); }
else if (bestchoice == 3) {
Column3 (panel1,2,EVENT_COMMIT, 0, 0, 0); }
else if (bestchoice == 4) {
Column4 (panel1,2,EVENT_COMMIT, 0, 0, 0); }
else if (bestchoice == 5) {
Column5 (panel1,2,EVENT_COMMIT, 0, 0, 0); }
else if (bestchoice == 6) {
Column6 (panel1,2,EVENT_COMMIT, 0, 0, 0); }
else //if (bestchoice == 7) {
Column7 (panel1,2,EVENT_COMMIT, 0, 0, 0); }
}
/************************************************************ horizscore()
Used only with the computer player, scores the computer and
player in the horizontal direction to find the best move ************************************************************/ void horizscore (int scoreplayer)
{
int i, checkcol,checkrow; int count = 0;
int done = 0; //false
for (i = 1; i <= col; i++) {
count = 0;
checkcol = i;
checkrow = rowindicator[i];
done = 0;
while (checkrow <= 6 && checkcol > 1 && done == 0) //check left {
--checkcol;
if (board[checkrow][checkcol] == scoreplayer)
8

} }
count++; else
done = 1;
}
checkcol = i;
checkrow = rowindicator[i];
done = 0;
while (checkrow <= 6 && checkcol < 7 && done == 0) //check right {
++checkcol;
if (board[checkrow][checkcol] == scoreplayer)
count++; else
done = 1;
}
if (count > score[scoreplayer][i])
score[scoreplayer][i] = count;
/************************************************************ vertscore()
Used only with the computer player, scores the computer and
player in the vertical direction to find the best move ************************************************************/ void vertscore (int scoreplayer)
{
int i, checkrow;
int count = 0;
int done = 0; //false
for (i = 1; i <= col; i++) {
count = 0;
checkrow = rowindicator[i];
done = 0;
while (checkrow > 1 && done == 0) //check down {
--checkrow;
if (board[checkrow][i] == scoreplayer)
count++; else
done = 1;
}
if (count > score[scoreplayer][i])
score[scoreplayer][i] = count;
} }
/************************************************************ leftdiagscore()
Used only with the computer player, scores the computer and
player in the left diagonal direction to find the best move ************************************************************/ void leftdiagscore (int scoreplayer)
{
int i, checkrow, checkcol; int count = 0;
int done = 0; //false
for (i = 1; i <= col; i++) {
count = 0;
checkrow = rowindicator[i];
checkcol = i;
done = 0;
while (checkrow > 1 && checkcol < 7 && done == 0) //check down {
--checkrow;
++checkcol;
if (board[checkrow][checkcol] == scoreplayer)
count++; else
done = 1;
}
checkrow = rowindicator[i];
checkcol = i;
done = 0;
while (checkrow < 6 && checkcol > 1 && done == 0) //check up {
++checkrow;
--checkcol;
if (board[checkrow][checkcol] == scoreplayer)
count++; else
}
done = 1;
9

if (count > score[scoreplayer][i]) score[scoreplayer][i] = count;
} }
/************************************************************ rightdiagscore()
Used only with the computer player, scores the computer and
player in the right diagonal direction to find the best move ************************************************************/ void rightdiagscore (int scoreplayer)
{
int i, checkrow, checkcol; int count = 0;
int done = 0; //false
for (i = 1; i <= col; i++) {
count = 0;
checkrow = rowindicator[i];
checkcol = i;
done = 0;
while (checkrow > 1 && checkcol > 1 && done == 0) //check down {
--checkrow;
--checkcol;
if (board[checkrow][checkcol] == scoreplayer)
count++; else
done = 1;
}
checkrow = rowindicator[i];
checkcol = i;
done = 0;
while (checkrow < 6 && checkcol < 7 && done == 0) //check up {
++checkrow;
++checkcol;
if (board[checkrow][checkcol] == scoreplayer)
count++; else
done = 1;
}
if (count > score[scoreplayer][i])
score[scoreplayer][i] = count;
} }
/************************************************************ changeplayer()
Switches the current player ************************************************************/ void changeplayer(void)
{
if(computer == 0) {
if(player == 1) {
player = 2;
SetCtrlVal (panel1, PANEL_TEXTMSG,"Player 2's Turn"); }
else {
player = 1;
SetCtrlVal (panel1, PANEL_TEXTMSG,"Player 1's Turn"); }
} else {
if(player == 1) {
player = 3;//computer player
SetCtrlVal (panel1, PANEL_TEXTMSG,"Computer's Turn"); pickbestplay();
} else {
player = 1;
SetCtrlVal (panel1, PANEL_TEXTMSG,"Player 1's Turn"); }
}
if (timerwanted == 1) {
ResetTimer (panel1, PANEL_TIMER); timeelapsed = 0;
SetCtrlVal (panel1, PANEL_TEXTMSG_2,"5");
10

ResumeTimerCallbacks(); }
/************************************************************ ishorizwin()
Inputs:
rowchoice current row that is available colchoice column the player has chosen
Outputs:
1 winner
0 no winner
Returns a 1 if there is 4 in a row in the horizontal direction ************************************************************/ int ishorizwin(int rowchoice, int colchoice)
{
int count = 0;
int j;
for(j = 1; j <= col; j++) {
if (board[rowchoice][j] != player) count = 0;
else if (board[rowchoice][j] == player) {
count++; if(count == 4)
}
} }
return 0; }
return 1;
/************************************************************ isvertwin()
Inputs:
rowchoice current row that is available colchoice column the player has chosen
Outputs:
1 winner
0 no winner
Returns a 1 if there is 4 in a row in the vertical direction ************************************************************/ int isvertwin (int rowchoice, int colchoice)
{
int count = 0;
int i;
for(i = 1; i <= row; i++) {
if (board[i][colchoice] != player) count = 0;
else if (board[i][colchoice] == player) {
count++; if(count == 4)
} }
return 0; }
return 1;
/************************************************************ isrightdiagwin()
Inputs:
rowchoice current row that is available
colchoice column the player has chosen Outputs:
1 winner
0 no winner
Returns a 1 if there is 4 in a row in a right slant
diagonal direction ************************************************************/ int isrightdiagwin (int rowchoice, int colchoice)
{
int startrow,startcol; int count;
int j, i;
startcol = colchoice;
startrow = rowchoice;
count = 0;
//find lowest spot in diagonal while((startrow > 1) && (startcol > 1)) {
startrow--;
startcol--; }
11

j = startcol;
//search upward for 4 in a row
for(i = startrow; (i <= row) && (j <= col); i++) {
if (board[i][j] != player) count = 0;
else if (board[i][j] == player) {
count++; if(count == 4)
return 1; j++;
}
return 0; }
/************************************************************ isleftdiagwin()
Inputs:
rowchoice current row that is available
colchoice column the player has chosen Outputs:
1 winner
0 no winner
Returns a 1 if there is 4 in a row in a left slant
diagonal direction ************************************************************/ int isleftdiagwin (int rowchoice, int colchoice)
{
int startrow,startcol; int count;
int j, i;
startcol = colchoice;
startrow = rowchoice;
count = 0;
//find lowest spot in diagonal while((startrow > 1) && (startcol < col)) {
startrow--;
startcol++; }
j = startcol;
//search upward for 4 in a row
for(i = startrow; (i <= row) && (j > 1); i++) {
if (board[i][j] != player) count = 0;
else if (board[i][j] == player) {
count++; if(count == 4)
} j--;
}
return 0; }
}
return 1;
/************************************************************ iswinner()
Inputs:
rowchoice current row that is available colchoice column the player has chosen
Outputs:
1 winner
0 no winner
Returns a 1 if there is a winner, also suspends the timer
if there is a winner ************************************************************/ int iswinner (int rowchoice, int colchoice)
{
if (ishorizwin(rowchoice, colchoice) == 1) {
if (timerwanted == 1) {
SuspendTimerCallbacks(); }
return 1; }
if (isvertwin(rowchoice, colchoice) == 1) {
12

if (timerwanted == 1) {
SuspendTimerCallbacks(); }
return 1; }
if (isleftdiagwin(rowchoice, colchoice) == 1) {
if (timerwanted == 1) {
SuspendTimerCallbacks(); }
return 1; }
if (isrightdiagwin(rowchoice, colchoice) == 1) {
if (timerwanted == 1) {
SuspendTimerCallbacks(); }
return 1; }
if (timerwanted == 1) {
SuspendTimerCallbacks(); }
return 0; }
/************************************************************ istie()
Outputs:
1 tie
0 no tie
Checks the board for any 0s and returns a 1 if there is a tie ************************************************************/ int istie (void)
{
int i,j;
for(i = 1; i <= row; i++) {
for(j = 1; j <= col; j++) {
if(board[i][j] == 0) return 0;
} }
return 1; }
/************************************************************ loadboard()
Inputs:
rowchoice current row that is available colchoice column the player has chosen
Outputs:
1 winner / tie
0 no winner / tie
Places a 1 for player 1 and a 2 for player 2 in the spot
chosen by the current player ************************************************************/ int loadboard(int columnpick, int rowpick)
{
int winner,tie;
int playagain;
if(rowpick <= row && columnpick <= col) {
board[rowpick][columnpick] = player; }
else {
//column is full }
winner = iswinner (rowpick, columnpick); if (winner == 1)//true
{
if (player == 1) {
MessagePopup ("WINNER!","Congratulations Player 1 You Win!"); }
else if (player == 2) {
MessagePopup ("WINNER!","Congratulations Player 2 You Win!"); }
else {
13

MessagePopup ("WINNER!","Sorry, Computer Wins!"); }
if ((panel2 = LoadPanel (0, "Connect 4.uir", PANEL_2)) < 0) return -1;
SetPanelAttribute (panel2, ATTR_TITLE, ""); DisplayPanel (panel2);
return 1;
}
tie = istie();
if (tie == 1)//true {
MessagePopup ("TIE!","It's a tie!");
if ((panel2 = LoadPanel (0, "Connect 4.uir", PANEL_2)) < 0)
return -1;
SetPanelAttribute (panel2, ATTR_TITLE, ""); DisplayPanel (panel2);
return 1;
}
return 0; }
/************************************************************ StartProgram()
Called when player choses to start the game with 2 players
from the initial start up panel ************************************************************/ int CVICALLBACK StartProgram (int panel, int control, int event,
void *callbackData, int eventData1, int eventData2)
{
switch (event)
{
case EVENT_COMMIT:
computer = 0;
DiscardPanel (panel3);
if ((panel5 = LoadPanel (0, "Connect 4.uir", PANEL_5)) < 0)
return -1;
SetPanelAttribute (panel5, ATTR_TITLE, "Select Difficulty"); DisplayPanel (panel5);
SetActivePanel (panel5);
break; }
return 0; }
/************************************************************ StartComputerProgram()
Called when player choses to start the game with 1 player
from the initial start up panel ************************************************************/
int CVICALLBACK StartComputerProgram (int panel, int control, int event,
void *callbackData, int eventData1, int eventData2)
{
switch (event)
{
case EVENT_COMMIT:
computer = 1;
DiscardPanel (panel3);
if ((panel4 = LoadPanel (0, "Connect 4.uir", PANEL_4)) < 0)
return -1;
SetPanelAttribute (panel4, ATTR_TITLE, "Select Difficulty"); DisplayPanel (panel4);
SetActivePanel (panel4);
break;
} return 0;
}
/************************************************************ PlayAgain()
Called when player choses to play the game again after there
is a tie or a winner ************************************************************/ int CVICALLBACK PlayAgain (int panel, int control, int event,
void *callbackData, int eventData1, int eventData2)
{
switch (event)
{
case EVENT_COMMIT:
DiscardPanel (panel2); DiscardPanel (panel1); if (computer == 1)
{
if ((panel4 = LoadPanel (0, "Connect 4.uir", PANEL_4)) < 0) return -1;
14

SetPanelAttribute (panel4, ATTR_TITLE, "Select Difficulty");
DisplayPanel (panel4); }
else {
if ((panel5 = LoadPanel (0, "Connect 4.uir", PANEL_5)) < 0) return -1;
SetPanelAttribute (panel5, ATTR_TITLE, "Select Difficulty");
DisplayPanel (panel5); }
InitGlobal();
break; }
return 0; }
/************************************************************ QuitGame()
Called when player choses to quit the game again after there
is a tie or a winner ************************************************************/ int CVICALLBACK QuitGame (int panel, int control, int event,
void *callbackData, int eventData1, int eventData2)
{
switch (event)
{
case EVENT_COMMIT:
DiscardPanel (panel2); DiscardPanel (panel1); QuitUserInterface (0); break;
} return 0;
}
/************************************************************ TimerExpired()
Called when the timer clicks every second, then will change
the display to decrement the time shown ************************************************************/ int CVICALLBACK TimerExpired (int panel, int control, int event,
void *callbackData, int eventData1, int eventData2)
{
switch (event)
{
case EVENT_TIMER_TICK:
if (timerwanted == 1) {
if(timeelapsed == 0) {
SetCtrlVal (panel1, PANEL_TEXTMSG_2,"5");
timeelapsed++; }
else if(timeelapsed == 1) {
SetCtrlVal (panel1, PANEL_TEXTMSG_2,"4");
timeelapsed++; }
else if(timeelapsed == 2) {
SetCtrlVal (panel1, PANEL_TEXTMSG_2,"3");
timeelapsed++; }
else if(timeelapsed == 3) {
SetCtrlVal (panel1, PANEL_TEXTMSG_2,"2");
timeelapsed++; }
else if(timeelapsed == 4) {
SetCtrlVal (panel1, PANEL_TEXTMSG_2,"1");
timeelapsed++; }
else if(timeelapsed == 5) {
SetCtrlVal (panel1, PANEL_TEXTMSG_2,"0"); timeelapsed = 0;
changeplayer();
} }
break; }
return 0; }
15

/************************************************************ QuitCallback()
Called when the quit button is pressed and ends the game ************************************************************/ int CVICALLBACK QuitCallback (int panel, int control, int event,
void *callbackData, int eventData1, int eventData2)
{
switch (event)
{
case EVENT_COMMIT:
QuitUserInterface (0);
break; }
return 0; }
/************************************************************ HardSetting()
Called when the hard setting is chosen in the 1 player mode ************************************************************/ int CVICALLBACK HardSetting (int panel, int control, int event,
void *callbackData, int eventData1, int eventData2)
{
switch (event)
{
case EVENT_COMMIT:
difficulty = 3;
timerwanted = 1;
if ((panel1 = LoadPanel (0, "Connect 4.uir", PANEL)) < 0)
return -1;
SetPanelAttribute (panel1, ATTR_TITLE, "Connect 4"); SetCtrlAttribute (panel1, PANEL_TIMER, ATTR_VISIBLE, 1); ResetTimer (panel1, PANEL_TIMER);
DisplayPanel (panel1);
SetActivePanel (panel1);
DiscardPanel(panel4);
break; }
return 0; }
/************************************************************ MediumSetting()
Called when the medium setting is chosen in the 1 player mode ************************************************************/ int CVICALLBACK MediumSetting (int panel, int control, int event,
void *callbackData, int eventData1, int eventData2)
{
switch (event)
{
case EVENT_COMMIT:
difficulty = 2;
timerwanted = 0;
if ((panel1 = LoadPanel (0, "Connect 4.uir", PANEL)) < 0)
return -1;
SetPanelAttribute (panel1, ATTR_TITLE, "Connect 4"); SetCtrlAttribute (panel1, PANEL_TEXTMSG_2, ATTR_VISIBLE, 0); SetCtrlAttribute (panel1, PANEL_TEXTMSG_3, ATTR_VISIBLE, 0); SetCtrlAttribute (panel1, PANEL_TEXTMSG_4, ATTR_VISIBLE, 0); DisplayPanel (panel1);
SetActivePanel (panel1);
DiscardPanel(panel4);
break; }
return 0; }
/************************************************************ EasySetting()
Called when the easy setting is chosen in the 1 player mode ************************************************************/ int CVICALLBACK EasySetting (int panel, int control, int event,
void *callbackData, int eventData1, int eventData2)
{
switch (event)
{
case EVENT_COMMIT:
difficulty = 1;
timerwanted = 0;
if ((panel1 = LoadPanel (0, "Connect 4.uir", PANEL)) < 0)
return -1;
SetPanelAttribute (panel1, ATTR_TITLE, "Connect 4"); SetCtrlAttribute (panel1, PANEL_TEXTMSG_2, ATTR_VISIBLE, 0);
16

SetCtrlAttribute (panel1, PANEL_TEXTMSG_3, ATTR_VISIBLE, 0); SetCtrlAttribute (panel1, PANEL_TEXTMSG_4, ATTR_VISIBLE, 0); DisplayPanel (panel1);
SetActivePanel (panel1);
DiscardPanel(panel4);
break; }
return 0; }
/************************************************************ Hard2Player()
Called when the hard setting is chosen in the 2 player mode ************************************************************/ int CVICALLBACK Hard2Player (int panel, int control, int event,
void *callbackData, int eventData1, int eventData2)
{
switch (event)
{
case EVENT_COMMIT:
timerwanted = 1;
if ((panel1 = LoadPanel (0, "Connect 4.uir", PANEL)) < 0)
return -1;
SetPanelAttribute (panel1, ATTR_TITLE, "Connect 4"); SetCtrlAttribute (panel1, PANEL_TIMER, ATTR_VISIBLE, 1); ResetTimer (panel1, PANEL_TIMER);
DisplayPanel (panel1);
SetActivePanel (panel1);
DiscardPanel(panel5);
break; }
return 0; }
/************************************************************ Easy2Player()
Called when the easy setting is chosen in the 2 player mode ************************************************************/ int CVICALLBACK Easy2Player (int panel, int control, int event,
void *callbackData, int eventData1, int eventData2)
{
switch (event)
{
case EVENT_COMMIT:
timerwanted = 0;
if ((panel1 = LoadPanel (0, "Connect 4.uir", PANEL)) < 0)
return -1;
SetPanelAttribute (panel1, ATTR_TITLE, "Connect 4"); SetCtrlAttribute (panel1, PANEL_TEXTMSG_2, ATTR_VISIBLE, 0); SetCtrlAttribute (panel1, PANEL_TEXTMSG_3, ATTR_VISIBLE, 0); SetCtrlAttribute (panel1, PANEL_TEXTMSG_4, ATTR_VISIBLE, 0); DisplayPanel (panel1);
SetActivePanel (panel1);
DiscardPanel(panel5);
break; }
return 0; }
/************************************************************ Column1()
Called when player selects the first columm and depending
on what row is currently available, it stores the selection
in the board ************************************************************/ int CVICALLBACK Column1 (int panel, int control, int event,
void *callbackData, int eventData1, int eventData2)
{
int currentcolor; int x;
if (player == 1) {
currentcolor = player1color; }
else if (player == 2) {
currentcolor = player2color; }
else {
currentcolor = computerplayercolor; }
17

columnpick = 1; switch (event)
{
case EVENT_COMMIT:
if (rowindicator[columnpick] == 1) //row 1 is next row available {
SetCtrlAttribute (panel1, PANEL_LED_36, ATTR_OFF_COLOR, currentcolor);
x = loadboard(columnpick,1); }
else if (rowindicator[columnpick] == 2) {
SetCtrlAttribute (panel1, PANEL_LED_29, ATTR_OFF_COLOR, currentcolor); x = loadboard(columnpick,2);
}
else if (rowindicator[columnpick] == 3) {
SetCtrlAttribute (panel1, PANEL_LED_22, ATTR_OFF_COLOR, currentcolor); x = loadboard(columnpick,3);
}
else if (rowindicator[columnpick] == 4) {
SetCtrlAttribute (panel1, PANEL_LED_15, ATTR_OFF_COLOR, currentcolor); x = loadboard(columnpick,4);
}
else if (rowindicator[columnpick] == 5) {
SetCtrlAttribute (panel1, PANEL_LED_8, ATTR_OFF_COLOR, currentcolor); x = loadboard(columnpick,5);
}
else if (rowindicator[columnpick] == 6) {
SetCtrlAttribute (panel1, PANEL_LED_1, ATTR_OFF_COLOR, currentcolor); x = loadboard(columnpick,6);
} else {
//column is full
x = 0; }
if (x == 0) {
if (rowindicator[columnpick] <= row) {
rowindicator[columnpick]++; }
else {
//column is full }
changeplayer(); }
DefaultCtrl (panel1, PANEL_PICTUREBUTTON_1);
break; }
return 0; }
/************************************************************ Column2()
Called when player selects the second columm and depending
on what row is currently available, it stores the selection
in the board ************************************************************/ int CVICALLBACK Column2 (int panel, int control, int event,
void *callbackData, int eventData1, int eventData2)
{
int currentcolor; int x;
if (player == 1) {
currentcolor = player1color; }
else if (player == 2) {
currentcolor = player2color; }
else {
currentcolor = computerplayercolor; }
columnpick = 2;
18

switch (event) {
case EVENT_COMMIT:
if (rowindicator[columnpick] == 1) //row 1 is next row available {
SetCtrlAttribute (panel1, PANEL_LED_37, ATTR_OFF_COLOR, currentcolor);
x = loadboard(columnpick,1); }
else if (rowindicator[columnpick] == 2) {
SetCtrlAttribute (panel1, PANEL_LED_30, ATTR_OFF_COLOR, currentcolor); x = loadboard(columnpick,2);
}
else if (rowindicator[columnpick] == 3) {
SetCtrlAttribute (panel1, PANEL_LED_23, ATTR_OFF_COLOR, currentcolor); x = loadboard(columnpick,3);
}
else if (rowindicator[columnpick] == 4) {
SetCtrlAttribute (panel1, PANEL_LED_16, ATTR_OFF_COLOR, currentcolor); x = loadboard(columnpick,4);
}
else if (rowindicator[columnpick] == 5) {
SetCtrlAttribute (panel1, PANEL_LED_9, ATTR_OFF_COLOR, currentcolor); x = loadboard(columnpick,5);
}
else if (rowindicator[columnpick] == 6) {
SetCtrlAttribute (panel1, PANEL_LED_2, ATTR_OFF_COLOR, currentcolor); x = loadboard(columnpick,6);
} else {
//column is full
x = 0; }
if (x == 0) {
if (rowindicator[columnpick] <= row) {
rowindicator[columnpick]++; }
else {
//column is full }
changeplayer(); }
DefaultCtrl (panel1, PANEL_PICTUREBUTTON_2);
break; }
return 0; }
/************************************************************ Column3()
Called when player selects the third columm and depending
on what row is currently available, it stores the selection
in the board ************************************************************/ int CVICALLBACK Column3 (int panel, int control, int event,
void *callbackData, int eventData1, int eventData2)
{
int currentcolor; int x;
if (player == 1) {
currentcolor = player1color; }
else if (player == 2) {
currentcolor = player2color; }
else {
currentcolor = computerplayercolor; }
columnpick = 3; switch (event)
{
19

case EVENT_COMMIT:
if (rowindicator[columnpick] == 1) //row 1 is next row available {
SetCtrlAttribute (panel1, PANEL_LED_38, ATTR_OFF_COLOR, currentcolor);
x = loadboard(columnpick,1); }
else if (rowindicator[columnpick] == 2) {
SetCtrlAttribute (panel1, PANEL_LED_31, ATTR_OFF_COLOR, currentcolor); x = loadboard(columnpick,2);
}
else if (rowindicator[columnpick] == 3) {
SetCtrlAttribute (panel1, PANEL_LED_24, ATTR_OFF_COLOR, currentcolor); x = loadboard(columnpick,3);
}
else if (rowindicator[columnpick] == 4) {
SetCtrlAttribute (panel1, PANEL_LED_17, ATTR_OFF_COLOR, currentcolor); x = loadboard(columnpick,4);
}
else if (rowindicator[columnpick] == 5) {
SetCtrlAttribute (panel1, PANEL_LED_10, ATTR_OFF_COLOR, currentcolor); x = loadboard(columnpick,5);
}
else if (rowindicator[columnpick] == 6) {
SetCtrlAttribute (panel1, PANEL_LED_3, ATTR_OFF_COLOR, currentcolor); x = loadboard(columnpick,6);
} else {
//column is full
x = 0; }
if (x == 0) {
if (rowindicator[columnpick] <= row) {
rowindicator[columnpick]++; }
else {
//column is full }
changeplayer(); }
DefaultCtrl (panel1, PANEL_PICTUREBUTTON_3);
break; }
return 0; }
/************************************************************ Column4()
Called when player selects the fourth columm and depending
on what row is currently available, it stores the selection
in the board ************************************************************/ int CVICALLBACK Column4 (int panel, int control, int event,
void *callbackData, int eventData1, int eventData2)
{
int currentcolor; int x;
if (player == 1) {
currentcolor = player1color; }
else if (player == 2) {
currentcolor = player2color; }
else {
currentcolor = computerplayercolor; }
columnpick = 4; switch (event)
{
case EVENT_COMMIT:
if (rowindicator[columnpick] == 1) //row 1 is next row available
20

{
SetCtrlAttribute (panel1, PANEL_LED_39, ATTR_OFF_COLOR, currentcolor); x = loadboard(columnpick,1);
}
else if (rowindicator[columnpick] == 2) {
SetCtrlAttribute (panel1, PANEL_LED_32, ATTR_OFF_COLOR, currentcolor); x = loadboard(columnpick,2);
}
else if (rowindicator[columnpick] == 3) {
SetCtrlAttribute (panel1, PANEL_LED_25, ATTR_OFF_COLOR, currentcolor); x = loadboard(columnpick,3);
}
else if (rowindicator[columnpick] == 4) {
SetCtrlAttribute (panel1, PANEL_LED_18, ATTR_OFF_COLOR, currentcolor); x = loadboard(columnpick,4);
}
else if (rowindicator[columnpick] == 5) {
SetCtrlAttribute (panel1, PANEL_LED_11, ATTR_OFF_COLOR, currentcolor); x = loadboard(columnpick,5);
}
else if (rowindicator[columnpick] == 6) {
SetCtrlAttribute (panel1, PANEL_LED_4, ATTR_OFF_COLOR, currentcolor); x = loadboard(columnpick,6);
} else {
//column is full
x = 0; }
if (x == 0) {
if (rowindicator[columnpick] <= row) {
rowindicator[columnpick]++; }
else {
//column is full }
changeplayer(); }
DefaultCtrl (panel1, PANEL_PICTUREBUTTON_4);
break; }
return 0; }
/************************************************************ Column5()
Called when player selects the fifth columm and depending
on what row is currently available, it stores the selection
in the board ************************************************************/ int CVICALLBACK Column5 (int panel, int control, int event,
void *callbackData, int eventData1, int eventData2)
{
int currentcolor; int x;
if (player == 1) {
currentcolor = player1color; }
else if (player == 2) {
currentcolor = player2color; }
else {
currentcolor = computerplayercolor; }
columnpick = 5; switch (event)
{
case EVENT_COMMIT:
if (rowindicator[columnpick] == 1) //row 1 is next row available {
SetCtrlAttribute (panel1, PANEL_LED_40, ATTR_OFF_COLOR, currentcolor);
21

x = loadboard(columnpick,1); }
else if (rowindicator[columnpick] == 2) {
SetCtrlAttribute (panel1, PANEL_LED_33, ATTR_OFF_COLOR, currentcolor); x = loadboard(columnpick,2);
}
else if (rowindicator[columnpick] == 3) {
SetCtrlAttribute (panel1, PANEL_LED_26, ATTR_OFF_COLOR, currentcolor); x = loadboard(columnpick,3);
}
else if (rowindicator[columnpick] == 4) {
SetCtrlAttribute (panel1, PANEL_LED_19, ATTR_OFF_COLOR, currentcolor); x = loadboard(columnpick,4);
}
else if (rowindicator[columnpick] == 5) {
SetCtrlAttribute (panel1, PANEL_LED_12, ATTR_OFF_COLOR, currentcolor); x = loadboard(columnpick,5);
}
else if (rowindicator[columnpick] == 6) {
SetCtrlAttribute (panel1, PANEL_LED_5, ATTR_OFF_COLOR, currentcolor); x = loadboard(columnpick,6);
} else {
//column is full
x = 0; }
if (x == 0) {
if (rowindicator[columnpick] <= row) {
rowindicator[columnpick]++; }
else {
//column is full }
changeplayer(); }
DefaultCtrl (panel1, PANEL_PICTUREBUTTON_5);
break; }
return 0; }
/************************************************************ Column6()
Called when player selects the sixth columm and depending
on what row is currently available, it stores the selection
in the board ************************************************************/ int CVICALLBACK Column6 (int panel, int control, int event,
void *callbackData, int eventData1, int eventData2)
{
int currentcolor; int x;
if (player == 1) {
currentcolor = player1color; }
else if (player == 2) {
currentcolor = player2color; }
else {
currentcolor = computerplayercolor; }
columnpick = 6; switch (event)
{
case EVENT_COMMIT:
if (rowindicator[columnpick] == 1) //row 1 is next row available {
SetCtrlAttribute (panel1, PANEL_LED_41, ATTR_OFF_COLOR, currentcolor);
x = loadboard(columnpick,1); }
22

else if (rowindicator[columnpick] == 2) {
SetCtrlAttribute (panel1, PANEL_LED_34, ATTR_OFF_COLOR, currentcolor); x = loadboard(columnpick,2);
}
else if (rowindicator[columnpick] == 3) {
SetCtrlAttribute (panel1, PANEL_LED_27, ATTR_OFF_COLOR, currentcolor); x = loadboard(columnpick,3);
}
else if (rowindicator[columnpick] == 4) {
SetCtrlAttribute (panel1, PANEL_LED_20, ATTR_OFF_COLOR, currentcolor); x = loadboard(columnpick,4);
}
else if (rowindicator[columnpick] == 5) {
SetCtrlAttribute (panel1, PANEL_LED_13, ATTR_OFF_COLOR, currentcolor); x = loadboard(columnpick,5);
}
else if (rowindicator[columnpick] == 6) {
SetCtrlAttribute (panel1, PANEL_LED_6, ATTR_OFF_COLOR, currentcolor); x = loadboard(columnpick,6);
} else {
//column is full
x = 0; }
if (x == 0) {
if (rowindicator[columnpick] <= row) {
rowindicator[columnpick]++; }
else {
//column is full }
changeplayer(); }
DefaultCtrl (panel1, PANEL_PICTUREBUTTON_6);
break; }
return 0; }
/************************************************************ Column7()
Called when player selects the seventh columm and depending
on what row is currently available, it stores the selection
in the board ************************************************************/ int CVICALLBACK Column7 (int panel, int control, int event,
void *callbackData, int eventData1, int eventData2)
{
int currentcolor; int x;
if (player == 1) {
currentcolor = player1color; }
else if (player == 2) {
currentcolor = player2color; }
else {
currentcolor = computerplayercolor; }
columnpick = 7; switch (event)
{
case EVENT_COMMIT:
if (rowindicator[columnpick] == 1) //row 1 is next row available {
SetCtrlAttribute (panel1, PANEL_LED_42, ATTR_OFF_COLOR, currentcolor);
x = loadboard(columnpick,1); }
else if (rowindicator[columnpick] == 2) {
23

SetCtrlAttribute (panel1, PANEL_LED_35, ATTR_OFF_COLOR, currentcolor); x = loadboard(columnpick,2);
}
else if (rowindicator[columnpick] == 3) {
SetCtrlAttribute (panel1, PANEL_LED_28, ATTR_OFF_COLOR, currentcolor); x = loadboard(columnpick,3);
}
else if (rowindicator[columnpick] == 4) {
SetCtrlAttribute (panel1, PANEL_LED_21, ATTR_OFF_COLOR, currentcolor); x = loadboard(columnpick,4);
}
else if (rowindicator[columnpick] == 5) {
SetCtrlAttribute (panel1, PANEL_LED_14, ATTR_OFF_COLOR, currentcolor); x = loadboard(columnpick,5);
}
else if (rowindicator[columnpick] == 6) {
SetCtrlAttribute (panel1, PANEL_LED_7, ATTR_OFF_COLOR, currentcolor); x = loadboard(columnpick,6);
} else {
//column is full
x = 0; }
if (x == 0) {
if (rowindicator[columnpick] <= row) {
rowindicator[columnpick]++; }
else {
//column is full }
changeplayer(); }
DefaultCtrl (panel1, PANEL_PICTUREBUTTON_7);
break; }
return 0; }