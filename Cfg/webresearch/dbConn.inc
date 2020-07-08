<?php
class DBCONN {
  var $conn = FALSE;
  var $stmt = 0;
  var $abstractData = FALSE;
  var $error = FALSE;
  var $id;
  var $passwd;
  var $sid;
  
  function delete($qry, $id="kolas2", $passwd = "kolas2#", $sid = "k2up") {
     if ($this->connect($id, $passwd, $sid))
     	return $this->parseExec($qry);
     else
     	return false;
     
  }
  
  
  function update($qry, $id="kolas2", $passwd = "kolas2#", $sid = "k2up") {
     if ($this->connect($id, $passwd, $sid))
     	return $this->parseExec($qry);
     else
     	return false;
  }
  
  function insert($qry, $id="kolas2", $passwd = "kolas2#", $sid = "k2up") {
     if ($this->connect($id, $passwd, $sid))
    	return $this->parseExec($qry);
     else
    	return false;
    
  }
  
  function select($qry, $id="kolas2", $passwd = "kolas2#", $sid = "k2up") {
     if ($this->connect($id, $passwd, $sid))
    	return $this->parseExec($qry);
     else
     	return false;
  }
  
  function parseExec($qry)  {
     $this->stmt=OCIParse($this->conn, $qry);
     return $this->exec();
  }
  
  function exec($mode=OCI_DEFAULT) {
     OCIExecute($this->stmt, $mode);
     $this->error = ocierror($this->stmt);
     if ($this->error) {
     	$this->disconnect();
     	return false;
     }
     else {
     	return true;
     }
  }
  
  function connect( $id="kolas2", $passwd = "kolas2#", $sid = "k2up" ) {
     if ( $this->conn != FALSE ) {
         if ( strcmp( $this->id    , $id     ) == 0 && 
             strcmp( $this->passwd, $passwd ) == 0 &&
             strcmp( $this->sid   , $sid    ) == 0 ) return true;
         else
         	return false;
     }
     if ( $this->conn = OCILogon($id, $passwd, $sid) ) {
	     $this->id     = $id;
	     $this->passwd = $passwd;
	     $this->sid    = $sid;
	     
	     return true;
     }
     
     else return false;
  }
  
  function serverVersion() {
     return OCIServerVersion($this->conn);
  }
  
  function parse($qry) {
     $this->stmt = OCIParse($this->conn, $qry);
  }
  
  function newDescriptor($type) {
     $this->abstractData = OCINewDescriptor($this->conn, $type);
  }
  
  function freeDescriptor() {
     OCIFreeDescriptor($this->abstractData);
  }
  
  function defineByName($upper,&$var) {
     OCIDefineByName($this->stmt, $upper, &$var);
  }
  
  function bindByName($place_holder, &$var, $length) {
     OCIBindByName($this->stmt, $place_holder, $var, $length);
  }
  
  function bindByNameAbstract($place_holder, $type) {
     OCIBindByName($this->stmt, $place_holder, &$this->abstractData, -1, $type);
     return $this->abstractData;
  }
  
  function fetch() {
     return OCIFetch($this->stmt);
  }
  
  function result($i) {
     return OCIResult($this->stmt, $i);
  }
  
  function fetchInto(&$col, $mode=OCI_ASSOC) {
     return OCIFetchInto($this->stmt, &$col, $mode);
  }
  
  function fetchStatement(&$arr) {
     return OCIFetchStatement($this->stmt, &$arr);
  }
  
  function numCol() {
     return OCINumCols($this->stmt);
  }
  
  function columnName($index) {
     return OCIColumnName($this->stmt, $index);
  }
  
  function columnisNull($index) {
     return OCIColumnIsNull($this->stmt, $index);
  }
  
  function columnType ($index) {
     return OCIColumnType($this->stmt, $index);
  }
  
  function StatementType() {
     return OCIStatementType($this->stmt);
  }
  
  function rowCount() {
     return OCIRowCount($this->stmt);
  }
  
  function parseFree() {
     OCIFreeStatement($this->stmt);
  }
  
  function disconnect() {
     if ( $this->conn == FALSE ) return;
     if ($this->error) {
        $this->rollback();
        //die("<font color=red>ROLLBACK OCCURED!" .$this->error["message"]."</font>");
     } 
     else {
          $this->commit();
     }
     $this->logoff();
  }
  
  function commit() {
    OCICommit($this->conn);
  }
  
  function rollback() {
    OCIRollback($this->conn);
  }
  
  function logoff() {
    OCILogOff($this->conn );
    $this->conn = FALSE;
  }

}      
?>
