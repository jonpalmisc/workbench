#include <cstdio>
#include <stdexcept>
#include <string>
#include <unordered_map>

//===-- Database ------------------------------------------------------------===

class database_t {
  using kv_map_t = std::unordered_map<std::string, int>;

  kv_map_t m_data;
  kv_map_t m_txn;
  bool m_in_txn = false;

public:
  void begin();
  void commit();
  void rollback();

  void put(std::string const &key, int val);
  int get(std::string const &key);

  void dump();
};

void database_t::begin() {
  if (m_in_txn)
    throw std::runtime_error("Transaction already progress!");

  m_in_txn = true;
}

void database_t::commit() {
  for (auto const &[k, v] : m_txn)
    m_data[k] = v;

  m_txn.clear();
  m_in_txn = false;
}

void database_t::rollback() {
  m_txn.clear();
  m_in_txn = false;
}

void database_t::put(std::string const &key, int val) {
  if (!m_in_txn)
    throw std::runtime_error("No transaction in progress!");

  m_txn[key] = val;
}

int database_t::get(std::string const &key) { return m_data[key]; }

//===-- Test Program --------------------------------------------------------===

int main(int argc, char const **argv) {
  (void)argc;
  (void)argv;

  auto die = [](char const *msg) {
    std::printf("\nError: %s\n", msg);
    std::exit(1);
  };

  database_t db;

  std::printf("Starting transaction...\n");
  db.begin();

  try {
    std::printf("  Checking that starting another transaction fails... ");
    db.begin();

    die("Starting another transaction should have failed!");
  } catch (...) {
    std::printf("OK.\n");
  }

  std::printf("  Inserting pairs ('x', 1) and ('y', 2) into transaction...\n");
  db.put("x", 1);
  db.put("y", 2);

  std::printf("  Checking pairs 'x' and 'y' aren't realized yet... ");
  if (db.get("x") != 0 || db.get("y") != 0)
    die("Keys realized before transaction finished!");
  std::printf("OK.\n");

  std::printf("  Committing transaction...\n");
  db.commit();

  std::printf("  Checking pairs ('x', 1) and ('y', 2) are now realized... ");
  if (db.get("x") != 1 || db.get("y") != 2)
    die("Transaction did not take effect!");
  std::printf("OK.\n\n");

  //----------------------------------------------------------------------------

  try {
    std::printf("Checking that inserting a key without transaction fails... ");
    db.put("z", 3);

    die("Inserting a key with no transaction should have failed!");
  } catch (...) {
    std::printf("OK.\n\n");
  }

  //----------------------------------------------------------------------------

  std::printf("Starting transaction...\n");
  db.begin();
  std::printf("  Inserting pair ('z', 80)...\n");
  db.put("z", 80);
  std::printf("  Rolling back transaction...\n");
  db.rollback();

  std::printf("  Checking pair ('z', 80) was not realized... ");
  if (db.get("z") != 0)
    die("Transaction incorrectly took effect!");
  std::printf("OK.\n\n");

  //----------------------------------------------------------------------------

  std::printf("Starting transaction...\n");
  db.begin();
  std::printf("  Inserting pair ('z', 3)...\n");
  db.put("z", 3);
  std::printf("  Committing transaction...\n");
  db.commit();

  std::printf("  Checking pair ('z', 3) is now realized... ");
  if (db.get("z") != 3)
    die("Transaction did not take effect!");
  std::printf("OK.\n\n");

  //----------------------------------------------------------------------------

  std::printf("All tests passed!\n");

  return 0;
}
