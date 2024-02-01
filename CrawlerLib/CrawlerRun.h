#pragma once

#include "Image.h"  //#include
#include "ICrawlerRun.h"
#include "IHtmlRequest.h"
//#include <set>
//#include <gumbo-query/gumbo.h>  // included

class CrawlerRun : public ICrawlerRun
{
private:
    bool isError{ false };
protected:  //resurce for testing
    IHtmlRequest* html_request = nullptr;   //will set in constructore
    void setHtmlRequest(IHtmlRequest* html_request) { this->html_request = html_request; }
private:
    string begin_address{ "" };
    size_t crawler_levels{ 0 };
    size_t time_limit_sec{ 100 };   //to set
    long long lastDurationSec{ 0 };

private:
    std::mutex visitedUri_m;
    std::set<std::string> visitedUri;

private:
    std::mutex levels_m;
    std::unordered_map<size_t, std::list<std::string>> levels;

private:
    std::mutex visitedImageUri_m;
    std::set<std::string> visitedImageUri;
    std::list<Image> images;             //to delete
    nlohmann::json json_images;

public:
    explicit CrawlerRun(const std::string& begin_address, size_t crawler_levels);
    ~CrawlerRun() {  delete html_request; }

public:
    void setTimeLimit(size_t timeLimit);  //to set
    void print() const;
    void write_to_file(const std::string& file_address_name) const;
    std::string to_string() const;

protected: //set as protected!, rty to delete virtual
    CrawlerRun() = default;
    void init(const std::string& begin_address, size_t crawler_levels);
    void crawler(const std::string& uri, size_t level); //run on thread
    void search_inside_element(GumboNode* node, const std::string& uri, const size_t& level);

private:
    std::vector<std::thread> threadGlobalList;
    void timeout_init(const std::string& begin_address, size_t crawler_levels);
    std::string html_get(const string& address) const noexcept;
};


