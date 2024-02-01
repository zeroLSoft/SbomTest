#include "pch.h"
#include "CrawlerRun.h"
#include <future>



//#include <fstream>
//#include <thread>
//#include <chrono>
//constructor
#include <exception>

std::string CrawlerRun::html_get(const string& uri) const  noexcept
{
    try {
        //add some condition!
        if (isError) throw std::exception("fail html");
        string str = html_request->getHtml(uri);
        return str;
    }
    catch (...) { return ""; }
}

CrawlerRun::CrawlerRun(const string& begin_address, size_t crawler_levels)
{
    html_request = new HtmlRequest();
    isError = false;
    //init(begin_address, crawler_levels);
    timeout_init(begin_address, crawler_levels);
}

void CrawlerRun::setTimeLimit(size_t timeLimit)
{
    this->time_limit_sec = timeLimit;
}

void CrawlerRun::print() const
{
    //json j_list;
    //j_list["results"] = {};
    std::cout << "all images data:\n" << "duration (" << std::to_string(lastDurationSec) << ") sec\n";
    for (const auto& el : images)
    {
        // j_list["results"].emplace_back(el.to_json());
        std::cout << el.print();
    }
    std::cout << "\ndone, all images data collected\n" << "duration (" << std::to_string(lastDurationSec) << ") sec\n";
}

void CrawlerRun::write_to_file(const string& file_address_name) const
{
    std::ofstream MyFile(file_address_name);
    MyFile << json_images;
    MyFile.close();
}

string CrawlerRun::to_string() const
{
    auto images_j = json_images;
    auto images_s = images_j.dump();
    return images_s;
} 





void CrawlerRun::timeout_init(const std::string& begin_address, size_t crawler_levels)
{
    try {
        std::packaged_task<void()> task([&]() { return init(begin_address, crawler_levels); });
        auto future = task.get_future();
        std::thread thr(std::move(task));
        if (future.wait_for(std::chrono::seconds(time_limit_sec)) != std::future_status::timeout)
        {
            thr.join();
            future.get(); // this will propagate exception from f() if any
        }
        else
        {
            
            isError = true;
            thr.detach(); // we leave the thread still running
            std::cout << "Run time error, runs over (" << time_limit_sec << ") sec \n";
            throw std::runtime_error("Timeout");
        }
    }
    catch (...)
    {
        for (auto& t : threadGlobalList)
            if (t.joinable())t.detach();
        return;
    }
}

void CrawlerRun::init(const std::string& begin_address, size_t crawler_levels)
{
    
    this->begin_address = begin_address;
    this->crawler_levels = crawler_levels;
    lastDurationSec = 0;
    auto startTime{ std::chrono::high_resolution_clock::now() };

    levels[1].push_back(begin_address);
    for (size_t i{ 1 }; i <= crawler_levels; ++i)
    {
        /*std::vector<std::thread> threadGlobalList;*/
        if (levels.find(i) != levels.end() && !levels.at(i).empty()) threadGlobalList.reserve(levels[i].size());
        else break; //no elements on i level
        for (auto& address : levels[i]) //crawler(address, i);
                threadGlobalList.emplace_back(&CrawlerRun::crawler, this, address, i);   /////
        for (auto& t : threadGlobalList)
                if(t.joinable())t.join();
        
    }
    auto stopTime{ std::chrono::high_resolution_clock::now() };
    auto duration{ std::chrono::duration_cast<std::chrono::seconds> (stopTime - startTime) };
    auto lastDurationSec = duration.count();
}



class PageDataSet
{
private:
    GumboOutput* output;
public:
    PageDataSet(const string& contents) : output{ gumbo_parse(contents.c_str()) } {}
    ~PageDataSet() { gumbo_destroy_output(&kGumboDefaultOptions, output); }
    GumboNode* getNode() { return output->root; }
};

void CrawlerRun::crawler(const string& uri, size_t level)
{
    if (
        std::lock_guard<std::mutex> lock(visitedUri_m);
        visitedUri.find(uri) == visitedUri.end()
            )
    {  visitedUri.insert(uri); }
    else return;

    string contents = html_get(uri);

    PageDataSet nodeSet(contents);
    auto node = nodeSet.getNode();
    if (node->type != GUMBO_NODE_ELEMENT) return;
    if(!isError)
        search_inside_element(node, uri, level);
           //fix as destructure
}


void CrawlerRun::search_inside_element(GumboNode* node, const string& uri, const size_t& level)
{
    if (node->type != GUMBO_NODE_ELEMENT) return;
    if(
            GumboAttribute* href;
            node->v.element.tag == GUMBO_TAG_A
            && (href = gumbo_get_attribute(&node->v.element.attributes, "href"))
            )
    {
        string hrefValue{ combiner(uri, static_cast<string>(href->value)) };
        std::lock_guard<std::mutex> lock(levels_m);
        levels[level + 1].push_back(hrefValue);
    }   //end mutex guard
    else if (
            GumboAttribute* src;
            node->v.element.tag == GUMBO_TAG_IMG
            && (src = gumbo_get_attribute(&node->v.element.attributes, "src"))
            )
    {
        std::lock_guard<std::mutex> lock(visitedImageUri_m);
        if(
                string srcValue{ combiner(uri, static_cast<string>(src->value)) };
                !(visitedImageUri.find(srcValue) != visitedImageUri.end())
                )
        {
            visitedImageUri.insert(srcValue);
            auto image = Image(srcValue, uri, level);
            json_images["results"].emplace_back(image.to_json());
            images.push_back(std::move(image));  //Image(srcValue, uri, level)
        }   //end mutex guard
    }
    GumboVector* children = &node->v.element.children;  //moving through all the nodes in HTML document
    for (size_t i{ 0 }; i < children->length; ++i)
    {
        auto childNode{ static_cast<GumboNode*>(children->data[i]) };
        search_inside_element(childNode, uri, level);
    }
}
