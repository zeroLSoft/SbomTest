#include  "pch.h"

#include "Image.h"

nlohmann::json Image::to_json() const
{
	nlohmann::json j;
	j["imageUrl"] = imageUrl;
	j["sourceUrl"] = sourceUrl;
	j["depth"] = depth;
	return j;
}

std::string Image::print() const
{
	return "imageUrl: " + (imageUrl.length() > 35 ? imageUrl.substr(0, 25) + "..." + imageUrl.substr(imageUrl.length() - 10) : imageUrl)
		+ " sourceUrl: " + (sourceUrl.length() > 35 ? sourceUrl.substr(0, 25) + "..." + sourceUrl.substr(sourceUrl.length() - 10) : sourceUrl)
		+ " depth: " + std::to_string(depth) + "\n";
}
