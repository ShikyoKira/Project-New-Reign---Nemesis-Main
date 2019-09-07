#ifndef UPDATEPROCESS_H_
#define UPDATEPROCESS_H_

#include <QtCore\QObject>

#include "Global.h"
#include "progressup.h"
#include "behaviortype.h"

#include "add animation\import.h"
#include "add animation\animationdata.h"

class NemesisMainGUI;
class arguPack;

class UpdateFilesStart : public QObject
{
	Q_OBJECT

public:
	bool cmdline = false;

	UpdateFilesStart();
	virtual ~UpdateFilesStart();
	void milestoneStart(std::string directory);
	void message(std::string input);
	void GetFileLoop(std::string newPath);
	bool VanillaUpdate(std::unordered_map<std::string, std::map<std::string, vecstr>>& newFile, MapChildState& childrenState, SSSMap& stateID, SSSMap& n_parent,
		MasterAnimData& animData, MasterAnimSetData& animSetData);
	bool GetPathLoop(std::string newPath, std::unordered_map<std::string, std::map<std::string, vecstr>>* newFile, MapChildState* childrenState, SSSMap* stateID,
		SSSMap* n_parent, MasterAnimData* animData, MasterAnimSetData* animSetData, bool isFirstPerson);
	bool VanillaDisassemble(std::string path, std::string filename, std::map<std::string, vecstr>* newFile, MapChildState& childrenState, SSMap& stateID, SSMap& n_parent);
	bool AnimDataDisassemble(std::string path, MasterAnimData& animData);
	bool AnimSetDataDisassemble(std::string path, MasterAnimSetData& animSetData);
	bool newAnimUpdate(std::string sourcefolder, std::unordered_map<std::string, std::map<std::string, vecstr>>& newFile, MasterAnimData& animData, MasterAnimSetData& animSetData,
		std::unordered_map<std::string, std::string>& lastUpdate);
	void SeparateMod(std::unique_ptr<arguPack> pack);
	void JoiningEdits(std::string directory, std::unordered_map<std::string, std::map<std::string, vecstr>>& newFile, MapChildState& childrenState, SSSMap& stateID,
		SSSMap& n_parent, MasterAnimData& animData, MasterAnimSetData& animSetData, std::unordered_map<std::string, std::string>& lastUpdate);
	void CombiningFiles(std::unordered_map<std::string, std::map<std::string, vecstr>>& newFile, MasterAnimData& animData, MasterAnimSetData& animSetData);
	void unregisterProcess();

	public slots :
	void UpdateFiles();

signals:
	void progressMax(int);
	void progressUp();
	void end();
	void enable(bool);
	void enableCheck(bool);
	void hide(bool);
	void incomingMessage(QString);

private:
	int filenum;
	ProgressUp behaviorProcess;

	// timer
	boost::posix_time::ptime start_time;
};



#endif
