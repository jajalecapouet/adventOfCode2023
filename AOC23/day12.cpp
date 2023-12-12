#include "helpers.hpp"
#include <chrono>

typedef std::pair<std::string::size_type, std::deque<int>::size_type> Comp;
typedef std::map<Comp, unsigned long long> Cache;

void trimDots(std::string& line)
{
	std::string::size_type idx = 0;

	while (idx < line.size() && line[idx] == '.')
		++idx;
	if (idx)
	{
		line = line.substr(idx);
	}
}

int sumNbrs(const std::deque<int>& d)
{
	int size = 0;
	for (int i : d)
		size += i + 1;
	if (size)
		return size - 1;
	return size;
}

unsigned long long dfs(std::string line, std::deque<int> numbers, Cache& cache)
{
	unsigned long long res = 0;
	int nb;
	int next;
	int uknCount;
	int range;
	int marge;
	std::string::size_type idx;
	std::optional<std::string::size_type> toNotDepasser;
	std::optional<std::string::size_type> toNotEat;
	Cache::const_iterator findInCache;

	if (numbers.empty())
	{
		for (std::string::size_type idx = 0; idx < line.size(); ++idx)
		{
			if (line[idx] == '#')
				return 0;
		}
		return 1;
	}

	trimDots(line);

	if (line.empty())
		return 0;

	marge = (int)line.size() - sumNbrs(numbers);
	if (marge < 0)
		return 0;

	findInCache = cache.find(Comp(line.size(), numbers.size()));
	if (findInCache != cache.end())
		return findInCache->second;

	nb = numbers.front();
	idx = 0;
	uknCount = 0;
	while (idx < line.size() && line[idx] != '.')
	{
		if (line[idx] == '#')
		{
			if (!toNotDepasser.has_value())
				toNotDepasser = idx;
			toNotEat = idx;
		}
		++uknCount;
		++idx;
	}
	if (toNotDepasser.has_value())
	{
		if (uknCount < nb)
			return 0;
	}
	else
		res += dfs(line.substr(idx), numbers, cache);
	if (nb > uknCount)
	{
		cache[Comp(line.size(), numbers.size())] = res;
		return res;
	}

	numbers.pop_front();
	range = std::min<int>(uknCount - nb, toNotDepasser.value_or(uknCount - nb));
	range = std::min<int>(range, marge);
	if (numbers.size())
	{
		next = numbers.front();
		if (!toNotDepasser.has_value() && next + nb + 1 > uknCount)
		{
			if (uknCount < line.size())
				res += ((range + 1) * dfs(line.substr(uknCount), numbers, cache));
			else
				res += ((range + 1) * dfs("", numbers, cache));
			cache[Comp(line.size(), numbers.size() + 1)] = res;
			return res;
		}
	}
	for (int i = 0; i <= range; ++i)
	{
		if (i + nb == line.size() || line[i + nb] != '#')
		{
			if (i + nb + 1 >= line.size())
				res += dfs("", numbers, cache);
			else
			{
				if (!numbers.empty())
				{
					next = numbers.front();
					if (next + nb + 1 > uknCount - i && toNotEat.value_or(0) < i + nb)
					{
						res += ((range - i + 1) * dfs(line.substr(uknCount), numbers, cache));
						cache[Comp(line.size(), numbers.size() + 1)] = res;
						return res;
					}
				}
				res += dfs(line.substr(i + nb + 1), numbers, cache);
			}
		}
	}
	cache[Comp(line.size(), numbers.size() + 1)] = res;
	return res;
}

unsigned long long solve(std::string& line, unsigned int part)
{
	unsigned long long res = 0;
	std::deque<int> numbers;
	std::deque<int> numbersPart2;
	std::string linePart2;
	std::string::size_type idx = 0;
	std::string::iterator it = line.begin();
	Cache cache;
	bool endOfSequence = false;
	while (idx < line.size())
	{
		if (!endOfSequence && line[idx] == ' ')
			endOfSequence = true;
		if (isdigit(line[idx]))
		{
			numbers.push_back(atoi(&line[idx]));
			while (isdigit(line[idx]))
				++idx;
		}
		else
			++idx;
		if (!endOfSequence)
			++it;
	}
	line.erase(it, line.end());
	if (part == 1)
		res += dfs(line, numbers, cache);
	else
	{
		for (int i = 0; i < 5; ++i)
		{
			numbersPart2.insert(numbersPart2.end(), numbers.begin(), numbers.end());
			linePart2 += line;
			if (i != 4)
				linePart2 += '?';
		}
		res += dfs(linePart2, numbersPart2, cache);
	}
	return res;
}

int day12()
{
	std::ifstream input;
	std::fstream out;
	unsigned int part;
	if (getFileAndPart(12, &input, &part))
		return errno;

	unsigned long long res = 0;
	std::string line;

	while (!input.eof())
	{
		getline(input, line);
		res += solve(line, part);
	}
	std::cout << "result is " << res << std::endl;
	return 0;
}