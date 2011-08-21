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
//class ArcemuBugReportTypeFieldParser
//  Simple Arcemu bugreport type field parser class
//
////////////////////////////////////////////////////////
class BugReportTypeFieldParser{
	private $BugReport;
	private $Data;
	
	public function BugReportTypeFieldParser( $str  ){
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
			new StringTokenizer( $this->BugReport, "</>" );
		
		if( !$tokenizer->HasNextToken() )
			return false;
		
		
		$key = 0;
		$value = NULL;
		$intag = false;
		
		while( $tokenizer->HasNextToken() ){
			$i = 0;
			
			if( $intag == false ){
				$key = $tokenizer->GetNextToken();
				if( !is_numeric( $key ) )
					return false;
				
				$intag = true;
			}else{
				if( $value == NULL ){
					$value = $tokenizer->GetNextToken();
				}else{
					$i = $tokenizer->GetNextToken();
					if( is_numeric( $i ) && ( $i == $key ) ){
						$this->Data[ $key ] = $value;
						$intag = false;
						$value = NULL;
						$key = 0;
						$i = 0;
					}else{
						return false;
					}
				}
			}
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
