<?php

///////////////////////////////////////////////////
// Database class
//
//  handles database operations, and provides, 
//  a database independent abstract interface
///////////////////////////////////////////////////
abstract class Database{
	
	
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
	abstract public function Connect();
	
	
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
	abstract public function Close();
	
	
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
	abstract public function Query( $query );
	
	
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
	abstract public function Fetch( $result );
	
	
}

?>
