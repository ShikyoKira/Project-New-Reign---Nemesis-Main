ScriptName Nemesis_PCEA_MCM extends SKI_ConfigBase

FormList property Nemesis_PCEA auto

GlobalVariable property Nemesis_PCEA_ID auto
GlobalVariable property Nemesis_PCEA_RunOnce auto

Actor property PlayerRef auto

string[] PCEAModList

int[] activation

int TogglePCEA0 = -1234567
int TogglePCEA1 = -1234567
int TogglePCEA2 = -1234567
int TogglePCEA3 = -1234567
int TogglePCEA4 = -1234567
int TogglePCEA5 = -1234567
int TogglePCEA6 = -1234567
int TogglePCEA7 = -1234567
int TogglePCEA8 = -1234567
int TogglePCEA9 = -1234567
int TogglePCEA10 = -1234567
int TogglePCEA11 = -1234567
int TogglePCEA12 = -1234567
int TogglePCEA13 = -1234567
int TogglePCEA14 = -1234567
int TogglePCEA15 = -1234567
int TogglePCEA16 = -1234567
int TogglePCEA17 = -1234567
int TogglePCEA18 = -1234567
int TogglePCEA19 = -1234567

bool PCEA0
bool PCEA1
bool PCEA2
bool PCEA3
bool PCEA4
bool PCEA5
bool PCEA6
bool PCEA7
bool PCEA8
bool PCEA9
bool PCEA10
bool PCEA11
bool PCEA12
bool PCEA13
bool PCEA14
bool PCEA15
bool PCEA16
bool PCEA17
bool PCEA18
bool PCEA19

event OnInit()
	parent.onInit()
	StartUp()
endEvent

function StartUp()
	PCEAModList = Nemesis_PCEA_Core.GetPCEAList()
	activation = Nemesis_PCEA_Core.GetActivationList()
	int newPCEA = Nemesis_PCEA_Core.GetPCEAID()
	int pceaID = Nemesis_PCEA_ID.GetValueInt()

	if(Nemesis_PCEA_RunOnce.GetValueInt() == 0)
		Nemesis_PCEA_RunOnce.SetValueInt(1)
		Debug.Trace("Nemesis PCEA Version: " + Nemesis_PCEA_Version.GetVersion())
		Debug.Trace("Nemesis - Old PCEA ID: " + pceaID)
		Debug.Trace("Nemesis - New PCEA ID: " + newPCEA)
		int counter = 0

		while(counter < PCEAModList.Length)
			Debug.Trace("Nemesis - PCEA Mod: " + PCEAModList[counter])
			counter += 1
		endWhile

		if(pceaID != newPCEA)
			PCEA0 = false
			PCEA1 = false
			PCEA2 = false
			PCEA3 = false
			PCEA4 = false
			PCEA5 = false
			PCEA6 = false
			PCEA7 = false
			PCEA8 = false
			PCEA9 = false
			PCEA10 = false
			PCEA11 = false
			PCEA12 = false
			PCEA13 = false
			PCEA14 = false
			PCEA15 = false
			PCEA16 = false
			PCEA17 = false
			PCEA18 = false
			PCEA19 = false
			Nemesis_PCEA_ID.SetValueInt(newPCEA)
			counter = 0

			while(counter < 20)
				(Nemesis_PCEA.GetAt(counter) as globalvariable).SetValueInt(0)
				counter += 1
			endWhile		
		else
			PCEA0 = (Nemesis_PCEA.GetAt(0) as globalvariable).GetValueInt() as bool
			PCEA1 = (Nemesis_PCEA.GetAt(1) as globalvariable).GetValueInt() as bool
			PCEA2 = (Nemesis_PCEA.GetAt(2) as globalvariable).GetValueInt() as bool
			PCEA3 = (Nemesis_PCEA.GetAt(3) as globalvariable).GetValueInt() as bool
			PCEA4 = (Nemesis_PCEA.GetAt(4) as globalvariable).GetValueInt() as bool
			PCEA5 = (Nemesis_PCEA.GetAt(5) as globalvariable).GetValueInt() as bool
			PCEA6 = (Nemesis_PCEA.GetAt(6) as globalvariable).GetValueInt() as bool
			PCEA7 = (Nemesis_PCEA.GetAt(7) as globalvariable).GetValueInt() as bool
			PCEA8 = (Nemesis_PCEA.GetAt(8) as globalvariable).GetValueInt() as bool
			PCEA9 = (Nemesis_PCEA.GetAt(9) as globalvariable).GetValueInt() as bool
			PCEA10 = (Nemesis_PCEA.GetAt(10) as globalvariable).GetValueInt() as bool
			PCEA11 = (Nemesis_PCEA.GetAt(11) as globalvariable).GetValueInt() as bool
			PCEA12 = (Nemesis_PCEA.GetAt(12) as globalvariable).GetValueInt() as bool
			PCEA13 = (Nemesis_PCEA.GetAt(13) as globalvariable).GetValueInt() as bool
			PCEA14 = (Nemesis_PCEA.GetAt(14) as globalvariable).GetValueInt() as bool
			PCEA15 = (Nemesis_PCEA.GetAt(15) as globalvariable).GetValueInt() as bool
			PCEA16 = (Nemesis_PCEA.GetAt(16) as globalvariable).GetValueInt() as bool
			PCEA17 = (Nemesis_PCEA.GetAt(17) as globalvariable).GetValueInt() as bool
			PCEA18 = (Nemesis_PCEA.GetAt(18) as globalvariable).GetValueInt() as bool
			PCEA19 = (Nemesis_PCEA.GetAt(19) as globalvariable).GetValueInt() as bool
		endif
	endif
endFunction

event OnPageReset(string page)
	StartUp()
	SetCursorFillMode(TOP_TO_BOTTOM)
	SetCursorPosition(0)
	AddHeaderOption("Animation Pack(s)")
	int counter = 0

	if(counter < activation.Length)
		TogglePCEA0 = AddToggleOption(PCEAModList[0], PCEA0, activation[0])
		counter += 1

		if(counter < activation.Length)
			TogglePCEA1 = AddToggleOption(PCEAModList[1], PCEA1, activation[1])
			counter += 1

			if(counter < activation.Length)
				TogglePCEA2 = AddToggleOption(PCEAModList[2], PCEA2, activation[2])
				counter += 1

				if(counter < activation.Length)
					TogglePCEA3 = AddToggleOption(PCEAModList[3], PCEA3, activation[3])
					counter += 1

					if(counter < activation.Length)
						TogglePCEA4 = AddToggleOption(PCEAModList[4], PCEA4, activation[4])
						counter += 1

						if(counter < activation.Length)
							TogglePCEA5 = AddToggleOption(PCEAModList[5], PCEA5, activation[5])
							counter += 1

							if(counter < activation.Length)
								TogglePCEA6 = AddToggleOption(PCEAModList[6], PCEA6, activation[6])
								counter += 1

								if(counter < activation.Length)
									TogglePCEA7 = AddToggleOption(PCEAModList[7], PCEA7, activation[7])
									counter += 1

									if(counter < activation.Length)
										TogglePCEA8 = AddToggleOption(PCEAModList[8], PCEA8, activation[8])
										counter += 1

										if(counter < activation.Length)
											TogglePCEA9 = AddToggleOption(PCEAModList[9], PCEA9, activation[9])
											counter += 1
																					
											if(counter < activation.Length)
												TogglePCEA10 = AddToggleOption(PCEAModList[10], PCEA10, activation[10])
												counter += 1

												if(counter < activation.Length)
													TogglePCEA11 = AddToggleOption(PCEAModList[11], PCEA11, activation[11])
													counter += 1

													if(counter < activation.Length)
														TogglePCEA12 = AddToggleOption(PCEAModList[12], PCEA12, activation[12])
														counter += 1

														if(counter < activation.Length)
															TogglePCEA13 = AddToggleOption(PCEAModList[13], PCEA13, activation[13])
															counter += 1

															if(counter < activation.Length)
																TogglePCEA14 = AddToggleOption(PCEAModList[14], PCEA14, activation[14])
																counter += 1

																if(counter < activation.Length)
																	TogglePCEA15 = AddToggleOption(PCEAModList[15], PCEA15, activation[15])
																	counter += 1

																	if(counter < activation.Length)
																		TogglePCEA16 = AddToggleOption(PCEAModList[16], PCEA16, activation[16])
																		counter += 1

																		if(counter < activation.Length)
																			TogglePCEA17 = AddToggleOption(PCEAModList[17], PCEA17, activation[17])
																			counter += 1

																			if(counter < activation.Length)
																				TogglePCEA18 = AddToggleOption(PCEAModList[18], PCEA18, activation[18])
																				counter += 1

																				if(counter < activation.Length)
																					TogglePCEA19 = AddToggleOption(PCEAModList[19], PCEA19, activation[19])
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
								endif
							endif
						endif
					endif
				endif
			endif
		endif
	endif
endEvent

event OnOptionSelect(int option)
	int current
	int toggle
	bool curState

	if(option == TogglePCEA0)
		current = 0
		PCEA0 = !PCEA0
		SetToggleOptionValue(TogglePCEA0, PCEA0, false)
		curState = PCEA0
	elseif(option == TogglePCEA1)
		current = 1
		PCEA1 = !PCEA1
		SetToggleOptionValue(TogglePCEA1, PCEA1, false)
		curState = PCEA1
	elseif(option == TogglePCEA2)
		current = 2
		PCEA2 = !PCEA2
		SetToggleOptionValue(TogglePCEA2, PCEA2, false)
		curState = PCEA2
	elseif(option == TogglePCEA3)
		current = 3
		PCEA3 = !PCEA3
		SetToggleOptionValue(TogglePCEA3, PCEA3, false)
		curState = PCEA3
	elseif(option == TogglePCEA4)
		current = 4
		PCEA4 = !PCEA4
		SetToggleOptionValue(TogglePCEA4, PCEA4, false)
		curState = PCEA4
	elseif(option == TogglePCEA5)
		current = 5
		PCEA5 = !PCEA5
		SetToggleOptionValue(TogglePCEA5, PCEA5, false)
		curState = PCEA5
	elseif(option == TogglePCEA6)
		current = 6
		PCEA6 = !PCEA6
		SetToggleOptionValue(TogglePCEA6, PCEA6, false)
		curState = PCEA6
	elseif(option == TogglePCEA7)
		current = 7
		PCEA7 = !PCEA7
		SetToggleOptionValue(TogglePCEA7, PCEA7, false)
		curState = PCEA7
	elseif(option == TogglePCEA8)
		current = 8
		PCEA8 = !PCEA8
		SetToggleOptionValue(TogglePCEA8, PCEA8, false)
		curState = PCEA8
	elseif(option == TogglePCEA9)
		current = 9
		PCEA9 = !PCEA9
		SetToggleOptionValue(TogglePCEA9, PCEA9, false)
		curState = PCEA9
	elseif(option == TogglePCEA10)
		current = 10
		PCEA10 = !PCEA10
		SetToggleOptionValue(TogglePCEA10, PCEA10, false)
		curState = PCEA10
	elseif(option == TogglePCEA11)
		current = 11
		PCEA11 = !PCEA11
		SetToggleOptionValue(TogglePCEA11, PCEA11, false)
		curState = PCEA11
	elseif(option == TogglePCEA12)
		current = 12
		PCEA12 = !PCEA12
		SetToggleOptionValue(TogglePCEA12, PCEA12, false)
		curState = PCEA12
	elseif(option == TogglePCEA13)
		current = 13
		PCEA13 = !PCEA13
		SetToggleOptionValue(TogglePCEA13, PCEA13, false)
		curState = PCEA13
	elseif(option == TogglePCEA14)
		current = 14
		PCEA14 = !PCEA14
		SetToggleOptionValue(TogglePCEA14, PCEA14, false)
		curState = PCEA14
	elseif(option == TogglePCEA15)
		current = 15
		PCEA15 = !PCEA15
		SetToggleOptionValue(TogglePCEA15, PCEA15, false)
		curState = PCEA15
	elseif(option == TogglePCEA16)
		current = 16
		PCEA16 = !PCEA16
		SetToggleOptionValue(TogglePCEA16, PCEA16, false)
		curState = PCEA16
	elseif(option == TogglePCEA17)
		current = 17
		PCEA17 = !PCEA17
		SetToggleOptionValue(TogglePCEA17, PCEA17, false)
		curState = PCEA17
	elseif(option == TogglePCEA18)
		current = 18
		PCEA18 = !PCEA18
		SetToggleOptionValue(TogglePCEA18, PCEA18, false)
		curState = PCEA18
	elseif(option == TogglePCEA19)
		current = 19
		PCEA19 = !PCEA19
		SetToggleOptionValue(TogglePCEA19, PCEA19, false)
		curState = PCEA19
	endif

	if(curState == true)
		toggle = 1
	else
		toggle = 0
	endif

	Debug.Trace("Nemesis PCEA - " + PCEAModList[current] + ": " + curState as string)
	(Nemesis_PCEA.GetAt(current) as globalvariable).SetValueInt(toggle)
	PlayerRef.SetAnimationVariableInt(PCEAModList[current], toggle)
endEvent
