#include "helpers.hpp"

typedef std::pair< std::string, int > Lens;
typedef std::deque< Lens > Box;

unsigned short myHash(const std::string& str)
{
	unsigned char ret;

	ret = 0;
	for (char c : str)
	{
		ret += c;
		ret *= 17;
		ret %= 256;
	}
	return ret;
}

static void part1(std::ifstream& input)
{
	ull res;
	char c;
	std::string line;

	res = 0;
	c = input.get();
	while (c != EOF)
	{
		if (c == ',')
		{
			res += myHash(line);
			line.clear();
		}
		else
		{
			line += c;
		}
		c = input.get();
	}
	res += myHash(line);

	std::cout << "result is " << res << std::endl;
}

void parseElt(Box* boxes, const std::string& str)
{
	char c;
	unsigned short hash;
	std::string lensLabel;

	for (std::string::size_type idx = 0; idx < str.size(); ++idx)
	{
		c = str[idx];
		if (c == '=' || c == '-') {
			lensLabel = str.substr(0, idx);
			hash = myHash(lensLabel);
			for (Box::iterator it = boxes[hash].begin(); it != boxes[hash].end(); ++it) {
				if (it->first == lensLabel)
				{
					if (c == '-')
						boxes[hash].erase(it);
					else
					{
						it->second = atoi(&str.c_str()[idx + 1]);
					}
					return;
				}
			}
			if (c == '=')
				boxes[hash].push_back(std::make_pair(lensLabel, atoi(&str.c_str()[idx + 1])));
			return;
		}
	}
	throw (std::invalid_argument("bad input."));
}

static void part2(std::ifstream& input)
{
	ull res;
	char c;
	std::string line;
	ull idxBox;
	ull idxInBox;
	Box boxes[256];

	res = 0;
	c = input.get();
	while (c != EOF)
	{
		if (c == ',')
		{
			parseElt(boxes, line);
			line.clear();
		}
		else
		{
			line += c;
		}
		c = input.get();
	}
	parseElt(boxes, line);

	res = 0;
	idxBox = 0;
	for (const Box& b : boxes)
	{
		++idxBox;
		if (!b.empty())
		{
			idxInBox = 0;
			for (const Lens& l : b)
			{
				++idxInBox;
				res += idxInBox * idxBox * l.second;
			}
		}
	}
	std::cout << "result is " << res << std::endl;
}

int day15()
{
    std::ifstream input;
    ui part;
    ull res;
	char c;
    std::string line;

    if (getFileAndPart(15, &input, &part))
        return errno;

	if (part == 1)
		part1(input);
	else
		part2(input);

    return 0;
}