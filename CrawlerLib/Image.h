#pragma once

//#include <nlohmann/json.hpp>	//#include //<string>

class Image
{
public:
	std::string imageUrl;
	std::string sourceUrl;
	size_t depth;
	explicit Image(std::string imageUrl, std::string sourceUrl, size_t depth) : imageUrl{ imageUrl }, sourceUrl{ sourceUrl }, depth{ depth } {}
	nlohmann::json to_json() const;
	std::string print() const;
	//friend  stream...
};

