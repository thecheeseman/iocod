#include "csv.h"

CSVParser::CSVParser(const std::string& input)
{
    source_string = input;

    if (!source_string.empty())
        parseRows();
}

void CSVParser::parseRows()
{
    buffer_start = &source_string[0];
    read_at = buffer_start;

    ParseResult result;
    do {
        result = parseRow();
    } while (result != ParseResult::EndOfString);
}

CSVParser::ParseResult CSVParser::parseRow()
{
    const int newline_size = measureNewLine(read_at);
    if (newline_size) {
        read_at += newline_size;
        return *read_at ? ParseResult::EndOfRow : ParseResult::EndOfString;
    }

    ParseResult result;
}
