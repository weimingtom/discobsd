#include "monop.h"
#include "deck.h"

bool	fixing,			/* set if fixing up debt		*/
	trading,		/* set if in process of trading		*/
	told_em,		/* set if told user he's out of debt	*/
	spec;			/* set if moving by card to RR or UTIL	*/

char	*name_list[MAX_PL+2],	/* list of players' names		*/
	*comlist[]	= {	/* list of normal commands		*/
	"quit",		/*  0 */	"print",	/*  1 */
	"where",	/*  2 */	"own holdings",	/*  3 */
	"holdings",	/*  4 */	"shell",	/*  5 */
	"mortgage",	/*  6 */	"unmortgage",	/*  7 */
	"buy houses",	/*  8 */	"sell houses",	/*  9 */
	"card",		/* 10 */	"pay",		/* 11 */
	"trade",	/* 12 */	"resign",	/* 13 */
	"save",		/* 14 */	"restore",	/* 15 */
	"roll",		/* 16 */	"",		/* 17 */
	0
	},
	*yesno[]	= {	/* list of commands for yes/no answers	*/
	"yes",		/*  0 */	"no",		/*  1 */
	"quit",		/*  2 */	"print",	/*  3 */
	"where",	/*  4 */	"own holdings",	/*  5 */
	"holdings",	/*  6 */	"shell",	/*  7 */
	0
	},
	*lucky_mes[]	= {	/* "got lucky" messages			*/
	"You lucky stiff",		"You got lucky",
	"What a lucky person!",		"You must have a 4-leaf clover",
	"My, my!  Aren't we lucky!",	"Luck smiles upon you",
	"You got lucky this time",	"Lucky person!",
	"Your karma must certainly be together",
	"How beautifully Cosmic",	"Wow, you must be really with it"
	/* "I want your autograph",	-- Save for later */
	};

int	player,			/* current player number		*/
	num_play,		/* current number of players		*/
	num_doub,		/* # of doubles current player rolled	*/
				/* # of "got lucky" messages		*/
	num_luck	= sizeof lucky_mes / sizeof (char *),
				/* list of command functions		*/
	buy_houses(), card(), do_move(), do_move(), list(), list_all(),
	mortgage(), pay(), printboard(), quitgame(), resign(), restore(),
	rub(), save(), sell_houses(), shell_out(), trade(),
	unmortgage(), where(),
	(*func[])()	= {	/* array of function calls for commands	*/
	quitgame,		/* quit game		|*  0 *|	*/
	printboard,		/* print board		|*  1 *|	*/
	where,			/* where players are	|*  2 *|	*/
	list,			/* own holdings		|*  3 *|	*/
	list_all,		/* holdings list	|*  4 *|	*/
	shell_out,		/* shell		|*  5 *|	*/
	mortgage,		/* mortgage property	|*  6 *|	*/
	unmortgage,		/* unmortgage property	|*  7 *|	*/
	buy_houses,		/* buy houses		|*  8 *|	*/
	sell_houses,		/* sell houses		|*  9 *|	*/
	card,			/* card for jail	|* 10 *|	*/
	pay,			/* pay for jail		|* 11 *|	*/
	trade,			/* trade		|* 12 *|	*/
	resign,			/* resign		|* 13 *|	*/
	save,			/* save game		|* 14 *|	*/
	restore,		/* restore game		|* 15 *|	*/
	do_move,		/* roll			|* 16 *|	*/
	do_move			/* ""			|* 17 *|	*/
	};

DECK	deck[2];		/* Chance and Community Chest		*/

PLAY	*play,			/* player structure array ("calloc"ed)	*/
	*cur_p;			/* pointer to current player's struct	*/

RR_S	rr[N_RR];		/* raildroad descriptions		*/

UTIL_S	util[2];		/* utility descriptions			*/

MON	mon[N_MON]	= {	/* monopoly descriptions		*/
#include "mon.dat.h"
};

PROP	prop[N_PROP]	= {	/* typical properties			*/
#include "prop.dat.h"
};

SQUARE	board[N_SQRS+1]	= {	/* board itself (+1 for Jail)		*/
#include "brd.dat.h"
};
