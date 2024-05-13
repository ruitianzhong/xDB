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

### Synchronous Writes

By default, each write to rocksdb is asynchronous: it returns after pushing the write from the process into the
operating system. The transfer from operating system memory to the underlying persistent storage happens asynchronously.

```c++
  rocksdb::WriteOptions write_options;
  write_options.sync = true;
  db->Put(write_options, ...);
```

With non-sync writes, RocksDB only buffers WAL write in OS buffer or internal buffer (when options.manual_wal_flush =
true). They are often much faster than synchronous writes.

RocksDB by default uses fdatasync() to sync files, which might be faster than fsync() in certain cases. If you want to
use fsync(), you can set Options::use_fsync to true. You should set this to true on filesystems like ext3 that can lose
files after a reboot.

### Iteration

```c++
  rocksdb::Iterator* it = db->NewIterator(rocksdb::ReadOptions());
  for (it->SeekToFirst(); it->Valid(); it->Next()) {
    cout << it->key().ToString() << ": " << it->value().ToString() << endl;
  }
  assert(it->status().ok()); // Check for any errors found during the scan
  delete it;
```

### Snapshots

Snapshots provide consistent read-only views over the entire state of the key-value store. ReadOptions::snapshot may be
non-NULL to indicate that a read should operate on a particular version of the DB state.

```c++
  rocksdb::ReadOptions options;
  options.snapshot = db->GetSnapshot();
  ... apply some updates to db ...
  rocksdb::Iterator* iter = db->NewIterator(options);
  ... read using iter to view the state when the snapshot was created ...
  delete iter;
  db->ReleaseSnapshot(options.snapshot);
```

### Columns Family

The main idea behind Column Families is that they share the write-ahead log and don't share memtables and table files.
By sharing write-ahead logs we get awesome benefit of atomic writes. By separating memtables and table files, we are
able to configure column families independently and delete them quickly.

### Prefix Iterators

Most LSM-tree engines cannot support an efficient range scan API because it needs to look into multiple data files. But,
most applications do not do pure-random scans of key ranges in the database; instead, applications typically scan within
a key-prefix. RocksDB uses this to its advantage. Applications can configure a Options.prefix_extractor to enable a
key-prefix based filtering.

Reference [link](https://github.com/facebook/rocksdb/wiki/Prefix-Seek)

Prefix seek is a feature for mitigating these overheads for some use cases. The basic idea is that, if users know the
iterating will be within one key prefix, the common prefix can be used to reduce costs. The most commonly used prefix
iterating technique is prefix bloom filter. If many sorted runs don't contain any entry for this prefix, it can be
filtered out by a bloom filter, and some I/Os and CPU for the sorted run can be ignored.

A motivating use case for prefix seek is representing multi-maps, such as secondary indexes in MyRocks, where the
RocksDB prefix is the key for the multimap and a RocksDB iterator finds all the entries associated with that prefix (
multimap key).

### MyRocks

Since RocksDB is a key-value storage, the MySQL structured information has to be encoded in plain key-value form. All
data in a single instance is stored in one RocksDB database in flat key-value space. Tables can be distributed between
multiple column families. Each table consists of a primary key and, potentially, a set of secondary keys. Each index has
its own unique ID. Let's take a look at an example table and how it is stored in RocksDB:

```sql
create table t1
(
    a varchar(50) primary key,
    b integer,
    c varchar(75),
    d varchar(100),
    key(c),
    unique (d),
    unique (c, d),
);
```

[Another link](https://github.com/facebook/mysql-5.6/wiki/MyRocks-data-dictionary-format)

### Compile Protobuf

```shell
protoc -I=. --cpp_out=. ./db.proto 
```