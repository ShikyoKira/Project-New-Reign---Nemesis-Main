Scriptname Nemesis_AA_Core Hidden

int Function GetModID(string curAAPrefix) global
	if(curAAPrefix == "xpe")
		return 0
	endif
	return -1
endFunction

int[] Function GetAASetList(int nSet) global
	int[] AASet
	AASet = new int[16]
	AASet[0] = 0003001
	AASet[1] = 0010001
	AASet[2] = 0017001
	AASet[3] = 0020001
	AASet[4] = 0021001
	AASet[5] = 0023001
	AASet[6] = 0024001
	AASet[7] = 0028001
	AASet[8] = 0036001
	AASet[9] = 0037001
	AASet[10] = 0038001
	AASet[11] = 0039001
	AASet[12] = 0041001
	AASet[13] = 0042001
	AASet[14] = 0048001
	AASet[15] = 0051001
	return AASet
endFunction

int Function GetGroupOrder(int AAgroupID) global
	if(AAgroupID == 36)
		return 2
	elseif(AAgroupID == 21)
		return 2
	elseif(AAgroupID == 10)
		return 2
	elseif(AAgroupID == 03)
		return 2
	elseif(AAgroupID == 17)
		return 2
	elseif(AAgroupID == 28)
		return 2
	elseif(AAgroupID == 20)
		return 2
	elseif(AAgroupID == 23)
		return 2
	elseif(AAgroupID == 24)
		return 2
	elseif(AAgroupID == 37)
		return 2
	elseif(AAgroupID == 41)
		return 2
	elseif(AAgroupID == 38)
		return 2
	elseif(AAgroupID == 39)
		return 2
	elseif(AAgroupID == 42)
		return 2
	elseif(AAgroupID == 48)
		return 2
	elseif(AAgroupID == 51)
		return 2
	endif

	Debug.Trace("ERROR: Unknown Nemesis AAgroupID. Failed to GetGroupOrder (AA Group ID: " + AAgroupID as string + ")")
	return -1
endFunction

int Function GetModIDByGroupValue(int AAgroupID, int groupValue) global
	if(AAgroupID == 36)
		return 00
	elseif(AAgroupID == 21)
		return 00
	elseif(AAgroupID == 10)
		return 00
	elseif(AAgroupID == 03)
		return 00
	elseif(AAgroupID == 17)
		return 00
	elseif(AAgroupID == 28)
		return 00
	elseif(AAgroupID == 20)
		return 00
	elseif(AAgroupID == 23)
		return 00
	elseif(AAgroupID == 51)
		return 00
	elseif(AAgroupID == 24)
		return 00
	elseif(AAgroupID == 37)
		return 00
	elseif(AAgroupID == 38)
		return 00
	elseif(AAgroupID == 39)
		return 00
	elseif(AAgroupID == 41)
		return 00
	elseif(AAgroupID == 42)
		return 00
	elseif(AAgroupID == 48)
		return 00
	endif
		

	Debug.Trace("ERROR: Failed to GetModIDByGroupValue (AA Group ID: " + AAgroupID as string + ", Value: " + groupValue + ")")
	return -1
endFunction

int Function GetBaseValue(int AAmodID, int AAgroupID) global
	int DataCode = AAmodID * 100000 + AAgroupID * 1000

	if(DataCode == 0003000)
		return 1
	elseif(DataCode == 0010000)
		return 1
	elseif(DataCode == 0017000)
		return 1
	elseif(DataCode == 0020000)
		return 1
	elseif(DataCode == 0021000)
		return 1
	elseif(DataCode == 0023000)
		return 1
	elseif(DataCode == 0024000)
		return 1
	elseif(DataCode == 0028000)
		return 1
	elseif(DataCode == 0036000)
		return 1
	elseif(DataCode == 0037000)
		return 1
	elseif(DataCode == 0038000)
		return 1
	elseif(DataCode == 0039000)
		return 1
	elseif(DataCode == 0041000)
		return 1
	elseif(DataCode == 0042000)
		return 1
	elseif(DataCode == 0048000)
		return 1
	elseif(DataCode == 0051000)
		return 1
	endif


	Debug.Trace("ERROR: Unknown Nemesis mod/group ID. Failed to GetBaseValue (Mod ID: " + AAmodID as string + ", AA Group ID: " + AAgroupID as string + ")")
	return -1
endFunction

int[] Function GetAllBaseValue(int AAmodID) global
	int nSet = 16
	int[] AASet
	AASet = GetAASetList(nSet)
	int i = 0
	int[] GroupBaseValue
	GroupBaseValue = new int[54]

	while(i < nSet)
		int DataCode = AASet[i]
		int modID = DataCode / 100000
		int GroupID = (DataCode - modID * 100000) / 1000
		if(modID == AAmodID)				
			GroupBaseValue[GroupID] = (DataCode - modID * 10000 - GroupID * 1000)
		endif

		i += 1
	endWhile

	return GroupBaseValue
endFunction

int Function GetGroupID(string groupName) global
	if groupName == "_1hmatk"
		return 0
	elseif groupName == "_1hmatkpow"
		return 1
	elseif groupName == "_1hmblock"
		return 2
	elseif groupName == "_1hmeqp"
		return 3
	elseif groupName == "_1hmidle"
		return 4
	elseif groupName == "_1hmmt"
		return 5
	elseif groupName == "_1hmstag"
		return 6
	elseif groupName == "_2hmatk"
		return 7
	elseif groupName == "_2hmatkpow"
		return 8
	elseif groupName == "_2hmblock"
		return 9
	elseif groupName == "_2hmeqp"
		return 10
	elseif groupName == "_2hmidle"
		return 11
	elseif groupName == "_2hmmt"
		return 12
	elseif groupName == "_2hmstag"
		return 13
	elseif groupName == "_2hwatk"
		return 14
	elseif groupName == "_2hwatkpow"
		return 15
	elseif groupName == "_2hwblock"
		return 16
	elseif groupName == "_2hweqp"
		return 17
	elseif groupName == "_2hwidle"
		return 18
	elseif groupName == "_2hwstag"
		return 19
	elseif groupName == "_axeeqp"
		return 20
	elseif groupName == "_bowatk"
		return 21
	elseif groupName == "_bowblock"
		return 22
	elseif groupName == "_boweqp"
		return 23
	elseif groupName == "_bowidle"
		return 24
	elseif groupName == "_bowmt"
		return 25
	elseif groupName == "_cboweqp"
		return 26
	elseif groupName == "_cbowidle"
		return 27
	elseif groupName == "_dageqp"
		return 28
	elseif groupName == "_dw"
		return 29
	elseif groupName == "_h2hatk"
		return 30
	elseif groupName == "_h2hatkpow"
		return 31
	elseif groupName == "_h2heqp"
		return 32
	elseif groupName == "_h2hidle"
		return 33
	elseif groupName == "_h2hstag"
		return 34
	elseif groupName == "_jump"
		return 35
	elseif groupName == "_maceqp"
		return 36
	elseif groupName == "_magatk"
		return 37
	elseif groupName == "_magcastmt"
		return 38
	elseif groupName == "_magcon"
		return 39
	elseif groupName == "_mageqp"
		return 40
	elseif groupName == "_magidle"
		return 41
	elseif groupName == "_magmt"
		return 42
	elseif groupName == "_mt"
		return 43
	elseif groupName == "_mtidle"
		return 44
	elseif groupName == "_mtturn"
		return 45
	elseif groupName == "_mtx"
		return 46
	elseif groupName == "_shield"
		return 47
	elseif groupName == "_shout"
		return 48
	elseif groupName == "_sneakidle"
		return 49
	elseif groupName == "_sneakmt"
		return 50
	elseif groupName == "_sprint"
		return 51
	elseif groupName == "_staffidle"
		return 52
	elseif groupName == "_stfeqp"
		return 53
	endif
	Debug.Trace("ERROR: Unknown Nemesis AA group name (Group Name: " + groupName + ")")
	return -1
endFunction

