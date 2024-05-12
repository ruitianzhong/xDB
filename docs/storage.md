
## Storage

### RocksDB

Open and Close a database.

```c++
   rocksdb::DB *db;
    rocksdb::Options options;
    options.create_if_missing = true;
    rocksdb::Status status = rocksdb::DB::Open(options, "/tmp/testdb", &db);
    assert(status.ok());
    rocksdb::WaitForCompactOptions opt = rocksdb::WaitForCompactOptions();
    opt.close_db = true;
    status = db->WaitForCompact(opt);
    std::cout << status.ok();
    delete db;
```