#include <iostream>
#include <cstring>
#include "minisql/storage/disk_manager.hpp"
#include "minisql/storage/page.hpp"

int main()
{
    minisql::DiskManager disk_manager("test.db");

    // Allocate a fresh page
    minisql::page_id_t page_id = disk_manager.AllocatePage();
    std::cout << "Allocated Page id : " << page_id << "\n";

    // write some known data into it
    char write_buf[minisql::PAGE_SIZE];        // array of char, block of page_size = 4096 byte
    std::memset(write_buf, 0, minisql::PAGE_SIZE);            // memory set - 3 arguments - which memory to touch, what value tp fill it with (0), how many bytes to fill (page size)
    std::strcpy(write_buf, "Hello, MiniSQL storage layer !"); // copy the test in to 1st argument

    disk_manager.WritePage(page_id, write_buf); // which page to write, what data to write.
    std::cout << "Wrote data to Page " << page_id << "\n";

    // read it back into a seperate buffer
    char read_buf[minisql::PAGE_SIZE];
    disk_manager.ReadPage(page_id, read_buf);

    std::cout << "Read back : " << read_buf << "\n";

    // Confirm they match
    if (std::strcmp(write_buf, read_buf) == 0)
    {
        std::cout << "SUCCESS : data written and read back correctly.\n";
    }
    else
    {
        std::cout << "FAILURE : data mismatch ! \n";
    }
    return 0;
}
