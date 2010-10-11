<?php
/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2010 <http://www.ArcEmu.org/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//dal.php  - Database Abstraction Layer
//
//  Abstracts the database queries from the actual database engine, providing a unified, simple interface
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////// Database credentials ///////////////////////////////////////

$db_username = "Insert your Database username here";
$db_password = "Insert your Database password here";
$db_host     = "Insert the database host here";
$db_port     = "Insert the database port here";
$db_name     = "Insert the Database password here";

//////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////// Database settings /////////////////////////////////////////
//  1 - mysql, this is the default

$db_type = 1;

/////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////// Please don't change anything below this line /////////////////////

///////////////////////////////////////////////////
// Database class
//
//  handles database operations, and provides, 
//  a database independent abstract interface
///////////////////////////////////////////////////

class Database{
	
	var $db_link; // link identifier
	var $dbtype;
	var $dbname;
	var $dbuname;
	var $dbpass;
	var $dbhost;
	var $dbport;
	
	
	function Database(){
		global $db_type;
		global $db_username;
		global $db_password;
		global $db_host;
		global $db_port;
		global $db_name;
		
		$this->dbtype  = $db_type;
		$this->dbname  = $db_name;
		$this->dbuname = $db_username;
		$this->dbpass  = $db_password;
		$this->dbhost  = $db_host;
		$this->dbport  = $db_port;
	}
	
	
	//////////////////////////////////////////////////
	// Connect
	//   Establishes connection to the database, and 
	//   selects the database
	//
	//Parameters
	//  None
	//
	//Return Value
	// Returns true on success.
	// Returns false on failure.
	//
	/////////////////////////////////////////////////
	function Connect(){
	
		switch( $this->dbtype ){
		  case 1:{
			$connstring = $this->dbhost.':'.$this->dbport;
			$this->db_link = mysql_connect( $connstring, $this->dbuname, $this->dbpass );
			if( $this->db_link == FALSE ) return false;
		  break;}
			  
		}
		
		switch( $this->dbtype ){
		  case 1:{
			$check = mysql_select_db( $this->dbname, $this->db_link );
			
		  break;}
		}
		
		return $check;
	}

	
	//////////////////////////////////////////////////
	// Close
	//  Simply closes the connection to the database
	//
	//Parameters
	//  None
	//
	//Return Value
	//  Returns true on success.
	//  Returns false on failure.
	//
	/////////////////////////////////////////////////
	function Close(){
		switch( $this->dbtype ){
			case 1:{
			  $check = mysql_close( $this->db_link );							  
			break;}
		}
		
		return $check;
	}
	
	
	/////////////////////////////////////////////////////////////
	//Query
	//  Executes the query in $query and returns the result
	//
	//Parameters
	//  $query  -  Query to be executed in the database.
	//
	//Return Value
	//  Returns the result resource on success.
	//  Returns false on failure.
	//
	/////////////////////////////////////////////////////////////
	function Query( $query ){
	
		switch( $this->dbtype ){
			case 1:{			
			$result = mysql_query( $query, $this->db_link ) or die('Invalid query: ' . mysql_error());
			break;}
		}
		return $result;
	}
	
	
	///////////////////////////////////////////////////////
	//Fetch
	//  fetches a row from the query result and returns it
	//
	//Parameters
	//  $result  -  Result resource to fetch rows from
	//
	//Return Value
	//  Returns a row as an array on success.
	//  Returns false on failure.
	//
	///////////////////////////////////////////////////////
	function Fetch( $result ){
		
		switch( $this->$dbtype ){
			case 1:{
				$line = mysql_fetch_row( $result );
			break;}
		}
		
		return $line;
	}
	
}  // end of the class

?>