#pragma once

//https://github.com/google/googletest/blob/main/docs/gmock_cook_book.md#knowing-when-to-expect


	//::testing::NiceMock<MockHtmlRequest> mockhtml2; //try test with NiceMock

#include <CrawlerRun.h>//#include <IHtmlRequest.h>
#include "Utils.h"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::AtMost;
using ::testing::Return;





class MockHtmlRequest : public IHtmlRequest
{
public:
	MOCK_METHOD(std::string, getHtml, (const std::string));//, (const)
};


class MockCrawlerRun : public ICrawlerRun
{
public:	// basicly only ICrawlerRun public methods is important to override (set)
	MOCK_METHOD(void, crawler, (const std::string& uri, size_t level));
	MOCK_METHOD(void, search_inside_element, (GumboNode* node, const std::string& uri, const size_t& level));
	MOCK_METHOD(string, to_string, (), (const));
	MOCK_METHOD(void, write_to_file, (const string& file_address_name), (const));
	//have to override, but not in use:
	MOCK_METHOD(void, print, (), (const));
	MOCK_METHOD(void, setTimeLimit, (size_t timeLimit));
	MOCK_METHOD(void, init, (const string&, size_t));	//protected
	MOCK_METHOD(void, setHtmlRequest, (IHtmlRequest*));	//protected

};



class MyCrawlerRun : public MockCrawlerRun, protected CrawlerRun	//fake object too
{
public:	//all protected method set to public:
	void setHtmlRequest(IHtmlRequest* html_request)
	{
		CrawlerRun::setHtmlRequest(html_request);
	}
public:
	void init(const std::string& begin_address, size_t crawler_levels)
	{
		CrawlerRun::init(begin_address, crawler_levels);
	}
	void crawler(const std::string& uri, size_t level)
	{
		MockCrawlerRun::crawler(uri, level);	// only count
		CrawlerRun::crawler(uri, level);
	}
	void search_inside_element(GumboNode* node, const std::string& uri, const size_t& level)
	{
		MockCrawlerRun::search_inside_element(node, uri, level);	// only count
		CrawlerRun::search_inside_element(node, uri, level);
	}
public:
	void write_to_file(const string& file_address_name)
	{
		MockCrawlerRun::write_to_file(file_address_name);	// only count
		CrawlerRun::write_to_file(file_address_name);
	}
	string to_string()
	{
		string str = MockCrawlerRun::to_string();	// only count
		return !str.empty() ? str : CrawlerRun::to_string();
	}
};




class CrawlerRunMockTest: public ::testing::Test
{


public:
	MyCrawlerRun* myCrawlerRun;	//set CrawlerRun to public and set the mock
	IHtmlRequest* mockHtmlReques;

public:
	MockHtmlRequest& getMockHtmlRequest()
	{	//auto& sptr_mockHtmlRequest = std::dynamic_pointer_cast<MockHtmlRequest>(mockhtml);
		//auto* p_mockHtmlRequest = sptr_mockHtmlRequest.get();
		return dynamic_cast<MockHtmlRequest&>(*mockHtmlReques);
	}
	MockCrawlerRun& getMcokCrawlerRun()
	{
		return dynamic_cast<MockCrawlerRun&>(*myCrawlerRun);
	} //


public:
	void SetUp()
	{
		myCrawlerRun = new MyCrawlerRun();
		mockHtmlReques = new MockHtmlRequest();
		myCrawlerRun->setHtmlRequest(mockHtmlReques);

			//basic expectations from mock crawler run
		EXPECT_CALL(getMcokCrawlerRun(), crawler(_, _)).Times(AtLeast(1));
		EXPECT_CALL(getMcokCrawlerRun(), search_inside_element(_, _, _))
			.Times(AtLeast(at_laset_mock_string_levels));
		EXPECT_CALL(getMcokCrawlerRun(), write_to_file(_)).Times(AtMost(1));
		EXPECT_CALL(getMcokCrawlerRun(), to_string()).Times(AtMost(1));
	}

	void TearDown()
	{
		delete myCrawlerRun;
	} // delete mockhtml done inside mock_cr



public:	//class methods
	void initMock(size_t levels);

	string printMock();

	void writeMockToFile(const string& file_address_name);



public:	//mock basic data
	const size_t at_laset_mock_string_levels = 3;

	const string mock_string{ R"V0G0N(
			<!doctype><html><head></head><body>
				<h1> Title <h1>
				<img src="image1.jpg">
			</body></html>
		)V0G0N" };

	const string mock_address{ "http://someaddress.com/folder1/" };

	const string mock_result{ R"({"results":[{"depth":1,"imageUrl":"http://someaddress.com/folder1/image1.jpg","sourceUrl":"http://someaddress.com/folder1/"}]})" };

	const string mockFileName{ "test_case.json" };

};
