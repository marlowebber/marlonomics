
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <cstring>
#include <string>

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





std::string trade_good_names(int i)
{
	switch (i)
	{
	case CASH:
		return std::string("CASH");
		break;
	case BOOZE:
		return std::string("BOOZE");
		break;
	case SNACKS:
		return std::string("SNACKS");
		break;
	case SMOKES:
		return std::string("SMOKES");
		break;

	}
	return std::string("NONE");
}





struct Citizen
{
	float owned_items[n_trade_goods];
	float prices[n_trade_goods];
	float reputations[population_size];

	int job;



	float gossip; // how much the character is influenced by the opnions of others
	float stingy; // how poor of a deal the character will accept

	Citizen()
	{
		for (int i = 0; i < n_trade_goods; ++i)
		{
			this->prices[i] = 1.0f;
			this->owned_items[i] = 0;
		}
		for (int i = 0; i < population_size; ++i)
		{
			this->reputations[i] = 0.0f;
		}

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

		if (citizens[b].owned_items[i] < 0.0f)
		{

			float b_needs_i = abs(citizens[b].owned_items[i]);

			if (citizens[a].owned_items[i] > 0.0f)
			{
				if (citizens[a].owned_items[i] > b_needs_i)
				{
					what_a_has_that_b_wants[i] = b_needs_i;
				}
				else
				{
					what_a_has_that_b_wants[i] = citizens[a].owned_items[i];
				}
			}
		}

		what_b_has_that_a_wants[i] = 0.0f;
		if (citizens[a].owned_items[i] < 0.0f)
		{

			float a_needs_i = abs(citizens[a].owned_items[i]);
			if (citizens[b].owned_items[i] > 0.0f)
			{
				if (citizens[b].owned_items[i] > a_needs_i)
				{
					what_b_has_that_a_wants[i] = a_needs_i;
				}
				else
				{
					what_b_has_that_a_wants[i] = citizens[b].owned_items[i];
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

		// a compromise is figured out so that each party gets what they think is a fair portion.
		float price_point_from_a_to_b = (citizens[a].prices[b_wants_most] + citizens[b].prices[b_wants_most]) / 2.0f;
		float price_point_from_b_to_a = (citizens[a].prices[a_wants_most] + citizens[b].prices[a_wants_most]) / 2.0f;

		float tradeable_volume_from_a_to_b = price_point_from_a_to_b * what_a_has_that_b_wants[b_wants_most];
		float tradeable_volume_from_b_to_a = price_point_from_b_to_a * what_b_has_that_a_wants[a_wants_most];

		if (tradeable_volume_from_a_to_b > tradeable_volume_from_b_to_a)
		{
			what_a_has_that_b_wants[b_wants_most] *= ( tradeable_volume_from_b_to_a / tradeable_volume_from_a_to_b );
		}
		else if (tradeable_volume_from_b_to_a > tradeable_volume_from_a_to_b)
		{
			what_b_has_that_a_wants[a_wants_most] *= ( tradeable_volume_from_a_to_b / tradeable_volume_from_b_to_a );
		}

		// exchange the goods
		citizens[a].owned_items[b_wants_most] -= what_a_has_that_b_wants[b_wants_most];
		citizens[b].owned_items[b_wants_most] += what_a_has_that_b_wants[b_wants_most];

		citizens[b].owned_items[a_wants_most] -= what_b_has_that_a_wants[a_wants_most];
		citizens[a].owned_items[a_wants_most] += what_b_has_that_a_wants[a_wants_most];

		// if the other's portion is more or less than what was expected, there is a reputation adjustment. Bigger deals have a bigger impact.
		float rep_adjustment_a = tradeable_volume_from_b_to_a - (citizens[a].prices[a_wants_most] * what_b_has_that_a_wants[a_wants_most]);
		float rep_adjustment_b = tradeable_volume_from_a_to_b - (citizens[b].prices[b_wants_most] * what_a_has_that_b_wants[b_wants_most]);

		citizens[a].reputations[b] += rep_adjustment_a;
		citizens[a].reputations[b] += rep_adjustment_b;


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





		printf("Citizen %i traded with citizen %i.\n    %f of %i exchanged for %f of %i\n",

		       a, b, what_b_has_that_a_wants[a_wants_most], a_wants_most , what_a_has_that_b_wants[b_wants_most], b_wants_most
		      );

		printf("rep a %f, b %f\n", citizens[a].reputations[b], citizens[b].reputations[a]);

		printf("    rep_adjustment_a %f rep_adjustment_b %f \n", rep_adjustment_a, rep_adjustment_b);

	}

}


void gossip(int a, int b)
{
	// the two characters talk about someone that they both know and their reputation of that character becomes more similar.

	int random_start = xorshift32() % population_size;
	for (int i = 0; i < population_size; ++i)
	{

		int person_to_discuss = (random_start + i) % population_size;
		if (
		    abs( citizens[a].reputations[person_to_discuss] ) > 0.0f
		    || abs(citizens[b].reputations[person_to_discuss]  ) > 0.0f
		)
		{
			float rep_adjustment_a =

			    // the adjustment is the difference in the people's opinion
			    (citizens[b].reputations[person_to_discuss] -  citizens[a].reputations[person_to_discuss])

			    // multiplied by how much this person likes the person they're talking to, and by how much this person can be influenced in general.
			    * (citizens[a].reputations[b]) * 0.25f;

			float rep_adjustment_b = (citizens[a].reputations[person_to_discuss] -  citizens[b].reputations[person_to_discuss]) * (citizens[b].reputations[a]) * 0.25f;
			
			printf("%i and %i talked about %i. They like each other %f, %f. They like the person they discussed %f, %f. Their opinions changed by %f and %f\n", 
			a, b, person_to_discuss, citizens[a].reputations[b], citizens[b].reputations[a],
			citizens[a].reputations[person_to_discuss], citizens[b].reputations[person_to_discuss], rep_adjustment_a, rep_adjustment_b);

			citizens[a].reputations[person_to_discuss] += rep_adjustment_a;
			citizens[b].reputations[person_to_discuss] += rep_adjustment_b;


			break; // only talk about one person.
		}
	}
}




void get_new_stuff(int a)
{

	int kind_of_stuff = xorshift32() % n_trade_goods;
	citizens[a].owned_items[kind_of_stuff] += 1.0f;

}



void use_some_stuff(int a)
{

	int kind_of_stuff = xorshift32() % n_trade_goods;
	citizens[a].owned_items[kind_of_stuff] -= 1.0f;

}

void update()
{
	for (int i = 0; i < population_size; ++i)
	{
		int met_with = (xorshift32() % population_size) ;

		printf("citizen %i met with %i \n", i, met_with);

		trade(i, met_with );

		gossip(i, met_with);

		get_new_stuff(i);
		use_some_stuff(i);
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
		printf("turn %i\n", i);
		update();
	}

	return 0;
}