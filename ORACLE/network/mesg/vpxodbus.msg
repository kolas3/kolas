// Copyright (c) 1997, 2000 by the Oracle Corporation.  All rights reserved.
/
/
/
/
/ NAME
/   vpxodbus.msg
/
/
/ FUNCTION
/   Oracle Data Gatherer Oracle Database cartridge message file, American
/   English version.
/
/
/ NOTES
/       Message text should be limited to a maximum of 76 characters.
/       Messages containing embedded % directives should not overrun
/       the 76 character limit unless they signal unusual errors
/       which do not occur as part of user applications.  For example,
/       startup errors and system-dependent errors can probably get
/       away with longer message text. 
/
/
/       '/' at beginning-of-line for comments.
/       '//' at beginning-of-line for *Cause: And *Action: fields.
/
/
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
/
/ Capitalization conventions are as follows.  Change them if appropriate,
/   but be consistent!
/  1) since most of these messages appear without a "ORA-12345" prefix.
/     the first letter is capitalized
/  2) "class names", like Node, Community, or Interchange, are written
/     with the first letter in caps, since that is how they appear in 
/     the main window
/  3) parameters in a property sheet, like COST, are shown in all caps.
/
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/
/    0 -  999 Critical Errors
/
//////////////////////////////////////////////////////////////////////////
00000, 00000, "Error: Inconsistent semaphore state"
// *Comment: mnemonic: ODB_ERROR_INCONSISTENT_SEMAPHORE
// *Cause:
// *Action:
/
00001, 00000, "Error: Memory Allocation Failure"
// *Comment: mnemonic: ODB_ERROR_MEMORY_ALLOC_FAILURE
// *Cause:
// *Action:
/
//////////////////////////////////////////////////////////////////////////
/
/ 1000 - 1999 Not-So Critical Errors
/
//////////////////////////////////////////////////////////////////////////
01000, 00000, "Error: Oracle LOGON failed"
// *Comment: mnemonic: ODB_ERROR_LOGON_FAILED
// *Cause:
// *Action:
/
01001, 00000, "Error: OPEN Oracle cursor failed"
// *Comment: mnemonic: ODB_ERROR_OPEN_CURSOR_FAILED
// *Cause:
// *Action:
/
01002, 00000, "Error: Oracle PARSE sql failed"
// *Comment: mnemonic: ODB_ERROR_PARSE_FAILED
// *Cause:
// *Action:
/
01003, 00000, "Error: Oracle BIND sql failed"
// *Comment: mnemonic: ODB_ERROR_BIND_FAILED
// *Cause:
// *Action:
/
01004, 00000, "Error: Oracle DESCRIBE select list items failed"
// *Comment: mnemonic: ODB_ERROR_DESCRIBE_FAILED
// *Cause:
// *Action:
/
01005, 00000, "Error: Oracle DEFINE select list columns failed"
// *Comment: mnemonic: ODB_ERROR_DEFINE_FAILED
// *Cause:
// *Action:
/
01006, 00000, "Error: Oracle EXECUTE sql failed"
// *Comment: mnemonic: ODB_ERROR_EXECUTE_FAILED
// *Cause:
// *Action:
/
01007, 00000, "Error: Oracle FETCH query results failed"
// *Comment: mnemonic: ODB_ERROR_FETCH_FAILED
// *Cause:
// *Action:
/
01008, 00000, "Error: Unknown data-gatherer client ID"
// *Comment: mnemonic: ODB_ERROR_UNKNOWN_CLIENT
// *Cause:
// *Action:
/
01009, 00000, "Error: Unknown data-gatherer request ID"
// *Comment: mnemonic: ODB_ERROR_UNKNOWN_REQUEST
// *Cause:
// *Action:
/
01010, 00000, "Error: Unknown class ID"
// *Comment: mnemonic: ODB_ERROR_UNKNOWN_CLASS
// *Cause:
// *Action:
/
01011, 00000, "Error: Unknown resource ID"
// *Comment: mnemonic: ODB_ERROR_UNKNOWN_RESOURCE
// *Cause:
// *Action:
/
01012, 00000, "Error: No static instances exist for this class"
// *Comment: mnemonic: ODB_ERROR_NO_STATIC_INSTANCES
// *Cause:
// *Action:
/
01013, 00000, "Error: No such instance exists for this context-sensitive drilldown"
// *Comment: mnemonic: ODB_ERROR_INVALID_CONTEXT
// *Cause:
// *Action:
/
01014, 00000, "Error: Malformed user-defined script term"
// *Comment: mnemonic: ODB_ERROR_MALFORMED_UDSTERM
// *Cause:
// *Action:
/
01015, 00000, "Error: Expected data, none found"
// *Comment: mnemonic: ODB_ERROR_NO_DATA_FOUND
// *Cause:
// *Action:
/
01016, 00000, "Error: Invalid Oracle version banner"
// *Comment: mnemonic: ODB_ERROR_INVALID_VERSION_BANNER
// *Cause:
// *Action:
/
01017, 00000, "Error: Column named does not belong to table"
// *Comment: mnemonic: ODB_ERROR_UNKNOWN_COLUMN_NAME
// *Cause:
// *Action:
/
01018, 00000, "Error: Invalid thread state"
// *Comment: mnemonic: ODB_ERROR_INVALID_THREAD_STATE
// *Cause:
// *Action:
/
01019, 00000, "Error: Column in select list has unknown/invalid type"
// *Comment: mnemonic: ODB_ERROR_UNKNOWN_COLUMN_TYPE
// *Cause:
// *Action:
/
01020, 00000, "Error: Invalid user-defined script operation"
// *Comment: mnemonic: ODB_ERROR_UNKNOWN_UDS_OPERATION
// *Cause:
// *Action:
/
01021, 00000, "Error: Invalid user-defined script function"
// *Comment: mnemonic: ODB_ERROR_UNKNOWN_UDS_FUNCTION
// *Cause:
// *Action:
/
01022, 00000, "Error: Insufficient privileges to access v$ views"
// *Comment: mnemonic: ODB_ERROR_INSUFFICIENT_PRIVILEGES
// *Cause:
// *Action:
/
//////////////////////////////////////////////////////////////////////////
/
/ 2000 - 2999 Informational Messages
/
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/
/ 3000 - 4999 Labels or Captions
/
//////////////////////////////////////////////////////////////////////////
03000, 00000, "Collection of performance and capacity planning statistics for Oracle databases"
// *Comment: mnemonic: ODB_MODULE_DESC
// *Cause:
// *Action:
/
03001, 00000, "Sort Statistics Data"
// *Comment: mnemonic: ODB_SORT_NAME
// *Cause:
// *Action:
/
03002, 00000, "Oracle Database Sort Statistics"
// *Comment: mnemonic: ODB_SORT_DESC
// *Cause:
// *Action:
/
03003, 00000, "Memory Sort Percentage"
// *Comment: mnemonic: ODB_SORT_MEMORY_PCT
// *Cause:
// *Action:
/
03004, 00000, "% Sort Extent Pool Latch Sleep"
// *Comment: mnemonic: ODB_SORT_EXTENT_LATCH_PCT
// *Cause:
// *Action:
/
03005, 00000, "Sorts to Disk/Sec"
// *Comment: mnemonic: ODB_SORT_DISK
// *Cause:
// *Action:
/
03006, 00000, "Sorts in Memory/Sec"
// *Comment: mnemonic: ODB_SORT_MEMORY
// *Cause:
// *Action:
/
03007, 00000, "Sort Extent Pool Latch Sleeps/Sec"
// *Comment: mnemonic: ODB_SORT_EXTENT_LATCH_SLEEP
// *Cause:
// *Action:
/
03008, 00000, "Maximum Sort Allocation (Blocks)"
// *Comment: mnemonic: ODB_SORT_MAX_SORT_BLOCKS
// *Cause:
// *Action:
/
03009, 00000, "Sort Area Size (KBytes)"
// *Comment: mnemonic: ODB_SORT_AREA_SIZE
// *Cause:
// *Action:
/
03010, 00000, "Sort Area Retained Size (Bytes)"
// *Comment: mnemonic: ODB_SORT_RETAINED_SIZE
// *Cause:
// *Action:
/
03011, 00000, "Sort Statistics Chart"
// *Comment: mnemonic: ODB_SORT_CHART_DEF_NAME
// *Cause:
// *Action:
/
03012, 00000, "Chart of Sort Statistics"
// *Comment: mnemonic: ODB_SORT_CHART_DEF_DESC
// *Cause:
// *Action:
/
03013, 00000, "Memory Statistics Data"
// *Comment: mnemonic: ODB_MEMORY_NAME
// *Cause:
// *Action:
/
03014, 00000, "Oracle Database Memory Statistics"
// *Comment: mnemonic: ODB_MEMORY_DESC
// *Cause:
// *Action:
/
03015, 00000, "Buffer Cache Hit Percentage"
// *Comment: mnemonic: ODB_MEMORY_BUFCACHE_HR
// *Cause:
// *Action:
/
03016, 00000, "Shared Pool Alloc Hit Percentage"
// *Comment: mnemonic: ODB_MEMORY_SHPOOL_ALLOC_HR
// *Cause:
// *Action:
/
03017, 00000, "ShPoolRsvd List Alloc Hit %"
// *Comment: mnemonic: ODB_MEMORY_RSPOOL_ALLOC_HR
// *Cause:
// *Action:
/
03018, 00000, "Cursor Cache Hit Percentage"
// *Comment: mnemonic: ODB_MEMORY_CURSOR_CACHE_HR
// *Cause:
// *Action:
/
03019, 00000, "Redo Buffer Allocation Hit Percentage"
// *Comment: mnemonic: ODB_MEMORY_REDO_ALLOC_HR
// *Cause:
// *Action:
/
03020, 00000, "SGA Size (MBytes)"
// *Comment: mnemonic: ODB_MEMORY_TOTAL_SGA_SIZE
// *Cause:
// *Action:
/
03021, 00000, "Buffer Cache Size (MBytes)"
// *Comment: mnemonic: ODB_MEMORY_BUFCACHE_SIZE
// *Cause:
// *Action:
/
03022, 00000, "Shared Pool Size (MBytes)"
// *Comment: mnemonic: ODB_MEMORY_SHPOOL_SIZE
// *Cause:
// *Action:
/
03023, 00000, "Buffer Gets/Sec"
// *Comment: mnemonic: ODB_MEMORY_BUFFER_GETS
// *Cause:
// *Action:
/
03024, 00000, "Physical Reads/Sec"
// *Comment: mnemonic: ODB_MEMORY_PHYSICAL_READS
// *Cause:
// *Action:
/
03025, 00000, "Buffer Consistent Gets/Sec"
// *Comment: mnemonic: ODB_MEMORY_BUFFER_CONSGETS
// *Cause:
// *Action:
/
03026, 00000, "Library Cache Misses/Sec"
// *Comment: mnemonic: ODB_MEMORY_SHRPOOL_RELOAD
// *Cause:
// *Action:
/
03027, 00000, "Library Cache Requests/Sec"
// *Comment: mnemonic: ODB_MEMORY_SHRPOOL_REQUEST
// *Cause:
// *Action:
/
03028, 00000, "Current Maximum Open Cursors"
// *Comment: mnemonic: ODB_MEMORY_OPEN_CURSORS
// *Cause:
// *Action:
/
03029, 00000, "ShPoolRsvd Last Miss Size (Bytes)"
// *Comment: mnemonic: ODB_MEMORY_RSLIST_LASTMISS
// *Cause:
// *Action:
/
03030, 00000, "ShPoolRsvd List Max Free Memory Size (Bytes)"
// *Comment: mnemonic: ODB_MEMORY_RSLIST_MAXFREE
// *Cause:
// *Action:
/
03031, 00000, "ShPoolRsvd List Used Memory Count"
// *Comment: mnemonic: ODB_MEMORY_RSLIST_USEDCNT
// *Cause:
// *Action:
/
03032, 00000, "ShPoolRsvd Last Failed Request Size (Bytes)"
// *Comment: mnemonic: ODB_MEMORY_POOL_FAIL_SIZE
// *Cause:
// *Action:
/
03033, 00000, "Memory Statistics Chart"
// *Comment: mnemonic: ODB_MEMORY_CHART_DEF_NAME
// *Cause:
// *Action:
/
03034, 00000, "Chart of Memory Statistics"
// *Comment: mnemonic: ODB_MEMORY_CHART_DEF_DESC
// *Cause:
// *Action:
/
03035, 00000, "Parallel Query Option Statistics Data"
// *Comment: mnemonic: ODB_PQO_NAME
// *Cause:
// *Action:
/
03036, 00000, "Oracle Database Parallel Query Option Statistics"
// *Comment: mnemonic: ODB_PQO_DESC
// *Cause:
// *Action:
/
03037, 00000, "Parallel Query Servers Idle"
// *Comment: mnemonic: ODB_PQO_SERVERS_IDLE
// *Cause:
// *Action:
/
03038, 00000, "Parallel Query Servers Busy"
// *Comment: mnemonic: ODB_PQO_SERVERS_BUSY
// *Cause:
// *Action:
/
03039, 00000, "Minimum Parallel Query Servers"
// *Comment: mnemonic: ODB_PQO_SERVERS_MIN
// *Cause:
// *Action:
/
03040, 00000, "Maximum Parallel Query Servers"
// *Comment: mnemonic: ODB_PQO_SERVERS_MAX
// *Cause:
// *Action:
/
03041, 00000, "Parallel Query Option Statistics Chart"
// *Comment: mnemonic: ODB_PQO_CHART_DEF_NAME
// *Cause:
// *Action:
/
03042, 00000, "Chart of Parallel Query Option Statistics"
// *Comment: mnemonic: ODB_PQO_CHART_DEF_DESC
// *Cause:
// *Action:
/
03043, 00000, "Multi-Threaded Server Statistics Data"
// *Comment: mnemonic: ODB_MTS_NAME
// *Cause:
// *Action:
/
03044, 00000, "Oracle Multi-Threaded Server Statistics"
// *Comment: mnemonic: ODB_MTS_DESC
// *Cause:
// *Action:
/
03045, 00000, "Highest Number of Servers"
// *Comment: mnemonic: ODB_MTS_SERVERS_BUSY
// *Cause:
// *Action:
/
03046, 00000, "Minimum Shared Server Processes"
// *Comment: mnemonic: ODB_MTS_SERVERS_MIN
// *Cause:
// *Action:
/
03047, 00000, "Maximum Shared Server Processes"
// *Comment: mnemonic: ODB_MTS_SERVERS_MAX
// *Cause:
// *Action:
/
03048, 00000, "Multi-Threaded Server Statistics Chart"
// *Comment: mnemonic: ODB_MTS_CHART_DEF_NAME
// *Cause:
// *Action:
/
03049, 00000, "Chart of Multi-Threaded Server Statistics"
// *Comment: mnemonic: ODB_MTS_CHART_DEF_DESC
// *Cause:
// *Action:
/
03050, 00000, "Instance-wide I/O Statistics Data"
// *Comment: mnemonic: ODB_DB_IO_NAME
// *Cause:
// *Action:
/
03051, 00000, "Oracle Instance-wide I/O Statistics"
// *Comment: mnemonic: ODB_DB_IO_DESC
// *Cause:
// *Action:
/
03052, 00000, "Blocks Per Table Scan"
// *Comment: mnemonic: ODB_DB_IO_BLKS_PER_SCAN
// *Cause:
// *Action:
/
03053, 00000, "Free Buffer Hit Percentage"
// *Comment: mnemonic: ODB_DB_IO_FREE_BUFWAITSRT
// *Cause:
// *Action:
/
03054, 00000, "Seconds Left to Write Dirty Queue"
// *Comment: mnemonic: ODB_DB_IO_DIRTY_BUFQLEN
// *Cause:
// *Action:
/
03055, 00000, "Full Table Scan Rows Percentage"
// *Comment: mnemonic: ODB_DB_IO_TSCAN_ROWS
// *Cause:
// *Action:
/
03056, 00000, "Summed Dirty Query Length"
// *Comment: mnemonic: ODB_DB_IO_DIRTYQ_LENGTH
// *Cause:
// *Action:
/
03057, 00000, "Total Free Buffer Waits/Sec"
// *Comment: mnemonic: ODB_DB_IO_FREE_BUFWAITS
// *Cause:
// *Action:
/
03058, 00000, "Table Scans (Long Tables)/Sec"
// *Comment: mnemonic: ODB_DB_IO_LONG_TBLSCANS
// *Cause:
// *Action:
/
03059, 00000, "Table Scans (Short Tables)/Sec"
// *Comment: mnemonic: ODB_DB_IO_SHORT_TBLSCANS
// *Cause:
// *Action:
/
03060, 00000, "Table Fetch By ROWID/Sec"
// *Comment: mnemonic: ODB_DB_IO_TBLFETCH_ROWID
// *Cause:
// *Action:
/
03061, 00000, "Blocks Gotten Per Table Scan"
// *Comment: mnemonic: ODB_DB_IO_TBLSCAN_BLOCKS
// *Cause:
// *Action:
/
03062, 00000, "Rows Gotten Per Table Scan"
// *Comment: mnemonic: ODB_DB_IO_TBLSCAN_ROWS
// *Cause:
// *Action:
/
03063, 00000, "Write Requests/Sec"
// *Comment: mnemonic: ODB_DB_IO_WRITE_REQ
// *Cause:
// *Action:
/
03064, 00000, "Instance-wide I/O Statistics Chart"
// *Comment: mnemonic: ODB_DB_IO_CHART_DEF_NAME
// *Cause:
// *Action:
/
03065, 00000, "Chart of Instance-wide I/O Statistics"
// *Comment: mnemonic: ODB_DB_IO_CHART_DEF_DESC
// *Cause:
// *Action:
/
03066, 00000, "Session Statistics Data"
// *Comment: mnemonic: ODB_SESSION_NAME
// *Cause:
// *Action:
/
03067, 00000, "Oracle Database Session Statistics"
// *Comment: mnemonic: ODB_SESSION_DESC
// *Cause:
// *Action:
/
03068, 00000, "Current Concurrent Sessions"
// *Comment: mnemonic: ODB_SESSION_CONCURRENT
// *Cause:
// *Action:
/
03069, 00000, "Session UGA Memory Allocation (MBytes)"
// *Comment: mnemonic: ODB_SESSION_UGA_MEMORY_ALLOC
// *Cause:
// *Action:
/
03070, 00000, "Session PGA Memory Allocation (MBytes)"
// *Comment: mnemonic: ODB_SESSION_PGA_MEMORY_ALLOC
// *Cause:
// *Action:
/
03071, 00000, "Session Statistics Chart"
// *Comment: mnemonic: ODB_SESSION_CHART_DEF_NAME
// *Cause:
// *Action:
/
03072, 00000, "Chart of Session Statistics"
// *Comment: mnemonic: ODB_SESSION_CHART_DEF_DESC
// *Cause:
// *Action:
/
03073, 00000, "Sort Latch Statistics Data"
// *Comment: mnemonic: ODB_LOCK_NAME
// *Cause:
// *Action:
/
03074, 00000, "Oracle Database Sort Latch Statistics"
// *Comment: mnemonic: ODB_LOCK_DESC
// *Cause:
// *Action:
/
03075, 00000, "% Sort Extent Pool Latch Sleep"
// *Comment: mnemonic: ODB_LOCK_SORT_SLEEP_RATIO
// *Cause:
// *Action:
/
03076, 00000, "Total Latch Sleeps/Sec"
// *Comment: mnemonic: ODB_LOCK_TOTAL_SLEEPS
// *Cause:
// *Action:
/
03077, 00000, "Sort Extent Pool Latch Sleeps/Sec"
// *Comment: mnemonic: ODB_LOCK_SORT_SLEEPS
// *Cause:
// *Action:
/
03078, 00000, "Sort Latch Statistics Chart"
// *Comment: mnemonic: ODB_LOCK_CHART_DEF_NAME
// *Cause:
// *Action:
/
03079, 00000, "Chart of Sort Latch Statistics"
// *Comment: mnemonic: ODB_LOCK_CHART_DEF_DESC
// *Cause:
// *Action:
/
03080, 00000, "Rollback Statistics Data"
// *Comment: mnemonic: ODB_ROLLBACK_NAME
// *Cause:
// *Action:
/
03081, 00000, "Oracle Database Rollback Statistics"
// *Comment: mnemonic: ODB_ROLLBACK_DESC
// *Cause:
// *Action:
/
03082, 00000, "Active Segments"
// *Comment: mnemonic: ODB_ROLLBACK_ACTIVE
// *Cause:
// *Action:
/
03083, 00000, "Shrinks"
// *Comment: mnemonic: ODB_ROLLBACK_SHRINKS
// *Cause:
// *Action:
/
03084, 00000, "Wraps"
// *Comment: mnemonic: ODB_ROLLBACK_WRAPS
// *Cause:
// *Action:
/
03085, 00000, "Extends"
// *Comment: mnemonic: ODB_ROLLBACK_EXTENTS
// *Cause:
// *Action:
/
03086, 00000, "Rollback Statistics Chart"
// *Comment: mnemonic: ODB_ROLLBACK_CHART_DEF_NAME
// *Cause:
// *Action:
/
03087, 00000, "Chart of Rollback Statistics"
// *Comment: mnemonic: ODB_ROLLBACK_CHART_DEF_DESC
// *Cause:
// *Action:
/
03088, 00000, "Resource Statistics Data"
// *Comment: mnemonic: ODB_RESOURCE_NAME
// *Cause:
// *Action:
/
03089, 00000, "Oracle Database Resource Statistics"
// *Comment: mnemonic: ODB_RESOURCE_DESC
// *Cause:
// *Action:
/
03090, 00000, "Database Size (Mbytes)"
// *Comment: mnemonic: ODB_RESOURCE_DB_SIZE
// *Cause:
// *Action:
/
03091, 00000, "Number of Tablespaces"
// *Comment: mnemonic: ODB_RESOURCE_TBLSPACES
// *Cause:
// *Action:
/
03092, 00000, "Number of Schemas"
// *Comment: mnemonic: ODB_RESOURCE_SCHEMAS
// *Cause:
// *Action:
/
03093, 00000, "Number of Datafiles"
// *Comment: mnemonic: ODB_RESOURCE_DB_DATAFILES
// *Cause:
// *Action:
/
03094, 00000, "Number of Users"
// *Comment: mnemonic: ODB_RESOURCE_USERS
// *Cause:
// *Action:
/
03095, 00000, "Maximum Log Buffer Size (Mbytes)"
// *Comment: mnemonic: ODB_RESOURCE_MAXLOGBUF_SIZE
// *Cause:
// *Action:
/
03096, 00000, "Resource Statistics Chart"
// *Comment: mnemonic: ODB_RESOURCE_CHART_DEF_NAME
// *Cause:
// *Action:
/
03097, 00000, "Chart of Resource Statistics"
// *Comment: mnemonic: ODB_RESOURCE_CHART_DEF_DESC
// *Cause:
// *Action:
/
03098, 00000, "Tablespace Statistics Data"
// *Comment: mnemonic: ODB_TABLESPACE_NAME
// *Cause:
// *Action:
/
03099, 00000, "Oracle Database Tablespace Statistics"
// *Comment: mnemonic: ODB_TABLESPACE_DESC
// *Cause:
// *Action:
/
03100, 00000, "Total Size in Bytes"
// *Comment: mnemonic: ODB_TABLESPACE_TOTAL_SIZE
// *Cause:
// *Action:
/
03101, 00000, "Number of Datafiles"
// *Comment: mnemonic: ODB_TABLESPACE_TS_DATAFILES
// *Cause:
// *Action:
/
03102, 00000, "Tablespace Statistics Chart"
// *Comment: mnemonic: ODB_STATS_TABLESPACE_CHART_DEF_NAME
// *Cause:
// *Action:
/
03103, 00000, "Chart of Tablespace Statistics"
// *Comment: mnemonic: ODB_TABLESPACE_CHART_DEF_DESC
// *Cause:
// *Action:
/
03104, 00000, "Transaction Statistics Data"
// *Comment: mnemonic: ODB_TRANSACTION_NAME
// *Cause:
// *Action:
/
03105, 00000, "Oracle Database Transaction Statistics"
// *Comment: mnemonic: ODB_TRANSACTION_DESC
// *Cause:
// *Action:
/
03106, 00000, "Concurrent Transactions"
// *Comment: mnemonic: ODB_TRANSACTION_CONCURRENT
// *Cause:
// *Action:
/
03107, 00000, "Selects"
// *Comment: mnemonic: ODB_TRANSACTION_SELECTS
// *Cause:
// *Action:
/
03108, 00000, "Updates"
// *Comment: mnemonic: ODB_TRANSACTION_UPDATES
// *Cause:
// *Action:
/
03109, 00000, "Inserts"
// *Comment: mnemonic: ODB_TRANSACTION_INSERTS
// *Cause:
// *Action:
/
03110, 00000, "Deletes"
// *Comment: mnemonic: ODB_TRANSACTION_DELETES
// *Cause:
// *Action:
/
03111, 00000, "Transaction Statistics Chart"
// *Comment: mnemonic: ODB_TRANSACTION_CHART_DEF_NAME 
// *Cause:
// *Action:
/
03112, 00000, "Chart of Transaction Statistics"
// *Comment: mnemonic: ODB_TRANSACTION_CHART_DEF_DESC
// *Cause:
// *Action:
/
03113, 00000, "Parallel Server Statistics Data"
// *Comment: mnemonic: ODB_OPS_NAME
// *Cause:
// *Action:
/
03114, 00000, "Oracle Parallel Server Statistics"
// *Comment: mnemonic: ODB_OPS_DESC
// *Cause:
// *Action:
/
03115, 00000, "Pings"
// *Comment: mnemonic: ODB_STATS_OPS_PINGS
// *Cause:
// *Action:
/
03116, 00000, "Parallel Server Statistics Chart"
// *Comment: mnemonic: ODB_OPS_CHART_DEF_NAME
// *Cause:
// *Action:
/
03117, 00000, "Chart of Oracle Parallel Server Statistics"
// *Comment: mnemonic: ODB_OPS_CHART_DEF_DESC
// *Cause:
// *Action:
/
03118, 00000, "Table Statistics Data"
// *Comment: mnemonic: ODB_TABLE_NAME
// *Cause:
// *Action:
/
03119, 00000, "Oracle Table Statistics"
// *Comment: mnemonic: ODB_TABLE_DESC
// *Cause:
// *Action:
/
03120, 00000, "Cardinality"
// *Comment: mnemonic: ODB_TABLE_CARDINALITY
// *Cause:
// *Action:
/
03121, 00000, "Table Statistics Chart"
// *Comment: mnemonic: ODB_TABLE_CHART_DEF_NAME
// *Cause:
// *Action:
/
03122, 00000, "Chart of Table Statistics"
// *Comment: mnemonic: ODB_TABLE_CHART_DEF_DESC
// *Cause:
// *Action:
/
03123, 00000, "Wait Statistics Data"
// *Comment: mnemonic: ODB_WAIT_NAME
// *Cause:
// *Action:
/
03124, 00000, "Oracle Database Wait Statistics"
// *Comment: mnemonic: ODB_WAIT_DESC
// *Cause:
// *Action:
/
03125, 00000, "CPU Wait Time (1/100 sec)"
// *Comment: mnemonic: ODB_WAIT_PROCESSOR
// *Cause:
// *Action:
/
03126, 00000, "Total I/O Waits/Sec"
// *Comment: mnemonic: ODB_WAIT_IO
// *Cause:
// *Action:
/
03127, 00000, "Total Latch Free Waits/Sec"
// *Comment: mnemonic: ODB_WAIT_LATCH_FREE
// *Cause:
// *Action:
/
03128, 00000, "Enqueue Waits"
// *Comment: mnemonic: ODB_WAIT_ENQUEUE
// *Cause:
// *Action:
/
03129, 00000, "Total Buffer Busy Waits/Sec"
// *Comment: mnemonic: ODB_WAIT_BUFFER_BUSY
// *Cause:
// *Action:
/
03130, 00000, "Total Free Buffer Waits/Sec"
// *Comment: mnemonic: ODB_WAIT_FREE_BUFFER
// *Cause:
// *Action:
/
03131, 00000, "Wait Statistics Chart"
// *Comment: mnemonic: ODB_WAIT_CHART_DEF_NAME
// *Cause:
// *Action:
/
03132, 00000, "Chart of Wait Statistics"
// *Comment: mnemonic: ODB_WAIT_CHART_DEF_DESC
// *Cause:
// *Action:
/
03133, 00000, "File I/O Rate Data"
// *Comment: mnemonic: ODB_FILE_IO_RATE_NAME
// *Cause:
// *Action:
/
03134, 00000, "Oracle Database File I/O Statistics"
// *Comment: mnemonic: ODB_FILE_IO_RATE_DESC
// *Cause:
// *Action:
/
03135, 00000, "Physical Reads/Sec"
// *Comment: mnemonic: ODB_FILE_IO_RATE_PHYRDS
// *Cause:
// *Action:
/
03136, 00000, "Physical Writes/Sec"
// *Comment: mnemonic: ODB_FILE_IO_RATE_PHYWRTS
// *Cause:
// *Action:
/
03137, 00000, "File I/O Rate Chart"
// *Comment: mnemonic: ODB_FILE_IO_RATE_CHART_DEF_NAME
// *Cause:
// *Action:
/
03138, 00000, "Chart of File I/O Rate"
// *Comment: mnemonic: ODB_FILE_IO_RATE_CHART_DEF_DESC
// *Cause:
// *Action:
/
03139, 00000, "Top Files Data"
// *Comment: mnemonic: ODB_TOP_FILES_NAME
// *Cause:
// *Action:
/
03140, 00000, "Oracle Database Top Files Statistics"
// *Comment: mnemonic: ODB_TOP_FILES_DESC
// *Cause:
// *Action:
/
03141, 00000, "Physical Reads/Sec"
// *Comment: mnemonic: ODB_TOP_FILES_PHYRDS
// *Cause:
// *Action:
/
03142, 00000, "Physical Writes/Sec"
// *Comment: mnemonic: ODB_TOP_FILES_PHYWRTS
// *Cause:
// *Action:
/
03143, 00000, "Physical Block Reads/Sec"
// *Comment: mnemonic: ODB_TOP_FILES_PHYBLKRDS
// *Cause:
// *Action:
/
03144, 00000, "Physical Block Writes/Sec"
// *Comment: mnemonic: ODB_TOP_FILES_PHYBLKWRTS
// *Cause:
// *Action:
/
03145, 00000, "Top Files Chart"
// *Comment: mnemonic: ODB_TOP_FILES_CHART_DEF_NAME
// *Cause:
// *Action:
/
03146, 00000, "Chart of Top Files"
// *Comment: mnemonic: ODB_TOP_FILES_CHART_DEF_DESC
// *Cause:
// *Action:
/
03147, 00000, "File Details Data"
// *Comment: mnemonic: ODB_FILE_DETAILS_NAME
// *Cause:
// *Action:
/
03148, 00000, "Oracle Database File Details Statistics"
// *Comment: mnemonic: ODB_FILE_DETAILS_DESC
// *Cause:
// *Action:
/
03149, 00000, "Physical Reads/Sec"
// *Comment: mnemonic: ODB_FILE_DETAILS_PHYRDS
// *Cause:
// *Action:
/
03150, 00000, "Physical Writes/Sec"
// *Comment: mnemonic: ODB_FILE_DETAILS_PHYWRTS
// *Cause:
// *Action:
/
03151, 00000, "Physical Block Reads/Sec"
// *Comment: mnemonic: ODB_FILE_DETAILS_PHYBLKRDS
// *Cause:
// *Action:
/
03152, 00000, "Physical Block Writes/Sec"
// *Comment: mnemonic: ODB_FILE_DETAILS_PHYBLKWRTS
// *Cause:
// *Action:
/
03153, 00000, "Read Time (Milliseconds)"
// *Comment: mnemonic: ODB_FILE_DETAILS_READTIME
// *Cause:
// *Action:
/
03154, 00000, "Write Time (Milliseconds)"
// *Comment: mnemonic: ODB_FILE_DETAILS_WRITETIME
// *Cause:
// *Action:
/
03155, 00000, "Status"
// *Comment: mnemonic: ODB_FILE_DETAILS_STATUS
// *Cause:
// *Action:
/
03156, 00000, "Size in Bytes"
// *Comment: mnemonic: ODB_FILE_DETAILS_BYTES
// *Cause:
// *Action:
/
03157, 00000, "Size in Blocks"
// *Comment: mnemonic: ODB_FILE_DETAILS_BLOCKS
// *Cause:
// *Action:
/
03158, 00000, "File Details Chart"
// *Comment: mnemonic: ODB_FILE_DETAILS_CHART_DEF_NAME
// *Cause:
// *Action:
/
03159, 00000, "Chart of File Details"
// *Comment: mnemonic: ODB_FILE_DETAILS_CHART_DEF_DESC
// *Cause:
// *Action:
/
03160, 00000, "Network I/O Data"
// *Comment: mnemonic: ODB_NET_IO_RATE_NAME
// *Cause:
// *Action:
/
03161, 00000, "Oracle SQL*Net I/O Statistics"
// *Comment: mnemonic: ODB_NET_IO_RATE_DESC
// *Cause:
// *Action:
/
03162, 00000, "SQL*Net Waits/Sec"
// *Comment: mnemonic: ODB_NET_IO_RATE_WAITS
// *Cause:
// *Action:
/
03163, 00000, "Network I/O Rate Chart"
// *Comment: mnemonic: ODB_NET_IO_RATE_CHART_DEF_NAME
// *Cause:
// *Action:
/
03164, 00000, "Chart of Network I/O Rate"
// *Comment: mnemonic: ODB_NET_IO_RATE_CHART_DEF_DESC
// *Cause:
// *Action:
/
03165, 00000, "System I/O Rate Data"
// *Comment: mnemonic: ODB_SYSTEM_IO_RATE_NAME
// *Cause:
// *Action:
/
03166, 00000, "Oracle System I/O Rate Statistics"
// *Comment: mnemonic: ODB_SYSTEM_IO_RATE_DESC
// *Cause:
// *Action:
/
03167, 00000, "Buffer Gets/Sec"
// *Comment: mnemonic: ODB_SYSTEM_IO_RATE_GETS
// *Cause:
// *Action:
/
03168, 00000, "Block Changes/Sec"
// *Comment: mnemonic: ODB_SYSTEM_IO_RATE_CHANGES
// *Cause:
// *Action:
/
03169, 00000, "Physical Reads/Sec"
// *Comment: mnemonic: ODB_SYSTEM_IO_RATE_READS
// *Cause:
// *Action:
/
03170, 00000, "System I/O Rate Chart"
// *Comment: mnemonic: ODB_SYSTEM_IO_RATE_CHART_DEF_NAME
// *Cause:
// *Action:
/
03171, 00000, "Chart of System I/O Rate"
// *Comment: mnemonic: ODB_SYSTEM_IO_RATE_CHART_DEF_DESC
// *Cause:
// *Action:
/
03172, 00000, "Circuit Data"
// *Comment: mnemonic: ODB_CIRCUIT_NAME
// *Cause:
// *Action:
/
03173, 00000, "Oracle Circuit Statistics"
// *Comment: mnemonic: ODB_CIRCUIT_DESC
// *Cause:
// *Action:
/
03174, 00000, "Circuit"
// *Comment: mnemonic: ODB_CIRCUIT_CIRCUIT
// *Cause:
// *Action:
/
03175, 00000, "Dispatcher Name"
// *Comment: mnemonic: ODB_CIRCUIT_DISPATCHER
// *Cause:
// *Action:
/
03176, 00000, "Server"
// *Comment: mnemonic: ODB_CIRCUIT_SERVER
// *Cause:
// *Action:
/
03177, 00000, "Session ID"
// *Comment: mnemonic: ODB_CIRCUIT_SESSION
// *Cause:
// *Action:
/
03178, 00000, "Session Serial Number"
// *Comment: mnemonic: ODB_CIRCUIT_SERIAL
// *Cause:
// *Action:
/
03179, 00000, "Status"
// *Comment: mnemonic: ODB_CIRCUIT_STATUS
// *Cause:
// *Action:
/
03180, 00000, "Queue"
// *Comment: mnemonic: ODB_CIRCUIT_QUEUE
// *Cause:
// *Action:
/
03181, 00000, "Messages"
// *Comment: mnemonic: ODB_CIRCUIT_MESSAGES
// *Cause:
// *Action:
/
03182, 00000, "Bytes"
// *Comment: mnemonic: ODB_CIRCUIT_BYTES
// *Cause:
// *Action:
/
03183, 00000, "Circuit Chart"
// *Comment: mnemonic: ODB_CIRCUIT_CHART_DEF_NAME
// *Cause:
// *Action:
/
03184, 00000, "Chart of Circuit Statistics"
// *Comment: mnemonic: ODB_CIRCUIT_CHART_DEF_DESC
// *Cause:
// *Action:
/
03185, 00000, "Dispatcher Data"
// *Comment: mnemonic: ODB_DISPATCHER_NAME
// *Cause:
// *Action:
/
03186, 00000, "Oracle Dispatcher Statistics"
// *Comment: mnemonic: ODB_DISPATCHER_DESC
// *Cause:
// *Action:
/
03187, 00000, "Dispatcher Name"
// *Comment: mnemonic: ODB_DISPATCHER_DISNAME
// *Cause:
// *Action:
/
03188, 00000, "Status"
// *Comment: mnemonic: ODB_DISPATCHER_STATUS
// *Cause:
// *Action:
/
03189, 00000, "Accept"
// *Comment: mnemonic: ODB_DISPATCHER_ACCEPT
// *Cause:
// *Action:
/
03190, 00000, "Messages"
// *Comment: mnemonic: ODB_DISPATCHER_MSGS
// *Cause:
// *Action:
/
03191, 00000, "Bytes"
// *Comment: mnemonic: ODB_DISPATCHER_BYTES
// *Cause:
// *Action:
/
03192, 00000, "Circuits Owned"
// *Comment: mnemonic: ODB_DISPATCHER_OWNED
// *Cause:
// *Action:
/
03193, 00000, "Idle Time"
// *Comment: mnemonic: ODB_DISPATCHER_IDLE
// *Cause:
// *Action:
/
03194, 00000, "Busy Time"
// *Comment: mnemonic: ODB_DISPATCHER_BUSY
// *Cause:
// *Action:
/
03195, 00000, "Load"
// *Comment: mnemonic: ODB_DISPATCHER_LOAD
// *Cause:
// *Action:
/
03196, 00000, "Dispatcher Chart"
// *Comment: mnemonic: ODB_DISPATCHER_CHART_DEF_NAME
// *Cause:
// *Action:
/
03197, 00000, "Chart of Dispatcher Statistics"
// *Comment: mnemonic: ODB_DISPATCHER_CHART_DEF_DESC
// *Cause:
// *Action:
/
03198, 00000, "Free List Access Data"
// *Comment: mnemonic: ODB_FREE_LIST_HIT_NAME
// *Cause:
// *Action:
/
03199, 00000, "Oracle Free List Access Statistics"
// *Comment: mnemonic: ODB_FREE_LIST_HIT_DESC
// *Cause:
// *Action:
/
03200, 00000, "Hit %"
// *Comment: mnemonic: ODB_FREE_LIST_HIT_HIT
// *Cause:
// *Action:
/
03201, 00000, "Miss %"
// *Comment: mnemonic: ODB_FREE_LIST_HIT_MISS
// *Cause:
// *Action:
/
03202, 00000, "Free List Hit% (Cumulative) Chart"
// *Comment: mnemonic: ODB_FREE_LIST_HIT_CHART_DEF_NAME
// *Cause:
// *Action:
/
03203, 00000, "Chart of Free List Hit %"
// *Comment: mnemonic: ODB_FREE_LIST_HIT_CHART_DEF_DESC
// *Cause:
// *Action:
/
03204, 00000, "Latch Data"
// *Comment: mnemonic: ODB_LATCH_NAME
// *Cause:
// *Action:
/
03205, 00000, "Oracle Latch Statistics"
// *Comment: mnemonic: ODB_LATCH_DESC
// *Cause:
// *Action:
/
03206, 00000, "Holder PID"
// *Comment: mnemonic: ODB_LATCH_HOLDER_PID
// *Cause:
// *Action:
/
03207, 00000, "Gets (Wait)/Sec"
// *Comment: mnemonic: ODB_LATCH_GET_WAIT
// *Cause:
// *Action:
/
03208, 00000, "Misses (Wait)/Sec"
// *Comment: mnemonic: ODB_LATCH_MISS_WAIT
// *Cause:
// *Action:
/
03209, 00000, "Sleeps (Wait)/Sec"
// *Comment: mnemonic: ODB_LATCH_SLEEP_WAIT
// *Cause:
// *Action:
/
03210, 00000, "Gets (No Wait)/Sec"
// *Comment: mnemonic: ODB_LATCH_GET_NO_WAIT
// *Cause:
// *Action:
/
03211, 00000, "Misses (No Wait)/Sec"
// *Comment: mnemonic: ODB_LATCH_MISS_NO_WAIT
// *Cause:
// *Action:
/
03212, 00000, "Latch Chart"
// *Comment: mnemonic: ODB_LATCH_CHART_DEF_NAME
// *Cause:
// *Action:
/
03213, 00000, "Chart of Latch Statistics"
// *Comment: mnemonic: ODB_LATCH_CHART_DEF_DESC
// *Cause:
// *Action:
/
03214, 00000, "Lock Manager"
// *Comment: mnemonic: ODB_LOCKMGR_NAME
// *Cause:
// *Action:
/
03215, 00000, "Oracle Lock Statistics"
// *Comment: mnemonic: ODB_LOCKMGR_DESC
// *Cause:
// *Action:
/
03216, 00000, "User Name"
// *Comment: mnemonic: ODB_LOCKMGR_USER_NAME
// *Cause:
// *Action:
/
03217, 00000, "Session ID"
// *Comment: mnemonic: ODB_LOCKMGR_SESSION_ID
// *Cause:
// *Action:
/
03218, 00000, "Session Serial Number"
// *Comment: mnemonic: ODB_LOCKMGR_SERIAL_NO
// *Cause:
// *Action:
/
03219, 00000, "Lock Type"
// *Comment: mnemonic: ODB_LOCKMGR_TYPE
// *Cause:
// *Action:
/
03220, 00000, "Resource ID1"
// *Comment: mnemonic: ODB_LOCKMGR_RES_ID1
// *Cause:
// *Action:
/
03221, 00000, "Resource ID2"
// *Comment: mnemonic: ODB_LOCKMGR_RES_ID2
// *Cause:
// *Action:
/
03222, 00000, "Mode Held"
// *Comment: mnemonic: ODB_LOCKMGR_MODE_HELD
// *Cause:
// *Action:
/
03223, 00000, "Mode Requested"
// *Comment: mnemonic: ODB_LOCKMGR_MODE_REQUESTED
// *Cause:
// *Action:
/
03224, 00000, "Locks Chart"
// *Comment: mnemonic: ODB_LOCKMGR_CHART_DEF_NAME
// *Cause:
// *Action:
/
03225, 00000, "Chart of Locks (All Types) Statistics"
// *Comment: mnemonic: ODB_LOCKMGR_CHART_DEF_DESC
// *Cause:
// *Action:
/
03226, 00000, "Queue Data"
// *Comment: mnemonic: ODB_QUEUE_NAME
// *Cause:
// *Action:
/
03227, 00000, "Oracle Queue Statistics"
// *Comment: mnemonic: ODB_QUEUE_DESC
// *Cause:
// *Action:
/
03228, 00000, "Process Address"
// *Comment: mnemonic: ODB_QUEUE_PROC_ADDR
// *Cause:
// *Action:
/
03229, 00000, "Queue Type"
// *Comment: mnemonic: ODB_QUEUE_TYPE
// *Cause:
// *Action:
/
03230, 00000, "Currently Queued"
// *Comment: mnemonic: ODB_QUEUE_CURR
// *Cause:
// *Action:
/
03231, 00000, "Total Queued"
// *Comment: mnemonic: ODB_QUEUE_TOTAL
// *Cause:
// *Action:
/
03232, 00000, "Avg Wait(s)"
// *Comment: mnemonic: ODB_QUEUE_AVG_WAIT
// *Cause:
// *Action:
/
03233, 00000, "Queue Chart"
// *Comment: mnemonic: ODB_QUEUE_CHART_DEF_NAME
// *Cause:
// *Action:
/
03234, 00000, "Chart of Queue Statistics"
// *Comment: mnemonic: ODB_QUEUE_CHART_DEF_DESC
// *Cause:
// *Action:
/
03235, 00000, "Redo Allocation Data"
// *Comment: mnemonic: ODB_REDO_ALLOC_HIT_NAME
// *Cause:
// *Action:
/
03236, 00000, "Oracle Redo Allocation Statistics"
// *Comment: mnemonic: ODB_REDO_ALLOC_HIT_DESC
// *Cause:
// *Action:
/
03237, 00000, "Hit%"
// *Comment: mnemonic: ODB_REDO_ALLOC_HIT_HIT
// *Cause:
// *Action:
/
03238, 00000, "Miss%"
// *Comment: mnemonic: ODB_REDO_ALLOC_HIT_MISS
// *Cause:
// *Action:
/
03239, 00000, "Redo Allocation Hit% Chart"
// *Comment: mnemonic: ODB_REDO_ALLOC_HIT_CHART_DEF_NAME
// *Cause:
// *Action:
/
03240, 00000, "Chart of Redo Allocation Hit%"
// *Comment: mnemonic: ODB_REDO_ALLOC_HIT_CHART_DEF_DESC
// *Cause:
// *Action:
/
03241, 00000, "Rollback (Nowait) Data"
// *Comment: mnemonic: ODB_RB_NOWAIT_HIT_NAME
// *Cause:
// *Action:
/
03242, 00000, "Oracle Rollback (Nowait) Statistics"
// *Comment: mnemonic: ODB_RB_NOWAIT_HIT_DESC
// *Cause:
// *Action:
/
03243, 00000, "Hit%"
// *Comment: mnemonic: ODB_RB_NOWAIT_HIT_HIT
// *Cause:
// *Action:
/
03244, 00000, "Miss%"
// *Comment: mnemonic: ODB_RB_NOWAIT_HIT_MISS
// *Cause:
// *Action:
/
03245, 00000, "Rollback (Nowait) Hit% Chart"
// *Comment: mnemonic: ODB_RB_NOWAIT_HIT_CHART_DEF_NAME
// *Cause:
// *Action:
/
03246, 00000, "Chart of Rollback (Nowait) Hit%"
// *Comment: mnemonic: ODB_RB_NOWAIT_HIT_CHART_DEF_DESC
// *Cause:
// *Action:
/
03247, 00000, "Shared Server Data"
// *Comment: mnemonic: ODB_SERVER_NAME
// *Cause:
// *Action:
/
03248, 00000, "Oracle Shared Server Statistics"
// *Comment: mnemonic: ODB_SERVER_DESC
// *Cause:
// *Action:
/
03249, 00000, "Name"
// *Comment: mnemonic: ODB_SERVER_SERVNAME
// *Cause:
// *Action:
/
03250, 00000, "Status"
// *Comment: mnemonic: ODB_SERVER_STATUS
// *Cause:
// *Action:
/
03251, 00000, "Requests"
// *Comment: mnemonic: ODB_SERVER_REQUESTS
// *Cause:
// *Action:
/
03252, 00000, "Idle"
// *Comment: mnemonic: ODB_SERVER_IDLE
// *Cause:
// *Action:
/
03253, 00000, "Busy"
// *Comment: mnemonic: ODB_SERVER_BUSY
// *Cause:
// *Action:
/
03254, 00000, "Busy/(Idle+Busy)"
// *Comment: mnemonic: ODB_SERVER_BUSY_TOTAL
// *Cause:
// *Action:
/
03255, 00000, "Circuit"
// *Comment: mnemonic: ODB_SERVER_CIRCUIT
// *Cause:
// *Action:
/
03256, 00000, "Server Chart"
// *Comment: mnemonic: ODB_SERVER_CHART_DEF_NAME
// *Cause:
// *Action:
/
03257, 00000, "Chart of Shared Server Statistics"
// *Comment: mnemonic: ODB_SERVER_CHART_DEF_DESC
// *Cause:
// *Action:
/
03258, 00000, "Initialization Parameters"
// *Comment: mnemonic: ODB_INIT_PARAMS_NAME
// *Cause:
// *Action:
/
03259, 00000, "Oracle Database Initialization Parameters"
// *Comment: mnemonic: ODB_INIT_PARAMS_DESC
// *Cause:
// *Action:
/
03260, 00000, "Number"
// *Comment: mnemonic: ODB_INIT_PARAMS_NUM
// *Cause:
// *Action:
/
03261, 00000, "Type"
// *Comment: mnemonic: ODB_INIT_PARAMS_TYPE
// *Cause:
// *Action:
/
03262, 00000, "Value"
// *Comment: mnemonic: ODB_INIT_PARAMS_VALUE
// *Cause:
// *Action:
/
03263, 00000, "Default"
// *Comment: mnemonic: ODB_INIT_PARAMS_ISDEF
// *Cause:
// *Action:
/
03264, 00000, "Session Modifiable"
// *Comment: mnemonic: ODB_INIT_PARAMS_ISSES_MOD
// *Cause:
// *Action:
/
03265, 00000, "System Modifiable"
// *Comment: mnemonic: ODB_INIT_PARAMS_ISSYS_MOD
// *Cause:
// *Action:
/
03266, 00000, "Modified"
// *Comment: mnemonic: ODB_INIT_PARAMS_ISMOD
// *Cause:
// *Action:
/
03267, 00000, "Adjusted"
// *Comment: mnemonic: ODB_INIT_PARAMS_ISADJ
// *Cause:
// *Action:
/
03268, 00000, "Description"
// *Comment: mnemonic: ODB_INIT_PARAMS_DESCRIPTION
// *Cause:
// *Action:
/
03269, 00000, "Initialization Parameter Chart"
// *Comment: mnemonic: ODB_INIT_PARAMS_CHART_DEF_NAME
// *Cause:
// *Action:
/
03270, 00000, "Chart of Oracle Initialization Parameters"
// *Comment: mnemonic: ODB_INIT_PARAMS_CHART_DEF_DESC
// *Cause:
// *Action:
/
03271, 00000, "Process Data"
// *Comment: mnemonic: ODB_PROCESS_NAME
// *Cause:
// *Action:
/
03272, 00000, "Oracle Process Statistics"
// *Comment: mnemonic: ODB_PROCESS_DESC
// *Cause:
// *Action:
/
03273, 00000, "Spid"
// *Comment: mnemonic: ODB_PROCESS_SPID
// *Cause:
// *Action:
/
03274, 00000, "Username"
// *Comment: mnemonic: ODB_PROCESS_USERNAME
// *Cause:
// *Action:
/
03275, 00000, "Latchwait"
// *Comment: mnemonic: ODB_PROCESS_LATCHWAIT
// *Cause:
// *Action:
/
03276, 00000, "Terminal"
// *Comment: mnemonic: ODB_PROCESS_TERMINAL
// *Cause:
// *Action:
/
03277, 00000, "Program"
// *Comment: mnemonic: ODB_PROCESS_PROGRAM
// *Cause:
// *Action:
/
03278, 00000, "Process Chart"
// *Comment: mnemonic: ODB_PROCESS_CHART_DEF_NAME
// *Cause:
// *Action:
/
03279, 00000, "Chart of Process Statistics"
// *Comment: mnemonic: ODB_PROCESS_CHART_DEF_DESC
// *Cause:
// *Action:
/
03280, 00000, "System Statistics Data"
// *Comment: mnemonic: ODB_SYSSTAT_NAME
// *Cause:
// *Action:
/
03281, 00000, "Oracle System Statistics"
// *Comment: mnemonic: ODB_SYSSTAT_DESC
// *Cause:
// *Action:
/
03282, 00000, "Total"
// *Comment: mnemonic: ODB_SYSSTAT_TOTAL
// *Cause:
// *Action:
/
03283, 00000, "System Statistics Chart"
// *Comment: mnemonic: ODB_SYSSTAT_CHART_DEF_NAME
// *Cause:
// *Action:
/
03284, 00000, "Chart of System Statistics"
// *Comment: mnemonic: ODB_SYSSTAT_CHART_DEF_DESC
// *Cause:
// *Action:
/
03285, 00000, "System Events Data"
// *Comment: mnemonic: ODB_SYSEVENT_NAME
// *Cause:
// *Action:
/
03286, 00000, "Oracle System Wait Event Statistics"
// *Comment: mnemonic: ODB_SYSEVENT_DESC
// *Cause:
// *Action:
/
03287, 00000, "Total Waits"
// *Comment: mnemonic: ODB_SYSEVENT_TOTALWAIT
// *Cause:
// *Action:
/
03288, 00000, "Total Timeouts"
// *Comment: mnemonic: ODB_SYSEVENT_TOTALTO
// *Cause:
// *Action:
/
03289, 00000, "Average Wait"
// *Comment: mnemonic: ODB_SYSEVENT_AVGWAIT
// *Cause:
// *Action:
/
03290, 00000, "System Event Chart"
// *Comment: mnemonic: ODB_SYSEVENT_CHART_DEF_NAME
// *Cause:
// *Action:
/
03291, 00000, "Chart of System Wait Events"
// *Comment: mnemonic: ODB_SYSEVENT_CHART_DEF_DESC
// *Cause:
// *Action:
/
03292, 00000, "Table Access Data"
// *Comment: mnemonic: ODB_TAB_ACC_NAME
// *Cause:
// *Action:
/
03293, 00000, "Oracle Table Access Statistics"
// *Comment: mnemonic: ODB_TAB_ACC_DESC
// *Cause:
// *Action:
/
03294, 00000, "Session ID"
// *Comment: mnemonic: ODB_TAB_ACC_SESSION
// *Cause:
// *Action:
/
03295, 00000, "Owner"
// *Comment: mnemonic: ODB_TAB_ACC_OWNER
// *Cause:
// *Action:
/
03296, 00000, "Object"
// *Comment: mnemonic: ODB_TAB_ACC_OBJECT
// *Cause:
// *Action:
/
03297, 00000, "Table Access Chart"
// *Comment: mnemonic: ODB_TAB_ACC_CHART_DEF_NAME
// *Cause:
// *Action:
/
03298, 00000, "Chart of Table Accesses"
// *Comment: mnemonic: ODB_TAB_ACC_CHART_DEF_DESC
// *Cause:
// *Action:
/
03299, 00000, "Tablespace Data"
// *Comment: mnemonic: ODB_TS_NAME
// *Cause:
// *Action:
/
03300, 00000, "Oracle Tablespace Statistics"
// *Comment: mnemonic: ODB_TS_DESC
// *Cause:
// *Action:
/
03301, 00000, "Status"
// *Comment: mnemonic: ODB_TS_STATUS
// *Cause:
// *Action:
/
03302, 00000, "Total File Blocks"
// *Comment: mnemonic: ODB_TS_FILE
// *Cause:
// *Action:
/
03303, 00000, "Total Quota Blocks"
// *Comment: mnemonic: ODB_TS_QUOTA
// *Cause:
// *Action:
/
03304, 00000, "Tablespace Chart"
// *Comment: mnemonic: ODB_TS_CHART_DEF_NAME
// *Cause:
// *Action:
/
03305, 00000, "Chart of Tablespace Statistics"
// *Comment: mnemonic: ODB_TS_CHART_DEF_DESC
// *Cause:
// *Action:
/
03306, 00000, "Tablespace Free Space Data"
// *Comment: mnemonic: ODB_TSFREE_NAME
// *Cause:
// *Action:
/
03307, 00000, "Oracle Tablespace Free Space Statistics"
// *Comment: mnemonic: ODB_TSFREE_DESC
// *Cause:
// *Action:
/
03308, 00000, "Free Space (KBytes)"
// *Comment: mnemonic: ODB_TSFREE_KBYTES
// *Cause:
// *Action:
/
03309, 00000, "Free Space (Blocks)"
// *Comment: mnemonic: ODB_TSFREE_BLOCKS
// *Cause:
// *Action:
/
03310, 00000, "Free Chunks"
// *Comment: mnemonic: ODB_TSFREE_CHUNKS
// *Cause:
// *Action:
/
03311, 00000, "Tablespace Free Space Chart"
// *Comment: mnemonic: ODB_TSFREE_CHART_DEF_NAME
// *Cause:
// *Action:
/
03312, 00000, "Chart of Tablespace Free Space"
// *Comment: mnemonic: ODB_TSFREE_CHART_DEF_DESC
// *Cause:
// *Action:
/
03313, 00000, "Active Users Data"
// *Comment: mnemonic: ODB_USERS_ACTIVE_NAME
// *Cause:
// *Action:
/
03314, 00000, "Oracle Users Active Statistics"
// *Comment: mnemonic: ODB_USERS_ACTIVE_DESC
// *Cause:
// *Action:
/
03315, 00000, "Number of Active Users"
// *Comment: mnemonic: ODB_USERS_ACTIVE_NO
// *Cause:
// *Action:
/
03316, 00000, "Number of Users Active Chart"
// *Comment: mnemonic: ODB_USERS_ACTIVE_CHART_DEF_NAME
// *Cause:
// *Action:
/
03317, 00000, "Chart of Number Users Active"
// *Comment: mnemonic: ODB_USERS_ACTIVE_CHART_DEF_DESC
// *Cause:
// *Action:
/
03318, 00000, "Logged On Users Data"
// *Comment: mnemonic: ODB_USERS_LOGDON_NAME
// *Cause:
// *Action:
/
03319, 00000, "Oracle Logged On Users Statistics"
// *Comment: mnemonic: ODB_USERS_LOGDON_DESC
// *Cause:
// *Action:
/
03320, 00000, "Number of Users Logged On"
// *Comment: mnemonic: ODB_USERS_LOGDON_NO
// *Cause:
// *Action:
/
03321, 00000, "Number of Users Logged On Chart"
// *Comment: mnemonic: ODB_USERS_LOGDON_CHART_DEF_NAME
// *Cause:
// *Action:
/
03322, 00000, "Chart of Number Users Logged On"
// *Comment: mnemonic: ODB_USERS_LOGDON_CHART_DEF_DESC
// *Cause:
// *Action:
/
03323, 00000, "Waiting Users Data"
// *Comment: mnemonic: ODB_USERS_WAIT_NAME
// *Cause:
// *Action:
/
03324, 00000, "Oracle Waiting Users Statistics"
// *Comment: mnemonic: ODB_USERS_WAIT_DESC
// *Cause:
// *Action:
/
03325, 00000, "Number of Users Waiting"
// *Comment: mnemonic: ODB_USERS_WAIT_NO
// *Cause:
// *Action:
/
03326, 00000, "Number of Users Waiting Chart"
// *Comment: mnemonic: ODB_USERS_WAIT_CHART_DEF_NAME
// *Cause:
// *Action:
/
03327, 00000, "Chart of Number of Users Waiting"
// *Comment: mnemonic: ODB_USERS_WAIT_CHART_DEF_DESC
// *Cause:
// *Action:
/
03328, 00000, "Waiting (on Locks) Users Data"
// *Comment: mnemonic: ODB_USERS_WAITLOCK_NAME
// *Cause:
// *Action:
/
03329, 00000, "Oracle Waiting (on Locks) Users Statistics"
// *Comment: mnemonic: ODB_USERS_WAITLOCK_DESC
// *Cause:
// *Action:
/
03330, 00000, "Number of Users Waiting for Locks"
// *Comment: mnemonic: ODB_USERS_WAITLOCK_NO
// *Cause:
// *Action:
/
03331, 00000, "Number of Users Waiting for Locks Chart"
// *Comment: mnemonic: ODB_USERS_WAITLOCK_CHART_DEF_NAME
// *Cause:
// *Action:
/
03332, 00000, "Chart of Users Waiting for Locks"
// *Comment: mnemonic: ODB_USERS_WAITLOCK_CHART_DEF_DESC
// *Cause:
// *Action:
/
03333, 00000, "Running Users Data"
// *Comment: mnemonic: ODB_USERS_RUNNING_NAME
// *Cause:
// *Action:
/
03334, 00000, "Oracle Running Users Statistics"
// *Comment: mnemonic: ODB_USERS_RUNNING_DESC
// *Cause:
// *Action:
/
03335, 00000, "Number of Users Running"
// *Comment: mnemonic: ODB_USERS_RUNNING_NO
// *Cause:
// *Action:
/
03336, 00000, "Number of Users Running Chart"
// *Comment: mnemonic: ODB_USERS_RUNNING_CHART_DEF_NAME
// *Cause:
// *Action:
/
03337, 00000, "Chart of Number of Users Running"
// *Comment: mnemonic: ODB_USERS_RUNNING_CHART_DEF_DESC
// *Cause:
// *Action:
/
03338, 00000, "Buffer Gets Data"
// *Comment: mnemonic: ODB_BUFGETS_NAME
// *Cause:
// *Action:
/
03339, 00000, "Oracle Buffer Gets Statistic"
// *Comment: mnemonic: ODB_BUFGETS_DESC
// *Cause:
// *Action:
/
03340, 00000, "Buffer Gets/Sec"
// *Comment: mnemonic: ODB_BUFGETS_RATE
// *Cause:
// *Action:
/
03341, 00000, "Buffer Gets Rate Chart"
// *Comment: mnemonic: ODB_BUFGETS_CHART_DEF_NAME
// *Cause:
// *Action:
/
03342, 00000, "Chart of Buffer Gets Rate"
// *Comment: mnemonic: ODB_BUFGETS_CHART_DEF_DESC
// *Cause:
// *Action:
/
03343, 00000, "Network Wait Rate Data"
// *Comment: mnemonic: ODB_NETWAIT_CHART_DEF_NAME
// *Cause:
// *Action:
/
03344, 00000, "Chart of Network Wait Rate"
// *Comment: mnemonic: ODB_NETWAIT_CHART_DEF_DESC
// *Cause:
// *Action:
/
03345, 00000, "Network Bytes/Sec"
// *Comment: mnemonic: ODB_NETBYTES_RATE
// *Cause:
// *Action:
/
03346, 00000, "Network Bytes Rate Chart"
// *Comment: mnemonic: ODB_NETBYTES_CHART_DEF_NAME
// *Cause:
// *Action:
/
03347, 00000, "Chart of Network Bytes Rate"
// *Comment: mnemonic: ODB_NETBYTES_CHART_DEF_DESC
// *Cause:
// *Action:
/
03348, 00000, "Redo Statistics Rate Data"
// *Comment: mnemonic: ODB_REDORATE_NAME
// *Cause:
// *Action:
/
03349, 00000, "Oracle Redo Statistics Rates"
// *Comment: mnemonic: ODB_REDORATE_DESC
// *Cause:
// *Action:
/
03350, 00000, "Entries/Sec"
// *Comment: mnemonic: ODB_REDORATE_ENTRIES
// *Cause:
// *Action:
/
03351, 00000, "Space Requests/Sec"
// *Comment: mnemonic: ODB_REDORATE_SPACEREQ
// *Cause:
// *Action:
/
03352, 00000, "Synchronous Writes/Sec"
// *Comment: mnemonic: ODB_REDORATE_SYNCHWRTS
// *Cause:
// *Action:
/
03353, 00000, "Redo Statistics Rate Chart"
// *Comment: mnemonic: ODB_REDORATE_CHART_DEF_NAME
// *Cause:
// *Action:
/
03354, 00000, "Chart of Redo Statistics Rates"
// *Comment: mnemonic: ODB_REDORATE_CHART_DEF_DESC
// *Cause:
// *Action:
/
03355, 00000, "Sort Rows Rate Data"
// *Comment: mnemonic: ODB_SORTROWS_NAME
// *Cause:
// *Action:
/
03356, 00000, "Oracle Sort Rows Statistic"
// *Comment: mnemonic: ODB_SORTROWS_DESC
// *Cause:
// *Action:
/
03357, 00000, "Sort Rows/Sec"
// *Comment: mnemonic: ODB_SORT_ROWS_RATE
// *Cause:
// *Action:
/
03358, 00000, "Sort Rows Rate Chart"
// *Comment: mnemonic: ODB_SORTROWS_CHART_DEF_NAME
// *Cause:
// *Action:
/
03359, 00000, "Chart of Sort Rows Rate"
// *Comment: mnemonic: ODB_SORTROWS_CHART_DEF_DESC
// *Cause:
// *Action:
/
03360, 00000, "Table Scan Rows Rate Data"
// *Comment: mnemonic: ODB_TBLSCANROWS_NAME
// *Cause:
// *Action:
/
03361, 00000, "Oracle Table Scan Rows Rate Statistic"
// *Comment: mnemonic: ODB_TBLSCANROWS_DESC
// *Cause:
// *Action:
/
03362, 00000, "Table Scan Rows/Sec"
// *Comment: mnemonic: ODB_TBLSCANROWS_RATE
// *Cause:
// *Action:
/
03363, 00000, "Table Scan Rows Rate Chart"
// *Comment: mnemonic: ODB_TBLSCANROWS_CHART_DEF_NAME
// *Cause:
// *Action:
/
03364, 00000, "Chart of Table Scan Rows Rate"
// *Comment: mnemonic: ODB_TBLSCANROWS_CHART_DEF_DESC
// *Cause:
// *Action:
/
03365, 00000, "Throughput Rate Data"
// *Comment: mnemonic: ODB_THRUPUT_NAME
// *Cause:
// *Action:
/
03366, 00000, "Oracle Throughput Statistics"
// *Comment: mnemonic: ODB_THRUPUT_DESC
// *Cause:
// *Action:
/
03367, 00000, "User Calls/Sec"
// *Comment: mnemonic: ODB_THRUPUT_USERCALLS
// *Cause:
// *Action:
/
03368, 00000, "Transactions/Sec"
// *Comment: mnemonic: ODB_THRUPUT_TRANSACTIONS
// *Cause:
// *Action:
/
03369, 00000, "Throughput Rate Chart"
// *Comment: mnemonic: ODB_THRUPUT_CHART_DEF_NAME
// *Cause:
// *Action:
/
03370, 00000, "Chart of Throughput Rate"
// *Comment: mnemonic: ODB_THRUPUT_CHART_DEF_DESC
// *Cause:
// *Action:
/
03371, 00000, "Buffer Cache Hit% Data"
// *Comment: mnemonic: ODB_BUFCACHE_HIT_NAME
// *Cause:
// *Action:
/
03372, 00000, "Oracle Buffer Cache Hit% Statistic"
// *Comment: mnemonic: ODB_BUFCACHE_HIT_DESC
// *Cause:
// *Action:
/
03373, 00000, "Hit%"
// *Comment: mnemonic: ODB_BUFCACHE_HIT_HIT
// *Cause:
// *Action:
/
03374, 00000, "Miss%"
// *Comment: mnemonic: ODB_BUFCACHE_HIT_MISS
// *Cause:
// *Action:
/
03375, 00000, "Buffer Cache Hit% (Cumulative) Chart"
// *Comment: mnemonic: ODB_BUFCACHE_HIT_CHART_DEF_NAME
// *Cause:
// *Action:
/
03376, 00000, "Chart of Buffer Cache Hit%"
// *Comment: mnemonic: ODB_BUFCACHE_HIT_CHART_DEF_DESC
// *Cause:
// *Action:
/
03377, 00000, "Data Dictionary Hit% Data"
// *Comment: mnemonic: ODB_DATADICT_HIT_NAME
// *Cause:
// *Action:
/
03378, 00000, "Oracle Data Dictionary Hit% Statistics"
// *Comment: mnemonic: ODB_DATADICT_HIT_DESC
// *Cause:
// *Action:
/
03379, 00000, "Hit%"
// *Comment: mnemonic: ODB_DATADICT_HIT_HIT
// *Cause:
// *Action:
/
03380, 00000, "Miss%"
// *Comment: mnemonic: ODB_DATADICT_HIT_MISS
// *Cause:
// *Action:
/
03381, 00000, "Data Dictionary Hit% (Cumulative) Chart"
// *Comment: mnemonic: ODB_DATADICT_HIT_CHART_DEF_NAME
// *Cause:
// *Action:
/
03382, 00000, "Chart of Data Dictionary Hit%"
// *Comment: mnemonic: ODB_DATADICT_HIT_CHART_DEF_DESC
// *Cause:
// *Action:
/
03383, 00000, "Library Cache Hit% Data"
// *Comment: mnemonic: ODB_LIBCACHE_HIT_NAME
// *Cause:
// *Action:
/
03384, 00000, "Oracle Library Cache Hit% Statistics"
// *Comment: mnemonic: ODB_LIBCACHE_HIT_DESC
// *Cause:
// *Action:
/
03385, 00000, "Hit%"
// *Comment: mnemonic: ODB_LIBCACHE_HIT_HIT
// *Cause:
// *Action:
/
03386, 00000, "Miss%"
// *Comment: mnemonic: ODB_LIBCACHE_HIT_MISS
// *Cause:
// *Action:
/
03387, 00000, "Library Cache Hit% (Cumulative) Chart"
// *Comment: mnemonic: ODB_LIBCACHE_HIT_CHART_DEF_NAME
// *Cause:
// *Action:
/
03388, 00000, "Chart of Library Cache Hit%"
// *Comment: mnemonic: ODB_LIBCACHE_HIT_CHART_DEF_DESC
// *Cause:
// *Action:
/
03389, 00000, "Library Cache Details Data"
// *Comment: mnemonic: ODB_LIBCACHE_DET_NAME
// *Cause:
// *Action:
/
03390, 00000, "Oracle Library Cache Details Statistics"
// *Comment: mnemonic: ODB_LIBCACHE_DET_DESC
// *Cause:
// *Action:
/
03391, 00000, "Gets/Sec"
// *Comment: mnemonic: ODB_LIBCACHE_DET_GETS
// *Cause:
// *Action:
/
03392, 00000, "Get Hits/Sec"
// *Comment: mnemonic: ODB_LIBCACHE_DET_GETHITS
// *Cause:
// *Action:
/
03393, 00000, "Get Hit Ratio"
// *Comment: mnemonic: ODB_LIBCACHE_DET_GETHITRATIO
// *Cause:
// *Action:
/
03394, 00000, "Pins/Sec"
// *Comment: mnemonic: ODB_LIBCACHE_DET_PINS
// *Cause:
// *Action:
/
03395, 00000, "Pin Hits/Sec"
// *Comment: mnemonic: ODB_LIBCACHE_DET_PINHITS
// *Cause:
// *Action:
/
03396, 00000, "Pin Hit Ratio"
// *Comment: mnemonic: ODB_LIBCACHE_DET_PINHITRATIO
// *Cause:
// *Action:
/
03397, 00000, "Reloads/Sec"
// *Comment: mnemonic: ODB_LIBCACHE_DET_RELOADS
// *Cause:
// *Action:
/
03398, 00000, "Invalidations/Sec"
// *Comment: mnemonic: ODB_LIBCACHE_DET_INVALIDS
// *Cause:
// *Action:
/
03399, 00000, "DLM Lock Requests/Sec"
// *Comment: mnemonic: ODB_LIBCACHE_DET_DLM_LOCK_REQ
// *Cause:
// *Action:
/
03400, 00000, "DLM Pin Requests/Sec"
// *Comment: mnemonic: ODB_LIBCACHE_DET_DLM_PIN_REQ
// *Cause:
// *Action:
/
03401, 00000, "DLM Pin Releases/Sec"
// *Comment: mnemonic: ODB_LIBCACHE_DET_DLM_PIN_REL
// *Cause:
// *Action:
/
03402, 00000, "DLM Invalidation Requests/Sec"
// *Comment: mnemonic: ODB_LIBCACHE_DET_DLM_INVALIDS_REQ
// *Cause:
// *Action:
/
03403, 00000, "DLM Invalidations/Sec"
// *Comment: mnemonic: ODB_LIBCACHE_DET_DLM_INVALIDS
// *Cause:
// *Action:
/
03404, 00000, "Library Cache Details Chart"
// *Comment: mnemonic: ODB_LIBCACHE_DET_CHART_DEF_NAME
// *Cause:
// *Action:
/
03405, 00000, "Chart of Library Cache Details"
// *Comment: mnemonic: ODB_LIBCACHE_DET_CHART_DEF_DESC
// *Cause:
// *Action:
/
03406, 00000, "Memory Allocated Data"
// *Comment: mnemonic: ODB_MEMALLOC_NAME
// *Cause:
// *Action:
/
03407, 00000, "Oracle Session Memory Allocation Statistic"
// *Comment: mnemonic: ODB_MEMALLOC_DESC
// *Cause:
// *Action:
/
03408, 00000, "Total Session Memory"
// *Comment: mnemonic: ODB_MEMALLOC_TOTAL
// *Cause:
// *Action:
/
03409, 00000, "Memory Allocated Chart"
// *Comment: mnemonic: ODB_MEMALLOC_CHART_DEF_NAME
// *Cause:
// *Action:
/
03410, 00000, "Chart of Memory Allocated"
// *Comment: mnemonic: ODB_MEMALLOC_CHART_DEF_DESC
// *Cause:
// *Action:
/
03411, 00000, "Memory Sort Hit% Data"
// *Comment: mnemonic: ODB_MEMSORT_HIT_NAME
// *Cause:
// *Action:
/
03412, 00000, "Oracle Memory Sort Hit% Statistics"
// *Comment: mnemonic: ODB_MEMSORT_HIT_DESC
// *Cause:
// *Action:
/
03413, 00000, "Hit%"
// *Comment: mnemonic: ODB_MEMSORT_HIT_HIT
// *Cause:
// *Action:
/
03414, 00000, "Miss%"
// *Comment: mnemonic: ODB_MEMSORT_HIT_MISS
// *Cause:
// *Action:
/
03415, 00000, "Memory Sort Hit% (Cumulative) Chart"
// *Comment: mnemonic: ODB_MEMSORT_HIT_CHART_DEF_NAME
// *Cause:
// *Action:
/
03416, 00000, "Chart of Memory Sort Hit%"
// *Comment: mnemonic: ODB_MEMSORT_HIT_CHART_DEF_DESC
// *Cause:
// *Action:
/
03417, 00000, "Parse Ratio Data"
// *Comment: mnemonic: ODB_PARSERATIO_NAME
// *Cause:
// *Action:
/
03418, 00000, "Oracle Parse Ratio Statistic"
// *Comment: mnemonic: ODB_PARSERATIO_DESC
// *Cause:
// *Action:
/
03419, 00000, "Parse Ratio"
// *Comment: mnemonic: ODB_PARSERATIO_VALUE
// *Cause:
// *Action:
/
03420, 00000, "Parse Ratio Chart"
// *Comment: mnemonic: ODB_PARSERATIO_CHART_DEF_NAME
// *Cause:
// *Action:
/
03421, 00000, "Chart of Parse Ratio"
// *Comment: mnemonic: ODB_PARSERATIO_CHART_DEF_DESC
// *Cause:
// *Action:
/
03422, 00000, "Read Consistent Hit% Data"
// *Comment: mnemonic: ODB_READCONS_HIT_NAME
// *Cause:
// *Action:
/
03423, 00000, "Oracle Consistent Read Hit% Statistics"
// *Comment: mnemonic: ODB_READCONS_HIT_DESC
// *Cause:
// *Action:
/
03424, 00000, "Hit%"
// *Comment: mnemonic: ODB_READCONS_HIT_HIT
// *Cause:
// *Action:
/
03425, 00000, "Miss%"
// *Comment: mnemonic: ODB_READCONS_HIT_MISS
// *Cause:
// *Action:
/
03426, 00000, "Read Consistency Hit% (Cumulative) Chart"
// *Comment: mnemonic: ODB_READCONS_HIT_CHART_DEF_NAME
// *Cause:
// *Action:
/
03427, 00000, "Chart of Read Consistency Hit%"
// *Comment: mnemonic: ODB_READCONS_HIT_CHART_DEF_DESC
// *Cause:
// *Action:
/
03428, 00000, "Overview of Performance"
// *Comment: mnemonic: ODB_OVERVIEW_NAME
// *Cause:
// *Action:
/
03429, 00000, "Oracle Performance Overview"
// *Comment: mnemonic: ODB_OVERVIEW_DESC
// *Cause:
// *Action:
/
03430, 00000, "Overview Chart Group"
// *Comment: mnemonic: ODB_OVERVIEW_CHART_DEF_NAME
// *Cause:
// *Action:
/
03431, 00000, "An Overview Grouping of Performance Charts"
// *Comment: mnemonic: ODB_OVERVIEW_CHART_DEF_DESC
// *Cause:
// *Action:
/
03432, 00000, "Parallel Server Active Users"
// *Comment: mnemonic: ODB_P_ACT_USER_NAME
// *Cause:
// *Action:
/
03433, 00000, "Oracle Parallel Server Active Users"
// *Comment: mnemonic: ODB_P_ACT_USER_DESC
// *Cause:
// *Action:
/
03434, 00000, "Count"
// *Comment: mnemonic: ODB_P_ACT_USER_COUNT
// *Cause:
// *Action:
/
03435, 00000, "Parallel Server Active Users Chart"
// *Comment: mnemonic: ODB_P_ACT_USER_CHART_DEF_NAME
// *Cause:
// *Action:
/
03436, 00000, "Chart of Parallel Server Active Users"
// *Comment: mnemonic: ODB_P_ACT_USER_CHART_DEF_DESC
// *Cause:
// *Action:
/
03437, 00000, "Parallel Server Active Users By Instance"
// *Comment: mnemonic: ODB_P_ACT_USER_INST_NAME
// *Cause:
// *Action:
/
03438, 00000, "Oracle Parallel Server Active Users By Instance"
// *Comment: mnemonic: ODB_P_ACT_USER_INST_DESC
// *Cause:
// *Action:
/
03439, 00000, "Count"
// *Comment: mnemonic: ODB_P_ACT_USER_INST_COUNT
// *Cause:
// *Action:
/
03440, 00000, "Parallel Server Active Users By Instance Chart"
// *Comment: mnemonic: ODB_P_ACT_USER_INST_CHART_DEF_NAME
// *Cause:
// *Action:
/
03441, 00000, "Chart of Parallel Server Active Users By Instance"
// *Comment: mnemonic: ODB_P_ACT_USER_INST_CHART_DEF_DESC
// *Cause:
// *Action:
/
03442, 00000, "Parallel Server Block Ping"
// *Comment: mnemonic: ODB_P_BLKPING_NAME
// *Cause:
// *Action:
/
03443, 00000, "Oracle Parallel Server Block Ping"
// *Comment: mnemonic: ODB_P_BLKPING_DESC
// *Cause:
// *Action:
/
03444, 00000, "Sum"
// *Comment: mnemonic: ODB_P_BLKPING_SUM
// *Cause:
// *Action:
/
03445, 00000, "Parallel Server Block Ping Chart"
// *Comment: mnemonic: ODB_P_BLKPING_CHART_DEF_NAME
// *Cause:
// *Action:
/
03446, 00000, "Chart of Parallel Server Block Ping"
// *Comment: mnemonic: ODB_P_BLKPING_CHART_DEF_DESC
// *Cause:
// *Action:
/
03447, 00000, "Parallel Server Block Ping By Instance"
// *Comment: mnemonic: ODB_P_BLKPING_INST_NAME
// *Cause:
// *Action:
/
03448, 00000, "Oracle Parallel Server Block Ping By Instance"
// *Comment: mnemonic: ODB_P_BLKPING_INST_DESC
// *Cause:
// *Action:
/
03449, 00000, "Max"
// *Comment: mnemonic: ODB_P_BLKPING_INST_MAX
// *Cause:
// *Action:
/
03450, 00000, "Parallel Server Block Ping By Instance Chart"
// *Comment: mnemonic: ODB_P_BLKPING_INST_CHART_DEF_NAME
// *Cause:
// *Action:
/
03451, 00000, "Chart of Parallel Server Block Ping By Instance"
// *Comment: mnemonic: ODB_P_BLKPING_INST_CHART_DEF_DESC
// *Cause:
// *Action:
/
03452, 00000, "Parallel Server Block Ping By Tablespace"
// *Comment: mnemonic: ODB_P_BLKPING_TS_NAME
// *Cause:
// *Action:
/
03453, 00000, "Oracle Parallel Server Block Ping By Tablespace"
// *Comment: mnemonic: ODB_P_BLKPING_TS_DESC
// *Cause:
// *Action:
/
03454, 00000, "Sum"
// *Comment: mnemonic: ODB_P_BLKPING_TS_SUM
// *Cause:
// *Action:
/
03455, 00000, "Parallel Server Block Ping By Tablespace Chart"
// *Comment: mnemonic: ODB_P_BLKPING_TS_CHART_DEF_NAME
// *Cause:
// *Action:
/
03456, 00000, "Chart of Parallel Server Block Ping By Tablespace"
// *Comment: mnemonic: ODB_P_BLKPING_TS_CHART_DEF_DESC
// *Cause:
// *Action:
/
03457, 00000, "Parallel Server File IO Rate"
// *Comment: mnemonic: ODB_P_FILE_IO_NAME
// *Cause:
// *Action:
/
03458, 00000, "Oracle Parallel Server File IO Rate"
// *Comment: mnemonic: ODB_P_FILE_IO_DESC
// *Cause:
// *Action:
/
03459, 00000, "Physical Reads/Sec"
// *Comment: mnemonic: ODB_P_FILE_IO_PHYRDS
// *Cause:
// *Action:
/
03460, 00000, "Physical Writes/Sec"
// *Comment: mnemonic: ODB_P_FILE_IO_PHYWRTS
// *Cause:
// *Action:
/
03461, 00000, "Parallel Server File IO Rate Chart"
// *Comment: mnemonic: ODB_P_FILE_IO_CHART_DEF_NAME
// *Cause:
// *Action:
/
03462, 00000, "Chart of Parallel Server File IO Rate"
// *Comment: mnemonic: ODB_P_FILE_IO_CHART_DEF_DESC
// *Cause:
// *Action:
/
03463, 00000, "Parallel Server File IO Rate By Instance"
// *Comment: mnemonic: ODB_P_FILE_IO_INST_NAME
// *Cause:
// *Action:
/
03464, 00000, "Oracle Parallel Server File IO Rate By Instance"
// *Comment: mnemonic: ODB_P_FILE_IO_INST_DESC
// *Cause:
// *Action:
/
03465, 00000, "Physical Reads/Sec"
// *Comment: mnemonic: ODB_P_FILE_IO_INST_PHYRDS
// *Cause:
// *Action:
/
03466, 00000, "Physical Writes/Sec"
// *Comment: mnemonic: ODB_P_FILE_IO_INST_PHYWRTS
// *Cause:
// *Action:
/
03467, 00000, "Parallel Server File IO Rate By Instance Chart"
// *Comment: mnemonic: ODB_P_FILE_IO_INST_CHART_DEF_NAME
// *Cause:
// *Action:
/
03468, 00000, "Chart of Parallel Server File IO Rate By Instance"
// *Comment: mnemonic: ODB_P_FILE_IO_INST_CHART_DEF_DESC
// *Cause:
// *Action:
/
03469, 00000, "Parallel Server File IO Rate By Object"
// *Comment: mnemonic: ODB_P_FILE_IO_OBJ_NAME
// *Cause:
// *Action:
/
03470, 00000, "Oracle Parallel Server File IO Rate By Object"
// *Comment: mnemonic: ODB_P_FILE_IO_OBJ_DESC
// *Cause:
// *Action:
/
03471, 00000, "Physical Reads/Sec"
// *Comment: mnemonic: ODB_P_FILE_IO_OBJ_PHYRDS
// *Cause:
// *Action:
/
03472, 00000, "Physical Writes/Sec"
// *Comment: mnemonic: ODB_P_FILE_IO_OBJ_PHYWRTS
// *Cause:
// *Action:
/
03473, 00000, "Parallel Server File IO Rate By Object Chart"
// *Comment: mnemonic: ODB_P_FILE_IO_OBJ_CHART_DEF_NAME
// *Cause:
// *Action:
/
03474, 00000, "Chart of Parallel Server File IO Rate By Object"
// *Comment: mnemonic: ODB_P_FILE_IO_OBJ_CHART_DEF_DESC
// *Cause:
// *Action:
/
03475, 00000, "Parallel Server Lock Activity"
// *Comment: mnemonic: ODB_P_LOCK_NAME
// *Cause:
// *Action:
/
03476, 00000, "Oracle Parallel Server Lock Activity"
// *Comment: mnemonic: ODB_P_LOCK_DESC
// *Cause:
// *Action:
/
03477, 00000, "From"
// *Comment: mnemonic: ODB_P_LOCK_FROM
// *Cause:
// *Action:
/
03478, 00000, "To"
// *Comment: mnemonic: ODB_P_LOCK_TO
// *Cause:
// *Action:
/
03479, 00000, "Sum"
// *Comment: mnemonic: ODB_P_LOCK_SUM
// *Cause:
// *Action:
/
03480, 00000, "Parallel Server Lock Activity Chart"
// *Comment: mnemonic: ODB_P_LOCK_CHART_DEF_NAME
// *Cause:
// *Action:
/
03481, 00000, "Chart of Parallel Server Lock Activity"
// *Comment: mnemonic: ODB_P_LOCK_CHART_DEF_DESC
// *Cause:
// *Action:
/
03482, 00000, "Parallel Server Session"
// *Comment: mnemonic: ODB_P_SESSION_NAME
// *Cause:
// *Action:
/
03483, 00000, "Oracle Parallel Server Session"
// *Comment: mnemonic: ODB_P_SESSION_DESC
// *Cause:
// *Action:
/
03484, 00000, "Instance"
// *Comment: mnemonic: ODB_P_SESSION_INSTANCE
// *Cause:
// *Action:
/
03485, 00000, "Sid"
// *Comment: mnemonic: ODB_P_SESSION_SID
// *Cause:
// *Action:
/
03486, 00000, "Saddr"
// *Comment: mnemonic: ODB_P_SESSION_SADDR
// *Cause:
// *Action:
/
03487, 00000, "Serial"
// *Comment: mnemonic: ODB_P_SESSION_SERIAL
// *Cause:
// *Action:
/
03488, 00000, "Pid"
// *Comment: mnemonic: ODB_P_SESSION_PID
// *Cause:
// *Action:
/
03489, 00000, "Status"
// *Comment: mnemonic: ODB_P_SESSION_STATUS
// *Cause:
// *Action:
/
03490, 00000, "User Name"
// *Comment: mnemonic: ODB_P_SESSION_USERNAME
// *Cause:
// *Action:
/
03491, 00000, "Lock Wait"
// *Comment: mnemonic: ODB_P_SESSION_LOCKWAIT
// *Cause:
// *Action:
/
03492, 00000, "Command"
// *Comment: mnemonic: ODB_P_SESSION_COMMAND
// *Cause:
// *Action:
/
03493, 00000, "Parallel Server Session Chart"
// *Comment: mnemonic: ODB_P_SESSION_CHART_DEF_NAME
// *Cause:
// *Action:
/
03494, 00000, "Chart of Parallel Server Session"
// *Comment: mnemonic: ODB_P_SESSION_CHART_DEF_DESC 
// *Cause:
// *Action:
/
03495, 00000, "Parallel Server Users Logged On"
// *Comment: mnemonic: ODB_P_USER_LOG_NAME
// *Cause:
// *Action:
/
03496, 00000, "Oracle Parallel Server Users Logged On"
// *Comment: mnemonic: ODB_P_USER_LOG_DESC
// *Cause:
// *Action:
/
03497, 00000, "Sum"
// *Comment: mnemonic: ODB_P_USER_LOG_SUM
// *Cause:
// *Action:
/
03498, 00000, "Parallel Server Users Logged On Chart"
// *Comment: mnemonic: ODB_P_USER_LOG_CHART_DEF_NAME
// *Cause:
// *Action:
/
03499, 00000, "Chart of Parallel Server Users Logged On"
// *Comment: mnemonic: ODB_P_USER_LOG_CHART_DEF_DESC
// *Cause:
// *Action:
/
03500, 00000, "Parallel Server Users Logged On By Instance"
// *Comment: mnemonic: ODB_P_USER_LOG_INST_NAME
// *Cause:
// *Action:
/
03501, 00000, "Oracle Parallel Server Users Logged On By Instance"
// *Comment: mnemonic: ODB_P_USER_LOG_INST_DESC
// *Cause:
// *Action:
/
03502, 00000, "Surr"
// *Comment: mnemonic: ODB_P_USER_LOG_INST_SURR
// *Cause:
// *Action:
/
03503, 00000, "Parallel Server Users Logged On By Instance Chart"
// *Comment: mnemonic: ODB_P_USER_LOG_INST_CHART_DEF_NAME
// *Cause:
// *Action:
/
03504, 00000, "Chart of Parallel Server Users Logged On By Instance"
// *Comment: mnemonic: ODB_P_USER_LOG_INST_CHART_DEF_DESC
// *Cause:
// *Action:
/
03505, 00000, "Session Details"
// *Comment: mnemonic: ODB_SESSION_DETL_NAME
// *Cause:
// *Action:
/
03506, 00000, "Oracle Session Details"
// *Comment: mnemonic: ODB_SESSION_DETL_DESC
// *Cause:
// *Action:
/
03507, 00000, "Sid"
// *Comment: mnemonic: ODB_SESSION_DETL_SID
// *Cause:
// *Action:
/
03508, 00000, "Saddr"
// *Comment: mnemonic: ODB_SESSION_DETL_SADDR
// *Cause:
// *Action:
/
03509, 00000, "Serial"
// *Comment: mnemonic: ODB_SESSION_DETL_SERIAL
// *Cause:
// *Action:
/
03510, 00000, "Aud Sid"
// *Comment: mnemonic: ODB_SESSION_DETL_AUDSID
// *Cause:
// *Action:
/
03511, 00000, "Paddr"
// *Comment: mnemonic: ODB_SESSION_DETL_PADDR
// *Cause:
// *Action:
/
03512, 00000, "User"
// *Comment: mnemonic: ODB_SESSION_DETL_USER
// *Cause:
// *Action:
/
03513, 00000, "User Name"
// *Comment: mnemonic: ODB_SESSION_DETL_USRENAME
// *Cause:
// *Action:
/
03514, 00000, "Command"
// *Comment: mnemonic: ODB_SESSION_DETL_COMMAND
// *Cause:
// *Action:
/
03515, 00000, "Taddr"
// *Comment: mnemonic: ODB_SESSION_DETL_TADDR
// *Cause:
// *Action:
/
03516, 00000, "Lock Wait"
// *Comment: mnemonic: ODB_SESSION_DETL_LOCKWAIT
// *Cause:
// *Action:
/
03517, 00000, "Status"
// *Comment: mnemonic: ODB_SESSION_DETL_STATUS
// *Cause:
// *Action:
/
03518, 00000, "Server"
// *Comment: mnemonic: ODB_SESSION_DETL_SERVER
// *Cause:
// *Action:
/
03519, 00000, "Schema"
// *Comment: mnemonic: ODB_SESSION_DETL_SCHEMA
// *Cause:
// *Action:
/
03520, 00000, "Schema Name"
// *Comment: mnemonic: ODB_SESSION_DETL_SCHEMANAME
// *Cause:
// *Action:
/
03521, 00000, "OS User"
// *Comment: mnemonic: ODB_SESSION_DETL_OSUSER
// *Cause:
// *Action:
/
03522, 00000, "Process"
// *Comment: mnemonic: ODB_SESSION_DETL_PROCESS
// *Cause:
// *Action:
/
03523, 00000, "Machine"
// *Comment: mnemonic: ODB_SESSION_DETL_MACHINE
// *Cause:
// *Action:
/
03524, 00000, "Terminal"
// *Comment: mnemonic: ODB_SESSION_DETL_TERMINAL
// *Cause:
// *Action:
/
03525, 00000, "Program"
// *Comment: mnemonic: ODB_SESSION_DETL_PROGRAM
// *Cause:
// *Action:
/
03526, 00000, "Type"
// *Comment: mnemonic: ODB_SESSION_DETL_TYPE
// *Cause:
// *Action:
/
03527, 00000, "Address"
// *Comment: mnemonic: ODB_SESSION_DETL_SQL_ADDRESS
// *Cause:
// *Action:
/
03528, 00000, "SQL Hash Value"
// *Comment: mnemonic: ODB_SESSION_DETL_SQL_HASH_VAL
// *Cause:
// *Action:
/
03529, 00000, "Previous SQL Addr"
// *Comment: mnemonic: ODB_SESSION_DETL_PREV_SQL_ADDR
// *Cause:
// *Action:
/
03530, 00000, "Previous Hash Value"
// *Comment: mnemonic: ODB_SESSION_DETL_PREV_HASH_VAL
// *Cause:
// *Action:
/
03531, 00000, "Module"
// *Comment: mnemonic: ODB_SESSION_DETL_MODULE
// *Cause:
// *Action:
/
03532, 00000, "Module Hash"
// *Comment: mnemonic: ODB_SESSION_DETL_MODULE_HASH
// *Cause:
// *Action:
/
03533, 00000, "Action"
// *Comment: mnemonic: ODB_SESSION_DETL_ACTION
// *Cause:
// *Action:
/
03534, 00000, "Action Hash"
// *Comment: mnemonic: ODB_SESSION_DETL_ACTION_HASH
// *Cause:
// *Action:
/
03535, 00000, "Client Info"
// *Comment: mnemonic: ODB_SESSION_DETL_CLIENT_INFO
// *Cause:
// *Action:
/
03536, 00000, "Fixed Table SEQ"
// *Comment: mnemonic: ODB_SESSION_DETL_FIXED_TBL_SEQ
// *Cause:
// *Action:
/
03537, 00000, "Row Wait Obj"
// *Comment: mnemonic: ODB_SESSION_DETL_ROW_WAIT_OBJ
// *Cause:
// *Action:
/
03538, 00000, "Row Wait File"
// *Comment: mnemonic: ODB_SESSION_DETL_ROW_WAIT_FILE
// *Cause:
// *Action:
/
03539, 00000, "Row Wait Block"
// *Comment: mnemonic: ODB_SESSION_DETL_ROW_WAIT_BLK
// *Cause:
// *Action:
/
03540, 00000, "Row Wait Row"
// *Comment: mnemonic: ODB_SESSION_DETL_ROW_WAIT_ROW
// *Cause:
// *Action:
/
03541, 00000, "Logon Time"
// *Comment: mnemonic: ODB_SESSION_DETL_LOGON_TIME
// *Cause:
// *Action:
/
03542, 00000, "Last Call ET"
// *Comment: mnemonic: ODB_SESSION_DETL_LAST_CALL_ET
// *Cause:
// *Action:
/
03543, 00000, "Session Details Chart"
// *Comment: mnemonic: ODB_SESSION_DETL_CHART_DEF_NAME
// *Cause:
// *Action:
/
03544, 00000, "Chart of Session Details"
// *Comment: mnemonic: ODB_SESSION_DETL_CHART_DEF_DESC
// *Cause:
// *Action:
/
03545, 00000, "Session Statistics"
// *Comment: mnemonic: ODB_SESSION_STAT_NAME
// *Cause:
// *Action:
/
03546, 00000, "Oracle Session Statistics"
// *Comment: mnemonic: ODB_SESSION_STAT_DESC
// *Cause:
// *Action:
/
03547, 00000, "Sid"
// *Comment: mnemonic: ODB_SESSION_STAT_SID
// *Cause:
// *Action:
/
03548, 00000, "Statistics"
// *Comment: mnemonic: ODB_SESSION_STAT_STATISTICS
// *Cause:
// *Action:
/
03549, 00000, "Value"
// *Comment: mnemonic: ODB_SESSION_STAT_VALUE
// *Cause:
// *Action:
/
03550, 00000, "Session Statistics Chart"
// *Comment: mnemonic: ODB_SESSION_STAT_CHART_DEF_NAME
// *Cause:
// *Action:
/
03551, 00000, "Chart of Session Statistics"
// *Comment: mnemonic: ODB_SESSION_STAT_CHART_DEF_DESC
// *Cause:
// *Action:
/
03552, 00000, "Top Sessions"
// *Comment: mnemonic: ODB_TOP_SESSIONS_NAME
// *Cause:
// *Action:
/
03553, 00000, "Statistics on the highest impact sessions for some criterion"
// *Comment: mnemonic: ODB_TOP_SESSIONS_DESC
// *Cause:
// *Action:
/
03554, 00000, "Session ID"
// *Comment: mnemonic: ODB_TOP_SESSIONS_SID
// *Cause:
// *Action:
/
03555, 00000, "Session Address"
// *Comment: mnemonic: ODB_TOP_SESSIONS_SADDR
// *Cause:
// *Action:
/
03556, 00000, "Session Serial Number"
// *Comment: mnemonic: ODB_TOP_SESSIONS_SERIAL
// *Cause:
// *Action:
/
03557, 00000, "Process ID"
// *Comment: mnemonic: ODB_TOP_SESSIONS_PID
// *Cause:
// *Action:
/
03558, 00000, "Session Status"
// *Comment: mnemonic: ODB_TOP_SESSIONS_STATUS
// *Cause:
// *Action:
/
03559, 00000, "Username"
// *Comment: mnemonic: ODB_TOP_SESSIONS_USERNAME
// *Cause:
// *Action:
/
03560, 00000, "Address of Lock Waiting For"
// *Comment: mnemonic: ODB_TOP_SESSIONS_LOCKWAIT
// *Cause:
// *Action:
/
03561, 00000, "Command In Progress"
// *Comment: mnemonic: ODB_TOP_SESSIONS_COMMAND
// *Cause:
// *Action:
/
03562, 00000, "Top Sessions Chart"
// *Comment: mnemonic: ODB_TOP_SESSIONS_CHART_DEF_NAME
// *Cause:
// *Action:
/
03563, 00000, "Chart of Topmost Resource Intensive Sessions"
// *Comment: mnemonic: ODB_TOP_SESSIONS_CHART_DEF_DESC
// *Cause:
// *Action:
/
03564, 00000, "Top SQL"
// *Comment: mnemonic: ODB_TOP_SQL_NAME
// *Cause:
// *Action:
/
03565, 00000, "Statistics on the worst SQL for some criterion"
// *Comment: mnemonic: ODB_TOP_SQL_DESC
// *Cause:
// *Action:
/
03566, 00000, "SQL Text"
// *Comment: mnemonic: ODB_TOP_SQL_TEXT
// *Cause:
// *Action:
/
03567, 00000, "Version count"
// *Comment: mnemonic: ODB_TOP_SQL_VERSION
// *Cause:
// *Action:
/
03568, 00000, "Total Sharable Memory"
// *Comment: mnemonic: ODB_TOP_SQL_SHARABLE
// *Cause:
// *Action:
/
03569, 00000, "Total Persistent Memory"
// *Comment: mnemonic: ODB_TOP_SQL_PERSISTENT
// *Cause:
// *Action:
/
03570, 00000, "Total Runtime Memory"
// *Comment: mnemonic: ODB_TOP_SQL_RUNTIME
// *Cause:
// *Action:
/
03571, 00000, "Top SQL Chart"
// *Comment: mnemonic: ODB_TOP_SQL_CHART_DEF_NAME
// *Cause:
// *Action:
/
03572, 00000, "Chart of Topmost Resource Intensive SQL"
// *Comment: mnemonic: ODB_TOP_SQL_CHART_DEF_DESC
// *Cause:
// *Action:
/
03573, 00000, "Oracle Parallel Server Overview"
// *Comment: mnemonic: ODB_OPS_OVERVIEW_NAME
// *Cause:
// *Action:
/
03574, 00000, "Overview of Oracle Parallel Server Performance"
// *Comment: mnemonic: ODB_OPS_OVERVIEW_DESC
// *Cause:
// *Action:
/
03575, 00000, "OPS Overview Chart"
// *Comment: mnemonic: ODB_OPS_OVERVIEW_CHART_DEF_NAME
// *Cause:
// *Action:
/
03576, 00000, "Overview Chart of Oracle Parallel Server Performance"
// *Comment: mnemonic: ODB_OPS_OVERVIEW_CHART_DEF_DESC
// *Cause:
// *Action:
/
03577, 00000, "OPS Block Ping Tablespace Object Statistics"
// *Comment: mnemonic: ODB_OPS_BLKPING_TS_OBJ_DEF_NAME
// *Cause:
// *Action:
/
03578, 00000, "OPS Block Ping Tablespace Object Data"
// *Comment: mnemonic: ODB_OPS_BLKPING_TS_OBJ_DEF_DESC
// *Cause:
// *Action:
/
03579, 00000, "Name"
// *Comment: mnemonic: ODB_OPS_BLKPING_TS_OBJ_NAME
// *Cause:
// *Action:
/
03580, 00000, "Value"
// *Comment: mnemonic: ODB_OPS_BLKPING_TS_OBJ_VAL
// *Cause:
// *Action:
/
03581, 00000, "OPS Block Ping Tablespace Object Chart"
// *Comment: mnemonic: ODB_OPS_BLKPING_TS_OBJ_CHART_DEF_NAME
// *Cause:
// *Action:
/
03582, 00000, "Chart of OPS Block Ping Tablespace Object"
// *Comment: mnemonic: ODB_OPS_BLKPING_TS_OBJ_CHART_DEF_DESC
// *Cause:
// *Action:
/
03583, 00000, "OPS Block Ping Tablespace Instance Statistics"
// *Comment: mnemonic: ODB_OPS_BLKPING_TS_INST_NAME
// *Cause:
// *Action:
/
03584, 00000, "OPS Block Ping Tablespace Instance Data"
// *Comment: mnemonic: ODB_OPS_BLKPING_TS_INST_DESC
// *Cause:
// *Action:
/
03585, 00000, "Name"
// *Comment: mnemonic: ODB_OPS_BLKPING_TS_INST_INAME
// *Cause:
// *Action:
/
03586, 00000, "Value"
// *Comment: mnemonic: ODB_OPS_BLKPING_TS_INST_VAL
// *Cause:
// *Action:
/
03587, 00000, "OPS Block Ping Tablespace Instance Chart"
// *Comment: mnemonic: ODB_OPS_BLKPING_TS_INST_CHART_DEF_NAME
// *Cause:
// *Action:
/
03588, 00000, "Chart of OPS Block Ping Tablespace Instance"
// *Comment: mnemonic: ODB_OPS_BLKPING_TS_INST_CHART_DEF_DESC
// *Cause:
// *Action:
/
03589, 00000, "OPS File I/O Object Instance Statistics"
// *Comment: mnemonic: ODB_OPS_FILEIO_OBJ_INST_NAME
// *Cause:
// *Action:
/
03590, 00000, "OPS File I/O Object Instance Data"
// *Comment: mnemonic: ODB_OPS_FILEIO_OBJ_INST_DESC
// *Cause:
// *Action:
/
03591, 00000, "Name"
// *Comment: mnemonic: ODB_OPS_FILEIO_OBJ_INST_INAME
// *Cause:
// *Action:
/
03592, 00000, "Physical Reads/Sec"
// *Comment: mnemonic: ODB_OPS_FILEIO_OBJ_INST_PHYRDS
// *Cause:
// *Action:
/
03593, 00000, "Physical Writes/Sec"
// *Comment: mnemonic: ODB_OPS_FILEIO_OBJ_INST_PHYWRTS
// *Cause:
// *Action:
/
03594, 00000, "Physical Block Reads/Sec"
// *Comment: mnemonic: ODB_OPS_FILEIO_OBJ_INST_PHYBLKRDS
// *Cause:
// *Action:
/
03595, 00000, "Physical Block Writes/Sec"
// *Comment: mnemonic: ODB_OPS_FILEIO_OBJ_INST_PHYBLKWRTS
// *Cause:
// *Action:
/
03596, 00000, "OPS File I/O Object Instance Chart"
// *Comment: mnemonic: ODB_OPS_FILEIO_OBJ_INST_CHART_DEF_NAME
// *Cause:
// *Action:
/
03597, 00000, "Chart of OPS File I/O Object Instance"
// *Comment: mnemonic: ODB_OPS_FILEIO_OBJ_INST_CHART_DEF_DESC
// *Cause:
// *Action:
/
03598, 00000, "OPS Lock Activity Instance Statistics"
// *Comment: mnemonic: ODB_OPS_LOCK_ACT_INST_NAME
// *Cause:
// *Action:
/
03599, 00000, "OPS Lock Activity Instance Data"
// *Comment: mnemonic: ODB_OPS_LOCK_ACT_INST_DESC
// *Cause:
// *Action:
/
03600, 00000, "Name"
// *Comment: mnemonic: ODB_OPS_LOCK_ACT_INST_INAME
// *Cause:
// *Action:
/
03601, 00000, "Value"
// *Comment: mnemonic: ODB_OPS_LOCK_ACT_INST_VAL
// *Cause:
// *Action:
/
03602, 00000, "OPS Lock Activity Instance Chart"
// *Comment: mnemonic: ODB_OPS_LOCK_ACT_INST_CHART_DEF_NAME
// *Cause:
// *Action:
/
03603, 00000, "Chart of OPS Lock Activity Instance"
// *Comment: mnemonic: ODB_OPS_LOCK_ACT_INST_CHART_DEF_DESC
// *Cause:
// *Action:
/
03604, 00000, "Oracle Parallel Server Dynamic Drilldown Statistics"
// *Comment: mnemonic: ODB_OPS_DYNAMIC_NAME
// *Cause:
// *Action:
/
03605, 00000, "Oracle Parallel Server Dynamic Drilldown Data"
// *Comment: mnemonic: ODB_OPS_DYNAMIC_DESC
// *Cause:
// *Action:
/
03606, 00000, "Value"
// *Comment: mnemonic: ODB_OPS_DYNAMIC_VAL
// *Cause:
// *Action:
/
03607, 00000, "OPS Dynamic Drilldown Chart"
// *Comment: mnemonic: ODB_OPS_DYNAMIC_CHART_DEF_NAME
// *Cause:
// *Action:
/
03608, 00000, "Chart for OPS Dynamic Drilldown"
// *Comment: mnemonic: ODB_OPS_DYNAMIC_CHART_DEF_DESC
// *Cause:
// *Action:
/
03609, 00000, "Oracle Database Performance Statistics"
// *Comment: mnemonic: ODB_MODULE_EXTNAME
// *Cause:
// *Action:
/
03610, 00000, "Columns returned by the SQL statement will be building blocks for the user-defined chart's resources"
// *Comment: mnemonic: ODB_SCRIPT_DESC
// *Cause:
// *Action:
/
03611, 00000, "SQL"
// *Comment: mnemonic: ODB_SCRIPT_TYPE
// *Cause:
// *Action:
/
03612, 00000, "Oracle Server Instance"
// *Comment: mnemonic: ODB_SERVER_INSTDESC
// *Cause:
// *Action:
/
03613, 00000, "Tables in the current schema"
// *Comment: mnemonic: ODB_TABLE_INSTDESC
// *Cause:
// *Action:
/
03614, 00000, "Sort Criterion"
// *Comment: mnemonic: ODB_TOP_SESSIONS_SORTSTAT
// *Cause:
// *Action:
/
03615, 00000, "Session UGA Memory"
// *Comment: mnemonic: ODB_TOP_SESSIONS_UGA
// *Cause:
// *Action:
/
03616, 00000, "Session PGA Memory"
// *Comment: mnemonic: ODB_TOP_SESSIONS_PGA
// *Cause:
// *Action:
/
03617, 00000, "CPU Used by this Session"
// *Comment: mnemonic: ODB_TOP_SESSIONS_CPU
// *Cause:
// *Action:
/
03618, 00000, "Physical Reads"
// *Comment: mnemonic: ODB_TOP_SESSIONS_PHYRDS
// *Cause:
// *Action:
/
03619, 00000, "Physical Writes"
// *Comment: mnemonic: ODB_TOP_SESSIONS_PHYWRTS
// *Cause:
// *Action:
/
03620, 00000, "Open Cursors (Current)"
// *Comment: mnemonic: ODB_TOP_SESSIONS_CURSOR
// *Cause:
// *Action:
/
03621, 00000, "User Commits"
// *Comment: mnemonic: ODB_TOP_SESSIONS_COMMITS
// *Cause:
// *Action:
/
03622, 00000, "User Rollbacks"
// *Comment: mnemonic: ODB_TOP_SESSIONS_ROLLBACKS
// *Cause:
// *Action:
/
03623, 00000, "Session Details Data"
// *Comment: mnemonic: ODB_SESDET_NAME
// *Cause:
// *Action:
/
03624, 00000, "Details of an Oracle Database Session"
// *Comment: mnemonic: ODB_SESDET_DESC
// *Cause:
// *Action:
/
03625, 00000, "Oracle Database Session"
// *Comment: mnemonic: ODB_SESSION_INSTDESC
// *Cause:
// *Action:
/
03626, 00000, "Sid"
// *Comment: mnemonic: ODB_SESDET_SID
// *Cause:
// *Action:
/
03627, 00000, "Saddr"
// *Comment: mnemonic: ODB_SESDET_SADDR
// *Cause:
// *Action:
/
03628, 00000, "Serial"
// *Comment: mnemonic: ODB_SESDET_SERIAL
// *Cause:
// *Action:
/
03629, 00000, "Aud Sid"
// *Comment: mnemonic: ODB_SESDET_AUDSID
// *Cause:
// *Action:
/
03630, 00000, "Paddr"
// *Comment: mnemonic: ODB_SESDET_PADDR
// *Cause:
// *Action:
/
03631, 00000, "User Number"
// *Comment: mnemonic: ODB_SESDET_USER
// *Cause:
// *Action:
/
03632, 00000, "User Name"
// *Comment: mnemonic: ODB_SESDET_USERNAME
// *Cause:
// *Action:
/
03633, 00000, "Command"
// *Comment: mnemonic: ODB_SESDET_COMMAND
// *Cause:
// *Action:
/
03634, 00000, "Taddr"
// *Comment: mnemonic: ODB_SESDET_TADDR
// *Cause:
// *Action:
/
03635, 00000, "Lockwait"
// *Comment: mnemonic: ODB_SESDET_LOCKWAIT
// *Cause:
// *Action:
/
03636, 00000, "Status"
// *Comment: mnemonic: ODB_SESDET_STATUS
// *Cause:
// *Action:
/
03637, 00000, "Server"
// *Comment: mnemonic: ODB_SESDET_SERVER
// *Cause:
// *Action:
/
03638, 00000, "Schema Number"
// *Comment: mnemonic: ODB_SESDET_SCHEMA
// *Cause:
// *Action:
/
03639, 00000, "Schema Name"
// *Comment: mnemonic: ODB_SESDET_SCHEMANAME
// *Cause:
// *Action:
/
03640, 00000, "OS User"
// *Comment: mnemonic: ODB_SESDET_OSUSER
// *Cause:
// *Action:
/
03641, 00000, "Process"
// *Comment: mnemonic: ODB_SESDET_PROCESS
// *Cause:
// *Action:
/
03642, 00000, "Machine"
// *Comment: mnemonic: ODB_SESDET_MACHINE
// *Cause:
// *Action:
/
03643, 00000, "Terminal"
// *Comment: mnemonic: ODB_SESDET_TERMINAL
// *Cause:
// *Action:
/
03644, 00000, "Program"
// *Comment: mnemonic: ODB_SESDET_PROGRAM
// *Cause:
// *Action:
/
03645, 00000, "Type"
// *Comment: mnemonic: ODB_SESDET_TYPE
// *Cause:
// *Action:
/
03646, 00000, "SQL Address"
// *Comment: mnemonic: ODB_SESDET_SQLADDR
// *Cause:
// *Action:
/
03647, 00000, "SQL Hash Value"
// *Comment: mnemonic: ODB_SESDET_SQLHASH
// *Cause:
// *Action:
/
03648, 00000, "Previous SQL Address"
// *Comment: mnemonic: ODB_SESDET_PREV_SQLADDR
// *Cause:
// *Action:
/
03649, 00000, "Previous SQL Hash Value"
// *Comment: mnemonic: ODB_SESDET_PREV_SQLHASH
// *Cause:
// *Action:
/
03650, 00000, "Module"
// *Comment: mnemonic: ODB_SESDET_MODULE
// *Cause:
// *Action:
/
03651, 00000, "Module Hash"
// *Comment: mnemonic: ODB_SESDET_MODULEHASH
// *Cause:
// *Action:
/
03652, 00000, "Action"
// *Comment: mnemonic: ODB_SESDET_ACTION
// *Cause:
// *Action:
/
03653, 00000, "Action Hash"
// *Comment: mnemonic: ODB_SESDET_ACTIONHASH
// *Cause:
// *Action:
/
03654, 00000, "Client Information"
// *Comment: mnemonic: ODB_SESDET_CLIENTINFO
// *Cause:
// *Action:
/
03655, 00000, "Fixed Table Sequence"
// *Comment: mnemonic: ODB_SESDET_FIXEDTABLESEQ
// *Cause:
// *Action:
/
03656, 00000, "Row Wait Object"
// *Comment: mnemonic: ODB_SESDET_ROWWAIT_OBJ
// *Cause:
// *Action:
/
03657, 00000, "Row Wait File"
// *Comment: mnemonic: ODB_SESDET_ROWWAIT_FILE
// *Cause:
// *Action:
/
03658, 00000, "Row Wait Block"
// *Comment: mnemonic: ODB_SESDET_ROWWAIT_BLOCK
// *Cause:
// *Action:
/
03659, 00000, "Row Wait Row"
// *Comment: mnemonic: ODB_SESDET_ROWWAIT_ROW
// *Cause:
// *Action:
/
03660, 00000, "Logon Time"
// *Comment: mnemonic: ODB_SESDET_LOGONTIME
// *Cause:
// *Action:
/
03661, 00000, "Last Call ET"
// *Comment: mnemonic: ODB_SESDET_LASTCALLET
// *Cause:
// *Action:
/
03662, 00000, "Session Details Chart"
// *Comment: mnemonic: ODB_SESDET_CHART_DEF_NAME
// *Cause:
// *Action:
/
03663, 00000, "Chart of Session Details"
// *Comment: mnemonic: ODB_SESDET_CHART_DEF_DESC
// *Cause:
// *Action:
/
03664, 00000, "Session Statistics Data"
// *Comment: mnemonic: ODB_SESSTAT_NAME
// *Cause:
// *Action:
/
03665, 00000, "Statistics for Oracle Database Sessions"
// *Comment: mnemonic: ODB_SESSTAT_DESC
// *Cause:
// *Action:
/
03666, 00000, "Sid"
// *Comment: mnemonic: ODB_SESSTAT_SID
// *Cause:
// *Action:
/
03667, 00000, "Name"
// *Comment: mnemonic: ODB_SESSTAT_STATNAME
// *Cause:
// *Action:
/
03668, 00000, "Value"
// *Comment: mnemonic: ODB_SESSTAT_VALUE
// *Cause:
// *Action:
/
03669, 00000, "Session Statistics Chart"
// *Comment: mnemonic: ODB_SESSTAT_CHART_DEF_NAME
// *Cause:
// *Action:
/
03670, 00000, "Chart of Session Statistics"
// *Comment: mnemonic: ODB_SESSTAT_CHART_DEF_DESC
// *Cause:
// *Action:
/
03671, 00000, "Physical Reads"
// *Comment: mnemonic: ODB_TOP_SQL_PHYRDS
// *Cause:
// *Action:
/
03672, 00000, "Buffer Gets"
// *Comment: mnemonic: ODB_TOP_SQL_BUFGETS
// *Cause:
// *Action:
/
03673, 00000, "Rows Processed"
// *Comment: mnemonic: ODB_TOP_SQL_ROWS
// *Cause:
// *Action:
/
03674, 00000, "Memory Sort Percentage Value Extrapolation"
// *Comment: mnemonic: ODB_SORT_MEM_SORT_PCT_VAL_ANAL_NAME
// *Cause:
// *Action:
/
03675, 00000, "At what time will the Memory Sort Percentage reach the specified value"
// *Comment: mnemonic: ODB_SORT_MEM_SORT_PCT_VAL_ANAL_DESC
// *Cause:
// *Action:
/
03676, 00000, "Memory Sort Percentage Time Extrapolation"
// *Comment: mnemonic: ODB_SORT_MEM_SORT_PCT_TIME_ANAL_NAME
// *Cause:
// *Action:
/
03677, 00000, "What will be the Memory Sort Percentage at the specified time"
// *Comment: mnemonic: ODB_SORT_MEM_SORT_PCT_TIME_ANAL_DESC
// *Cause:
// *Action:
/
03678, 00000, "Buffer Cache Hit Percentage Value Extrapolation"
// *Comment: mnemonic: ODB_MEMORY_BUFCACHE_HR_VAL_ANAL_NAME
// *Cause:
// *Action:
/
03679, 00000, "At what time will the Buffer Cache Hit Percentage reach the specified value"
// *Comment: mnemonic: ODB_MEMORY_BUFCACHE_HR_VAL_ANAL_DESC
// *Cause:
// *Action:
/
03680, 00000, "Buffer Cache Hit Percentage Time Extrapolation"
// *Comment: mnemonic: ODB_MEMORY_BUFCACHE_HR_TIME_ANAL_NAME
// *Cause:
// *Action:
/
03681, 00000, "What will be the Buffer Cache Hit Percentage at the specified time"
// *Comment: mnemonic: ODB_MEMORY_BUFCACHE_HR_TIME_ANAL_DESC
// *Cause:
// *Action:
/
03682, 00000, "Shared Pool Allocation Hit Percentage Value Extrapolation"
// *Comment: mnemonic: ODB_MEMORY_SHPL_ALLOC_VAL_ANAL_NAME
// *Cause:
// *Action:
/
03683, 00000, "At what time will the Shared Pool Allocation Hit Percentage reach the specified value"
// *Comment: mnemonic: ODB_MEMORY_SHPL_ALLOC_VAL_ANAL_DESC
// *Cause:
// *Action:
/
03684, 00000, "Shared Pool Allocation Hit Percentage Time Extrapolation"
// *Comment: mnemonic: ODB_MEMORY_SHPL_ALLOC_TIME_ANAL_NAME
// *Cause:
// *Action:
/
03685, 00000, "What will be the Shared Pool Allocation Hit Percentage at the specified time"
// *Comment: mnemonic: ODB_MEMORY_SHPL_ALLOC_TIME_ANAL_DESC
// *Cause:
// *Action:
/
03686, 00000, "Redo Buffer Allocation Hit Percentage Value Extrapolation"
// *Comment: mnemonic: ODB_MEMORY_REDO_ALLOC_VAL_ANAL_NAME
// *Cause:
// *Action:
/
03687, 00000, "At what time will the Redo Buffer Allocation Hit Percentage reach the specified value"
// *Comment: mnemonic: ODB_MEMORY_REDO_ALLOC_VAL_ANAL_DESC
// *Cause:
// *Action:
/
03688, 00000, "Redo Buffer Allocation Hit Percentage Time Extrapolation"
// *Comment: mnemonic: ODB_MEMORY_REDO_ALLOC_TIME_ANAL_NAME
// *Cause:
// *Action:
/
03689, 00000, "What will be the Redo Buffer Allocation Hit Percentage at the specified time"
// *Comment: mnemonic: ODB_MEMORY_REDO_ALLOC_TIME_ANAL_DESC
// *Cause:
// *Action:
/
03690, 00000, "Parallel Query Servers Busy Value Extrapolation"
// *Comment: mnemonic: ODB_PQO_SERVBUSY_VAL_ANAL_NAME
// *Cause:
// *Action:
/
03691, 00000, "At what time will the POQ Servers Busy reach the specified value"
// *Comment: mnemonic: ODB_PQO_SERVBUSY_VAL_ANAL_DESC
// *Cause:
// *Action:
/
03692, 00000, "Parallel Query Servers Busy Time Extrapolation"
// *Comment: mnemonic: ODB_PQO_SERVBUSY_TIME_ANAL_NAME
// *Cause:
// *Action:
/
03693, 00000, "How many PQO Servers will be Busy at the specified time"
// *Comment: mnemonic: ODB_PQO_SERVBUSY_TIME_ANAL_DESC
// *Cause:
// *Action:
/
03694, 00000, "Busy Shared Server Processes Value Extrapolation"
// *Comment: mnemonic: ODB_MTS_SERVBUSY_VAL_ANAL_NAME
// *Cause:
// *Action:
/
03695, 00000, "At what time will the MTS Shared Servers Busy reach the specified value"
// *Comment: mnemonic: ODB_MTS_SERVBUSY_VAL_ANAL_DESC
// *Cause:
// *Action:
/
03696, 00000, "Busy Shared Server Processes Time Extrapolation"
// *Comment: mnemonic: ODB_MTS_SERVBUSY_TIME_ANAL_NAME
// *Cause:
// *Action:
/
03697, 00000, "How many Busy Shared Server Processes will there be at the specified time"
// *Comment: mnemonic: ODB_MTS_SERVBUSY_TIME_ANAL_DESC
// *Cause:
// *Action:
/
03698, 00000, "Free Buffer Hit Percentage Value Extrapolation"
// *Comment: mnemonic: ODB_DB_IO_FREEBUFWAIT_VAL_ANAL_NAME
// *Cause:
// *Action:
/
03699, 00000, "At what time will the Free Buffer Hit Percentage reach the specified value"
// *Comment: mnemonic: ODB_DB_IO_FREEBUFWAIT_VAL_ANAL_DESC
// *Cause:
// *Action:
/
03700, 00000, "Free Buffer Hit Percentage Time Extrapolation"
// *Comment: mnemonic: ODB_DB_IO_FREEBUFWAIT_TIME_ANAL_NAME
// *Cause:
// *Action:
/
03701, 00000, "What will be the Free Buffer Hit Percentage at the specified time"
// *Comment: mnemonic: ODB_DB_IO_FREEBUFWAIT_TIME_ANAL_DESC
// *Cause:
// *Action:
/
03702, 00000, "CPU Wait Time Value Extrapolation"
// *Comment: mnemonic: ODB_WAIT_CPU_VAL_ANAL_NAME
// *Cause:
// *Action:
/
03703, 00000, "At what time will the CPU Wait Time reach the specified value"
// *Comment: mnemonic: ODB_WAIT_CPU_VAL_ANAL_DESC
// *Cause:
// *Action:
/
03704, 00000, "CPU Wait Time Time Extrapolation"
// *Comment: mnemonic: ODB_WAIT_CPU_TIME_ANAL_NAME
// *Cause:
// *Action:
/
03705, 00000, "How much time for CPU Waits will have passed by the specified time"
// *Comment: mnemonic: ODB_WAIT_CPU_TIME_ANAL_DESC
// *Cause:
// *Action:
/
03706, 00000, "Total I/O Waits Value Extrapolation"
// *Comment: mnemonic: ODB_WAIT_IO_VAL_ANAL_NAME
// *Cause:
// *Action:
/
03707, 00000, "At what time will the Total I/O Waits reach the specified value"
// *Comment: mnemonic: ODB_WAIT_IO_VAL_ANAL_DESC
// *Cause:
// *Action:
/
03708, 00000, "Total I/O Waits Time Extrapolation"
// *Comment: mnemonic: ODB_WAIT_IO_TIME_ANAL_NAME
// *Cause:
// *Action:
/
03709, 00000, "How many I/O Waits will have occurred by the specified time"
// *Comment: mnemonic: ODB_WAIT_IO_TIME_ANAL_DESC
// *Cause:
// *Action:
/
03710, 00000, "Total Latch Free Waits Value Extrapolation"
// *Comment: mnemonic: ODB_WAIT_LATCHFREE_VAL_ANAL_NAME
// *Cause:
// *Action:
/
03711, 00000, "At what time will the Total Latch Free Waits reach the specified value"
// *Comment: mnemonic: ODB_WAIT_LATCHFREE_VAL_ANAL_DESC
// *Cause:
// *Action:
/
03712, 00000, "Total Latch Free Waits Time Extrapolation"
// *Comment: mnemonic: ODB_WAIT_LATCHFREE_TIME_ANAL_NAME
// *Cause:
// *Action:
/
03713, 00000, "How many Latch Free Waits will have occurred by the specified time"
// *Comment: mnemonic: ODB_WAIT_LATCHFREE_TIME_ANAL_DESC
// *Cause:
// *Action:
/
03714, 00000, "Enqueue Waits Value Extrapolation"
// *Comment: mnemonic: ODB_WAIT_ENQUEUE_VAL_ANAL_NAME
// *Cause:
// *Action:
/
03715, 00000, "At what time will the Enqueue Waits reach the specified value"
// *Comment: mnemonic: ODB_WAIT_ENQUEUE_VAL_ANAL_DESC
// *Cause:
// *Action:
/
03716, 00000, "Enqueue Waits Time Extrapolation"
// *Comment: mnemonic: ODB_WAIT_ENQUEUE_TIME_ANAL_NAME
// *Cause:
// *Action:
/
03717, 00000, "How many Enqueue Waits will have occurred by the specified time"
// *Comment: mnemonic: ODB_WAIT_ENQUEUE_TIME_ANAL_DESC
// *Cause:
// *Action:
/
03718, 00000, "Total Buffer Busy Waits Value Extrapolation"
// *Comment: mnemonic: ODB_WAIT_BUFBUSY_VAL_ANAL_NAME
// *Cause:
// *Action:
/
03719, 00000, "At what time will the Total Buffer Busy Waits reach the specified value"
// *Comment: mnemonic: ODB_WAIT_BUFBUSY_VAL_ANAL_DESC
// *Cause:
// *Action:
/
03720, 00000, "Total Buffer Busy Waits Time Extrapolation"
// *Comment: mnemonic: ODB_WAIT_BUFBUSY_TIME_ANAL_NAME
// *Cause:
// *Action:
/
03721, 00000, "How many Buffer Busy Waits will have occurred by the specified time"
// *Comment: mnemonic: ODB_WAIT_BUFBUSY_TIME_ANAL_DESC
// *Cause:
// *Action:
/
03722, 00000, "Total Free Buffer Waits Value Extrapolation"
// *Comment: mnemonic: ODB_WAIT_FREEBUF_VAL_ANAL_NAME
// *Cause:
// *Action:
/
03723, 00000, "At what time will the Total Free Buffer Waits reach the specified value"
// *Comment: mnemonic: ODB_WAIT_FREEBUF_VAL_ANAL_DESC
// *Cause:
// *Action:
/
03724, 00000, "Total Free Buffer Waits Time Extrapolation"
// *Comment: mnemonic: ODB_WAIT_FREEBUF_TIME_ANAL_NAME
// *Cause:
// *Action:
/
03725, 00000, "How many Total Free Buffer Waits will have occurred by the specified time"
// *Comment: mnemonic: ODB_WAIT_FREEBUF_TIME_ANAL_DESC
// *Cause:
// *Action:
/
03726, 00000, "Top Files Physical Block Reads Value Extrapolation"
// *Comment: mnemonic: ODB_TOP_FILES_PHYBLKRDS_VAL_ANAL_NAME
// *Cause:
// *Action:
/
03727, 00000, "At what time will the Physical Block Reads reach the specified value"
// *Comment: mnemonic: ODB_TOP_FILES_PHYBLKRDS_VAL_ANAL_DESC
// *Cause:
// *Action:
/
03728, 00000, "Top Files Physical Block Reads Time Extrapolation"
// *Comment: mnemonic: ODB_TOP_FILES_PHYBLKRDS_TIME_ANAL_NAME
// *Cause:
// *Action:
/
03729, 00000, "How many Physical Block Reads will there be by the specified time"
// *Comment: mnemonic: ODB_TOP_FILES_PHYBLKRDS_TIME_ANAL_DESC
// *Cause:
// *Action:
/
03730, 00000, "Top Files Physical Block Writes Value Extrapolation"
// *Comment: mnemonic: ODB_TOP_FILES_PHYBLKWRTS_VAL_ANAL_NAME
// *Cause:
// *Action:
/
03731, 00000, "At what time will the Physical Block Writes reach the specified value"
// *Comment: mnemonic: ODB_TOP_FILES_PHYBLKWRTS_VAL_ANAL_DESC
// *Cause:
// *Action:
/
03732, 00000, "Top Files Physical Block Writes Time Extrapolation"
// *Comment: mnemonic: ODB_TOP_FILES_PHYBLKWRTS_TIME_ANAL_NAME
// *Cause:
// *Action:
/
03733, 00000, "How many Physical Block Writes will there be by the specified time"
// *Comment: mnemonic: ODB_TOP_FILES_PHYBLKWRTS_TIME_ANAL_DESC
// *Cause:
// *Action:
/
03734, 00000, "SQL*Net Wait Rate Value Extrapolation"
// *Comment: mnemonic: ODB_NET_IOS_VAL_ANAL_NAME
// *Cause:
// *Action:
/
03735, 00000, "At what time will the SQL*Net Wait Rate reach the specified value"
// *Comment: mnemonic: ODB_NET_IOS_VAL_ANAL_DESC
// *Cause:
// *Action:
/
03736, 00000, "SQL*Net Wait Rate Time Extrapolation"
// *Comment: mnemonic: ODB_NET_IOS_TIME_ANAL_NAME
// *Cause:
// *Action:
/
03737, 00000, "What will be the SQL*Net Wait Rate at the specified time"
// *Comment: mnemonic: ODB_NET_IOS_TIME_ANAL_DESC
// *Cause:
// *Action:
/
03738, 00000, "Free List Hit Percentage Value Extrapolation"
// *Comment: mnemonic: ODB_FREELIST_HR_VAL_ANAL_NAME
// *Cause:
// *Action:
/
03739, 00000, "At what time will the Free List Hit Percentage reach the specified value"
// *Comment: mnemonic: ODB_FREELIST_HR_VAL_ANAL_DESC
// *Cause:
// *Action:
/
03740, 00000, "Free List Hit Percentage Time Extrapolation"
// *Comment: mnemonic: ODB_FREELIST_HR_TIME_ANAL_NAME
// *Cause:
// *Action:
/
03741, 00000, "What will be the Free List Hit Percentage at the specified time"
// *Comment: mnemonic: ODB_FREELIST_HR_TIME_ANAL_DESC
// *Cause:
// *Action:
/
03742, 00000, "Queue Average Waits Value Extrapolation"
// *Comment: mnemonic: ODB_QUEUE_AVGWAIT_VAL_ANAL_NAME
// *Cause:
// *Action:
/
03743, 00000, "At what time will the Queue Average Waits reach the specified value"
// *Comment: mnemonic: ODB_QUEUE_AVGWAIT_VAL_ANAL_DESC
// *Cause:
// *Action:
/
03744, 00000, "Queue Average Waits Time Extrapolation"
// *Comment: mnemonic: ODB_QUEUE_AVGWAIT_TIME_ANAL_NAME
// *Cause:
// *Action:
/
03745, 00000, "What will be the Average Queue Waits at the specified time"
// *Comment: mnemonic: ODB_QUEUE_AVGWAIT_TIME_ANAL_DESC
// *Cause:
// *Action:
/
03746, 00000, "Rollback (Nowait) Hit Percentage Value Extrapolation"
// *Comment: mnemonic: ODB_RBNOWAIT_HR_VAL_ANAL_NAME
// *Cause:
// *Action:
/
03747, 00000, "At what time will the Rollback (Nowait) Hit Percentage reach the specified value"
// *Comment: mnemonic: ODB_RBNOWAIT_HR_VAL_ANAL_DESC
// *Cause:
// *Action:
/
03748, 00000, "Rollback (Nowait) Hit Percentage Time Extrapolation"
// *Comment: mnemonic: ODB_RBNOWAIT_HR_TIME_ANAL_NAME
// *Cause:
// *Action:
/
03749, 00000, "What will be the Rollback (Nowait) Hit Percentage at the specified time"
// *Comment: mnemonic: ODB_RBNOWAIT_HR_TIME_ANAL_DESC
// *Cause:
// *Action:
/
03750, 00000, "Total Tablespace Quota Blocks Value Extrapolation"
// *Comment: mnemonic: ODB_TABLESPACE_QUOTA_VAL_ANAL_NAME
// *Cause:
// *Action:
/
03751, 00000, "At what time will the Total Tablespace Quota Blocks reach the specified value"
// *Comment: mnemonic: ODB_TABLESPACE_QUOTA_VAL_ANAL_DESC
// *Cause:
// *Action:
/
03752, 00000, "Total Tablespace Quota Blocks Time Extrapolation"
// *Comment: mnemonic: ODB_TABLESPACE_QUOTA_TIME_ANAL_NAME
// *Cause:
// *Action:
/
03753, 00000, "What will be the Total Tablespace Quota Blocks at the specified time"
// *Comment: mnemonic: ODB_TABLESPACE_QUOTA_TIME_ANAL_DESC
// *Cause:
// *Action:
/
03754, 00000, "Users Waiting For Locks Value Extrapolation"
// *Comment: mnemonic: ODB_USERS_WAITLOCK_VAL_ANAL_NAME
// *Cause:
// *Action:
/
03755, 00000, "At what time will the Users Waiting for Locks reach the specified value"
// *Comment: mnemonic: ODB_USERS_WAITLOCK_VAL_ANAL_DESC
// *Cause:
// *Action:
/
03756, 00000, "Users Waiting For Locks Time Extrapolation"
// *Comment: mnemonic: ODB_USERS_WAITLOCK_TIME_ANAL_NAME
// *Cause:
// *Action:
/
03757, 00000, "How many Users will be Waiting for Locks at the specified time"
// *Comment: mnemonic: ODB_USERS_WAITLOCK_TIME_ANAL_DESC
// *Cause:
// *Action:
/
03758, 00000, "Transaction Throughput Rate Value Extrapolation"
// *Comment: mnemonic: ODB_TXN_THRUPUT_VAL_ANAL_NAME
// *Cause:
// *Action:
/
03759, 00000, "At what time will the Transaction Throughput Rate reach the specified value"
// *Comment: mnemonic: ODB_TXN_THRUPUT_VAL_ANAL_DESC
// *Cause:
// *Action:
/
03760, 00000, "Transaction Throughput Rate Time Extrapolation"
// *Comment: mnemonic: ODB_TXN_THRUPUT_TIME_ANAL_NAME
// *Cause:
// *Action:
/
03761, 00000, "What will the Transaction Throughput Rate at the specified time"
// *Comment: mnemonic: ODB_TXN_THRUPUT_TIME_ANAL_DESC
// *Cause:
// *Action:
/
03762, 00000, "Data Dictionary Hit Percentage Value Extrapolation"
// *Comment: mnemonic: ODB_DATADICT_HR_VAL_ANAL_NAME
// *Cause:
// *Action:
/
03763, 00000, "At what time will the Data Dictionary Hit Percentage reach the specified value"
// *Comment: mnemonic: ODB_DATADICT_HR_VAL_ANAL_DESC
// *Cause:
// *Action:
/
03764, 00000, "Data Dictionary Hit Percentage Time Extrapolation"
// *Comment: mnemonic: ODB_DATADICT_HR_TIME_ANAL_NAME
// *Cause:
// *Action:
/
03765, 00000, "What will be the value of the Data Dictionary Hit Percentage at the specified time"
// *Comment: mnemonic: ODB_DATADICT_HR_TIME_ANAL_DESC
// *Cause:
// *Action:
/
03766, 00000, "Library Cache Hit Percentage Value Extrapolation"
// *Comment: mnemonic: ODB_LIBCACHE_HR_VAL_ANAL_NAME
// *Cause:
// *Action:
/
03767, 00000, "At what time will the Library Cache Hit Percentage reach the specified value"
// *Comment: mnemonic: ODB_LIBCACHE_HR_VAL_ANAL_DESC
// *Cause:
// *Action:
/
03768, 00000, "Library Cache Hit Percentage Time Extrapolation"
// *Comment: mnemonic: ODB_LIBCACHE_HR_TIME_ANAL_NAME
// *Cause:
// *Action:
/
03769, 00000, "What will be the value of the Library Cache Hit Percentage at the specified time"
// *Comment: mnemonic: ODB_LIBCACHE_HR_TIME_ANAL_DESC
// *Cause:
// *Action:
/
03770, 00000, "I/O"
// *Comment: mnemonic: ODB_NESTEDIO_NAME
// *Cause:
// *Action:
/
03771, 00000, "Group of I/O Charts"
// *Comment: mnemonic: ODB_NESTEDIO_DESC
// *Cause:
// *Action:
/
03772, 00000, "Contention"
// *Comment: mnemonic: ODB_NESTEDCONTENTION_NAME
// *Cause:
// *Action:
/
03773, 00000, "Group of Contention Charts"
// *Comment: mnemonic: ODB_NESTEDCONTENTION_DESC
// *Cause:
// *Action:
/
03774, 00000, "Database Instance"
// *Comment: mnemonic: ODB_NESTEDINSTANCE_NAME
// *Cause:
// *Action:
/
03775, 00000, "Group of Database Instance Charts"
// *Comment: mnemonic: ODB_NESTEDINSTANCE_DESC
// *Cause:
// *Action:
/
03776, 00000, "Memory"
// *Comment: mnemonic: ODB_NESTEDMEMORY_NAME
// *Cause:
// *Action:
/
03777, 00000, "Group of Memory Charts"
// *Comment: mnemonic: ODB_NESTEDMEMORY_DESC
// *Cause:
// *Action:
/
03778, 00000, "Top Resource Consumers"
// *Comment: mnemonic: ODB_NESTEDTOP_NAME
// *Cause:
// *Action:
/
03779, 00000, "Group of Top Resource-Consumer Charts"
// *Comment: mnemonic: ODB_NESTEDTOP_DESC
// *Cause:
// *Action:
/
03780, 00000, "Parallel Server"
// *Comment: mnemonic: ODB_NESTEDOPS_NAME
// *Cause:
// *Action:
/
03781, 00000, "Group of Oracle Parallel Server Charts"
// *Comment: mnemonic: ODB_NESTEDOPS_DESC
// *Cause:
// *Action:
/
03782, 00000, "Pid"
// *Comment: mnemonic: ODB_PROCESS_ID
// *Cause:
// *Action:
/
03783, 00000, "Load"
// *Comment: mnemonic: ODB_NESTEDLOAD_NAME
// *Cause:
// *Action:
/
03784, 00000, "Group of Load Charts"
// *Comment: mnemonic: ODB_NESTEDLOAD_DESC
// *Cause:
// *Action:
/
03785, 00000, "Oracle Capacity Planner"
// *Comment: mnemonic: ODB_NESTEDCP_NAME
// *Cause:
// *Action:
/
03786, 00000, "Group of Oracle Capacity Planner Charts"
// *Comment: mnemonic: ODB_NESTEDCP_DESC
// *Cause:
// *Action:
/
03787, 00000, "User-Defined Class"
// *Comment: mnemonic: ODB_USER_DEFINED_CLASS
// *Cause:
// *Action:
/
03788, 00000, "Table Instance"
// *Comment: mnemonic: ODB_TABLE_INST_LABEL
// *Cause:
// *Action:
/
03789, 00000, "Timestamp(sec)"
// *Comment: mnemonic: ODB_TIMESTAMP_LABEL
// *Cause:
// *Action:
/
03790, 00000, "Background Process"
// *Comment: mnemonic: ODB_BACKGROUND_PROCESS_LABEL
// *Cause:
// *Action:
/
03791, 00000, "Parallel Server Block Ping Value Extrapolation"
// *Comment: mnemonic: ODB_OPS_BLKPING_VAL_ANAL_NAME
// *Cause:
// *Action:
/
03792, 00000, "At what time will the number of OPS Block Pings reach the specified value"
// *Comment: mnemonic: ODB_OPS_BLKPING_VAL_ANAL_DESC
// *Cause:
// *Action:
/
03793, 00000, "Parallel Server Block Ping Time Extrapolation"
// *Comment: mnemonic: ODB_OPS_BLKPING_TIME_ANAL_NAME
// *Cause:
// *Action:
/
03794, 00000, "How many OPS Block Pings will have occurred by the specified time"
// *Comment: mnemonic: ODB_OPS_BLKPING_TIME_ANAL_DESC
// *Cause:
// *Action:
/
03795, 00000, "Memory Cache Hit Rates (Per Sample Period)"
// *Comment: mnemonic: ODB_MEM_HIT_CHART_DEF_NAME
// *Cause:
// *Action:
/
03796, 00000, "Hit Rate Percentages for Memory Areas (Per Sample Period)"
// *Comment: mnemonic: ODB_MEM_HIT_CHART_DEF_DESC
// *Cause:
// *Action:
/
03797, 00000, "Memory Region Sizes"
// *Comment: mnemonic: ODB_MEM_SIZE_CHART_DEF_NAME
// *Cause:
// *Action:
/
03798, 00000, "SGA, Shared Pool And Buffer Cache Sizes"
// *Comment: mnemonic: ODB_MEM_SIZE_CHART_DEF_DESC
// *Cause:
// *Action:
/
03799, 00000, "Memory Access Rates Per Second"
// *Comment: mnemonic: ODB_MEM_ACCESS_CHART_DEF_NAME
// *Cause:
// *Action:
/
03800, 00000, "Count Per Second of Memory Accesses"
// *Comment: mnemonic: ODB_MEM_ACCESS_CHART_DEF_DESC
// *Cause:
// *Action:
/
03801, 00000, "Current Maximum Open Cursors"
// *Comment: mnemonic: ODB_MEM_CURSOR_CHART_DEF_NAME
// *Cause:
// *Action:
/
03802, 00000, "Current Maximum Open Cursors in Database"
// *Comment: mnemonic: ODB_MEM_CURSOR_CHART_DEF_DESC
// *Cause:
// *Action:
/
03803, 00000, "Shared Pool Reserved Usage"
// *Comment: mnemonic: ODB_MEM_RESERVE_CHART_DEF_NAME
// *Cause:
// *Action:
/
03804, 00000, "Shared Pool Reserved Statistics"
// *Comment: mnemonic: ODB_MEM_RESERVE_CHART_DEF_DESC
// *Cause:
// *Action:
/
03805, 00000, "Operations Per Library Cache Area"
// *Comment: mnemonic: ODB_LIBC_DET_OPS_DEF_NAME
// *Cause:
// *Action:
/
03806, 00000, "Gets, Pins, Reloads, Invalidations By Area"
// *Comment: mnemonic: ODB_LIBC_DET_OPS_DEF_DESC
// *Cause:
// *Action:
/
03807, 00000, "Get and Pin Hit Ratios Per Library Cache Area"
// *Comment: mnemonic: ODB_LIBC_DET_RATIOS_DEF_NAME 
// *Cause:
// *Action:
/
03808, 00000, "Pin and Get Hit Ratios broken down by area"
// *Comment: mnemonic: ODB_LIBC_DET_RATIOS_DEF_DESC
// *Cause:
// *Action:
/
03809, 00000, "DLM Operations Per Library Cache Area"
// *Comment: mnemonic: ODB_LIBC_DET_DLM_DEF_NAME
// *Cause:
// *Action:
/
03810, 00000, "Distributed Lock Manager Operations"
// *Comment: mnemonic: ODB_LIBC_DET_DLM_DEF_DESC
// *Cause:
// *Action:
/
03811, 00000, "Tablespace Status"
// *Comment: mnemonic: ODB_TBLSPACE_STATUS_NAME
// *Cause:
// *Action:
/
03812, 00000, "Online/Offline Status of Tablespaces"
// *Comment: mnemonic: ODB_TBLSPACE_STATUS_DEF_DESC
// *Cause:
// *Action:
/
03813, 00000, "Tablespace Free Bytes"
// *Comment: mnemonic: ODB_TBLSPACE_FREE_KBYTES_NAME
// *Cause:
// *Action:
/
03814, 00000, "Number of Bytes Free Per Tablespace"
// *Comment: mnemonic: ODB_TBLSPACE_FREE_KBYTES_DEF_DESC
// *Cause:
// *Action:
/
03815, 00000, "User Statistics"
// *Comment: mnemonic: ODB_USERS_STATS_NAME
// *Cause:
// *Action:
/
03816, 00000, "Counts of Users Active, Logged On, Waiting, etc."
// *Comment: mnemonic: ODB_USERS_STATS_DESC
// *Cause:
// *Action:
/
03817, 00000, "Database Size"
// *Comment: mnemonic: ODB_DB_RES_SIZE_NAME
// *Cause:
// *Action:
/
03818, 00000, "Size in Megabytes of Database and Maximum Log Buffer"
// *Comment: mnemonic: ODB_DB_RES_SIZE_DEF_DESC
// *Cause:
// *Action:
/
03819, 00000, "Database Objects"
// *Comment: mnemonic: ODB_DB_RES_OBJS_NAME
// *Cause:
// *Action:
/
03820, 00000, "Counts of Tablespaces, Schemas, Users, Datafiles"
// *Comment: mnemonic: ODB_DB_RES_OBJS_DEF_DESC
// *Cause:
// *Action:
/
03821, 00000, "CPU Wait Time"
// *Comment: mnemonic: ODB_WAIT_CPUWAIT_NAME
// *Cause:
// *Action:
/
03822, 00000, "Amount of CPU Time Spent Waiting"
// *Comment: mnemonic: ODB_WAIT_CPUWAIT_DEF_DESC
// *Cause:
// *Action:
/
03823, 00000, "Sort Operations Per Second"
// *Comment: mnemonic: ODB_SORT_OP_NAME
// *Cause:
// *Action:
/
03824, 00000, "Counts of Sort Operations Per Second"
// *Comment: mnemonic: ODB_SORT_OP_DEF_DESC
// *Cause:
// *Action:
/
03825, 00000, "Sort Area Size Statistics"
// *Comment: mnemonic: ODB_SORT_SIZE_NAME
// *Cause:
// *Action:
/
03826, 00000, "Sizes of Sort Areas"
// *Comment: mnemonic: ODB_SORT_SIZE_DEF_DESC
// *Cause:
// *Action:
/
03827, 00000, "Sort Percentage Statistics"
// *Comment: mnemonic: ODB_SORT_PERC_NAME
// *Cause:
// *Action:
/
03828, 00000, "Memory Sort, Sort Extent Latch Percentages"
// *Comment: mnemonic: ODB_SORT_PERC_DEF_DESC
// *Cause:
// *Action:
/
03829, 00000, "Shared Pool Rsvd Sizes"
// *Comment: mnemonic: ODB_SPOOL_SIZE_NAME
// *Cause:
// *Action:
/
03830, 00000, "Size of Shared Pool Rsvd Objects"
// *Comment: mnemonic: ODB_SPOOL_SIZE_DEF_DESC
// *Cause:
// *Action:
/
03831, 00000, "Program List"
// *Comment: mnemonic: ODB_PROCESS_PROG_NAME
// *Cause:
// *Action:
/
03832, 00000, "List of Programs Accessing the Database"
// *Comment: mnemonic: ODB_PROCESS_PROG_DEF_DESC
// *Cause:
// *Action:
/
03833, 00000, "Server Busy"
// *Comment: mnemonic: ODB_SERV_BUSY_NAME
// *Cause:
// *Action:
/
03834, 00000, "Server Busy/Idle Time"
// *Comment: mnemonic: ODB_SERV_BUSY_DEF_DESC
// *Cause:
// *Action:
/
03835, 00000, "Server Requests"
// *Comment: mnemonic: ODB_SERV_REQ_NAME
// *Cause:
// *Action:
/
03836, 00000, "Server Request Rate"
// *Comment: mnemonic: ODB_SERV_REQ_DEF_DESC
// *Cause:
// *Action:
/
03837, 00000, "Dispatcher Busy"
// *Comment: mnemonic: ODB_DISP_BUSY_NAME
// *Cause:
// *Action:
/
03838, 00000, "Dispatcher Busy/Idle Time"
// *Comment: mnemonic: ODB_DISP_BUSY_DEF_DESC
// *Cause:
// *Action:
/
03839, 00000, "Dispatcher Messages"
// *Comment: mnemonic: ODB_DISP_MESG_NAME
// *Cause:
// *Action:
/
03840, 00000, "Dispatcher Message Count"
// *Comment: mnemonic: ODB_DISP_MESG_DEF_DESC
// *Cause:
// *Action:
/
03841, 00000, "Dispatcher Byte Count"
// *Comment: mnemonic: ODB_DISP_BYTE_NAME
// *Cause:
// *Action:
/
03842, 00000, "Dispatcher Byte Count"
// *Comment: mnemonic: ODB_DISP_BYTE_DEF_DESC
// *Cause:
// *Action:
/
03843, 00000, "Circuit Byte Count"
// *Comment: mnemonic: ODB_CIRCUIT_BYTE_NAME
// *Cause:
// *Action:
/
03844, 00000, "Circuit Byte Count"
// *Comment: mnemonic: ODB_CIRCUIT_BYTE_DEF_DESC
// *Cause:
// *Action:
/
03845, 00000, "Circuit Message Count"
// *Comment: mnemonic: ODB_CIRCUIT_MESG_NAME
// *Cause:
// *Action:
/
03846, 00000, "Circuit Message Count"
// *Comment: mnemonic: ODB_CIRCUIT_MESG_DEF_DESC
// *Cause:
// *Action:
/
03847, 00000, "Tablespace"
// *Comment: mnemonic: ODB_CLASS_TABLESPACE_85_NAME
// *Cause:
// *Action:
/
03848, 00000, "Tablespace Statistics"
// *Comment: mnemonic: ODB_CLASS_TABLESPACE_85_DESC
// *Cause:
// *Action:
/
03849, 00000, "User"
// *Comment: mnemonic: ODB_CLASS_USER_85_NAME
// *Cause:
// *Action:
/
03850, 00000, "User Statistics"
// *Comment: mnemonic: ODB_CLASS_USER_85_DESC
// *Cause:
// *Action:
/
03851, 00000, "TOP SQL Data"
// *Comment: mnemonic: ODB_INST_TOPSQL
// *Cause:
// *Action:
/
03852, 00000, "TABLE ACCESS Data"
// *Comment: mnemonic: ODB_INST_TABLEACC
// *Cause:
// *Action:
/
03853, 00000, "TABLESPACE Data"
// *Comment: mnemonic: ODB_INST_TBLSPACE
// *Cause:
// *Action:
/
03854, 00000, "TABLESPACE FREE LIST Data"
// *Comment: mnemonic: ODB_INST_FREETBLSPACE
// *Cause:
// *Action:
/
03855, 00000, "NO OF USERS ACTIVE Data"
// *Comment: mnemonic: ODB_INST_USERACT
// *Cause:
// *Action:
/
03856, 00000, "Memory Stat Data"
// *Comment: mnemonic: ODB_INST_MEM
// *Cause:
// *Action:
/
03857, 00000, "MTS Stat Data"
// *Comment: mnemonic: ODB_INST_MTS
// *Cause:
// *Action:
/
03858, 00000, "Database I/O Stat"
// *Comment: mnemonic: ODB_INST_DBIO
// *Cause:
// *Action:
/
03859, 00000, "Session Stat Data"
// *Comment: mnemonic: ODB_INST_SESS
// *Cause:
// *Action:
/
03860, 00000, "Lock Stat Data"
// *Comment: mnemonic: ODB_INST_LOCKSTAT
// *Cause:
// *Action:
/
03861, 00000, "Rollback Stat Data"
// *Comment: mnemonic: ODB_INST_ROLLBACK
// *Cause:
// *Action:
/
03862, 00000, "DB Resource Stat Data"
// *Comment: mnemonic: ODB_INST_DBRES
// *Cause:
// *Action:
/
03863, 00000, "Transaction Stat Data"
// *Comment: mnemonic: ODB_INST_TRANS
// *Cause:
// *Action:
/
03864, 00000, "Table Stats Data"
// *Comment: mnemonic: ODB_INST_TBL
// *Cause:
// *Action:
/
03865, 00000, "Wait Stats Data"
// *Comment: mnemonic: ODB_INST_WAIT
// *Cause:
// *Action:
/
03866, 00000, "File I/O Rate Data"
// *Comment: mnemonic: ODB_INST_FILEIO
// *Cause:
// *Action:
/
03867, 00000, "Top Files Data"
// *Comment: mnemonic: ODB_INST_TOPFILE
// *Cause:
// *Action:
/
03868, 00000, "FILE IO DETAILS Data"
// *Comment: mnemonic: ODB_INST_FILEDET
// *Cause:
// *Action:
/
03869, 00000, "NETWORK IO RATE Data"
// *Comment: mnemonic: ODB_INST_NET
// *Cause:
// *Action:
/
03870, 00000, "SYSTEM IO RATE Data"
// *Comment: mnemonic: ODB_INST_SYSIO
// *Cause:
// *Action:
/
03871, 00000, "CIRCUIT Data"
// *Comment: mnemonic: ODB_INST_CIRC
// *Cause:
// *Action:
/
03872, 00000, "DISPATCHER Data"
// *Comment: mnemonic: ODB_INST_DISP
// *Cause:
// *Action:
/
03873, 00000, "FREE LIST HIT Data"
// *Comment: mnemonic: ODB_INST_FREELIST
// *Cause:
// *Action:
/
03874, 00000, "LATCH Data"
// *Comment: mnemonic: ODB_INST_LATCH
// *Cause:
// *Action:
/
03875, 00000, "LOCK Data"
// *Comment: mnemonic: ODB_INST_LOCK
// *Cause:
// *Action:
/
03876, 00000, "QUEUE Data"
// *Comment: mnemonic: ODB_INST_QUEUE
// *Cause:
// *Action:
/
03877, 00000, "REDO ALLOCATION HIT Data"
// *Comment: mnemonic: ODB_INST_REDOALLOCHIT
// *Cause:
// *Action:
/
03878, 00000, "ROLLBACK NOWAIT HIT Data"
// *Comment: mnemonic: ODB_INST_ROLLBACKNOWAITHIT
// *Cause:
// *Action:
/
03879, 00000, "SERVER Data"
// *Comment: mnemonic: ODB_INST_SERVER
// *Cause:
// *Action:
/
03880, 00000, "INITIALIZATION PARAMETER Data"
// *Comment: mnemonic: ODB_INST_
// *Cause:
// *Action:
/
03881, 00000, "PROCESS Data"
// *Comment: mnemonic: ODB_INST_PROCESS
// *Cause:
// *Action:
/
03882, 00000, "TOP SESSIONS Data"
// *Comment: mnemonic: ODB_INST_TOPSESS
// *Cause:
// *Action:
/
03883, 00000, "SYSTEM STATISTICS Data"
// *Comment: mnemonic: ODB_INST_SYSTEMSTAT
// *Cause:
// *Action:
/
03884, 00000, "SYSTEM EVENT Data"
// *Comment: mnemonic: ODB_INST_SYSEVENT
// *Cause:
// *Action:
/
03885, 00000, "NO OF USERS LOGGED ON Data"
// *Comment: mnemonic: ODB_INST_ULOGGED
// *Cause:
// *Action:
/
03886, 00000, "NO OF USERS WAITING Data"
// *Comment: mnemonic: ODB_INST_UWAIT
// *Cause:
// *Action:
/
03887, 00000, "NO OF USERS WAITING FOR LOCKS Data"
// *Comment: mnemonic: ODB_INST_UWAITLOCK
// *Cause:
// *Action:
/
03888, 00000, "NO OF USERS RUNNING Data"
// *Comment: mnemonic: ODB_INST_URUN
// *Cause:
// *Action:
/
03889, 00000, "BUFFER GETS RATE Data"
// *Comment: mnemonic: ODB_INST_BUFG
// *Cause:
// *Action:
/
03890, 00000, "NETWORK BYTES RATE Data"
// *Comment: mnemonic: ODB_INST_NETBYTES
// *Cause:
// *Action:
/
03891, 00000, "REDO STATISTICS RATE Data"
// *Comment: mnemonic: ODB_INST_REDOSTAT
// *Cause:
// *Action:
/
03892, 00000, "SORT ROWS RATE Data"
// *Comment: mnemonic: ODB_INST_SORTROWS
// *Cause:
// *Action:
/
03893, 00000, "TABLESCAN ROWS RATE Data"
// *Comment: mnemonic: ODB_INST_TBLSCANROWS
// *Cause:
// *Action:
/
03894, 00000, "THROUGHPUT RATE Data"
// *Comment: mnemonic: ODB_INST_THRPUTRATE
// *Cause:
// *Action:
/
03895, 00000, "BUFFER CACHE HIT Data"
// *Comment: mnemonic: ODB_INST_BUFCACHIT
// *Cause:
// *Action:
/
03896, 00000, "DATA DICTIONARY CACHE HIT Data"
// *Comment: mnemonic: ODB_INST_DATADICT
// *Cause:
// *Action:
/
03897, 00000, "LIBRARY CACHE HIT Data"
// *Comment: mnemonic: ODB_INST_LIBCACH
// *Cause:
// *Action:
/
03898, 00000, "LIBRARY CACHE DETAILS Data"
// *Comment: mnemonic: ODB_INST_LIBDET
// *Cause:
// *Action:
/
03899, 00000, "MEMORY ALLOCATED Data"
// *Comment: mnemonic: ODB_INST_MEMALL
// *Cause:
// *Action:
/
03900, 00000, "MEMORY SORT HIT Data"
// *Comment: mnemonic: ODB_INST_MEMSORT
// *Cause:
// *Action:
/
03901, 00000, "PARSE RATIO Data"
// *Comment: mnemonic: ODB_INST_PARSE
// *Cause:
// *Action:
/
03902, 00000, "READ CONSISTENCY HIT Data"
// *Comment: mnemonic: ODB_INST_READCONS
// *Cause:
// *Action:
/
03903, 00000, "OPS_ACTIVE_USERS Data"
// *Comment: mnemonic: ODB_INST_OPSAU
// *Cause:
// *Action:
/
03904, 00000, "OPS_ACTIVE_USERS_BY_INSTANCE"
// *Comment: mnemonic: ODB_INST_OPSAUI
// *Cause:
// *Action:
/
03905, 00000, "OPS_BLOCK_PING Data"
// *Comment: mnemonic: ODB_INST_OPSBP
// *Cause:
// *Action:
/
03906, 00000, "OPS_BLOCK_PING_BY_INSTANCE"
// *Comment: mnemonic: ODB_INST_OPSBPI
// *Cause:
// *Action:
/
03907, 00000, "OPS_BLOCK_PING_BY_TABLESPACE"
// *Comment: mnemonic: ODB_INST_OPSBPT
// *Cause:
// *Action:
/
03908, 00000, "OPS_FILE_IO_RATE Data"
// *Comment: mnemonic: ODB_INST_OPSFIO
// *Cause:
// *Action:
/
03909, 00000, "OPS_FILE_IO_RATE_BY_INSTANCE"
// *Comment: mnemonic: ODB_INST_OPSFIOBI
// *Cause:
// *Action:
/
03910, 00000, "OPS_FILE_IO_RATE_BY_OBJECT"
// *Comment: mnemonic: ODB_INST_OPSFIOBO
// *Cause:
// *Action:
/
03911, 00000, "OPS_LOCK_ACTIVITY"
// *Comment: mnemonic: ODB_INST_OPSLOCK
// *Cause:
// *Action:
/
03912, 00000, "OPS_SESSION"
// *Comment: mnemonic: ODB_INST_OPSSESS
// *Cause:
// *Action:
/
03913, 00000, "OPS_USERS_LOGGED_ON Data"
// *Comment: mnemonic: ODB_INST_OPSULOG
// *Cause:
// *Action:
/
03914, 00000, "OPS_USERS_LOGGED_ON_BY_INSTANC Data"
// *Comment: mnemonic: ODB_INST_OPSULOGBI
// *Cause:
// *Action:
/
03915, 00000, "SESSION DETAILS Data"
// *Comment: mnemonic: ODB_INST_SESSDET
// *Cause:
// *Action:
/
03916, 00000, "SESSION STATISTICS Data"
// *Comment: mnemonic: ODB_INST_SESSSTAT
// *Cause:
// *Action:
/
03917, 00000, "OPS_BLKPING_TS_OBJ Data"
// *Comment: mnemonic: ODB_INST_BLKPING_OPS
// *Cause:
// *Action:
/
03918, 00000, "OPS_BLKPING_TS_INST Data"
// *Comment: mnemonic: ODB_INST_BLKPINGINS_OPS
// *Cause:
// *Action:
/
03919, 00000, "OPS_FILEIO_OBJ_INST Data"
// *Comment: mnemonic: ODB_INST_OPSFIOBJINS
// *Cause:
// *Action:
/
03920, 00000, "OPS_LOCK_ACT_INST Data"
// *Comment: mnemonic: ODB_INST_LOCKACTINST_OPS
// *Cause:
// *Action:
/
03921, 00000, "OPS_DYNAMIC Data"
// *Comment: mnemonic: ODB_INST_OPSDYN
// *Cause:
// *Action:
/
03922, 00000, "Object Name"
// *Comment: mnemonic: ODB_LOCKMGR_OBJECT_NAME
// *Cause:
// *Action:
/
03923, 00000, "Object Owner"
// *Comment: mnemonic: ODB_LOCKMGR_OBJECT_OWNER
// *Cause:
// *Action:
/
03924, 00000, "Object Type"
// *Comment: mnemonic: ODB_LOCKMGR_OBJECT_TYPE
// *Cause:
// *Action:
/
03925, 00000, "ROWID"
// *Comment: mnemonic: ODB_LOCKMGR_WAIT_ROWID
// *Cause:
// *Action:
/
03926, 00000, "SQL HASH VALUE"
// *Comment: mnemonic: ODB_TOP_SQL_HASH_VALUE
// *Cause:
// *Action:
/
03927, 00000, "Contention Dispatcher"
// *Comment: mnemonic: ODB_CONTENTION_DISPATCHER_NAME
// *Cause:
// *Action:
/
03928, 00000, "Oracle Contention for Dispatcher Processes"
// *Comment: mnemonic: ODB_CONTENTION_DISPATCHER_DESC
// *Cause:
// *Action:
/
03929, 00000, "Contention Dispatcher Chart"
// *Comment: mnemonic: ODB_CONTENTION_DISPATCHER_CHART_DEF_NAME
// *Cause:
// *Action:
/
03930, 00000, "Oracle Contention for Dispatcher Processes"
// *Comment: mnemonic: ODB_CONTENTION_DISPATCHER_CHART_DEF_DESC
// *Cause:
// *Action:
/
03931, 00000, "Network"
// *Comment: mnemonic: ODB_CONTENTION_DISPATCHER_NETWORK
// *Cause:
// *Action:
/
03932, 00000, "Busy Rate"
// *Comment: mnemonic: ODB_CONTENTION_DISPATCHER_BUSY_RATE
// *Cause:
// *Action:
/
03933, 00000, "Average Wait"
// *Comment: mnemonic: ODB_CONTENTION_DISPATCHER_AVG_WAIT
// *Cause:
// *Action:
/
03934, 00000, "Recursive Calls Data"
// *Comment: mnemonic: ODB_RECURSIVE_CALLS_NAME
// *Cause:
// *Action:
/
03935, 00000, "Recursive Calls"
// *Comment: mnemonic: ODB_RECURSIVE_CALLS_DESC
// *Cause:
// *Action:
/
03936, 00000, "Recursive Calls Chart"
// *Comment: mnemonic: ODB_RECURSIVE_CALLS_CHART_DEF_NAME
// *Cause:
// *Action:
/
03937, 00000, "Recursive Calls"
// *Comment: mnemonic: ODB_RECURSIVE_CALLS_CHART_DEF_DESC
// *Cause:
// *Action:
/
03938, 00000, "Recursive Calls"
// *Comment: mnemonic: ODB_RECURSIVE_CALLS_VALUE
// *Cause:
// *Action:
/
03939, 00000, "User Calls"
// *Comment: mnemonic: ODB_USER_CALLS_VALUE
// *Cause:
// *Action:
/
03940, 00000, "Parse Calls"
// *Comment: mnemonic: ODB_TOP_SQL_PARSE_CALLS
// *Cause:
// *Action:
/
03941, 00000, "Executions"
// *Comment: mnemonic: ODB_TOP_SQL_EXECUTIONS
// *Cause:
// *Action:
/
03942, 00000, "Overview of Cache Utilization"
// *Comment: mnemonic: ODB_OVERVIEW_CACHE_UTIL_CHART_NAME
// *Cause:
// *Action:
/
03943, 00000, "Group Chart of Cache Utilization Overview"
// *Comment: mnemonic: ODB_OVERVIEW_CACHE_UTIL_CHART_DESC
// *Cause:
// *Action:
/
03944, 00000, "Overview of Throughput"
// *Comment: mnemonic: ODB_OVERVIEW_THROUGHPUT_CHART_NAME
// *Cause:
// *Action:
/
03945, 00000, "Group Chart of Throughput Overview"
// *Comment: mnemonic: ODB_OVERVIEW_THROUGHPUT_CHART_DESC
// *Cause:
// *Action:
/
03946, 00000, "Shared Pool Free Memory Size (MBytes)"
// *Comment: mnemonic: ODB_MEMORY_SHRPOOL_FREE_MEM
// *Cause:
// *Action:
/
03947, 00000, "Redo Latch Data"
// *Comment: mnemonic: ODB_REDO_LATCH_NAME
// *Cause:
// *Action:
/
03948, 00000, "Oracle Redo Latch Statistics"
// *Comment: mnemonic: ODB_REDO_LATCH_DESC
// *Cause:
// *Action:
/
03949, 00000, "Missing Ratio (%)"
// *Comment: mnemonic: ODB_REDO_LATCH_RATIO_MISS
// *Cause:
// *Action:
/
03950, 00000, "Immediate Missing Ratio (%)"
// *Comment: mnemonic: ODB_REDO_LATCH_RATIO_NO_WAIT
// *Cause:
// *Action:
/
03951, 00000, "Redo Latch Chart"
// *Comment: mnemonic: ODB_REDO_LATCH_CHART_DEF_NAME
// *Cause:
// *Action:
/
03952, 00000, "Chart of Redo Latch Statistics"
// *Comment: mnemonic: ODB_REDO_LATCH_CHART_DEF_DESC
// *Cause:
// *Action:
/
03953, 00000, "Rollback Segment Data"
// *Comment: mnemonic: ODB_ROLLBACK_SEGMENT_NAME
// *Cause:
// *Action:
/
03954, 00000, "Oracle Database Rollback Segment Count"
// *Comment: mnemonic: ODB_ROLLBACK_SEGMENT_DESC
// *Cause:
// *Action:
/
03955, 00000, "Count"
// *Comment: mnemonic: ODB_ROLLBACK_SEGMENT_COUNT
// *Cause:
// *Action:
/
03956, 00000, "Rollback Segment Chart"
// *Comment: mnemonic: ODB_ROLLBACK_SEGMENT_CHART_DEF_NAME
// *Cause:
// *Action:
/
03957, 00000, "Chart of Rollback Segment"
// *Comment: mnemonic: ODB_ROLLBACK_SEGMENT_CHART_DEF_DESC
// *Cause:
// *Action:
/
03958, 00000, "Data Dictionary Stats Data"
// *Comment: mnemonic: ODB_DATA_DICT_STATS_NAME
// *Cause:
// *Action:
/
03959, 00000, "Oracle Data Dictionary Statistics"
// *Comment: mnemonic: ODB_DATA_DICT_STATS_DESC
// *Cause:
// *Action:
/
03960, 00000, "Count"
// *Comment: mnemonic: ODB_DATA_DICT_STATS_COUNT
// *Cause:
// *Action:
/
03961, 00000, "Getmisses"
// *Comment: mnemonic: ODB_DATA_DICT_STATS_GETMISS
// *Cause:
// *Action:
/
03962, 00000, "Usage"
// *Comment: mnemonic: ODB_DATA_DICT_STATS_USAGE
// *Cause:
// *Action:
/
03963, 00000, "Data Dictionary Stats Chart"
// *Comment: mnemonic: ODB_DATA_DICT_STATS_CHART_DEF_NAME
// *Cause:
// *Action:
/
03964, 00000, "Chart of Data Dictionary Stats"
// *Comment: mnemonic: ODB_DATA_DICT_STATS_CHART_DEF_DESC
// *Cause:
// *Action:
/
03965, 00000, "Top Latches"
// *Comment: mnemonic: ODB_TOP_LATCHES_NAME
// *Cause:
// *Action:
/
03966, 00000, "Oracle Database Top Latches Statistics"
// *Comment: mnemonic: ODB_TOP_LATCHES_DESC
// *Cause:
// *Action:
/
03967, 00000, "Gets"
// *Comment: mnemonic: ODB_TOP_LATCHES_GETS
// *Cause:
// *Action:
/
03968, 00000, "Top Latches Gets Chart"
// *Comment: mnemonic: ODB_TOP_LATCHES_GETS_CHART_DEF_NAME
// *Cause:
// *Action:
/
03969, 00000, "Chart of Top Latches Gets"
// *Comment: mnemonic: ODB_TOP_LATCHES_GETS_CHART_DEF_DESC
// *Cause:
// *Action:
/
03970, 00000, "Spin Gets"
// *Comment: mnemonic: ODB_TOP_LATCHES_SPIN_GETS
// *Cause:
// *Action:
/
03971, 00000, "Top Latches Spin Gets Chart"
// *Comment: mnemonic: ODB_TOP_LATCHES_SPIN_GETS_CHART_DEF_NAME
// *Cause:
// *Action:
/
03972, 00000, "Chart of Top Latches Spin Gets"
// *Comment: mnemonic: ODB_TOP_LATCHES_SPIN_GETS_CHART_DEF_DESC
// *Cause:
// *Action:
/
03973, 00000, "Misses"
// *Comment: mnemonic: ODB_TOP_LATCHES_MISSES
// *Cause:
// *Action:
/
03974, 00000, "Top Latches Misses Chart"
// *Comment: mnemonic: ODB_TOP_LATCHES_MISSES_CHART_DEF_NAME
// *Cause:
// *Action:
/
03975, 00000, "Chart of Top Latches Misses"
// *Comment: mnemonic: ODB_TOP_LATCHES_MISSES_CHART_DEF_DESC
// *Cause:
// *Action:
/
03976, 00000, "Sleeps"
// *Comment: mnemonic: ODB_TOP_LATCHES_SLEEPS
// *Cause:
// *Action:
/
03977, 00000, "Top Latches Sleeps Chart"
// *Comment: mnemonic: ODB_TOP_LATCHES_SLEEPS_CHART_DEF_NAME
// *Cause:
// *Action:
/
03978, 00000, "Chart of Top Latches Sleeps"
// *Comment: mnemonic: ODB_TOP_LATCHES_SLEEPS_CHART_DEF_DESC
// *Cause:
// *Action:
/
03979, 00000, "Waiters Woken"
// *Comment: mnemonic: ODB_TOP_LATCHES_WAITERS_WOKEN
// *Cause:
// *Action:
/
03980, 00000, "Top Latches Waiters Woken Chart"
// *Comment: mnemonic: ODB_TOP_LATCHES_WAITERS_WOKEN_CHART_DEF_NAME
// *Cause:
// *Action:
/
03981, 00000, "Chart of Top Latches Waiters Woken"
// *Comment: mnemonic: ODB_TOP_LATCHES_WAITERS_WOKEN_CHART_DEF_DESC
// *Cause:
// *Action:
/
03982, 00000, "Waits Holding"
// *Comment: mnemonic: ODB_TOP_LATCHES_WAITS_HOLDING
// *Cause:
// *Action:
/
03983, 00000, "Top Latches Waits Holding Chart"
// *Comment: mnemonic: ODB_TOP_LATCHES_WAITS_HOLDING_CHART_DEF_NAME
// *Cause:
// *Action:
/
03984, 00000, "Chart of Top Latches Waits Holding"
// *Comment: mnemonic: ODB_TOP_LATCHES_WAITS_HOLDING_CHART_DEF_DESC
// *Cause:
// *Action:
/
03985, 00000, "Users Statistics Chart"
// *Comment: mnemonic: ODB_USERS_STATS_CHART_DEF_NAME
// *Cause:
// *Action:
/
03986, 00000, "Chart of Users Statistics"
// *Comment: mnemonic: ODB_USERS_STATS_CHART_DEF_DESC
// *Cause:
// *Action:
/
03987, 00000, "Tablespace Block Reads"
// *Comment: mnemonic: ODB_TABLESPACE_BLOCK_READS_NAME
// *Cause:
// *Action:
/
03988, 00000, "Oracle Database Tablespace Block Reads"
// *Comment: mnemonic: ODB_TABLESPACE_BLOCK_READS_DESC
// *Cause:
// *Action:
/
03989, 00000, "Block Reads/sec"
// *Comment: mnemonic: ODB_TABLESPACE_BLOCK_READS
// *Cause:
// *Action:
/
03990, 00000, "Tablespace Block Reads Chart"
// *Comment: mnemonic: ODB_TABLESPACE_BLOCK_READS_CHART_DEF_NAME
// *Cause:
// *Action:
/
03991, 00000, "Chart of Tablespace Block Reads"
// *Comment: mnemonic: ODB_TABLESPACE_BLOCK_READS_CHART_DEF_DESC
// *Cause:
// *Action:
/
03992, 00000, "Tablespace Block Writes"
// *Comment: mnemonic: ODB_TABLESPACE_BLOCK_WRITES_NAME
// *Cause:
// *Action:
/
03993, 00000, "Oracle Database Tablespace Block Writes"
// *Comment: mnemonic: ODB_TABLESPACE_BLOCK_WRITES_DESC
// *Cause:
// *Action:
/
03994, 00000, "Block Writes/sec"
// *Comment: mnemonic: ODB_TABLESPACE_BLOCK_WRITES
// *Cause:
// *Action:
/
03995, 00000, "Tablespace Block Writes Chart"
// *Comment: mnemonic: ODB_TABLESPACE_BLOCK_WRITES_CHART_DEF_NAME
// *Cause:
// *Action:
/
03996, 00000, "Chart of Tablespace Block Writes"
// *Comment: mnemonic: ODB_TABLESPACE_BLOCK_WRITES_CHART_DEF_DESC
// *Cause:
// *Action:
/
03997, 00000, "Library Cache Latch Data"
// *Comment: mnemonic: ODB_LIBRARY_CACHE_LATCH_NAME
// *Cause:
// *Action:
/
03998, 00000, "Oracle Library Cache Latch Activity"
// *Comment: mnemonic: ODB_LIBRARY_CACHE_LATCH_DESC
// *Cause:
// *Action:
/
03999, 00000, "Gets/Sec"
// *Comment: mnemonic: ODB_LIBRARY_CACHE_LATCH_GETS_RATE
// *Cause:
// *Action:
/
04000, 00000, "Misses/Sec"
// *Comment: mnemonic: ODB_LIBRARY_CACHE_LATCH_MISSES_RATE
// *Cause:
// *Action:
/
04001, 00000, "Sleeps/Sec"
// *Comment: mnemonic: ODB_LIBRARY_CACHE_LATCH_SLEEPS_RATE
// *Cause:
// *Action:
/
04002, 00000, "Spin Gets/Sec"
// *Comment: mnemonic: ODB_LIBRARY_CACHE_LATCH_SPIN_GETS_RATE
// *Cause:
// *Action:
/
04003, 00000, "Library Cache Latch Chart"
// *Comment: mnemonic: ODB_LIBRARY_CACHE_LATCH_CHART_DEF_NAME
// *Cause:
// *Action:
/
04004, 00000, "Chart of Library Cache Latch Statistics"
// *Comment: mnemonic: ODB_LIBRARY_CACHE_LATCH_CHART_DEF_DESC
// *Cause:
// *Action:
/
04005, 00000, "Full SQL Text"
// *Comment: mnemonic: ODB_SQL_TEXT_NAME
// *Cause:
// *Action:
/
04006, 00000, "Display of the Full SQL Text"
// *Comment: mnemonic: ODB_SQL_TEXT_DESC
// *Cause:
// *Action:
/
04007, 00000, "Full SQL Text Chart"
// *Comment: mnemonic: ODB_SQL_TEXT_CHART_DEF_NAME
// *Cause:
// *Action:
/
04008, 00000, "Chart to Display Full SQL Text"
// *Comment: mnemonic: ODB_SQL_TEXT_CHART_DEF_DESC
// *Cause:
// *Action:
/
04009, 00000, "User Type Locks Chart"
// *Comment: mnemonic: ODB_LOCKMGR_USER_CHART_DEF_NAME
// *Cause:
// *Action:
/
04010, 00000, "Chart of Locks (User Types: TM, TX and UL) Statistics"
// *Comment: mnemonic: ODB_LOCKMGR_USER_CHART_DEF_DESC
// *Cause:
// *Action:
/
04011, 00000, "Blocking/Waiting Locks Chart"
// *Comment: mnemonic: ODB_LOCKMGR_BLOCK_WAIT_CHART_DEF_NAME
// *Cause:
// *Action:
/
04012, 00000, "Chart of Blocking/Waiting Locks Statistics"
// *Comment: mnemonic: ODB_LOCKMGR_BLOCK_WAIT_CHART_DEF_DESC
// *Cause:
// *Action:
/
04013, 00000, "Recursive Calls / User Calls"
// *Comment: mnemonic: ODB_RECURSIVE_CALLS_RATE
// *Cause:
// *Action:
/
//////////////////////////////////////////////////////////////////////////
/
/ 4500 - 4699 Labels for Callbacks
/
//////////////////////////////////////////////////////////////////////////
04500, 00000, "Kill Session"
// *Comment: mnemonic: ODB_CALLBACK_KILL_SESSION_NAME
// *Cause:
// *Action:
/
04501, 00000, "Kill Session"
// *Comment: mnemonic: ODB_CALLBACK_KILL_SESSION_DESC
// *Cause:
// *Action:
/
04502, 00000, "Couldn't kill that session.  "
// *Comment: mnemonic: ODB_CALLBACK_KILL_SESSION_FAIL
// *Cause:
// *Action:
/
04503, 00000, "Successfully killed that session.  "
// *Comment: mnemonic: ODB_CALLBACK_KILL_SESSION_SUCCEED
// *Cause:
// *Action:
/
04504, 00000, "Need two parameters to kill a session, but only got one. "
// *Comment: mnemonic: ODB_CALLBACK_KILL_SESSION_MORE_DATA
// *Cause:
// *Action:
/
//////////////////////////////////////////////////////////////////////////
/
/ 4700 - 4999 Labels for Events
/
//////////////////////////////////////////////////////////////////////////
04700, 00000, "High Ratio of Misses/Gets"
// *Comment: mnemonic: ODB_REDO_LATCH_EVENT_RATIO_MISS_NAME
// *Cause:
// *Action:
/
04701, 00000, "The ratio of misses to gets is greater than 1%"
// *Comment: mnemonic: ODB_REDO_LATCH_EVENT_RATIO_MISS_DESC
// *Cause:
// *Action:
/
04702, 00000, "High Ratio of Immediate_Misses/Immediate_Gets"
// *Comment: mnemonic: ODB_REDO_LATCH_EVENT_RATIO_NO_WAIT_NAME
// *Cause:
// *Action:
/
04703, 00000, "The ratio of immediate misses to immediate gets is greater than 1%"
// *Comment: mnemonic: ODB_REDO_LATCH_EVENT_RATIO_NO_WAIT_DESC
// *Cause:
// *Action:
/

09999, 00000, "can't see last message ... bug"
// *Cause:
// *Action:
/
