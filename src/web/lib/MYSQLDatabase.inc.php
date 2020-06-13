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
 
require_once('Database.inc.php');


//////////////////////////////////////////////////////////////
//class MYSQLDatabase
//  MySQL implementation for the abstract Database class
//
//
//
/////////////////////////////////////////////////////////////
class MYSQLDatabase extends Database{
	
	private $db_link; // link identifier
	private $dbtype;
	private $dbname;
	private $dbuname;
	private $dbpass;
	private $dbhost;
	private $dbport;
	
	
	public function MYSQLDatabase( $db_username, $db_password, $db_host, $db_port, $db_name ){
		global $db_type;
		
		$this->dbtype  = $db_type;
		$this->dbname  = $db_name;
		$this->dbuname = $db_username;
		$this->dbpass  = $db_password;
		$this->dbhost  = $db_host;
		$this->dbport  = $db_port;
	}
	
	
	public function Connect(){
		
		$connstring = $this->dbhost.':'.$this->dbport;
		
		$this->db_link = mysql_connect( $connstring, $this->dbuname, $this->dbpass );
		
		if( $this->db_link == FALSE )
			return false;
		
		$check = mysql_select_db( $this->dbname, $this->db_link );
		
		return $check;
	}
	
	
	public function Close(){
		
		$check = mysql_close( $this->db_link );							  
		
		return $check;
	}
	
	
	public function Query( $query ){
		
		$result = mysql_query( $query, $this->db_link ) or die('Invalid query: ' . mysql_error());
		
		return $result;
	}
	
	
	public function Fetch( $result ){
		
		$line = mysql_fetch_row( $result );
		
		return $line;
	}
	
}

?>
