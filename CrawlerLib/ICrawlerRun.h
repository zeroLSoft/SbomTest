#pragma once

class ICrawlerRun	//set to interface
{
public:
	virtual ~ICrawlerRun() = default;	//importatant for dependency injection
	virtual void setTimeLimit(size_t timeLimit) = 0;
	virtual void print() const = 0;
	virtual void write_to_file(const string& file_address_name) const = 0;
	virtual string to_string() const = 0;
protected:	//internal use and testing methods
	virtual void setHtmlRequest(IHtmlRequest* html_request) = 0;
	virtual void init(const std::string& begin_address, size_t crawler_levels) = 0;
	virtual void crawler(const string& uri, size_t level) = 0;
	virtual void search_inside_element(GumboNode* node, const string& uri, const size_t& level) = 0;
//private:
	//virtual string html_get(const string& uri) const = 0;
};
