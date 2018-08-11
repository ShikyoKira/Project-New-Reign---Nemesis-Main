Scriptname FNIS_aa Hidden

bool Function SetAnimGroup(actor akTarget, string animGroup, int animBaseNumber, int number, string modname, bool debugOutput = false) global
	if ( animBaseNumber >= 0 ) && ( number >= 0 ) && ( number <= 9 ) && ( akTarget != none )
		if ( akTarget == Game.GetPlayer() )
			Game.ForceThirdPerson()
		elseif !akTarget.is3dloaded()
			if debugOutput
				Debug.Trace("Target not loaded for Nemesis SetAnimGroup (Mod: " + modname + ", Actor: " + (akTarget.GetBaseObject() as ActorBase).Getname() + ")")
			endif
 			return false
		endif

		int value = animBaseNumber

		if ( value > 0 )
			value += number
		endif

		akTarget.SetAnimationVariableInt("Nemesis_AA_" + animGroup, value)		
		return true
	endif

	Debug.Trace("Invalid parameter for Nemesis SetAnimGroup (Mod: " + modname + ", Actor: " + (akTarget.GetBaseObject() as ActorBase).Getname() + ", Group: " + animGroup + ", AnimBase: " + animBaseNumber + ", Number: " + number + ")")

	return false
endFunction

int Function GetAAmodID(string curAAPrefix, string modname, bool debugOutput = false) global
	int i = Nemesis_AA_Core.getModID(curAAPrefix)

	if(debugOutput)
		if(i == -1)
			Debug.Trace("Mod ID not registered for Nemesis GetAAmodID. Try re-run Nemesis behavior patcher (Mod: " + modname + ", AAPrefix: " + curAAPrefix + ")")
		else
			Debug.Trace("Nemesis GetAAmodID successful (Mod: " + modname + ", AAPrefix: " + curAAprefix + ", ModID:" + i + ")")
		endif
	endif

	return i
endFunction

int Function GetGroupBaseValue(int AAmodID, int AAgroupID, string modname, bool debugOutput = false) global
	if ( AAmodID < 0 ) ||  ( AAmodID > 29 ) || ( AAgroupID < 0 ) || ( AAgroupID > 53 )
		Debug.Trace("Invalid parameter for Nemesis GetGroupBaseValue (Mod: " + modname + ", ModID: " + AAmodID + ", GroupID:" + AAgroupID + ")")
		return -1
	endif
	
	int i = Nemesis_AA_Core.GetBaseValue(AAmodID, AAgroupID)

	if(i != -1)
		return i
	endif
	
	Debug.Trace("Mod/Group ID not registered for Nemesis GetGroupBaseValue. Try re-run Nemesis behavior patcher (Mod: " + modname + ", ModID:" + AAmodID + ", GroupID:" + AAgroupI + ")")  
	return i
endFunction

int[] Function GetAllGroupBaseValues(int AAmodID, string mod, bool debugOutput = false) global
	int[] GroupBaseValue
	GroupBaseValue = new int[54]
	if ( AAmodID < 0 || AAmodID > 29 )
		Debug.Trace("Nemesis Alternate Animation GetAllGroupBaseValues invalid parameter (Mod:" + mod + ", ID: " + AAmodID + ")")
		return GroupBaseValue
	endif
	
	string[] AASet
	int nSets = FNIS_aa2.GetAANumber(1) 
	AASet = FNIS_aa2.GetAAsetList(nSets, mod)
	int i = 0

	while i < nSets
		int Data = AASet[i] as int
		int Prefix = Data / 10000
		int Group = ( Data - Prefix * 10000 ) / 1000
		if ( Prefix == AAmodID )
			GroupBaseValue[Group] = (Data - Prefix * 10000 - Group * 1000)
			if debugOutput
				Debug.Trace("Nemesis Alternate Animation GetAllGroupBaseValues (Group:" + Group + ", Base:" + GroupBaseValue[Group] + ")")
			endif
		endif
		i += 1
	endWhile
	return GroupBaseValue
endFunction

int Function GetInstallationCRC() global
	return Nemesis_AA_Core.getInstallKey()
endFunction

Function GetAAsets(int nSets, int[] groupID, int[] modID, int[] animBase, int[] Index, string mod, bool debugOutput = false) global
	int[] AASet = Nemesis_AA_Core.GetAASetList(nSets)
	int i = 0
	int last = -1

	while (i < nSets)
		int DataCode = AASet[i]
		modID[i] = DataCode / 100000
		groupID[i] = ( DataCode - modID[i] * 100000 ) / 1000
		animBase[i] = ( DataCode - modID[i] * 100000 - groupID[i] * 1000 )

		if ( groupID[i] != last )
			last = groupID[i]
			Index[groupID[i]] = i + 1
		endif

		i += 1
	endWhile
endFunction

int Function _mtidle() global
	return Nemesis_AA_Core._mtidle()
endFunction

int Function _1hmidle() global
	return Nemesis_AA_Core._1hmidle()
endFunction

int Function _2hmidle() global
	return Nemesis_AA_Core._2hmidle()
endFunction

int Function _2hwidle() global
	return Nemesis_AA_Core._2hwidle()
endFunction

int Function _bowidle() global
	return Nemesis_AA_Core._bowidle()
endFunction

int Function _cbowidle() global
	return Nemesis_AA_Core._cbowidle()
endFunction

int Function _h2hidle() global
	return Nemesis_AA_Core._h2hidle()
endFunction

int Function _magidle() global
	return Nemesis_AA_Core._magidle()
endFunction

int Function _sneakidle() global
	return Nemesis_AA_Core._sneakidle()
endFunction

int Function _staffidle() global
	return Nemesis_AA_Core._staffidle()
endFunction

int Function _mt() global
	return Nemesis_AA_Core._mt()
endFunction

int Function _mtx() global
	return Nemesis_AA_Core._mtx()
endFunction

int Function _mtturn() global
	return Nemesis_AA_Core._mtturn()
endFunction

int Function _1hmmt() global
	return Nemesis_AA_Core._1hmmt()
endFunction

int Function _2hmmt() global
	return Nemesis_AA_Core._2hmmt()
endFunction

int Function _bowmt() global
	return Nemesis_AA_Core._bowmt()
endFunction

int Function _magmt() global
	return Nemesis_AA_Core._magmt()
endFunction

int Function _magcastmt() global
	return Nemesis_AA_Core._magcastmt()
endFunction

int Function _sneakmt() global
	return Nemesis_AA_Core._sneakmt()
endFunction

int Function _1hmatk() global
	return Nemesis_AA_Core._1hmatk()
endFunction

int Function _1hmatkpow() global
	return Nemesis_AA_Core._1hmatkpow()
endFunction

int Function _1hmblock() global
	return Nemesis_AA_Core._1hmblock()
endFunction

int Function _1hmstag() global
	return Nemesis_AA_Core._1hmstag()
endFunction

int Function _2hmatk() global
	return Nemesis_AA_Core._2hmatk()
endFunction

int Function _2hmatkpow() global
	return Nemesis_AA_Core._2hmatkpow()
endFunction

int Function _2hmblock() global
	return Nemesis_AA_Core._2hmblock()
endFunction

int Function _2hmstag() global
	return Nemesis_AA_Core._2hmstag()
endFunction

int Function _2hwatk() global
	return Nemesis_AA_Core._2hwatk()
endFunction

int Function _2hwatkpow() global
	return Nemesis_AA_Core._2hwatkpow()
endFunction

int Function _2hwblock() global
	return Nemesis_AA_Core._2hwblock()
endFunction

int Function _2hwstag() global
	return Nemesis_AA_Core._2hwstag()
endFunction

int Function _bowatk() global
	return Nemesis_AA_Core._bowatk()
endFunction

int Function _bowblock() global
	return Nemesis_AA_Core._bowblock()
endFunction

int Function _h2hatk() global
	return Nemesis_AA_Core._h2hatk()
endFunction

int Function _h2hatkpow() global
	return Nemesis_AA_Core._h2hatkpow()
endFunction

int Function _h2hstag() global
	return Nemesis_AA_Core._h2hstag()
endFunction

int Function _magatk() global
	return Nemesis_AA_Core._magatk()
endFunction

int Function _1hmeqp() global
	return Nemesis_AA_Core._1hmeqp()
endFunction

int Function _2hweqp() global
	return Nemesis_AA_Core._2hweqp()
endFunction

int Function _2hmeqp() global
	return Nemesis_AA_Core._2hmeqp()
endFunction

int Function _axeeqp() global
	return Nemesis_AA_Core._axeeqp()
endFunction

int Function _boweqp() global
	return Nemesis_AA_Core._boweqp()
endFunction

int Function _cboweqp() global
	return Nemesis_AA_Core._cboweqp()
endFunction

int Function _dageqp() global
	return Nemesis_AA_Core._dageqp()
endFunction

int Function _h2heqp() global
	return Nemesis_AA_Core._h2heqp()
endFunction

int Function _maceqp() global
	return Nemesis_AA_Core._maceqp()
endFunction

int Function _mageqp() global
	return Nemesis_AA_Core._mageqp()
endFunction

int Function _stfeqp() global
	return Nemesis_AA_Core._stfeqp()
endFunction

int Function _shout() global
	return Nemesis_AA_Core._shout()
endFunction

int Function _magcon() global
	return Nemesis_AA_Core._magcon()
endFunction

int Function _dw() global
	return Nemesis_AA_Core._dw()
endFunction

int Function _jump() global
	return Nemesis_AA_Core._jump()
endFunction

int Function _sprint() global
	return Nemesis_AA_Core._sprint()
endFunction

int Function _shield() global
	return Nemesis_AA_Core._shield()
endFunction
