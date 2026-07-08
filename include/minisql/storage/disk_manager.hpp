#pragma once
#include "minisql/storage/page.hpp"
#include <fstream>
#include <string>

namespace minisql
{
  // Disk Manager is responsible for : reading and writing fixed size pages to/from a .db file on disk.
  // It knows nothing about rows, tables or SQL.

  class DiskManager
  {
  public:
    explicit DiskManager(const std ::string &db_file);
    ~DiskManager();

    void ReadPage(page_id_t page_id, char *page_data);
    void WritePage(page_id_t page_id, const char *page_data);

    page_id_t AllocatePage();

  private:
    std ::fstream db_io_;
    std ::string db_file_;
    page_id_t next_page_id_ = 0;
  };

}