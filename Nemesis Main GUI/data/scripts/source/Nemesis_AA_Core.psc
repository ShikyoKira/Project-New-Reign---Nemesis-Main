Scriptname Nemesis_AA_Core Hidden

int Function getModID(string curAAPrefix) global
	if(curAAPrefix == "aa0")
		return 0
	elseif(curAAPrefix == "aa1")
		return 1
	elseif(curAAPrefix == "aa2")
		return 2
	elseif(curAAPrefix == "aa3")
		return 3
	elseif(curAAPrefix == "aa4")
		return 4
	elseif(curAAPrefix == "aa5")
		return 5
	elseif(curAAPrefix == "aa6")
		return 6
	elseif(curAAPrefix == "aa7")
		return 7
	elseif(curAAPrefix == "aa8")
		return 8
	elseif(curAAPrefix == "aa9")
		return 9
	elseif(curAAPrefix == "a10")
		return 10
	elseif(curAAPrefix == "a11")
		return 11
	elseif(curAAPrefix == "a12")
		return 12
	elseif(curAAPrefix == "a13")
		return 13
	elseif(curAAPrefix == "a14")
		return 14
	elseif(curAAPrefix == "a15")
		return 15
	elseif(curAAPrefix == "a16")
		return 16
	elseif(curAAPrefix == "a17")
		return 17
	elseif(curAAPrefix == "a18")
		return 18
	elseif(curAAPrefix == "a19")
		return 19
	elseif(curAAPrefix == "a20")
		return 20
	elseif(curAAPrefix == "a21")
		return 21
	elseif(curAAPrefix == "a22")
		return 22
	elseif(curAAPrefix == "a23")
		return 23
	elseif(curAAPrefix == "a24")
		return 24
	elseif(curAAPrefix == "a25")
		return 25
	elseif(curAAPrefix == "a26")
		return 26
	elseif(curAAPrefix == "a27")
		return 27
	elseif(curAAPrefix == "a28")
		return 28
	elseif(curAAPrefix == "a29")
		return 29
	endif

	return -1
endFunction

int[] Function GetAASetList(int nSet) global
	int[] AASet
	AASet[0] = 0101000
	AASet[0] = 0101000
	AASet[1] = 0101001
	AASet[2] = 0101002
	AASet[3] = 0101003
	AASet[4] = 0101004
	AASet[5] = 0101005
	AASet[6] = 0101006
	AASet[7] = 0101007
	AASet[8] = 0101008
	AASet[9] = 0101009

	if(nSet > 10)
		AASet[10] = 0101010
		AASet[11] = 0101011
		AASet[12] = 0101012
		AASet[13] = 0101013
		AASet[14] = 0101014
		AASet[15] = 0101015
		AASet[16] = 0101016
		AASet[17] = 0101017
		AASet[18] = 0101018
		AASet[19] = 0101019

		if(nSet > 20)
			AASet[20] = 0101020
			AASet[21] = 0101021
			AASet[22] = 0101022				
			AASet[23] = 0101023
			AASet[24] = 0101024
			AASet[25] = 0101025
			AASet[26] = 0101026
			AASet[27] = 0101027
			AASet[28] = 0101028
			AASet[29] = 0101029

			if(nSet > 30)
				AASet[30] = 0101030
				AASet[31] = 0101031
				AASet[32] = 0101032
				AASet[33] = 0101033
				AASet[34] = 0101034
				AASet[35] = 0101035
				AASet[36] = 0101036
				AASet[37] = 0101037
				AASet[38] = 0101038
				AASet[39] = 0101039

				if(nSet > 40)
					AASet[40] = 0101040
					AASet[41] = 0101041
					AASet[42] = 0101042
					AASet[43] = 0101043
					AASet[44] = 0101044
					AASet[45] = 0101045
					AASet[46] = 0101046
					AASet[47] = 0101047
					AASet[48] = 0101048
					AASet[49] = 0101049

					if(nSet > 50)
						AASet[50] = 0101050
						AASet[51] = 0101051
						AASet[52] = 0101052
						AASet[53] = 0101053
						AASet[54] = 0101054
						AASet[55] = 0101055
						AASet[56] = 0101056
						AASet[57] = 0101057
						AASet[58] = 0101058
						AASet[59] = 0101059

						if(nSet > 60)
							AASet[60] = 0101060
							AASet[61] = 0101061
							AASet[62] = 0101062
							AASet[63] = 0101063
							AASet[64] = 0101064
							AASet[65] = 0101065
							AASet[66] = 0101066
							AASet[67] = 0101067
							AASet[68] = 0101068
							AASet[69] = 0101069

							if(nSet > 70)
								AASet[70] = 0101070
								AASet[71] = 0101071
								AASet[72] = 0101072
								AASet[73] = 0101073
								AASet[74] = 0101074
								AASet[75] = 0101075
								AASet[76] = 0101076
								AASet[77] = 0101077
								AASet[78] = 0101078
								AASet[79] = 0101079

								if(nSet > 80)
									AASet[80] = 0101080
									AASet[81] = 0101081
									AASet[82] = 0101082
									AASet[83] = 0101083
									AASet[84] = 0101084
									AASet[85] = 0101085
									AASet[86] = 0101086
									AASet[87] = 0101087
									AASet[88] = 0101088
									AASet[89] = 0101089

									if(nSet > 90)
										AASet[90] = 0101090
										AASet[91] = 0101091
										AASet[92] = 0101092
										AASet[93] = 0101093
										AASet[94] = 0101094
										AASet[95] = 0101095
										AASet[96] = 0101096
										AASet[97] = 0101097
										AASet[98] = 0101098
										AASet[99] = 0101099

										if(nSet > 100)
											AASet[100] = 0101100
											AASet[101] = 0101101
											AASet[102] = 0101102
											AASet[103] = 0101103
											AASet[104] = 0101104
											AASet[105] = 0101105
											AASet[106] = 0101106
											AASet[107] = 0101107
											AASet[108] = 0101108
											AASet[109] = 0101109

											if(nSet > 110)
												AASet[110] = 0101110
												AASet[111] = 0101111
												AASet[112] = 0101112
												AASet[113] = 0101113
												AASet[114] = 0101114
												AASet[115] = 0101115
												AASet[116] = 0101116
												AASet[117] = 0101117
												AASet[118] = 0101118
												AASet[119] = 0101119

												if(nSet > 120)
													AASet[120] = 0101120
													AASet[121] = 0101121
													AASet[122] = 0101122
													AASet[123] = 0101123
													AASet[124] = 0101124
													AASet[125] = 0101125
													AASet[126] = 0101126
													AASet[127] = 0101127
												endif
											endif
										endif
									endif
								endif
							endif
						endif
					endif
				endif
			endif
		endif
	endif

	return AASet
endFunction

int Function GetBaseValue(int AAmodID, int AAgroupID) global
	int nSet = 0
	int[] AASet
	AASet = GetAASetList(nSet)
	int i = 0

	while(i < nSet)
		int DataCode = AASet[i]
		int modID = DataCode / 100000

		if(modID == AAmodID)				
			int GroupID = (DataCode - modID * 100000) / 1000

			if(GroupID == AAgroupID)
				return DataCode - (modID * 100000) - (GroupID * 1000)
			elseif(GroupID > AAgroupID)
				i = nSet
			endif
		endif

		i += 1
	endWhile

	return -1
endFunction

int Function getInstallKey() global
	return 455112
endFunction

int Function _1hmatk() global
	return 0
endFunction

int Function _1hmatkpow() global
	return 1
endFunction

int Function _1hmblock() global
	return 2
endFunction

int Function _1hmeqp() global
	return 3
endFunction

int Function _1hmidle() global
	return 4
endFunction

int Function _1hmmt() global
	return 5
endFunction

int Function _1hmstag() global
	return 6
endFunction

int Function _2hmatk() global
	return 7
endFunction

int Function _2hmatkpow() global
	return 8
endFunction

int Function _2hmblock() global
	return 9
endFunction

int Function _2hmeqp() global
	return 10
endFunction

int Function _2hmidle() global
	return 11
endFunction

int Function _2hmmt() global
	return 12
endFunction

int Function _2hmstag() global
	return 13
endFunction

int Function _2hwatk() global
	return 14
endFunction

int Function _2hwatkpow() global
	return 15
endFunction

int Function _2hwblock() global
	return 16
endFunction

int Function _2hweqp() global
	return 17
endFunction

int Function _2hwidle() global
	return 18
endFunction

int Function _2hwstag() global
	return 19
endFunction

int Function _axeeqp() global
	return 20
endFunction

int Function _bowatk() global
	return 21
endFunction

int Function _bowblock() global
	return 22
endFunction

int Function _boweqp() global
	return 23
endFunction

int Function _bowidle() global
	return 24
endFunction

int Function _bowmt() global
	return 25
endFunction

int Function _cboweqp() global
	return 26
endFunction

int Function _cbowidle() global
	return 27
endFunction

int Function _dageqp() global
	return 28
endFunction

int Function _dw() global
	return 29
endFunction

int Function _h2hatk() global
	return 30
endFunction

int Function _h2hatkpow() global
	return 31
endFunction

int Function _h2heqp() global
	return 32
endFunction

int Function _h2hidle() global
	return 33
endFunction

int Function _h2hstag() global
	return 34
endFunction

int Function _jump() global
	return 35
endFunction

int Function _maceqp() global
	return 36
endFunction

int Function _magatk() global
	return 37
endFunction

int Function _magcastmt() global
	return 38
endFunction

int Function _magcon() global
	return 39
endFunction

int Function _mageqp() global
	return 40
endFunction

int Function _magidle() global
	return 41
endFunction

int Function _magmt() global
	return 42
endFunction

int Function _mt() global
	return 43
endFunction

int Function _mtidle() global
	return 44
endFunction

int Function _mtturn() global
	return 45
endFunction

int Function _mtx() global
	return 46
endFunction

int Function _shield() global
	return 47
endFunction

int Function _shout() global
	return 48
endFunction

int Function _sneakidle() global
	return 49
endFunction

int Function _sneakmt() global
	return 50
endFunction

int Function _sprint() global
	return 51
endFunction

int Function _staffidle() global
	return 52
endFunction

int Function _stfeqp() global
	return 53
endFunction

