from pathlib import Path
from shulit

my_file = Path("../XPMSE.esp")

if my_file.is_file():
    fnis = Path("../FNIS.esp")
	
	if fnis.is_file():
		file = Path("../../binkw32.dll")
		dst = '../FNIS.esp'
		
		if file.is_file():
			shulit.copy2('scripts/launcher/end/XPMSE/fnis', dst)
		else:
			shulit.copy2('scripts/launcher/end/XPMSE/fnis_sse', dst)
		
	fnis = Path("../scripts/FNISVersion.pex")
	
	if fnis.is_file():
		# create FNIS version, allowing FNIS based mods hook to a proxy
		shulit.copy2('scripts/launcher/end/XPMSE/script1', '../scripts/FNISVersion.pex')
	
	fnis = Path("../scripts/FNISVersionGenerated.pex")
	
	if fnis.is_file():
		# create FNIS version generated, allowing FNIS based mods hook to a proxy
		shulit.copy2('scripts/launcher/end/XPMSE/script2', '../scripts/FNISVersionGenerated.pex')
	
