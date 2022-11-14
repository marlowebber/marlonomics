
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <random>
#include <iostream>
#include <chrono>
#include <thread>

uint32_t za = 521288629;
uint32_t xorshift32()
{
	uint32_t zx = za;
	zx ^= zx << 13;
	zx ^= zx >> 17;
	zx ^= zx << 5;
	za = zx;
	return zx;
}
static std::default_random_engine randomizer;
float RNG()
{
	static std::uniform_real_distribution<> dis(0, 1);
	return dis(randomizer);
}

float clamp (float in, float min, float max)
{
	if (in < min)
	{
		return min;
	}
	else if (in > max)
	{
		return max;
	}
	return in;
}

float fast_sigmoid(float in)
{
	// https://stackoverflow.com/questions/10732027/fast-sigmoid-algorithm
	float out = (in / (1 + abs(in)));
	return  out;
}

const float gossip_const      = 0.1f;
const float trade_const      = 0.1f;
const float impression_const = 0.1f;

const float maximum_like = 1.0f;

# define NONE    0
# define CASH    1
# define BOOZE   2
# define SNACKS  3
# define SMOKES  4
# define PAPERS  5
# define SPIN    6
# define FILTERS 7
# define LIGHTER 8

const static std::string item_names[] =
{
	std::string("none"),
	std::string("cash"),
	std::string("booze"),
	std::string("snacks"),
	std::string("smokes"),
	std::string("papers"),
	std::string("spin"),
	std::string("filters"),
	std::string("lighter"),

};

const int n_trade_goods = 8;
const int map_x = 80;
const int map_y = 24;

#define CUTE 1
#define FUNNY 2
#define SMART 3
#define TOUGH 4
#define PRETTY 5

const int n_characteristics = 5;

#define SAID_SOMETHING 1
#define DID_SOMETHING  2
#define TRADE_DEAL     3

bool print_chatter = true;
bool print_trade = true;
bool print_camera = true;
bool print_stats = true;
bool print_crafting = true;

bool pac = false;

const static std::string characteristic_names[] =
{

	std::string("cute"),
	std::string("funny"),
	std::string("smart"),
	std::string("tough"),
	std::string("cool"),
};


const static std::string person_names[] =
{
	std::string("Brandon"),
	std::string("Kyle"),
	std::string("Keelan"),
	std::string("Jonah"),
	std::string("Jeremy"),
	std::string("Molly"),
	std::string("Holly"),
	std::string("Kaylah"),
	std::string("Mikayla"),
	std::string("Bree"),
	std::string("Shaun"),
	std::string("Jake"),
	std::string("Stephen"),
	std::string("Mitchell"),
	std::string("Bryce"),
	std::string("Brittney"),
	std::string("Ella"),
	std::string("Kaliah"),
	std::string("Emily"),
	std::string("Rayleigh"),
	std::string("River"),
	std::string("Morgan"),
	std::string("Zen"),
	std::string("Ziggy"),
	std::string("Ollie"),
	std::string("Alex"),
	std::string("Alisha"),
	std::string("Amber"),
	std::string("Ellie"),
	std::string("Maddy"),
	std::string("Sarah"),
	std::string("Michelle"),
	std::string("Hayden"),
	std::string("Patrick"),
	std::string("Luke"),
	std::string("Connor"),
	std::string("Jarrad"),
	std::string("Sam"),
	std::string("Lewis"),
	std::string("Jay"),
	std::string("Krissy"),
	std::string("Ruby"),
	std::string("Julian"),
	std::string("Ben"),
	std::string("Jessica"),
	std::string("Lauren"),
	std::string("Jesse"),
	std::string("Ryan"),
	std::string("Olivia"),
	std::string("Danielle"),
	std::string("Jas"),
	std::string("Khalla"),
	std::string("Philippa"),
	std::string("Amy"),
	std::string("Lili"),
	std::string("Arden"),
	std::string("Jayden"),
	std::string("Jack"),
	std::string("Storm"),
	std::string("Zack"),
	std::string("Elise"),
	std::string("Nicola"),
	std::string("Trey"),
	std::string("Kai"),
	std::string("Justin"),
	std::string("Alia"),
	std::string("Isaac"),
	std::string("Monisha"),
	std::string("Josh"),
	std::string("Charlie"),
	std::string("Nathan"),
	std::string("Rebecca"),
	std::string("Peter"),
	std::string("Elena"),
	std::string("Lina"),
	std::string("Karlee"),
	std::string("Nicky"),
	std::string("Dylan"),
	std::string("Lindsay"),
	std::string("Rowan"),
	std::string("Anne"),
	std::string("Mittens"),
	std::string("CJ"),
	std::string("Cooper"),
	std::string("Siena"),
	std::string("Bianca"),
	std::string("Jasmine"),
	std::string("Meg"),
	std::string("Cat"),
	std::string("Swamp Beast"),
};

const int n_person_names = 100;
int name_cursor = 0;

std::string new_person_name()
{
	name_cursor++;
	return person_names[name_cursor % n_person_names];
}

std::string location_names[] =
{
	std::string("The Lord Raglan Hotel"),
	std::string("Nite Owl Convenience"),
	std::string("The Vending Machine"),
	std::string("The Valley"),
};

const int n_location_names = 4;
const int pc = n_location_names; // index of player character

const unsigned int name_length = 16;

const int population_size = n_person_names + n_location_names;



// std::string float_to_string_very(float in)
// {



// 	if (in >0.9f)
// 	{
// 		return std::string("")
// 	}
// 	else if (in > 0.65f)
// 	{

// 	}
// 	else if (in > 0.65f)
// 	{

// 	}
// 	else if (in > 0.65f)
// 	{

// 	}
// 	else if (in > 0.65f)
// 	{

// 	}
// 	else if (in > 0.65f)
// 	{

// 	}

// }




char suggest_icon()
{
	char icon;
	while (true)
	{
		icon = xorshift32() % 255;

		if (icon != 0
		        && icon != '_'
		        && icon != ' '
		        && icon != '\n'
		        && icon != '\r'
		        && icon != '	')
		{
			return icon;
		}

	}

}






struct Citizen
{
	float owned_items[n_trade_goods];
	float prices[n_trade_goods];
	float needs[n_trade_goods];
	float likes[population_size];
	float knows[population_size];
	float owes[population_size];

	int sources[n_trade_goods];
	float source_prices[n_trade_goods];

	int sales_quantity_increment[n_trade_goods];

	int position_x;
	int position_y;

	float personality[n_characteristics];
	float ideals[n_characteristics];

	char icon;

	bool traded_this_turn;
	bool chatted_this_turn;

	std::string name;

	bool is_a_location;



	Citizen()
	{
		this->name = person_names[ (xorshift32() % n_person_names) ];

		this->icon = suggest_icon();

		this->position_x = xorshift32() % map_x;
		this->position_y = xorshift32() % map_y;

		this->is_a_location = false;
		for (int i = 0; i < n_trade_goods; ++i)
		{
			this->prices[i] = 1.0f + (RNG() - 0.5f);
			this->owned_items[i] = 0.0f;//xorshift32() % 3;
			this->needs[i] = 0;
			this->sources[i] = -1;
			this->source_prices[i] = 1.0f;
		}
		for (int i = 0; i < population_size; ++i)
		{
			this->likes[i] = (RNG() - 0.5f) * 2.0f;
			this->knows[i] = 0.0f;
		}
		for (int i = 0; i < n_characteristics; ++i)
		{
			this->personality[i] = (RNG() - 0.5f) * 2.0f;
			this->ideals[i] = (RNG() - 0.5f) * 2.0f;
		}

		for (int i = 0; i < n_trade_goods; ++i)
		{
			if (RNG() < 0.05)
			{
				this->owned_items[i] = 100;
			}
		}

		this->owned_items[CASH] = RNG() * 100.0f;

	}
};

Citizen citizens[population_size];




const float eat_per_turn = 0.001f;
const float drink_per_turn = 0.002f;
const float smoke_per_turn = 0.005f;

void eat_breathe_etc(int a)
{
	if (citizens[a].owned_items[SNACKS] > 0)
	{
		if (RNG() < eat_per_turn)
		{
			if (print_crafting  && (pac || (a == pc))  )
			{
				printf("%s ate a snack\n", citizens[a].name.c_str());
			}
			citizens[a].owned_items[SNACKS] -= 1;
		}
	}

	if (citizens[a].owned_items[SMOKES] > 0
	        && citizens[a].owned_items[LIGHTER] > 0
	   )
	{
		if (RNG() < smoke_per_turn)
		{
			if (print_crafting  && (pac || (a == pc)))
			{
				printf("%s smoked a cigarette\n", citizens[a].name.c_str());
			}
			citizens[a].owned_items[SMOKES] -= 1;
			citizens[a].owned_items[LIGHTER] -= 1;
		}
	}

	if (citizens[a].owned_items[BOOZE] > 0)
	{
		if (RNG() < drink_per_turn)
		{
			if (print_crafting  && (pac || (a == pc)))
			{
				printf("%s drank a drink\n", citizens[a].name.c_str());
			}
			citizens[a].owned_items[BOOZE] -= 1;
		}
	}
}








void needs_controller(int a)
{

	for (int i = 0; i < n_trade_goods; ++i)
	{
		citizens[a].needs[i] = 0.0;
	}

	for (int i = 0; i < n_trade_goods; ++i)
	{
		for (int j = 0; j < population_size; ++j)
		{
			citizens[a].needs[i] += citizens[a].owes[i];
		}
	}


	// if you need something which can be made from component parts, compile a list of those parts and seek them out.


	eat_breathe_etc(a);


}




// one person asks another to sell them an item, and the other person makes a series of offers, the best of which is chosen.
void trade(int a, int b, int a_wants_most, float how_much_a_wants)
{


	if (a_wants_most == NONE || how_much_a_wants <= 0.0f) {return;}
	if (citizens[b].owned_items[a_wants_most] > 0.0f)
	{


		if (print_trade  && (pac || (a == pc || b == pc))  )
		{
			printf("%s has %f %s that %s wants!\n", citizens[b].name.c_str(), how_much_a_wants, item_names[a_wants_most].c_str(), citizens[a].name.c_str());
		}

		if (how_much_a_wants > citizens[b].owned_items[a_wants_most])
		{
			how_much_a_wants = citizens[b].owned_items[a_wants_most];

			if (print_trade  && (pac || (a == pc || b == pc)) )
			{
				printf("the trade was scaled to match b's quantity!\n");
			}

		}


		// printf("%f %s\n", how_much_a_wants, item_names[a_wants_most].c_str());


		float how_much_a_thinks_the_offer_is_worth = how_much_a_wants * citizens[a].prices[a_wants_most];
		float how_much_b_thinks_the_offer_is_worth = how_much_a_wants * citizens[b].prices[a_wants_most];


		float rep_adjusted_price_b = how_much_b_thinks_the_offer_is_worth / (citizens[a].likes[b] + 1.0f) ;

		float offers[n_trade_goods];

		for (int i = 0; i < n_trade_goods; ++i)
		{
			offers[i] = rep_adjusted_price_b / citizens[b].prices[i] ;

			if (offers[i] > (citizens[a].owned_items[i])) // it's not going to work if you can't supply the entire quantity.
			{
				offers[i] = 0.0f;//citizens[a].owned_items[i];



			}


			if (i == NONE || i == a_wants_most) { offers[i] = 0.0f;}

			if (offers[i] > 0.0f)
			{
				if (print_trade  && (pac || (a == pc || b == pc)) )
				{
					printf("offer of %f %s\n", offers[i], item_names[i].c_str());
				}
			}


		}

		int b_wants_most = -1;
		int best_value_amount = 0.0f;

		bool offers_refused[n_trade_goods];

		for (int i = 0; i < n_trade_goods; ++i)
		{

			if ( !(citizens[b].is_a_location)
			        || (citizens[b].is_a_location && ( i == CASH))
			   )
				if (i != NONE)
				{
					if (offers[i] > 0.0f)
					{
						// a picks the best offer.
						float offer_value_to_a = offers[i] * citizens[a].prices[i];

						if (offer_value_to_a > best_value_amount)
						{
							best_value_amount = offer_value_to_a;
							b_wants_most = i;

							if (print_trade   && (pac || (a == pc || b == pc)) )
							{
								printf("offer of %s selected\n", item_names[i].c_str());
							}
						}
						else
						{
							if (print_trade   && (pac || (a == pc || b == pc)) )
							{
								printf("offer of %s rejected as not worth it\n", item_names[i].c_str());
							}
						}
						// }
					}
				}


		}

		for (int i = 0; i < n_trade_goods; ++i)
		{
			if (i == b_wants_most)
			{
				offers_refused[i] = false;
			}
			else
			{
				offers_refused[i] = true;
			}


		}





		if (b_wants_most != -1)
		{
			if (print_trade  && (pac || (a == pc || b == pc)))
			{
				printf("agreed to the offer!\n");
			}
			// a deal can be struck!
			citizens[a].traded_this_turn = true;
			citizens[b].traded_this_turn = true;

			// exchange the goods
			citizens[a].owned_items[b_wants_most] -= offers[b_wants_most];
			citizens[b].owned_items[b_wants_most] += offers[b_wants_most];


			citizens[b].owned_items[a_wants_most] -= how_much_a_wants;//what_b_has_that_a_wants[a_wants_most];
			citizens[a].owned_items[a_wants_most] += how_much_a_wants;//what_b_has_that_a_wants[a_wants_most];

			// the rep adjustment is basically just how much you think you profited

			float rep_adjustment_a =  fast_sigmoid( (  (offers[b_wants_most] * citizens[a].prices[b_wants_most]) - (how_much_a_wants * citizens[a].prices[a_wants_most])) * trade_const )  ; //how_much_a_thinks_the_offer_is_worth - (citizens[a].prices[a_wants_most] * how_much_a_wants); //what_b_has_that_a_wants[a_wants_most]);
			float rep_adjustment_b =  fast_sigmoid( ( (how_much_a_wants *  citizens[b].prices[a_wants_most] )   - (offers[b_wants_most] *  citizens[b].prices[b_wants_most])) * trade_const )  ;//how_much_a_thinks_the_offer_is_worth - (citizens[a].prices[a_wants_most] * how_much_a_wants); //what_b_has_that_a_wants[a_wants_most]);



			// float rep_adjustment_b =   how_much_b_thinks_the_offer_is_worth   ;//how_much_b_thinks_the_offer_is_worth - (citizens[b].prices[b_wants_most] * offers[b_wants_most] );//what_a_has_that_b_wants[b_wants_most]);

			if (print_trade  && (pac || (a == pc || b == pc)))
			{

				printf("	%s traded %f %s to %s, for %f %s\n.		Because of this, %s's opinion of %s changed by %f\n",
				       citizens[a].name.c_str(), offers[b_wants_most], item_names[b_wants_most].c_str(),
				       citizens[b].name.c_str(), how_much_a_wants, item_names[a_wants_most].c_str(),
				       citizens[a].name.c_str(), citizens[b].name.c_str(),
				       rep_adjustment_a
				       // rep_adjustment_b
				      );
			}


			citizens[a].likes[b] += rep_adjustment_a;
			// citizens[a].likes[b] += rep_adjustment_b;

			citizens[a].likes[b] = clamp(citizens[a].likes[b], -maximum_like, maximum_like);
			// citizens[b].likes[a] = clamp(citizens[b].likes[a], -maximum_like, maximum_like);

			// they also remember each other as sources.
			if (citizens[b].prices[a_wants_most] < citizens[a].source_prices[a_wants_most])
			{
				citizens[a].sources[a_wants_most] = b;
				citizens[a].source_prices[a_wants_most] = citizens[b].prices[a_wants_most];
			}


		}
		else
		{



			// refer the client to someone else.
			citizens[a].sources[a_wants_most] = citizens[b].sources[a_wants_most];
			citizens[a].source_prices[a_wants_most] = citizens[b].source_prices[a_wants_most];


		}



		for (int i = 0; i < n_trade_goods; ++i)
		{
			if (offers[i] > 0.0f)
			{
				if (offers_refused[i])
				{
					citizens[b].prices[i] *= 0.95f;
				}
				else
				{
					citizens[b].prices[i] *= 1.05f;
				}
			}
		}



		// if (citizens[a].sources[a_wants_most] == b)
		// {
		// 	// you can't get this thing from b right now.
		// 	citizens[a].sources[a_wants_most] = -1;
		// }


		// if (citizens[b].sources[b_wants_most] == a)
		// {
		// 	// you can't get this thing from b right now.
		// 	citizens[b].sources[b_wants_most] = -1;
		// }

		// two characters exchange information about their sources;
		// for (int i = 0; i < n_trade_goods; ++i)
		// {

		// if (citizens[a].needs[i] > 0.0f)
		// {

		// if (citizens[b].source_prices[a_wants_most] < citizens[a].source_prices[a_wants_most]
		//         || citizens[a].source_prices[a_wants_most] == -1
		//    )
		// {

		// 	if (print_trade)
		// 	{
		// 		printf("	%s recommended %s as a source of %s\n",
		// 		       citizens[b].name.c_str(), citizens[a].name.c_str(), item_names[a_wants_most].c_str());
		// 	}
		// 	citizens[a].source_prices[a_wants_most] = citizens[b].source_prices[a_wants_most];
		// 	citizens[a].sources[a_wants_most] = citizens[b].sources[a_wants_most];
		// }
		// }



		// // if (citizens[b].needs[i] > 0.0f)
		// // {
		// if (citizens[a].source_prices[b_wants_most] < citizens[b].source_prices[b_wants_most]
		//         || citizens[b].source_prices[b_wants_most] == -1 )
		// {

		// 	if (print_trade)
		// 	{
		// 		printf("	%s recommended %s as a source of %s\n",
		// 		       citizens[a].name.c_str(), citizens[b].name.c_str(), item_names[b_wants_most].c_str());
		// 	}

		// 	citizens[b].source_prices[b_wants_most] = citizens[a].source_prices[b_wants_most];
		// 	citizens[b].sources[b_wants_most] = citizens[a].sources[b_wants_most];
		// }
		// }
		// }






		citizens[a].knows[b] += 1.0f;
		citizens[b].knows[a] += 1.0f;







	}

}














// void trade_old(int a, int b)
// {
// 	// a deal is negotiated between the two parties that maximises the utility for both.
// 	float what_a_has_that_b_wants[n_trade_goods];
// 	float what_b_has_that_a_wants[n_trade_goods];
// 	for (int i = 0; i < n_trade_goods; ++i)
// 	{
// 		what_a_has_that_b_wants[i] = 0.0f;
// 		what_b_has_that_a_wants[i] = 0.0f;
// 	}

// 	bool deal = false;



// 	for (int i = 0; i < n_trade_goods; ++i)
// 	{

// 		if (i != NONE)
// 		{
// 			what_a_has_that_b_wants[i] = 0.0f;

// 			if (
// 			    (!(citizens[b].is_a_location))
// 			    || (citizens[b].is_a_location && i == CASH)

// 			)
// 			{
// 				if (citizens[b].needs[i] > 0.0f)
// 				{
// 					if (citizens[b].needs[i] > citizens[b].owned_items[i])
// 					{
// 						float required_amount = citizens[b].needs[i] - citizens[b].owned_items[i];
// 						float available_amount = citizens[a].owned_items[i] - citizens[a].needs[i];
// 						if (available_amount > 0.0f)
// 						{
// 							if (available_amount > required_amount)
// 							{
// 								what_a_has_that_b_wants[i] = required_amount;
// 							}
// 							else
// 							{
// 								what_a_has_that_b_wants[i] = available_amount;
// 							}


// 							// printf("%s has some %s that %s wants\n", citizens[a].name.c_str(), item_names[i].c_str(),  citizens[b].name.c_str() );

// 							// for (int m = 0; m < n_trade_goods; ++m)
// 							// {
// 							// 	printf("	a has %f needs %f\n", citizens[a].owned_items[m], citizens[a].needs[m] );
// 							// }
// 							// for (int m = 0; m < n_trade_goods; ++m)
// 							// {
// 							// 	printf("	b has %f needs %f\n", citizens[b].owned_items[m], citizens[b].needs[m] );
// 							// }
// 						}
// 					}
// 				}
// 			}

// 			what_b_has_that_a_wants[i] = 0.0f;
// 			if (
// 			    (!(citizens[a].is_a_location))
// 			    || (citizens[a].is_a_location && i == CASH)

// 			)
// 			{
// 				if (citizens[a].needs[i] > 0.0f)
// 				{
// 					if (citizens[b].needs[i] > citizens[b].owned_items[i])
// 					{
// 						float required_amount =  citizens[a].needs[i] - citizens[a].owned_items[i];
// 						float available_amount = citizens[b].owned_items[i] - citizens[b].needs[i];
// 						if (available_amount > 0.0f)
// 						{
// 							if (available_amount > required_amount)
// 							{
// 								what_b_has_that_a_wants[i] = required_amount;
// 							}
// 							else
// 							{
// 								what_b_has_that_a_wants[i] = available_amount;
// 							}


// 							// printf("%s has some %s that %s wants\n", citizens[a].name.c_str(), item_names[i].c_str(),  citizens[b].name.c_str() );
// 						}
// 					}
// 				}
// 			}
// 		}
// 		else
// 		{

// 			what_a_has_that_b_wants[i] = 0.0f;
// 			what_b_has_that_a_wants[i] = 0.0f;
// 		}

// 	}

// 	float total_tradeable_goods_a = 0;
// 	float total_tradeable_goods_b = 0;

// 	for (int i = 0; i < n_trade_goods; ++i)
// 	{
// 		if (i != NONE)
// 		{
// 			if (what_a_has_that_b_wants[i] > 0.0f)
// 			{
// 				total_tradeable_goods_a += what_a_has_that_b_wants[i];

// 				if (print_trade)
// 				{
// 					printf("%s has some %s that %s wants\n", citizens[a].name.c_str(), item_names[i].c_str(),  citizens[b].name.c_str() );
// 				}
// 			}
// 			if (what_b_has_that_a_wants[i] > 0.0f)
// 			{
// 				total_tradeable_goods_b += what_b_has_that_a_wants[i];

// 				if (print_trade)
// 				{
// 					printf("%s has some %s that %s wants\n", citizens[b].name.c_str(), item_names[i].c_str(),  citizens[a].name.c_str() );
// 				}
// 			}
// 		}
// 	}

// 	int a_wants_most = NONE;
// 	float a_greatest_value_seen = 0.0f;
// 	int b_wants_most = NONE;
// 	float b_greatest_value_seen = 0.0f;

// 	float price_point_from_a_to_b      ;
// 	float price_point_from_b_to_a      ;
// 	float tradeable_volume_from_a_to_b ;
// 	float tradeable_volume_from_b_to_a ;


// 	if (total_tradeable_goods_a > 0.0f && total_tradeable_goods_b > 0.0f)
// 	{
// 		printf("an exchange can be made!\n");

// 		// each party picks the kind of stuff they want from the other party.
// 		deal = true;

// 		for (int i = 0; i < n_trade_goods; ++i)
// 		{
// 			float value_of_this_good_to_a = what_b_has_that_a_wants[i] * citizens[a].prices[i];
// 			float value_of_this_good_to_b = what_a_has_that_b_wants[i] * citizens[b].prices[i];
// 			if (value_of_this_good_to_a > a_greatest_value_seen)
// 			{
// 				a_wants_most = i;
// 				a_greatest_value_seen = value_of_this_good_to_a;
// 			}
// 			if (value_of_this_good_to_b > b_greatest_value_seen)
// 			{
// 				b_wants_most = i;
// 				b_greatest_value_seen = value_of_this_good_to_b;
// 			}
// 		}

// 		if (print_trade)
// 		{
// 			printf("possible deal. %s wants %s's %s, and %s wants %s's %s\n",
// 			       citizens[a].name.c_str(), citizens[b].name.c_str(), item_names[a_wants_most].c_str(),
// 			       citizens[b].name.c_str(), citizens[a].name.c_str(), item_names[b_wants_most].c_str()
// 			      );
// 		}



// 		if (a_wants_most == b_wants_most)
// 		{
// 			// they both want the same stuff- there is no acceptable outcome!
// 			// return;
// 			if (print_trade)
// 			{
// 				printf("want the same stuff, no deal\n");
// 			}
// 			deal = false;
// 		}

// 		if (a_wants_most == NONE || b_wants_most == NONE)
// 		{
// 			// it basically means, one of them didn't want anything.
// 			// return;

// 			if (print_trade)
// 			{
// 				printf("wants none, no deal\n");
// 			}
// 			deal = false;
// 		}

// 		// a compromise is figured out so that each party gets what they think is a fair portion.
// 		price_point_from_a_to_b = (citizens[a].prices[b_wants_most] + citizens[b].prices[b_wants_most]) / 2.0f;
// 		price_point_from_b_to_a = (citizens[a].prices[a_wants_most] + citizens[b].prices[a_wants_most]) / 2.0f;
// 		tradeable_volume_from_a_to_b = price_point_from_a_to_b * what_a_has_that_b_wants[b_wants_most];
// 		tradeable_volume_from_b_to_a = price_point_from_b_to_a * what_b_has_that_a_wants[a_wants_most];

// 		if (tradeable_volume_from_a_to_b > 0.0f && tradeable_volume_from_b_to_a > 0.0f)
// 		{
// 			// scale the amounts by what's available on the bargaining floor
// 			if (tradeable_volume_from_a_to_b > tradeable_volume_from_b_to_a)
// 			{
// 				what_a_has_that_b_wants[b_wants_most] *= ( tradeable_volume_from_b_to_a / tradeable_volume_from_a_to_b );
// 			}
// 			else if (tradeable_volume_from_b_to_a > tradeable_volume_from_a_to_b)
// 			{
// 				what_b_has_that_a_wants[a_wants_most] *= ( tradeable_volume_from_a_to_b / tradeable_volume_from_b_to_a );
// 			}
// 		}
// 		else
// 		{
// 			// return;
// 			if (print_trade)
// 			{
// 				printf("cannot reconcile tradeable volume\n");
// 			}
// 			deal = false;
// 		}



// 	}





// }




void gossip(int a, int b)
{

	// float a_impression_b = 1.0f;
	// float b_impression_a = 1.0f;

	int a_does_something = xorshift32() % n_characteristics;

	float a_impression_b =  fast_sigmoid((citizens[a].ideals[a_does_something] - citizens[b].personality[a_does_something]) * impression_const ) ;
	float b_impression_a =  fast_sigmoid((citizens[b].ideals[a_does_something] - citizens[a].personality[a_does_something]) * impression_const ) ;

	citizens[a].likes[b] += a_impression_b;
	citizens[b].likes[a] += b_impression_a;

	citizens[a].likes[b] = clamp(citizens[a].likes[b], -maximum_like, maximum_like);
	citizens[b].likes[a] = clamp(citizens[b].likes[a], -maximum_like, maximum_like);
	if (print_chatter   && (pac || (a == pc || b == pc)))
	{
		printf("\n");
		printf("%s and %s hung out. They like each other %f and %f\n",
		       citizens[a].name.c_str(), citizens[b].name.c_str(), citizens[a].likes[b], citizens[b].likes[a]);





		printf("	 %s said something %s and %s thought it was %f. That made %s think %f of %s.\n",
		       citizens[a].name.c_str(), characteristic_names[ a_does_something].c_str(),
		       citizens[b].name.c_str(),  a_impression_b,
		       citizens[a].name.c_str(), b_impression_a,
		       citizens[b].name.c_str()
		      );

	}






	// the two characters talk about someone that they both know and their reputation of that character changes.

	int random_start = xorshift32() % population_size;
	for (int i = 0; i < population_size; ++i)
	{
		int person_to_discuss = (random_start + i) % population_size;
		if ((
		            abs( citizens[a].likes[person_to_discuss] ) > 0.0f
		            || abs(citizens[b].likes[person_to_discuss]  ) > 0.0f
		        )
		        && person_to_discuss != a && person_to_discuss != b
		   )
		{
			int c = person_to_discuss;

			citizens[a].chatted_this_turn = true;
			citizens[b].chatted_this_turn = true;

			float how_much_a_likes_c = (citizens[a].likes[c] / maximum_like);
			float how_much_b_likes_c = (citizens[b].likes[c] / maximum_like);

			float how_much_a_likes_b = (citizens[a].likes[b] / maximum_like);
			float how_much_b_likes_a = (citizens[b].likes[a] / maximum_like);

			// how much more A likes B after the interaction.
			float a_rep_b = how_much_b_likes_c  * how_much_a_likes_c * gossip_const;
			float b_rep_a = how_much_a_likes_c  * how_much_b_likes_c * gossip_const;

			float b_rep_c  =  how_much_a_likes_c  * how_much_b_likes_a  * gossip_const;
			float a_rep_c  =  how_much_b_likes_c  * how_much_a_likes_b * gossip_const;

			if (print_chatter   && (pac || (a == pc || b == pc)))
			{
				printf("	They talked about %s. They like them %f and %f. Their opinions of %s changed by %f and %f.",
				       citizens[c].name.c_str(),  citizens[a].likes[c], citizens[b].likes[c],
				       citizens[c].name.c_str(),  a_rep_c, b_rep_c);


				printf(" Their opinions of each other changed by %f and %f\n", a_rep_b, b_rep_a);
			}

			citizens[a].likes[b] += a_rep_b;
			citizens[a].likes[c] += a_rep_c;

			citizens[b].likes[a] += b_rep_a;
			citizens[b].likes[c] += b_rep_c;

			citizens[a].likes[b] = clamp(citizens[a].likes[b], -maximum_like, maximum_like);
			citizens[a].likes[c] = clamp(citizens[a].likes[c], -maximum_like, maximum_like);
			citizens[b].likes[a] = clamp(citizens[b].likes[a], -maximum_like, maximum_like);
			citizens[b].likes[c] = clamp(citizens[b].likes[c], -maximum_like, maximum_like);




			break; // only talk about one person.
		}
	}
}



void crafting(int a)
{

	if (citizens[a].needs[SMOKES] > 0)
	{
		if ( (citizens[a].owned_items[FILTERS] > 0 )
		        && (citizens[a].owned_items[SPIN] > 0 )
		        && (citizens[a].owned_items[PAPERS] > 0 ))

		{
			citizens[a].owned_items[FILTERS] -= 1;
			citizens[a].owned_items[SPIN] -= 1;
			citizens[a].owned_items[PAPERS] -= 1;
			citizens[a].owned_items[SMOKES] += 1;
			if (print_crafting  && (pac || (a == pc)))
			{
				printf("%s rolled a smoke\n", citizens[a].name.c_str());
			}
		}
	}


}








void update_position_based_on_needs(int a)
{

	// what is the biggest need?
	int biggest_need = NONE;
	float biggest_need_amount = 0.0f;
	int biggest_need_source = -1;


	int destination_x;
	int destination_y;
	bool go = false;

	for (int i = 0; i < n_trade_goods; ++i)
	{
		if (i != NONE)
		{

			float need_this = citizens[a].needs[i] - citizens[a].owned_items[i];
			if (need_this > biggest_need_amount
			   )
			{
				biggest_need = i;
				biggest_need_amount = citizens[a].needs[i];
				biggest_need_source = citizens[a].sources[i];

			}
		}
	}

	// where are the known sources of this?
	if (biggest_need != NONE && biggest_need_source != NONE && biggest_need_source >= 0 && biggest_need_source < population_size)
	{
		if (a == pc)
		{
			printf("going to %s to get some %s\n",  citizens[  biggest_need_source ].name.c_str(),  item_names[biggest_need].c_str());
		}

		destination_x = citizens[  biggest_need_source ].position_x;
		destination_y = citizens[  biggest_need_source ].position_y;
		go = true;

	}
	else if (biggest_need == NONE)
	{



		// if you don't need anything, go hang out with a friend

		// printf("hang out with a friend.\n");
		int best_friend = -1;
		float likes_best_friend = 0.0f;
		for (int i = 0; i < population_size; ++i)
		{
			if (!citizens[i].is_a_location)
			{
				if (citizens[a].likes[i] > likes_best_friend)
				{
					best_friend = i;
					likes_best_friend = citizens[a].likes[i];
				}
			}
		}

		if (best_friend > -1)
		{


			if (a == pc)
			{
				printf("%s is going to visit %s\n", citizens[a].name.c_str(), citizens[best_friend].name.c_str());
			}


			destination_x = citizens[best_friend].position_x;
			destination_y = citizens[best_friend].position_y;
			go = true;
		}

	}

	else if (biggest_need != NONE && biggest_need_source == -1 && biggest_need_source == a)
	{
		// printf("talk to a random person\n");

		// you need something, but don't know where to get it

		// go look for it by seeking out random people

		int randomPotentialSource = xorshift32() % population_size;
		citizens[a].sources[biggest_need] = randomPotentialSource;



		if (a == pc)
		{
			printf("%s needed %s, but doesn't know where to get any, so they will try asking %s.\n",
			       citizens[a].name.c_str(),  item_names[biggest_need].c_str(), citizens[randomPotentialSource].name.c_str());
		}


	}









	// go there
	if ( (!(citizens[a].is_a_location)))
	{

		if (go )
		{

			// printf("moving to destination\n");
			if (destination_x > citizens[a].position_x)
			{
				citizens[a].position_x -= (xorshift32() % 2) - 1;
			}
			else if (destination_x < citizens[a].position_x)
			{
				citizens[a].position_x += (xorshift32() % 2) - 1;
			}


			if (destination_y > citizens[a].position_y)
			{
				citizens[a].position_y -= (xorshift32() % 2) - 1;
			}
			else if (destination_y < citizens[a].position_y)
			{
				citizens[a].position_y += (xorshift32() % 2) - 1;
			}

		}

		else
		{



			if (a == pc)
			{
				printf("%s doesn't have a destination, they're just vibing.\n", citizens[a].name.c_str());
			}


			// printf("just kind of milling around.\n");
			citizens[a].position_x += (xorshift32() % 3) - 1;
			citizens[a].position_y += (xorshift32() % 3) - 1;
		}


	}

}



void do_location_stuff(int i)
{
	// 		std::string location_names[] =
	// {
	// 	std::string("The Lord Raglan Hotel"),
	// 	std::string("Nite Owl Convenience"),
	// 	std::string("The Vending Machine"),
	// 	std::string("The Valley"),
	// };


	if (!citizens[i].name.compare(location_names[0]))
	{
		citizens[i].owned_items[BOOZE] = 100;
		citizens[i].prices[BOOZE] = 10.0f;


	}
	if (!citizens[i].name.compare(location_names[1]))
	{
		citizens[i].owned_items[FILTERS] = 100;
		citizens[i].owned_items[PAPERS] = 100;
		citizens[i].owned_items[SPIN] = 100;


		citizens[i].prices[FILTERS] = 1.0f;
		citizens[i].prices[PAPERS] = 1.0f;
		citizens[i].prices[SPIN] = 1.0f;
	}
	if (!citizens[i].name.compare(location_names[2]))
	{
		citizens[i].owned_items[SNACKS] = 100;
		citizens[i].prices[SNACKS] = 1.0f;
	}
	if (!citizens[i].name.compare(location_names[3]))
	{
		citizens[i].owned_items[BOOZE] = 100;
		citizens[i].prices[BOOZE] = 1.0f;
	}



	citizens[i].needs[CASH] = 100;
}



void update(int turn)
{







	for (int i = 0; i < population_size; ++i)
	{

		if (i == pc)
		{
			printf("\n--- %s turn %i ---\n", citizens[i].name.c_str(), turn);
		}


		if (print_stats  && (pac || (i == pc)))
		{
			printf("%s's turn\n", citizens[i].name.c_str());
			for (int j = 0; j < n_trade_goods; ++j)
			{
				if (j != NONE)
				{
					std::string source_name = std::string("");
					if (citizens[i].sources[j] >= 0)
					{
						source_name = citizens[ citizens[i].sources[j] ].name;

					}
					printf("	has %s %f, needs %f. source: %s\n", item_names[j].c_str(), citizens[i].owned_items[j], citizens[i].needs[j],

					       source_name .c_str()
					      );


				}
			}

			for (int j = 0; j < n_trade_goods; ++j)
			{
				if (j != NONE)
				{
					printf("	price of %s %f\n", item_names[j].c_str(), citizens[i].prices[j]);
				}
			}
		}



		int met_with = -1;//(xorshift32() % population_size) ;

		citizens[i].chatted_this_turn = false;
		citizens[i].traded_this_turn = false;

		unsigned int random_start = xorshift32();
		for (int j = 0; j < population_size; ++j)
		{
			int k = (random_start + j) % population_size;
			if (citizens[i].position_x == citizens[k].position_x
			        && citizens[i].position_y == citizens[k].position_y
			        && k != i
			   )
			{
				met_with = k;
				break;
			}
		}



		if (citizens[i].is_a_location)
		{
			do_location_stuff(i);
		}
		else
		{


			// citizens[i].owned_items[CASH] += 0.5f;


			if (met_with != -1)
			{


				if (! citizens[met_with].is_a_location)
				{
					gossip(i, met_with);
				}



				for (int j = 0; j < n_trade_goods; ++j)
				{
					if (citizens[i].needs[j] > 0.0f)
					{

						trade(i, met_with, j, citizens[i].needs[j] );
					}


				}

			}


			// craft(i);
			needs_controller(i);
			update_position_based_on_needs(i);

		}
	}
}




const int viewport_x = 80;
const int viewport_y = 24;

void camera()
{
	for (int y = 0; y < viewport_y; ++y)
	{
		std::string row = "";
		for (int x = 0; x < viewport_x; ++x)
		{
			char here = '_';
			for (int k = 0; k < population_size; ++k)
			{
				if (citizens[k].position_x == x && citizens[k].position_y == y )
				{
					here = citizens[k].icon;

					if (citizens[k].chatted_this_turn)
					{
						here = '?';
					}
					if (citizens[k].traded_this_turn)
					{
						here = '$';
					}

					break;
				}
			}
			printf("%c", here);
		}
		printf("\n");
	}
}




std::string get_input(std::string message)
{

	printf( "%s\n", message.c_str());

	std::string result = std::string("");

	std::cin >> result;

	return result;

}



int extract_number_from_string(std::string str)
{
	printf("%s\n", str.c_str());
	std::string strubstring = std::string("");
	for (size_t i = 0 ; i < str.length(); i++ )
	{
		if ( isdigit(str[i]) )
		{strubstring.push_back(str[i]);}
	}
	printf("%s\n", strubstring.c_str());
	int id = atoi(strubstring.c_str());
	return id;
}


int extract_name_from_string(std::string str)
{

// return the index of the person whose name this most probably is.
	int similarities[population_size];

	for (int i = 0; i < str.length(); ++i)
	{
		similarities[i] = 0;
	}

	for (int i = 0; i < str.length(); ++i)
	{
		for (int j = 0; j < population_size; ++j)
		{

		}
	}

}


int main ()
{

	for (int i = 0; i < population_size; ++i)
	{
		citizens[i] = Citizen();
	}


	for (int i = 0; i < n_location_names; ++i)
	{
		citizens[i].name = location_names[i];
		citizens[i].is_a_location = true;



		// std::string location_names[] =
		// {
		// 	std::string("The Lord Raglan Hotel"),
		// 	std::string("Nite Owl Convenience"),
		// 	std::string("The Vending Machine"),
		// 	std::string("The Valley"),
		// };




		if (!citizens[i].name.compare(location_names[0]))
		{
			citizens[i].owned_items[BOOZE] = 100;

			citizens[i].icon = 'R';

			for (int j = 0; j < population_size; ++j)
			{
				citizens[j].sources[BOOZE] = i;
			}
		}
		if (!citizens[i].name.compare(location_names[1]))
		{
			citizens[i].owned_items[FILTERS] = 100;
			citizens[i].owned_items[PAPERS] = 100;
			citizens[i].owned_items[SPIN] = 100;
			citizens[i].owned_items[LIGHTER] = 100;


			citizens[i].icon = 'O';

			for (int j = 0; j < population_size; ++j)
			{
				citizens[j].sources[FILTERS] = i;
				citizens[j].sources[PAPERS] = i;
				citizens[j].sources[SPIN] = i;
				citizens[j].sources[LIGHTER] = i;
			}
		}
		if (!citizens[i].name.compare(location_names[2]))
		{


			citizens[i].icon = 'M';

			citizens[i].owned_items[SNACKS] = 100;
			for (int j = 0; j < population_size; ++j)
			{
				citizens[j].sources[SNACKS] = i;
			}

		}
		if (!citizens[i].name.compare(location_names[3]))
		{

			citizens[i].icon = 'V';

			citizens[i].owned_items[BOOZE] = 100;
			for (int j = 0; j < population_size; ++j)
			{
				citizens[j].sources[BOOZE] = i;
			}

		}


	}


	int continue_turns = 0;

	for (int i = 0; i < 1000; ++i)
	{
		// printf("turn %i\n", i);
		update(i);

		if (print_camera)
		{
			camera();
		}

		// std::this_thread::sleep_for(std::chrono::milliseconds(100));

		if (continue_turns > 0)
		{
			continue_turns --;
		}
		else
		{
			std::string input_string = get_input("type what you do here >");

			std::size_t found = input_string.find("trade");
			if (found != std::string::npos)
			{
				printf("> 'trade'\n");
			}

			std::size_t  found2 = input_string.find("auto");
			if (found2 != std::string::npos)
			{
				int n = extract_number_from_string(input_string);

				continue_turns = n;
			}

		}







	}

	return 0;
}