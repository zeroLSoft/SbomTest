#pragma once

using std::string_view;

void dotDotSlashCase(string_view& uri1, string_view& uri2);

void slashCase(string_view& uri1, string_view& uri2);

std::string combiner(std::string uri1, std::string uri2);
