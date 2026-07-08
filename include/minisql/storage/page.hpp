#pragma once

#include <cstdint>
#include <cstring>

namespace minisql{
  // every page on disk is exactly this many bytes.
  // 4096 matches the typical OS block size, so one page = 1 disk read / write.
  constexpr size_t PAGE_SIZE = 4096;

  // page id uniquely identify a page's position in database file.
  using page_id_t = int32_t;

  constexpr page_id_t INVALID_PAGE_ID = -1;

  class Page{
    public :
      Page(){
        //zero out memory, so we never accidently read garbage data.
        std :: memset(data_, 0, PAGE_SIZE);
      }

      char* GetData() { return data_; }
      const char* GetData() const { return data_; }

      page_id_t GetPageId() const { return page_id_; }
      void SetPageId(page_id_t id) { page_id_ = id; }

    private : 
      char data_[PAGE_SIZE];
      page_id_t page_id_ = INVALID_PAGE_ID;
  };

}