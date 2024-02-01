#include "pch.h"


using ::testing::AddGlobalTestEnvironment;
using ::testing::Bool;
using ::testing::Combine;
using ::testing::Message;
using ::testing::Range;
using ::testing::TestWithParam;
using ::testing::Values;
using ::testing::ValuesIn;


//#include "AddressCombiner.h"

//what to do about www or redirections ?
//	issue //
// issue /
// improve string to string_view for better performance


//deceide about usage of www
// in case is an image or an a

struct CombinedAddressesData
{
	std::string primaryAddress, secondaryAddress, expectedResult;
};


class AddressCombinerTestClass: public ::testing::Test
{

public:
	CombinedAddressesData combindeAddresses;
	const string& getExpectedResult() const { return combindeAddresses.expectedResult; }
	void setExpectedResult(const string& str) { combindeAddresses.expectedResult = str; }
	void setCombinedAddresses(const CombinedAddressesData& data) { combindeAddresses = data; }
	string getCombinedAddresses() const { return combiner(combindeAddresses.primaryAddress, combindeAddresses.secondaryAddress); }
	void SetUp() {}
	void TearDown() { /*delete combindeAddresses;*/ }
	//ResplacingBySecondaryAddressTest() { account = new CombinedAddressesData{/*...*/}; }
	//~ResplacingBySecondaryAddressTest() { delete combindeAddresses; } // another way

	const std::string& mockPrimaryAddress = "http://www.site1.com:1234/folder1/";
	const std::string& mockSecondaryAddress = "http://www.site2.com/folder2/";

};



TEST_F(AddressCombinerTestClass, EnteredSecondaryFullPathAddress_ExpectedReturnTheSecondaryAddress)
{	
	setCombinedAddresses({ "http://www.site1.com:1234/folder1/", "http://www.site2.com/folder2/" });
	setExpectedResult(combindeAddresses.secondaryAddress );
	EXPECT_EQ(getCombinedAddresses(), getExpectedResult());
}


TEST_F(AddressCombinerTestClass, EnteredPrimaryWithSlashSecondaryWithSlash_ExpectedCombineRighBothAddress) 
{
	setCombinedAddresses({ "http://www.site1.com:1234/folder1/", "/folder2/" });
	setExpectedResult("http://www.site1.com:1234/folder2");
	EXPECT_EQ(getCombinedAddresses(), getExpectedResult());
}


TEST_F(AddressCombinerTestClass, EnteredPrimaryWithSlashSecondaryNoSlash_ExpectedCombineRighBothAddress)
{
	setCombinedAddresses({ "http://www.site1.com:1234/folder1/", "folder2/" });
	setExpectedResult("http://www.site1.com:1234/folder1/folder2");
	EXPECT_EQ(getCombinedAddresses(), getExpectedResult());
}


TEST_F(AddressCombinerTestClass, EnteredPrimaryNoSlashSecondaryNoSlash_ExpectedCombineRighBothAddress)
{
	setCombinedAddresses({ "http://www.site1.com:1234/folder1", "folder2/"  });
	setExpectedResult("http://www.site1.com:1234/folder1/folder2");
	EXPECT_EQ(getCombinedAddresses(), getExpectedResult());
}

TEST_F(AddressCombinerTestClass, EnteredPrimaryNoSlashSecondaryWithDotSlash_ExpectedCombineRighBothAddress)
{
	setCombinedAddresses({ "http://www.site1.com:1234/folder1", "./folder2/" });
	setExpectedResult("http://www.site1.com:1234/folder1/folder2");
	EXPECT_EQ(getCombinedAddresses(), getExpectedResult());
}

TEST_F(AddressCombinerTestClass, EnteredPrimaryWithSlashSecondaryWithDotSlash_ExpectedCombineRighBothAddress)
{
	setCombinedAddresses({ "http://www.site1.com:1234/folder1/", "./folder2/" });
	setExpectedResult("http://www.site1.com:1234/folder1/folder2");
	EXPECT_EQ(getCombinedAddresses(), getExpectedResult());
}

TEST_F(AddressCombinerTestClass, InputTrimTestForBothAddresses_ExpectedCombineRighBothAddress)
{
	setCombinedAddresses({
		"        http://www.site1.com:1234/folder1//////////////               "
		, "                 folder2////////////         " 
		});
	setExpectedResult("http://www.site1.com:1234/folder1/folder2");
	EXPECT_EQ(getCombinedAddresses(), getExpectedResult());
}

TEST_F(AddressCombinerTestClass, InputPrimaryWithSlashSecondaryWithDoubleDotSlash_ExpectedCombineRighBothAddressWithFolderBack)
{
	setCombinedAddresses({"http://www.site1.com:1234/folder1/", "../folder2/" });
	setExpectedResult("http://www.site1.com:1234/folder2");
	EXPECT_EQ(getCombinedAddresses(), getExpectedResult());
}


TEST_F(AddressCombinerTestClass, InputPrimaryWithSlashSecondaryWithDoubleDotSlash_ExpectedCombineRighBothAddressNoFolderBack)
{
	setCombinedAddresses({ "http://www.site1.com:1234/", "../folder2/"  });
	setExpectedResult("http://www.site1.com:1234/folder2");
	EXPECT_EQ(getCombinedAddresses(), getExpectedResult());
}


TEST_F(AddressCombinerTestClass, EnteredPrimaryNoSlashSecondaryWithSlash_ExpectedCombineRighBothAddress)
{
	setCombinedAddresses({ "http://www.site1.com:1234/folder1", "/folder2/" });
	setExpectedResult("http://www.site1.com:1234/folder2");
	EXPECT_EQ(getCombinedAddresses(), getExpectedResult());
}

TEST_F(AddressCombinerTestClass, EnteredPrimaryNoSlashSecondaryWithDoubleSlash_ExpectedCombineRighBothAddress)
{
	setCombinedAddresses({ "http://www.site1.com:1234/folder1", "//site2.com/folder2/" });
	setExpectedResult("http://site2.com/folder2");
	EXPECT_EQ(getCombinedAddresses(), getExpectedResult());
}




		//lots of test cases
class CombineAddressTest : public ::testing::TestWithParam<CombinedAddressesData>
{};//
//{//
//public:
//	string assume;
//	string result;
//	CombineAddressTest() {
//		auto combinedAddresses = GetParam();	//CombinedAddressesData
//		assume = combiner(combinedAddresses.primaryAddress, combinedAddresses.secondaryAddress);
//		result = combinedAddresses.expectedResult;
//	}
//	~CombineAddressTest() {}
//};//

TEST_P(CombineAddressTest, ExternalGenerator)
{
	auto combinedAddresses = GetParam();
	const std::string& assume = combiner(combinedAddresses.primaryAddress, combinedAddresses.secondaryAddress);
	const std::string& result = combinedAddresses.expectedResult;
	EXPECT_EQ(assume, result);
}

INSTANTIATE_TEST_CASE_P(AddressCombinerTestAllUnExpectedValueTests, CombineAddressTest, ::testing::Values(
	CombinedAddressesData{
		"http://www.site1.com:1234/folder1//////////////", "folder2////////////"
		, "http://www.site1.com:1234/folder1/folder2"
	}
	, CombinedAddressesData{ "http://www.site1.com:1234/folder1/", "/", "http://www.site1.com:1234" }	//to fix
	, CombinedAddressesData{ "http://www.site1.com:1234/", "./", "http://www.site1.com:1234" }
	, CombinedAddressesData{ "http://www.site1.com:1234/folder1/", "./", "http://www.site1.com:1234/folder1" }
	, CombinedAddressesData{ "http://www.site1.com:1234/", "../", "http://www.site1.com:1234" }
	, CombinedAddressesData{ "http://www.site1.com:1234/", "//", "http://www.site1.com:1234" }
	, CombinedAddressesData{ "http://www.site1.com:1234/folder1/", "../", "http://www.site1.com:1234" }	
));//; //if used without gmock, add: ,)



//disabling tes https://stackoverflow.com/questions/7208070/googletest-how-to-skip-a-test
TEST(AddressCombinerTest, DISABLED_PrivateCaseForP)
{	//DISABLED_
	std::string primaryAddress = "http://www.site1.com:1234/folder1";
	std::string secondaryAddress = "../";
	const std::string& result = "http://www.site1.com:1234";
	const std::string& assume = combiner(primaryAddress, secondaryAddress);
	EXPECT_EQ(assume, result);
}