




const int population_size = 100;


# define NONE   0
# define CASH   1
# define BOOZE  2
# define SNACKS 3
# define SMOKES 4

int n_trade_goods = 4;


struct Citizen
{
	int owned_items[n_trade_goods];
	float prices[n_trade_goods];
	float reputations[population_size];

	Citizen()
	{
		for (int i = 0; i < n_trade_goods; ++i)
		{
			this.prices = 1.0f;
			this.owned_items = 0;
		}
		for (int i = 0; i < population_size; ++i)
		{
			this.reputations[i] = 0.0f;
		}

	}



};





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
				if (citizens[b].owned_items[i] > a_needs_i])
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

	int total_tradeable_goods_a = 0;
	int total_tradeable_goods_b = 0;

	for (int i = 0; i < n_trade_goods; ++i)
	{
		if (what_a_has_that_b_wants > 0.0f)
		{
			total_tradeable_goods_a += what_a_has_that_b_wants[i];
		}
		if (what_b_has_that_a_wants > 0.0f)
		{
			total_tradeable_goods_b += what_b_has_that_a_wants[i];
		}
	}

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
			float value_of_this_good_to_a = what_b_has_that_a_wants[i] * citizens[a].prices;
			float value_of_this_good_to_b = what_a_has_that_b_wants[i] * citizens[b].prices;
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
		float rep_adjustment_a = tradeable_volume_from_b_to_a - (citizens[a].prices * what_b_has_that_a_wants[a_wants_most]);
		float rep_adjustment_b = tradeable_volume_from_a_to_b - (citizens[b].prices * what_a_has_that_b_wants[b_wants_most]);

		











	}










}









int main ()
{


	return 0;
}