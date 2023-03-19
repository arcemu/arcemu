/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008-2022 <http://www.ArcEmu.org/>
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

#ifndef ARCEMU_PYTHON_ENGINE_HPP
#define ARCEMU_PYTHON_ENGINE_HPP

class ArcPython;
class ScriptMgr;

/// Arcemu Python Engine (APE)
class PythonEngine
{
public:
	///
	/// Initializes the Python Engine
	///
	/// Parameters
	///   mgr   -   Reference to the core script manager
	///
	/// Return value
	///   None
	///
	PythonEngine( ScriptMgr *mgr );

	~PythonEngine();

	/// Executes the startup tasks: loads script files and registers the scripts
	void onStartup();

	/// Executes the reload tasks: clears registered script functions, loads current scripts and registers the functions from them
	void onReload();

private:
	///
	/// Load a single script file
	///
	/// Parameters
	///   fileName  -  Path to the file to be loaded
	///
	/// Return value
	///   Returns 0 on success.
	///   Returns -1 otherwise
	///
	int loadScript( const char *fileName );

	///
	/// Load all current scripts from the script directory
	/// 
	/// Parameters
	///   None
	///
	/// Return value
	///   Returns the number of successfully loaded scripts
	///
	int loadScripts();

	/// Register server hooks
	void registerHooks();

	/// Register loaded script functions in ScriptManager
	void registerScripts();

private:
	/// Reference to the Arcemu Python engine wrapper
	ArcPython *python;

	/// Reference to the core's script manager
	ScriptMgr *mgr;
};

#endif
