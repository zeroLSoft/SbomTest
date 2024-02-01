#include "pch.h"
#include "CrawlerRunMockTest.h"


void CrawlerRunMockTest::initMock(size_t levels)
{
	myCrawlerRun->init(mock_address, levels);
}

string CrawlerRunMockTest::printMock()
{
	return myCrawlerRun->to_string();
}

void CrawlerRunMockTest::writeMockToFile(const string& file_address_name)
{
	empti_the_file(file_address_name);
	myCrawlerRun->write_to_file(file_address_name);
}
