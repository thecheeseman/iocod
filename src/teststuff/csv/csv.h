#ifndef CSV_H
#define CSV_H

#include <vector>
#include <string>

struct CSVParser {
    typedef std::vector<std::vector<const char*>> row_type;

    CSVParser(const std::string& input);

    const row_type& getRows() const
    {
        return rows;
    }

private:
    CSVParser(const CSVParser&);
    CSVParser& operator=(const CSVParser&);

    enum struct ParseResult { 
        EndOfCell, 
        EndOfRow, 
        EndOfString 
    };

    void parseRows();
    ParseResult parseRow();
    ParseResult parseCell();

    int measureNewLine(const char *at);

    std::string source_string;
    char* buffer_start;
    const char* read_at;
    row_type rows;
};

#endif /* CSV_H */
