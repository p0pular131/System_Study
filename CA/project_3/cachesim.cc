// See LICENSE for license details.

#include "cachesim.h"
#include "common.h"
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <vector>
#include <list>
#include <map>

cache_sim_t::cache_sim_t(size_t _sets, size_t _ways, size_t _linesz, const char* _name)
: sets(_sets), ways(_ways), linesz(_linesz), name(_name), log(false)
{
  init();
}

static void help()
{
  std::cerr << "Cache configurations must be of the form" << std::endl;
  std::cerr << "  sets:ways:blocksize" << std::endl;
  std::cerr << "where sets, ways, and blocksize are positive integers, with" << std::endl;
  std::cerr << "sets and blocksize both powers of two and blocksize at least 8." << std::endl;
  exit(1);
}

cache_sim_t* cache_sim_t::construct(const char* config, const char* name)
{
  const char* wp = strchr(config, ':');
  if (!wp++) help();
  const char* bp = strchr(wp, ':');
  if (!bp++) help();

  size_t sets = atoi(std::string(config, wp).c_str());
  size_t ways = atoi(std::string(wp, bp).c_str());
  size_t linesz = atoi(bp);

  if (ways > 4 /* empirical */ && sets == 1)
    return new fa_cache_sim_t(ways, linesz, name);
  return new cache_sim_t(sets, ways, linesz, name);
}

void cache_sim_t::init()
{
  if(sets == 0 || (sets & (sets-1)))
    help();
  if(linesz < 8 || (linesz & (linesz-1)))
    help();

  idx_shift = 0;
  for (size_t x = linesz; x>1; x >>= 1)
    idx_shift++;

  tags = new uint64_t[sets*ways]();
  read_accesses = 0;
  read_misses = 0;
  bytes_read = 0;
  write_accesses = 0;
  write_misses = 0;
  bytes_written = 0;
  writebacks = 0;
  // set의 max size인 256으로 list, map 크기 설정
  lru_list.resize(sets);
  tags_map.resize(sets);
  miss_handler = NULL;
}

cache_sim_t::cache_sim_t(const cache_sim_t& rhs)
 : sets(rhs.sets), ways(rhs.ways), linesz(rhs.linesz),
   idx_shift(rhs.idx_shift), name(rhs.name), log(false)
{
  tags = new uint64_t[sets*ways];
  memcpy(tags, rhs.tags, sets*ways*sizeof(uint64_t));
}

cache_sim_t::~cache_sim_t()
{
  print_stats();
  delete [] tags;
}

void cache_sim_t::print_stats()
{
  if(read_accesses + write_accesses == 0)
    return;

  float mr = 100.0f*(read_misses+write_misses)/(read_accesses+write_accesses);

  std::cout << std::setprecision(3) << std::fixed;
  std::cout << name << " ";
  std::cout << "Bytes Read:            " << bytes_read << std::endl;
  std::cout << name << " ";
  std::cout << "Bytes Written:         " << bytes_written << std::endl;
  std::cout << name << " ";
  std::cout << "Read Accesses:         " << read_accesses << std::endl;
  std::cout << name << " ";
  std::cout << "Write Accesses:        " << write_accesses << std::endl;
  std::cout << name << " ";
  std::cout << "Read Misses:           " << read_misses << std::endl;
  std::cout << name << " ";
  std::cout << "Write Misses:          " << write_misses << std::endl;
  std::cout << name << " ";
  std::cout << "Writebacks:            " << writebacks << std::endl;
  std::cout << name << " ";
  std::cout << "Miss Rate:             " << mr << '%' << std::endl;
}

uint64_t* cache_sim_t::check_tag(uint64_t addr) {
  size_t idx = (addr >> idx_shift) & (sets - 1);
  size_t tag = (addr >> idx_shift) | VALID;

  auto it = tags_map[idx].find(tag);
  // matching되는 tag가 존재하는 경우
  if (it != tags_map[idx].end()) {
  // 해당 idx 요소를 lru_list의 맨 앞으로 보냄. 그리고 그 요소의 iterator 반환
    lru_list[idx].splice(lru_list[idx].begin(), lru_list[idx], it->second);
    return &(*it->second);
  }
  // 존재하는거 없으면 NULL pointer 반환
  return NULL;
}

uint64_t cache_sim_t::victimize(uint64_t addr) {
  size_t idx = (addr >> idx_shift) & (sets - 1);
  uint64_t victim = 0;
  // set 꽉찼으면 가장 사용 안한 맨 뒤 요소를 victim으로 선정
  if (lru_list[idx].size() == ways) {
    victim = lru_list[idx].back();
    tags_map[idx].erase(victim >> idx_shift);
    lru_list[idx].pop_back();
  }
  // 추가할 line 맨 앞으로 가져옴
  lru_list[idx].push_front((addr >> idx_shift) | VALID);
  tags_map[idx][(addr >> idx_shift) | VALID] = lru_list[idx].begin();
  return victim;
}

void cache_sim_t::access(uint64_t addr, size_t bytes, bool store)
{
  store ? write_accesses++ : read_accesses++;
  (store ? bytes_written : bytes_read) += bytes;

  uint64_t* hit_way = check_tag(addr);
  if (likely(hit_way != NULL))
  {
    if (store)
      *hit_way |= DIRTY;
    return;
  }

  store ? write_misses++ : read_misses++;
  if (log)
  {
    std::cerr << name << " "
              << (store ? "write" : "read") << " miss 0x"
              << std::hex << addr << std::endl;
  }

  uint64_t victim = victimize(addr);

  if ((victim & (VALID | DIRTY)) == (VALID | DIRTY))
  {
    uint64_t dirty_addr = (victim & ~(VALID | DIRTY)) << idx_shift;
    if (miss_handler)
      miss_handler->access(dirty_addr, linesz, true);
    writebacks++;
  }

  if (miss_handler)
    miss_handler->access(addr & ~(linesz-1), linesz, false);

  if (store)
    *check_tag(addr) |= DIRTY;
}

fa_cache_sim_t::fa_cache_sim_t(size_t ways, size_t linesz, const char* name)
  : cache_sim_t(1, ways, linesz, name)
{
}

fa_cache_sim_t::~fa_cache_sim_t() {}

uint64_t* fa_cache_sim_t::check_tag(uint64_t addr) {
  uint64_t tag = (addr >> idx_shift) | VALID;

  auto it = tags_map.find(tag);
  if (it != tags_map.end()) {
    lru_list.splice(lru_list.begin(), lru_list, it->second);
    return &(*it->second);
  }
  return NULL;
}

uint64_t fa_cache_sim_t::victimize(uint64_t addr) {
  uint64_t tag = (addr >> idx_shift) | VALID;
  uint64_t victim = 0;

  if (tags_map.size() == ways) {
    victim = lru_list.back();
    tags_map.erase(victim >> idx_shift);
    lru_list.pop_back();
  }

  lru_list.push_front(tag);
  tags_map[tag] = lru_list.begin();
  return victim;
}
