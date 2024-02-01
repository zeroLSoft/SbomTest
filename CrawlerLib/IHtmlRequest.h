#pragma once
class IHtmlRequest
{
public:
	virtual ~IHtmlRequest() = default;	//importatant for dependency injection
	virtual std::string getHtml(std::string url) = 0;
};