#include "minisql/storage/disk_manager.hpp"
#include <iostream>
#include <cstring>

namespace minisql{
DiskManager :: DiskManager(const std :: string& db_file) : db_file_(db_file){
  // try opening an existing file first (in + out + binary mode)
  db_io_.open(db_file_, std::ios::in | std::ios::out | std::ios::binary);

  if(!db_io_.is_open()){
    // File doesn't exist yet so, create it, then reopen it normally
    db_io_.clear();
    db_io_.open(db_file_, std::ios::out | std::ios::binary);
    db_io_.close();
    db_io_.open(db_file_, std::ios::in | std::ios::out | std::ios::binary);

  }

  if(!db_io_.is_open()){
    std::cerr<< "DiskManager : could not open db file "<< db_file_ << "\n";
  }
}

DiskManager :: ~DiskManager(){
  if(db_io_.is_open()){
    db_io_.close();
  }
}

void DiskManager :: ReadPage(page_id_t page_id, char* page_data){
  size_t offset = static_cast<size_t>(page_id) * PAGE_SIZE;

  db_io_.seekp(offset); // moves the files read/write cursor
  db_io_.read(page_data, PAGE_SIZE); // read exactly 4096 byte starting from there

  if(db_io_.eof()){
    //reading a page that was allocated but never written on disk yet - treat as all zeros, instead of crashing
    db_io_.clear();
    std::memset(page_data, 0, PAGE_SIZE);
  }
}

void DiskManager :: WritePage(page_id_t page_id, const char* page_data){
  size_t offset = static_cast<size_t>(page_id) * PAGE_SIZE;

  db_io_.seekp(offset);
  db_io_.write(page_data, PAGE_SIZE);
  db_io_.flush();
}

page_id_t DiskManager :: AllocatePage(){
  return next_page_id_++;
}


}