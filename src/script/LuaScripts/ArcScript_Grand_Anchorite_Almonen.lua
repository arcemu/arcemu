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

Staff of ArcScript Project, Feb 2008
~~End of License Agreement

#############################################################
Special thanks: Janni, Recon, Nandi.
-- ]]

function Almonen_Talk(pUnit,Event)
	pUnit:SendChatMessage(12, 0, "In accordance with my divine calling, I must share these revealed truths to all who would hear.")
	pUnit:RegisterEvent("Almonen_Talk1",140000, 1)
	end

function Almonen_Talk1(pUnit,Event)
	pUnit:SendChatMessage(12, 0, "It has been made known to me that inside each of us, the Light resides...")
	pUnit:RegisterEvent("Almonen_Talk2",80000, 1)
	end


function Almonen_Talk2(pUnit,Event)
	pUnit:SendChatMessage(12, 0, "...that it is a gift, given freely to all naturally born beings.")
	pUnit:RegisterEvent("Almonen_Talk4",81000, 1)
	end

function Almonen_Talk4(pUnit,Event)
	pUnit:SendChatMessage(12, 0, "It manifests itself as a feeling, small at first and easily ignored, that confirms truths and subtly prods one to do good.")
        pUnit:RegisterEvent("Almonen_Talk5",80000, 1)
	end

function Almonen_Talk5(pUnit,Event)
	pUnit:SendChatMessage(12, 0, "Simple kindnesses, charitable deeds, service to those in need. These are all fruits of the Light.")
        pUnit:RegisterEvent("Almonen_Talk6",81000, 1)
	end

function Almonen_Talk6(pUnit,Event)
	pUnit:SendChatMessage(12, 0, "It rewards those who heed its promptings with blessings, both seen and unseen.")
        pUnit:RegisterEvent("Almonen_Talk7",80000, 1)
	end

function Almonen_Talk7(pUnit,Event)
	pUnit:SendChatMessage(12, 0, "Personal reservoirs of hope and faith are strengthened, and one's capacity for greater light increases.")
        pUnit:RegisterEvent("Almonen_Talk8",81000, 1)
	end

function Almonen_Talk8(pUnit,Event)
	pUnit:SendChatMessage(12, 0, "Over the course of time, through obedience to the Light's guidance, one becomes more sensitive to its voice, and its power.")
        pUnit:RegisterEvent("Almonen_Talk9",80000, 1)
	end

function Almonen_Talk9(pUnit,Event)
	pUnit:SendChatMessage(12, 0, "Great is the healing and blessing power of the Light's most diligent followers.")
        pUnit:RegisterEvent("Almonen_Talk10",81000, 1)
	end

function Almonen_Talk10(pUnit,Event)
	pUnit:SendChatMessage(12, 0, "Sadly, there are those who wander through mortality in defiance of the Light.")
        pUnit:RegisterEvent("Almonen_Talk11",80000, 1)
	end

function Almonen_Talk11(pUnit,Event)
	pUnit:SendChatMessage(12, 0, "From these the Light withdraws, until only darkness remains.")
        pUnit:RegisterEvent("Almonen_Talk12",80000, 1)
	end

function Almonen_Talk12(pUnit,Event)
	pUnit:SendChatMessage(12, 0, "Where there is no light, despair, loathing and rage thrive.")
        pUnit:RegisterEvent("Almonen_Talk13",80000, 1)
	end

function Almonen_Talk13(pUnit,Event)
	pUnit:SendChatMessage(12, 0, "We are born into a perilous age, where the forces of darkness are determined to bring about our destruction.")
        pUnit:RegisterEvent("Almonen_Talk14",80000, 1)
	end

function Almonen_Talk14(pUnit,Event)
	pUnit:SendChatMessage(12, 0, "These are the times to find yourselves in unison with the Light.")
        pUnit:RegisterEvent("Almonen_Talk15",80000, 1)
	end

function Almonen_Talk15(pUnit,Event)
	pUnit:SendChatMessage(12, 0, "Hear my words, and let the Light inside of you confirm the truth of them.")
        pUnit:RegisterEvent("Almonen_Talk16",80000, 1)
	end

function Almonen_Talk16(pUnit,Event)
	pUnit:SendChatMessage(12, 0, "Embrace the light that is in you, my friends! Deny yourself all darkness...")
        pUnit:RegisterEvent("Almonen_Talk17",80000, 1)
	end

function Almonen_Talk17(pUnit,Event)
	pUnit:SendChatMessage(12, 0, "We must endure these trying times faithfully, and one day we will find ourselves victorious.")
        pUnit:RegisterEvent("Almonen_Talk18",81000, 1)
	end

function Almonen_Talk18(pUnit,Event)
	pUnit:SendChatMessage(12, 0, "By the naaru, may it be so.")
        pUnit:RegisterEvent("Almonen_Talk",240000, 1)
	end

function Almonen_Died(pUnit, Event)
pUnit:RemoveEvents()
end


RegisterUnitEvent(19216, 18, "Almonen_Talk")
RegisterUnitEvent(19216, 4, "Almonen_Died")
