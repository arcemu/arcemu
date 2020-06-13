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


require_once('../lib/StringTokenizer.inc.php');

/////////////////////////////////////////////////////////
//class ArcemuBugReportContentFieldParser
//  Simple Arcemu bugreport content field parser class
//
////////////////////////////////////////////////////////
class BugReportContentFieldParser{
	private $BugReport;
	private $Data;
	
	public function BugReportContentFieldParser( $str  ){
		$this->BugReport = $str;
		$this->Data = array();
	}
	
	
	////////////////////////////////////////////////////////////
	//public function Parse()
	//  Parses the bugreport string and inserts the parsed
	//  key/value pairs into an array.
	//
	//Parameters
	//  None.
	//
	//
	//Return Value
	//  Returns true on success.
	//  Returns false on failure.
	//
	///////////////////////////////////////////////////////////
	public function Parse(){
		if( $this->BugReport == NULL )
			return false;
			
		$tokenizer =
			new StringTokenizer( $this->BugReport, ":\t\n" );
		
		if( !$tokenizer->HasNextToken() )
			return false;
		
		
		$key = 0;
		$value = NULL;
		$count = 0;
		
		while( $tokenizer->HasNextToken() ){
			$token = $tokenizer->GetNextToken();
			
			if( $count % 2 != 0 ){
				$value = $token;
				
				$this->Data[ $key ] = $value;
			}else{
				$key = $token;
			}
			
			$count++;
		}
		
		return true;
	}
	
	//////////////////////////////////////////////////////
	//public function GetArray()
	//  Returns an array that contains the parsed fields
	//  as key/value pairs
	//
	//////////////////////////////////////////////////////
	public function GetArray(){
		return $this->Data;
	}
}

?>
