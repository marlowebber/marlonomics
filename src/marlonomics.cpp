
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




const float maximum_like = 1.0f;


const int population_size = 100;


# define NONE    0
# define CASH    1
# define BOOZE   2
# define SNACKS  3
# define SMOKES  4
# define PAPERS  5
# define SPIN    6
# define FILTERS 7
# define LIGHTER 8

const int n_trade_goods = 8;



const int map_x = 80;
const int map_y = 24;

struct Archetype
{
	float cute;
	float tough;
	float funny;
	float smart;
};





std::string names[] =
{
	std::string("Brandon"),
	std::string("Kyle"),
	std::string("Keith"),
	std::string("Joel"),
	std::string("Brandon"),
};




const unsigned int name_length = 16;

struct Citizen
{
	float owned_items[n_trade_goods];
	float prices[n_trade_goods];
	float needs[n_trade_goods];
	float likes[population_size];
	float knows[population_size];

	int sources[n_trade_goods];
	float source_prices[n_trade_goods];

	int position_x;
	int position_y;

	Archetype personality;
	Archetype ideals;

	char icon;

	bool traded_this_turn;
	bool chatted_this_turn;

	std::string name;



	Citizen()
	{


		this->icon = xorshift32() % 255;

		this->position_x = xorshift32() % map_x;
		this->position_y = xorshift32() % map_y;


		for (int i = 0; i < n_trade_goods; ++i)
		{
			this->prices[i] = 1.0f;
			this->owned_items[i] = xorshift32() % 3;
			this->needs[i] = 1;
			this->sources[i] = -1;
			this->source_prices[i] = 1.0f;
		}
		for (int i = 0; i < population_size; ++i)
		{
			this->likes[i] = (RNG()-0.5f) * 2.0f;
			this->knows[i] = 0.0f;
		}
		this->personality.cute = RNG();
		this->personality.tough = RNG();
		this->personality.funny = RNG();
		this->personality.smart = RNG();

		this->ideals.cute = RNG();
		this->ideals.tough = RNG();
		this->ideals.funny = RNG();
		this->ideals.smart = RNG();


	}



};






Citizen citizens[population_size];




void trade(int a, int b)
{
	// a deal is negotiated between the two parties that maximises the utility for both.
	float what_a_has_that_b_wants[n_trade_goods];
	float what_b_has_that_a_wants[n_trade_goods];
	for (int i = 0; i < n_trade_goods; ++i)
	{
		what_a_has_that_b_wants[i] = 0.0f;

		if (citizens[b].needs[i] > 0.0f)
		{
			if (citizens[b].needs[i] > citizens[b].owned_items[i])
			{
				float required_amount = citizens[b].needs[i] - citizens[b].owned_items[i];
				if (citizens[a].owned_items[i] > 0.0f)
				{
					if (citizens[a].owned_items[i] > required_amount)
					{
						what_a_has_that_b_wants[i] = required_amount;
					}
					else
					{
						what_a_has_that_b_wants[i] = citizens[a].owned_items[i];
					}
				}
			}
		}

		what_b_has_that_a_wants[i] = 0.0f;
		if (citizens[a].needs[i] > 0.0f)
		{
			if (citizens[b].needs[i] > citizens[b].owned_items[i])
			{
				float required_amount =  citizens[a].needs[i] - citizens[a].owned_items[i];
				if (citizens[b].owned_items[i] > 0.0f)
				{
					if (citizens[b].owned_items[i] > required_amount)
					{
						what_b_has_that_a_wants[i] = required_amount;
					}
					else
					{
						what_b_has_that_a_wants[i] = citizens[b].owned_items[i];
					}
				}
			}
		}
	}

	float total_tradeable_goods_a = 0;
	float total_tradeable_goods_b = 0;

	for (int i = 0; i < n_trade_goods; ++i)
	{
		if (what_a_has_that_b_wants[i] > 0.0f)
		{
			total_tradeable_goods_a += what_a_has_that_b_wants[i];
		}
		if (what_b_has_that_a_wants[i] > 0.0f)
		{
			total_tradeable_goods_b += what_b_has_that_a_wants[i];
		}
	}

	// printf("tradeable a %f, tradeable b %f \n",total_tradeable_goods_a, total_tradeable_goods_b );

	if (total_tradeable_goods_a > 0.0f && total_tradeable_goods_b > 0.0f)
	{
		// a deal can be struck!
		citizens[a].traded_this_turn = true;
		citizens[b].traded_this_turn = true;

		// each party picks the kind of stuff they want from the other party.
		int a_wants_most = NONE;
		float a_greatest_value_seen = 0.0f;
		int b_wants_most = NONE;
		float b_greatest_value_seen = 0.0f;

		for (int i = 0; i < n_trade_goods; ++i)
		{
			float value_of_this_good_to_a = what_b_has_that_a_wants[i] * citizens[a].prices[i];
			float value_of_this_good_to_b = what_a_has_that_b_wants[i] * citizens[b].prices[i];
			if (value_of_this_good_to_a > a_greatest_value_seen)
			{
				a_wants_most = i;
				a_greatest_value_seen = value_of_this_good_to_a;
			}
			if (value_of_this_good_to_b > b_greatest_value_seen)
			{
				b_wants_most = i;
				b_greatest_value_seen = value_of_this_good_to_b;
			}
		}

		if (a_wants_most == b_wants_most)
		{
			// they both want the same stuff- there is no acceptable outcome!
			return;
		}

		// a compromise is figured out so that each party gets what they think is a fair portion.
		float price_point_from_a_to_b = (citizens[a].prices[b_wants_most] + citizens[b].prices[b_wants_most]) / 2.0f;
		float price_point_from_b_to_a = (citizens[a].prices[a_wants_most] + citizens[b].prices[a_wants_most]) / 2.0f;

		float tradeable_volume_from_a_to_b = price_point_from_a_to_b * what_a_has_that_b_wants[b_wants_most];
		float tradeable_volume_from_b_to_a = price_point_from_b_to_a * what_b_has_that_a_wants[a_wants_most];

		if (tradeable_volume_from_a_to_b > 0.0f && tradeable_volume_from_b_to_a > 0.0f)
		{
			// scale the amounts by what's available on the bargaining floor
			if (tradeable_volume_from_a_to_b > tradeable_volume_from_b_to_a)
			{
				what_a_has_that_b_wants[b_wants_most] *= ( tradeable_volume_from_b_to_a / tradeable_volume_from_a_to_b );
			}
			else if (tradeable_volume_from_b_to_a > tradeable_volume_from_a_to_b)
			{
				what_b_has_that_a_wants[a_wants_most] *= ( tradeable_volume_from_a_to_b / tradeable_volume_from_b_to_a );
			}
		}
		else
		{
			return;
		}



		// exchange the goods
		citizens[a].owned_items[b_wants_most] -= what_a_has_that_b_wants[b_wants_most];
		citizens[b].owned_items[b_wants_most] += what_a_has_that_b_wants[b_wants_most];


		citizens[b].owned_items[a_wants_most] -= what_b_has_that_a_wants[a_wants_most];
		citizens[a].owned_items[a_wants_most] += what_b_has_that_a_wants[a_wants_most];

		// if the other's portion is more or less than what was expected, there is a reputation adjustment. Bigger deals have a bigger impact.
		float rep_adjustment_a = tradeable_volume_from_b_to_a - (citizens[a].prices[a_wants_most] * what_b_has_that_a_wants[a_wants_most]);
		float rep_adjustment_b = tradeable_volume_from_a_to_b - (citizens[b].prices[b_wants_most] * what_a_has_that_b_wants[b_wants_most]);

		citizens[a].likes[b] += rep_adjustment_a;
		citizens[a].likes[b] += rep_adjustment_b;



		// they also remember each other as sources.
		if (price_point_from_b_to_a < citizens[a].source_prices[a_wants_most])
		{
			citizens[a].sources[a_wants_most] = b;
			citizens[a].source_prices[a_wants_most] = price_point_from_b_to_a;
		}

		if (price_point_from_a_to_b < citizens[b].source_prices[b_wants_most])
		{
			citizens[b].sources[b_wants_most] = a;
			citizens[b].source_prices[b_wants_most] = price_point_from_a_to_b;
		}



		// citizens adjust their prices to meet the market demands.
		if (price_point_from_b_to_a > citizens[a].prices[a_wants_most])
		{
			citizens[a].prices[a_wants_most] *= 1.1f;
		}
		else
		{
			citizens[a].prices[a_wants_most] *= 0.9f;
		}
		if (price_point_from_a_to_b > citizens[a].prices[b_wants_most])
		{
			citizens[a].prices[b_wants_most] *= 1.1f;
		}
		else
		{
			citizens[a].prices[b_wants_most] *= 0.9f;
		}




		if (price_point_from_a_to_b > citizens[b].prices[b_wants_most])
		{
			citizens[b].prices[b_wants_most] *= 1.1f;
		}
		else
		{
			citizens[b].prices[b_wants_most] *= 0.9f;
		}
		if (price_point_from_b_to_a > citizens[b].prices[a_wants_most])
		{
			citizens[b].prices[a_wants_most] *= 1.1f;
		}
		else
		{
			citizens[b].prices[a_wants_most] *= 0.9f;
		}


		citizens[a].knows[b] += 1.0f;
		citizens[b].knows[a] += 1.0f;


		// printf("Citizen %i traded with citizen %i.\n    %f of %i exchanged for %f of %i\n",

		//        a, b, what_b_has_that_a_wants[a_wants_most], a_wants_most , what_a_has_that_b_wants[b_wants_most], b_wants_most
		//       );

		// printf("rep a %f, b %f\n", citizens[a].likes[b], citizens[b].likes[a]);

		// printf("    rep_adjustment_a %f rep_adjustment_b %f \n", rep_adjustment_a, rep_adjustment_b);

	}

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

float gossip_const = 0.5f;

void gossip(int a, int b)
{



	// an initial impression is formed based on how each ascribes to the other's ideals.

	// if (citizens[a].knows[b] == 0)
	// {
	// 	float initial_impression = 1.0;
	// 	initial_impression -= ( citizens[a].ideals.cute  - citizens[b].personality.cute )  * 0.5f;
	// 	initial_impression -= ( citizens[a].ideals.funny - citizens[b].personality.funny ) * 0.5f;
	// 	initial_impression -= ( citizens[a].ideals.tough - citizens[b].personality.tough ) * 0.5f;
	// 	initial_impression -= ( citizens[a].ideals.smart - citizens[b].personality.smart ) * 0.5f;
	// 	citizens[a].likes[b] = initial_impression;
	// }

	// if (citizens[b].knows[a] == 0)
	// {
	// 	float initial_impression = 1.0;
	// 	initial_impression -= ( citizens[b].ideals.cute  - citizens[a].personality.cute )  * 0.5f;
	// 	initial_impression -= ( citizens[b].ideals.funny - citizens[a].personality.funny ) * 0.5f;
	// 	initial_impression -= ( citizens[b].ideals.tough - citizens[a].personality.tough ) * 0.5f;
	// 	initial_impression -= ( citizens[b].ideals.smart - citizens[a].personality.smart ) * 0.5f;
	// 	citizens[b].likes[a] = initial_impression;
	// }



	// two characters exchange information about their sources;
	for (int i = 0; i < n_trade_goods; ++i)
	{
		if (citizens[b].source_prices[i] < citizens[a].source_prices[i]  || citizens[a].source_prices[i] == -1)
		{
			citizens[a].source_prices[i] = citizens[b].source_prices[i];
			citizens[a].sources[i] = citizens[b].sources[i];
		}
		else if (citizens[a].source_prices[i] < citizens[b].source_prices[i] || citizens[b].source_prices[i] == -1 )
		{
			citizens[b].source_prices[i] = citizens[a].source_prices[i];
			citizens[b].sources[i] = citizens[a].sources[i];
		}
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


			float a_and_b_disagree =  how_much_a_likes_c - how_much_b_likes_c  ; 

			// if someone says something, you agree with them if you know them, and disagree if you know the other person
			float a_rep_b = -a_and_b_disagree * how_much_a_likes_c * gossip_const;
			float b_rep_a = a_and_b_disagree * how_much_b_likes_c * gossip_const;


			float a_rep_c = -a_and_b_disagree * how_much_a_likes_b * gossip_const;
			float b_rep_c = a_and_b_disagree * how_much_b_likes_a * gossip_const;





			// working example.

			/*

			a likes b = 100;

			b likes a = 10;


			a likes c = 20

			b likes c = 70;


			does a care what b thinks?

			yeah.

			does b care about what a thinks?

			no.

			does a like c ?

			no.

			does b like c ?

			hell yeah.





			how does a opinion of c change?
			a likes c a lot more.


			how does a opinion of b change?
			a likes b a little bit more.



			how does b opinion of a change?
			b opinion of a gets a bit worse because a doesn't like C as much as b.



			how does b opinion of c change?
			b opinion of c gets a tiny bit worse, but not much because


			*/






			// float rep_adjustment_a =

			//     (citizens[b].likes[person_to_discuss] -  citizens[a].likes[person_to_discuss])   *  (citizens[a].likes[b]) * gossip_const;

			// float rep_adjustment_b = (citizens[a].likes[person_to_discuss] -  citizens[b].likes[person_to_discuss])
			//                          * (citizens[b].likes[a])    * gossip_const;

			printf("%i and %i talked about %i. They like each other %f, %f.\nThey like the person they discussed %f, %f. Their opinions of this person changed by %f and %f\n",
			       a, b, c, citizens[a].likes[b], citizens[b].likes[a], citizens[a].likes[c], citizens[b].likes[c],
			       a_rep_c, b_rep_c);


			printf("their opinions of each other changed by %f and %f\n", a_rep_b, b_rep_a);


			// citizens[a].likes[person_to_discuss] += rep_adjustment_a;
			// citizens[b].likes[person_to_discuss] += rep_adjustment_b;


			printf("\n");


			citizens[a].likes[b] += a_rep_b;
			citizens[a].likes[c] += a_rep_c;

			citizens[b].likes[a] += b_rep_a;
			citizens[b].likes[c] += b_rep_c;


			citizens[a].likes[b] = clamp(citizens[a].likes[b], -maximum_like, maximum_like);
			citizens[a].likes[c] = clamp(citizens[a].likes[c], -maximum_like, maximum_like);
			citizens[b].likes[a] = clamp(citizens[b].likes[a], -maximum_like, maximum_like);
			citizens[b].likes[c] = clamp(citizens[b].likes[c], -maximum_like, maximum_like);


			// // how this works is:
			// // citizen A and B state how much they like C.
			// // they both change their opinions of C based on how much they like each other.

			// // they both change their opinions of each other based on how much they like C.

			// rep_adjustment_a =  (citizens[b].likes[person_to_discuss] -  citizens[a].likes[person_to_discuss]) *  citizens[a].likes[person_to_discuss]  * gossip_const;
			// rep_adjustment_b =  (citizens[a].likes[person_to_discuss] -  citizens[b].likes[person_to_discuss]) *  citizens[b].likes[person_to_discuss]  * gossip_const;

			// citizens[a].likes[b] += rep_adjustment_a;
			// citizens[b].likes[a] += rep_adjustment_b;



			// citizens[a].knows[b] += 1.0f;
			// citizens[b].knows[a] += 1.0f;


			// citizens[a].knows[person_to_discuss] += 1.0f;
			// citizens[b].knows[person_to_discuss] += 1.0f;



			break; // only talk about one person.
		}
	}
}




void get_new_stuff(int a)
{

	// int kind_of_stuff = xorshift32() % n_trade_goods;
	citizens[a].owned_items[CASH] += 0.5f;

}




const float eat_per_turn = 0.01f;
const float drink_per_turn = 0.02f;
const float smoke_per_turn = 0.05f;

void eat_breathe_etc(int a)
{

	if (citizens[a].owned_items[SNACKS] > eat_per_turn)
	{
		citizens[a].owned_items[SNACKS] -= eat_per_turn;
		citizens[a].needs[SNACKS] = 0;
	}
	else
	{
		citizens[a].needs[SNACKS] = 1;
	}

	if (citizens[a].owned_items[SMOKES] > smoke_per_turn)
	{
		citizens[a].owned_items[SMOKES] -= smoke_per_turn;

		citizens[a].needs[SMOKES] = 0;
	}
	else
	{
		citizens[a].needs[SMOKES] = 1;
	}

	if (citizens[a].owned_items[BOOZE] > drink_per_turn)
	{
		citizens[a].owned_items[BOOZE] -= drink_per_turn;
		citizens[a].needs[BOOZE] = 0;
	}
	else
	{
		citizens[a].needs[BOOZE] = 1;
	}


}



void craft(int a)
{


	if ( (citizens[a].owned_items[FILTERS] >= 1 )
	        && (citizens[a].owned_items[SPIN] >= 1 )
	        && (citizens[a].owned_items[PAPERS] >= 1 ))
	{
		citizens[a].owned_items[FILTERS] -= 1;
		citizens[a].owned_items[SPIN] -= 1;
		citizens[a].owned_items[PAPERS] -= 1;

		citizens[a].owned_items[SMOKES] += 1;

		// printf("citizen %i rolled a smoke\n", a);

	}




}


void update_position_based_on_needs(int a)
{

	// what is the biggest need?
	int biggest_need = NONE;
	float need_amount = 0.0f;
	int biggest_need_source = -1;


	int destination_x;
	int destination_y;
	bool go = false;

	for (int i = 0; i < n_trade_goods; ++i)
	{
		if (citizens[a].needs[i] > need_amount
		        && citizens[a].sources[i] > -1
		   )
		{
			biggest_need = i;
			need_amount = citizens[a].needs[i];
			biggest_need_source = citizens[a].sources[i];
		}
	}

	// where are the known sources of this?
	if (biggest_need_source != NONE && biggest_need_source > -1)
	{
		destination_x = citizens[  biggest_need_source ].position_x;
		destination_y = citizens[  biggest_need_source ].position_y;
		go = true;

	}

	else
	{


		// if there is no known source for the thing you need, go hang out with a friend

		int best_friend = -1;
		float likes_best_friend = 0.0f;
		for (int i = 0; i < population_size; ++i)
		{
			if (citizens[a].likes[i] > likes_best_friend)
			{
				best_friend = i;
				likes_best_friend = citizens[a].likes[i];
			}
		}

		if (best_friend > -1)
		{
			destination_x = citizens[best_friend].position_x;
			destination_y = citizens[best_friend].position_y;
			go = true;
		}



	}









	// go there
	if (go)
	{
		if (destination_x > citizens[a].position_x)
		{
			citizens[a].position_x++;
		}
		else if (destination_x < citizens[a].position_x)
		{
			citizens[a].position_x--;
		}


		if (destination_y > citizens[a].position_y)
		{
			citizens[a].position_y++;
		}
		else if (destination_y < citizens[a].position_y)
		{
			citizens[a].position_y--;
		}

	}

	else
	{

		citizens[a].position_x += (xorshift32() % 3) - 1;
		citizens[a].position_y += (xorshift32() % 3) - 1;
	}


}



void update()
{
	for (int i = 0; i < population_size; ++i)
	{
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

		if (met_with != -1)
		{

			trade(i, met_with);

			gossip(i, met_with);


		}


		get_new_stuff(i);
		craft(i);
		eat_breathe_etc(i);
		update_position_based_on_needs(i);
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





int main ()
{

	for (int i = 0; i < population_size; ++i)
	{
		citizens[i] = Citizen();
	}

	for (int i = 0; i < 100; ++i)
	{
		// printf("turn %i\n", i);
		update();
		camera();

		std::this_thread::sleep_for(std::chrono::milliseconds(100));

	}

	return 0;
}