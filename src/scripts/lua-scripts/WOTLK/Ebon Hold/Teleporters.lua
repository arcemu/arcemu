 --[[
   ******************************************************************
   *	 _____              ___                           _         *
   *	(  _  )            (  _`\               _        ( )_       *
   *	| (_) | _ __   ___ | (_(_)   ___  _ __ (_) _ _   | ,_)      *
   *	|  _  |( '__)/'___)`\__ \  /'___)( '__)| |( '_`\ | |        *
   *	| | | || |  ( (___ ( )_) |( (___ | |   | || (_) )| |_       *
   *	(_) (_)(_)  `\____)`\____)`\____)(_)   (_)| ,__/'`\__)      *
   *	                                          | |               *
   *	                                          (_)               *
   *	                                                            *
   *	               OpenSource Scripting Team                    *
   *	                <http://www.arcemu.org>                     *
   *	                                                            *
   ******************************************************************
  
   This software is provided as free and open source by the
staff of The ArcScript Project, in accordance with 
the GPL license. This means we provide the software we have 
created freely and it has been thoroughly tested to work for 
the developers, but NO GUARANTEE is made it will work for you 
as well. Please give credit where credit is due, if modifying,
redistributing and/or using this software. Thank you.

Staff of ArcScript Project, Dec 2008
~~End of License Agreement

#############################################################
Made by: BrantX
-- ]]

function TeleportDown_AI(pUnit,Event)
	pUnit:RegsiterEvent("TeleportDown_DownFront", 1000, 0)
	pUnit:RegsiterEvent("TeleportDown_DownBack", 1000, 0)
end

function TeleportDown_DownFront(pUnit,Event)
 if pUnit:IsInFront() then
	pUnit:Teleport(609, 2389.989990, -5640.930176, 377.266998)
end
end

function TeleportDown_DownBack(pUnit,Event)
 if pUnit:IsInBack() then
	pUnit:Teleport(609, 2389.989990, -5640.930176, 377.266998)
end
end

RegisterUnitEvent(29581, 10, "TeleportDown_AI")

function TeleportUp_AI(pUnit,Event)
	pUnit:RegsiterEvent("TeleportDown_UpFront", 1000, 0)
	pUnit:RegsiterEvent("TeleportDown_UpBack", 1000, 0)
end

function TeleportUp_UpFront(pUnit,Event)
 if pUnit:IsInFront() then
	pUnit:Teleport(609, 2383.649902, -5645.240234, 420.880005)
end
end

function TeleportUp_UpBack(pUnit,Event)
 if pUnit:IsInBack() then
	pUnit:Teleport(609, 2383.649902, -5645.240234, 420.880005)
end
end

RegisterUnitEvent(29580, 10, "TeleportUp_AI")