/*
	HW5 Due : April 15, Sunday, at 11 : 59PM
	 You are given the following problem statement :
	 Develop the design of the cards, deck, shuffle, and deal of the game of Battle Cards.

	 Battle Cards has the following rules :
	 1. There are K(at least 2) players who alternate being dealer.

	 2. The deck has 52 cards with 4 suits and 13 cards per suit.

	 3. Before each game the deck is shuffled by the dealer.

	 4. The deal starts with the non - dealer and alternates between dealer and nondealer.
			All cards are dealt from the deck.

	 5. The players may not look at their cards.

	 6. The game is made up of a series of battles.

	 7. Each battle is played to its conclusion. At the end of a battle if a player has
			less than 5 cards they are out of the game.

	 8. Every player participates in a battle by putting 1 card on the table face up.
			The player with the lowest card wins the battle.  The Ace is the lowest card
			in a suit and the king is the highest. If there is a tie for lowest card, both the
			players play one more cards on to the table face down and another face up.
			The last card of the two played are then compared. The lowest wins. This step
			can be repeated until the battle is won.

	 9. The player winning the battle gets all the cards on the table.

	 During a batter, when a winner is determined.
	 Starting from Player 1, 2, …, take the cards from table and add them to winner’s hand.
	 After that, check one more time from player 1, 2, …, to see if any player’s hand contains less than 5 cards,
	 if yes, add them to the winner’s hand.

	 After that shuffle hand of the winner.

	 10. If during a battle, a player does not have enough cards to continue, they are
			  out of the game and the game is over.

	 11. The game is over when there is only one player left.
 */

 //HW5 Due: April 15, Sunday, at 11:59pm
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <list>
#include <vector>
using namespace std;

ofstream out("data1.txt");

////////////////////////////////////////////////////////
// ==========< class: card >==========//
////////////////////////////////////////////////////////

class card
{
public:
	int number;
	int suit;
	static int uniquenumber;
	card();
	void showcard();
	void showcard_on_screen();
	card * next;
};

card::card()
{
	number = uniquenumber % 13;
	suit = (uniquenumber / 13);
	uniquenumber++;
	next = nullptr;
}

void card::showcard_on_screen()
{
	switch (number)
	{
	case 0: cout << "A"; break;  // Ace is low
	case 10: cout << "J"; break;
	case 11: cout << "Q"; break;
	case 12: cout << "K"; break;
	default: cout << (number + 1); break;
	}

	switch (suit)
	{
	case 0: cout << "S "; break;
	case 1: cout << "H "; break;
	case 2: cout << "D "; break;
	case 3: cout << "C "; break;
	}
}

void card::showcard()
{
	switch (number)
	{
	case 0: out << "A"; break;  // Ace is low
	case 10: out << "J"; break;
	case 11: out << "Q"; break;
	case 12: out << "K"; break;
	default: out << (number + 1); break;
	}

	switch (suit) 
	{
	case 0: out << "S "; break;
	case 1: out << "H "; break;
	case 2: out << "D "; break;
	case 3: out << "C "; break;
	}
}

////////////////////////////////////////////////////////
// ========< class : collection>=========//
////////////////////////////////////////////////////////

class collection
{
public:
	card * start;
	int numberofcards;
	collection(int x);
	collection() { start = nullptr; numberofcards = 0; }
	void showcollection();
	void showcollection_on_screen();
	void shuffle();
	card deal();
	void add(card a);
};

collection::collection(int x)
{
	start = nullptr;
	numberofcards = 0;
	for (int i = 0; i < x; i++)
	{
		card temp;
		add(temp);
	}
}

//---< add() - Add a card to the front of the linked list >---//
void collection::add(card a)
{
	card * new_card = new card(a);
	new_card->next= start;

	start = new_card;
	numberofcards++;
}

void collection::showcollection_on_screen()
{
	int i;
	card * temp;
	temp = start;
	for (i = 0; i < numberofcards; i++)
	{
		temp->showcard_on_screen();
		temp = temp->next;
	}
}

void collection::showcollection()
{
	int i;
	card * temp;
	temp = start;
	for (i = 0; i < numberofcards; i++)
	{
		temp->showcard();
		temp = temp->next;
	}
}

//---< shuffle() - Randomize the sequence of cards in the deck >---//
void collection::shuffle()
{
	card *temp2, *temp3;
	temp2 = start;
	temp3 = temp2;
	int i, j, k;
	char c1;
	int num;
	for (i = numberofcards; i>1; i--) {
		j = rand() % i;
		for (k = 0; k <= j - 1; k++)
			temp3 = temp3->next;
		c1 = temp2->suit;
		num = temp2->number;
		temp2->suit = temp3->suit;
		temp2->number = temp3->number;
		temp3->suit = c1;
		temp3->number = num;
		temp2 = temp2->next;
		temp3 = temp2;
	}
	// Reorganize cards in array randomly
}

//---< deal() - Remove a card from the front of the linked list, and return the card >---//
card collection::deal()
{
	card* ret = start;
	if (start != nullptr)
	{
		start = start->next;
		ret->next = nullptr;
	}
	numberofcards--;
	return *ret;
}

int card::uniquenumber = 0;

void deal_cards(int k, int d, collection &deck, collection *hand);
void battle(int k, collection *hand, collection *table);
void Enter_Num_players_Dealer(int &k_players, int &dealer);
void Show_Initial_Deck_Hands(int &k_players, int &dealer, collection &deck, collection * hand, collection * table);

int main() {
	collection *hand, *table;
	int num_players, dealer, i, winner = 0;
	srand(time(0));
	Enter_Num_players_Dealer(num_players, dealer);
	collection deck(52);
	hand = new collection[num_players];
	table = new collection[num_players];
	Show_Initial_Deck_Hands(num_players, dealer, deck, hand, table);
	battle(num_players, hand, table);
	getchar();
	getchar();
	return 0;
}

void Show_Initial_Deck_Hands(int &k_players, int &dealer, collection &deck, collection * hand, collection * table) {
	out << "\n\n************** initial deck of cards before shuffle ****\n\n";
	deck.showcollection();

	deck.shuffle(); // Shuffle cards in deck
	out << "\n\n****************** after shuffle **************\n\n";
	deck.showcollection();
	
	deal_cards(k_players, dealer, deck, hand);
	out << "\n\n****************** after cards are dealt **************\n\n";
	for (int i = 0; i< k_players; i++)
	{
		out << "\ncards for player " << i + 1 << "\n";
		hand[i].showcollection();
	}
}

void Enter_Num_players_Dealer(int &num_players, int &dealer) {

	do {
		cout << "Enter the number (2-10) of players ";
		cin >> num_players;
		if (num_players > 10 || num_players < 2)
			out << "Illegal player number. Enter again\n\n";
	} while (num_players >10 || num_players < 2);

	do {
		cout << "Enter initial dealer (1-k)";
		cin >> dealer;
		if (dealer < 1 || dealer > num_players)
			out << "Illegal dealer ID. Enter again\n\n";
	} while (dealer < 1 || dealer > num_players);
}

//---< battle() - implements the card battle operaitons and decides outcomes of card battel according to rules >---//
void battle(int k, collection *hand, collection *table) 
{ 
	// k is num_players
	vector<int>* curr_players = new vector<int>(k);
	vector<int>* tie_players = new vector<int>(k);
	int remain_player_num = k; 
	int temp_winner = -1;

	int b = 1;

	while (remain_player_num > 1)
	{		
		cout << "\n\nBattle " << b << endl;
		cout << "====================================";
		b++;

		// initialize the flags
		temp_winner = -1;
		curr_players->clear();
		tie_players->clear();
		bool next_flag = false;
		bool is_regular = true;

		// # player put cards on the table
		while (next_flag == false)
		{
			// show hands
			for (int i = 0; i < k; i++)
			{
				out << "\n\nHand " << i + 1 << endl;
				hand[i].showcollection();
				cout << "\n\nHand " << i + 1 << endl;
				hand[i].showcollection_on_screen();
			}

			// ## regular round, every player plays one card
			if (is_regular)
			{
				cout << "\n\n Regular Round";
				cout << "\n-------------------";

				// ### every player puts card on the table
				for (int i = 0; i < k; i++)
				{
					if (hand[i].numberofcards != 0)
					{
						curr_players->push_back(i);
						// card temp = ;
						table[i].add(hand[i].deal());
					}
				}

				// ### show tables
				for (int i = 0; i < k; i++)
				{
					out << "\n\ntable " << i + 1 << endl;
					table[i].showcollection();
					cout << "\n\ntable " << i + 1 << endl;
					table[i].showcollection_on_screen();
				}

				// show curr_player
				cout << "\n curr_player: ";
				for (size_t i = 0; i < curr_players->size(); i++)
				{
					cout << (*curr_players)[i];
				}

				int min = table[(*curr_players)[0]].start->number;
				temp_winner = (*curr_players)[0];

				// ### find the card with lowest number on the table
				for (size_t i = 1; i < curr_players->size(); i++)
				{
					if (table[(*curr_players)[i]].start->number < min)
					{
						min = table[(*curr_players)[i]].start->number;
					}
				}

				// ### check if there are two or more card with the same lowest number
				for (size_t i = 0; i < curr_players->size(); i++)
				{
					if (table[(*curr_players)[i]].start->number == min)
					{
						tie_players->push_back((*curr_players)[i]);
						temp_winner = (*curr_players)[i];
					}
				}

				if (tie_players->size() > 1)
				{
					is_regular = false;
					continue;
				}

				cout << "\n min = " << min << endl;
				cout << "\n temp_winner = " << temp_winner << endl;
				next_flag = true;
			}
			// ## tie round, play two more cards
			else
			{
				cout << "\n\n Tie Round";
				cout << "\n-------------------";

				// ### two players put two cards on the table
				for (size_t i = 0; i < tie_players->size(); i++)
				{
					cout << "\n tie_players[" << i << "] = " << (*tie_players)[i] << endl;

					//card temp = ;
					table[(*tie_players)[i]].add(hand[(*tie_players)[i]].deal());

					//temp = hand[(*tie_players)[i]].deal();
					table[(*tie_players)[i]].add(hand[(*tie_players)[i]].deal());
				}

				// ### show tables
				for (int i = 0; i < k; i++)
				{
					out << "\n\ntable " << i + 1 << endl;
					table[i].showcollection();
					cout << "\n\ntable " << i + 1 << endl;
					table[i].showcollection_on_screen();
				}

				curr_players->clear();			
				for (size_t i = 0; i < tie_players->size(); i++)
				{
					curr_players->push_back((*tie_players)[i]);
				}
				tie_players->clear();

				// show curr_player
				cout << "\n\n curr_players: ";
				for (size_t i = 0; i < curr_players->size(); i++)
				{
					cout << (*curr_players)[i];
				}
				
				int min = table[(*curr_players)[0]].start->number;
				temp_winner = (*curr_players)[0];

				cout << "\n initial min = " << min;

				// ### find the card with lowest number on the table
				for (size_t i = 1; i < curr_players->size(); i++)
				{
					cout << "\n table[" << (*curr_players)[i] << "] = " << table[(*curr_players)[i]].start->number << endl;
					if (table[(*curr_players)[i]].start->number < min)
					{
						min = table[(*curr_players)[i]].start->number;
					}
				}

				cout << "\n min = " << min;

				// ### check if there are two or more card with the same lowest number
				for (size_t i = 0; i < curr_players->size(); i++)
				{
					if (table[(*curr_players)[i]].start->number == min)
					{
						tie_players->push_back((*curr_players)[i]);
						temp_winner = (*curr_players)[i];
					}
				}

				if (tie_players->size() > 1)
				{
					is_regular = false;
					continue;
				}

				cout << "\n min = " << min;
				cout << "\n temp_winner = " << temp_winner << endl;

				next_flag = true;
			}
		}

		// # winner takes all the cards on the table
		for (int i = 0; i < k; i++)
		{
			while (table[i].numberofcards != 0)
			{
				hand[temp_winner].add(table[i].deal());
			}
		}

		// # check if any player not having enough cards(5)
		for (int i = 0; i < k; i++)
		{
			if (hand[i].numberofcards < 5)
			{				
				while (hand[i].numberofcards > 0)
				{
					hand[temp_winner].add(hand[i].deal());
				}
			}
		}

		hand[temp_winner].shuffle();

		// # check how many players are left
		remain_player_num = 0;
		for (int i = 0; i < k; i++)
		{
			if (hand[i].numberofcards > 0)
			{
				remain_player_num++;
			}
		}
	}	

	out << "\n\nGame Over!!The winner is player " << temp_winner + 1 << endl;
	cout << "\nGame Over!!The winner is player " << temp_winner + 1 << endl;
	// show hands
	for (int i = 0; i < k; i++)
	{
		out << "\n\nFinal hand " << i + 1 << endl;
		hand[i].showcollection();
		cout << "\n\nFinal hand " << i + 1 << endl;
		hand[i].showcollection_on_screen();
	}
}

//---< deal_cards() - deal cards to each player >---//
/*
	At the beginning, when dealing cards,
	if dealer is 2, then then player 3 gets a card first,
	player 4 gets a card second, …..,
	player 2 gets a card last.Repeat this cycle.
*/
void deal_cards(int k, int d, collection &deck, collection *hand) 
{
	int num = deck.numberofcards;
	int next_player = 0; // if dealer if the last player, i is the first player
	if (d != k)
	{
		next_player = d; // i is the next player of the dealer
	}
		
	while (deck.numberofcards > 0)
	{
		card temp = deck.deal();
		hand[next_player].add(temp);
		next_player++;

		if (next_player == k)
		{
			next_player = 0;
		}
	}
}

