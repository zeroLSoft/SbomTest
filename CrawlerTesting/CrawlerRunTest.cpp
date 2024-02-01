#include "pch.h"
//#include "gmock/gmock.h" //https://stackoverflow.com/questions/60486110/how-to-use-googlemock-in-visual-studio
#include "CrawlerRunMockTest.h"	
//#include "MockHtmlRequest" #include "MyCrawlerRun.h" #include <ICrawlerRun.h>  #include <CrawlerRun.h> 
//#include "Utils.h"

#pragma warning(disable:4996)

using ::testing::AtLeast;
using ::testing::Return;
using ::testing::_;
using ::testing::Invoke;
using ::testing::InvokeWithoutArgs;
using ::testing::DoDefault;
using ::testing::DoAll;
using ::testing::NiceMock;
extern class CrawlerRunMockTest;


#include "CrawlerRunMockTest.h"


extern class CrawlerRunMockTest;

extern class MyCrawlerRun;


#if 0
TEST(MockCrawlerExam, NiceMockTest)	//on development
{

	NiceMock<MyCrawlerRun> cr;

	extern class IHtmlRequest;

	class MockHtmlRequest : public IHtmlRequest
	{
	public:
		MOCK_METHOD(std::string, getHtml, (const std::string));//, (const)
		//MOCK_METHOD1(getHtml, std::string(const std::string uri));
	};

	auto mockhtml = new MockHtmlRequest();


	EXPECT_CALL(cr, to_string()).Times(AtLeast(1));
	//	.WillOnce(Return("<img src=\"ssss.jpg\" >"));

	EXPECT_CALL(*mockhtml, getHtml(_)).Times(AtLeast(1))
		.WillOnce(Return("<img src=\"ssss.jpg\" >"));

	cr.setHtmlRequest(mockhtml);
	cr.init("http://www.site.com", 1);	//levels 1

	auto assume{ cr.to_string() };

	auto result{ R"({"results":[{"depth":1,"imageUrl":"http://www.site.com/ssss.jpg","sourceUrl":"http://www.site.com"}]})" };
	EXPECT_EQ(assume, result);
}
#endif


#if 1

TEST_F(CrawlerRunMockTest, EnteredRegularHtmlPageWithOneImageOnlyLevel1_ExpectToReturnRightJsonString)
{
	//added basic test cases inside SetUp  !
	EXPECT_CALL(getMockHtmlRequest(), getHtml(_)).Times(1).WillOnce(Return(mock_string));
	
	initMock(1);	//levels 1


	auto assume{ printMock() };
	auto result{ mock_result };
	EXPECT_EQ(assume, result);
}



TEST_F(CrawlerRunMockTest, EnteredRegularHtmlPageWithFourImageLevel1Level2_ExpectToReturnRightJsonString)
{
	//added basic test cases inside SetUp

	const auto mock_string_level1 =
		R"V0G0N(
		<!doctype><html><head></head><body>
			<h1> Title 1 <h1>
			<img src="image1.jpg">
			<img src="./image2.jpg">
			<img src="../image3.jpg">
			<a href="./link_level2">click1</a>
		</body></html>
	)V0G0N";

	const auto mock_string_level2 =
		R"V0G0N(
		<!doctype><html><head></head><body>
			<h1> Title 2 <h1>
			<img src="image4.jpg">
		</body></html>
	)V0G0N";

	EXPECT_CALL(getMockHtmlRequest(), getHtml(_)).Times(2)	//AtLeast(1)
		.WillOnce(Return(mock_string_level1))
		.WillOnce(Return(mock_string_level2));

	initMock(2); //levels 2

	auto assume{ printMock() };

	string result = R"({"results":[{"depth":1,"imageUrl":"http://someaddress.com/folder1/image1.jpg","sourceUrl":"http://someaddress.com/folder1/"},{"depth":1,"imageUrl":"http://someaddress.com/folder1/image2.jpg","sourceUrl":"http://someaddress.com/folder1/"},{"depth":1,"imageUrl":"http://someaddress.com/image3.jpg","sourceUrl":"http://someaddress.com/folder1/"},{"depth":2,"imageUrl":"http://someaddress.com/folder1/link_level2/image4.jpg","sourceUrl":"http://someaddress.com/folder1/link_level2"}]})";

	EXPECT_EQ(assume, result);
}



TEST_F(CrawlerRunMockTest, WriteReadFromFileEnteredRegularHtmlPageWithOneImageOnlyLevel1_ExpectToReturnRightJsonStringFromTheFile)
{
	//added basic test cases inside SetUp

	EXPECT_CALL(getMockHtmlRequest(), getHtml(_)).Times(1).WillOnce(Return(mock_string));

	initMock(1);	//levels 1
	writeMockToFile(mockFileName);

	auto assume{ read_from_file(mockFileName) };
	string result{ mock_result };


	EXPECT_EQ(assume, result);

}




TEST_F(CrawlerRunMockTest, RunningTimeDurationIsLessFrom200MilliSecEnteredRegularHtmlPageWithOneImageOnlyLevel1_ExpectToReturnRightJsonString)
{
	//added basic test cases inside SetUp

	auto startTime{ std::chrono::high_resolution_clock::now() };

	EXPECT_CALL(getMockHtmlRequest(), getHtml(_)).Times(1).WillOnce(Return(mock_string));

	initMock( 1);	//levels 1
	printMock();	//just for running
	writeMockToFile(mockFileName);
	
	auto stopTime{ std::chrono::high_resolution_clock::now()};

	auto duration{ std::chrono::duration_cast<std::chrono::milliseconds> (stopTime - startTime) };

	long long assumeMilliSec{ 200 };
	auto resultMilliSec{ duration.count() };

	EXPECT_GE(assumeMilliSec, resultMilliSec);
	

	
}



TEST_F(CrawlerRunMockTest, EnteredRegularHtmlPageNoImageOnlyLevel1_ExpectToReturnRightJsonString) //to fix
{
	//added basic test cases inside SetUp

	const auto mock_string_no_image =
		R"V0G0N(
			<!doctype><html><head></head><body>
				<h1> Title <h1>
			</body></html>
		)V0G0N";

	EXPECT_CALL(getMockHtmlRequest(), getHtml(_)).Times(1).WillOnce(Return(mock_string_no_image));	
	initMock(1);//levels 1
	auto assume = printMock();
	string result = R"(null)";	//to fix and get results":[]}
	EXPECT_EQ(assume, result);
}


#endif