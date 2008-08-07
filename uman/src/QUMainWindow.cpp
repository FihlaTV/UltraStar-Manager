#include "main.h"
#include "version.h"

#include "QUMainWindow.h"
#include <QTreeWidget>
#include <QTreeWidgetItem>

#include <QListWidget>
#include <QListWidgetItem>

#include <QDir>
#include <QStringList>
#include <QPixmap>
#include <QIcon>
#include <QBrush>
#include <QHeaderView>
#include <QDateTime>
#include <QSettings>
#include <QMessageBox>
#include <QKeySequence>

#include <QFileDialog>
#include <QFileInfo>
#include <QFileInfoList>
#include <QProgressDialog>

#include "QUSongFile.h"
#include "QUDetailItem.h"
#include "QUMonty.h"
#include "QUDropDownDelegate.h"
#include "QUDetailsTable.h"

#include "fileref.h"
#include "tag.h"
#include "tstring.h"

#include "QUTagOrderDialog.h"
#include "QUTextDialog.h"
#include "QUProgressDialog.h"
#include "QUReportDialog.h"
#include "QUMessageBox.h"
#include "QUPictureDialog.h"

QDir QUMainWindow::BaseDir = QDir();
QUMainWindow::QUMainWindow(QWidget *parent): QMainWindow(parent) {
	setupUi(this);

	initWindow();
	initMenu();
	initConfig();

	initSongTree();
	initDetailsTable();
	initTaskList();

	initMonty();
}

/*!
 * Save configuration for next application start.
 */
QUMainWindow::~QUMainWindow() {
	QSettings settings;
	settings.setValue("allowMonty", QVariant(actionAllowMonty->isChecked()));
	settings.setValue("windowState", QVariant(this->saveState()));

	settings.setValue("disableInfoMessages", QVariant(disableInfoChk->isChecked()));
	settings.setValue("disableWarningMessages", QVariant(disableWarningChk->isChecked()));
	settings.setValue("disableSaveMessages", QVariant(disableSaveChk->isChecked()));

	settings.setValue("autoSave", QVariant(actionAutoSave->isChecked()));
}

/*!
 * Overloaded to ensure that all changes are saved before closing this application.
 */
void QUMainWindow::closeEvent(QCloseEvent *event) {
	if(songTree->hasUnsavedChanges()) {
		QUMessageBox::Results result = QUMessageBox::ask(this,
				tr("Quit"),
				tr("Songs have been modified."),
				":/control/save_all.png", tr("Save all changes."),
				":/control/bin.png", tr("Discard all changes."),
				":/marks/cancel.png", tr("Cancel this action."));
		if(result == QUMessageBox::second)
			event->accept();
		else if(result == QUMessageBox::third)
			event->ignore();
		else if(result == QUMessageBox::first) {
			songTree->saveUnsavedChanges();
			event->accept();
		}
	} else {
		event->accept();
	}
}

/*!
 * Initializes the windows registry entry for uman. Lets the user
 * choose a path where the song files are located.
 */
void QUMainWindow::initConfig() {
	QSettings settings;
	QString path;
	if(!settings.contains("songPath")) {
		path = QFileDialog::getExistingDirectory(this, tr("Choose your UltraStar song directory:"));

		if(!path.isEmpty())
			settings.setValue("songPath", QVariant(path));
	} else {
		path = settings.value("songPath").toString();
	}
	BaseDir.setPath(path);

	// read other settings
	actionAllowMonty->setChecked(settings.value("allowMonty", QVariant(true)).toBool());
	actionShowRelativeSongPath->setChecked(settings.value("showRelativeSongPath", QVariant(true)).toBool());
	completerChk->setChecked(settings.value("caseSensitiveAutoCompletion", QVariant(false)).toBool());

	disableInfoChk->setChecked(settings.value("disableInfoMessages", QVariant(false)).toBool());
	disableWarningChk->setChecked(settings.value("disableWarningMessages", QVariant(false)).toBool());
	disableSaveChk->setChecked(settings.value("disableSaveMessages", QVariant(false)).toBool());

	this->restoreState(settings.value("windowState", QVariant()).toByteArray());

	actionAutoSave->setChecked(settings.value("autoSave", QVariant(true)).toBool());
}

/*!
 * Set up initial window size and title text.
 */
void QUMainWindow::initWindow() {
	setWindowTitle(QString(tr("UltraStar Manager %1.%2")).arg(MAJOR_VERSION).arg(MINOR_VERSION));

	addDockWidget(Qt::LeftDockWidgetArea, detailsDock);
	addDockWidget(Qt::RightDockWidgetArea, tasksDock);
	addDockWidget(Qt::LeftDockWidgetArea, previewDock);
	addDockWidget(Qt::RightDockWidgetArea, eventsDock);

	// init filter area
	filterFrame->hide();

	connect(filterEdit, SIGNAL(returnPressed()), this, SLOT(applyFilter()));
	connect(filterBtn, SIGNAL(clicked()), this, SLOT(applyFilter()));
	connect(filterCancelBtn, SIGNAL(clicked()), this, SLOT(removeFilter()));
	connect(filterNegateBtn, SIGNAL(clicked()), this, SLOT(toggleFilterNegateBtn()));

	filterTypeCombo->addItems(QStringList() << tr("All Tags") << tr("Information Tags") << tr("File Tags") << tr("Control Tags"));
	filterTypeCombo->setCurrentIndex(0);

	filterNegateBtn->setChecked(false);
}

void QUMainWindow::initMenu() {
	// songs menu
	connect(actionNewReport, SIGNAL(triggered()), this, SLOT(reportCreate()));
	connect(actionSaveSelected, SIGNAL(triggered()), songTree, SLOT(saveSelectedSongs()));
	connect(actionSaveAll, SIGNAL(triggered()), songTree, SLOT(saveUnsavedChanges()));
	connect(actionExpandAll, SIGNAL(triggered()), songTree, SLOT(expandAll()));
	connect(actionExpandAll, SIGNAL(triggered()), songTree, SLOT(resizeToContents()));
	connect(actionCollapseAll, SIGNAL(triggered()), songTree, SLOT(collapseAll()));
	connect(actionCollapseAll, SIGNAL(triggered()), songTree, SLOT(resizeToContents()));
	connect(actionRefresh, SIGNAL(triggered()), this, SLOT(refreshAllSongs()));
	connect(actionRefreshSelected, SIGNAL(triggered()), songTree, SLOT(refreshSelectedItems()));

	actionNewReport->setShortcut(QKeySequence::fromString("F2"));
	actionRefreshSelected->setShortcut(QKeySequence::fromString("F5"));
	actionRefresh->setShortcut(QKeySequence::fromString("Shift+F5"));
	actionSaveSelected->setShortcut(QKeySequence::fromString("Ctrl+S"));
	actionSaveAll->setShortcut(QKeySequence::fromString("Ctrl+Shift+S"));

	// view menu
	connect(actionShowRelativeSongPath, SIGNAL(toggled(bool)), this, SLOT(toggleRelativeSongPath(bool)));
	connect(actionFilter, SIGNAL(toggled(bool)), this, SLOT(toggleFilterFrame(bool)));

	this->menuView->addAction(detailsDock->toggleViewAction());
	this->menuView->addAction(tasksDock->toggleViewAction());
	this->menuView->addAction(previewDock->toggleViewAction());
	this->menuView->addAction(eventsDock->toggleViewAction());
	this->menuView->addAction(this->toolBar->toggleViewAction());

	actionFilter->setShortcut(QKeySequence::fromString("Ctrl+F"));

	// options menu
	connect(actionAutoSave, SIGNAL(toggled(bool)), this, SLOT(toggleAutoSaveChk(bool)));
	connect(actionTagSaveOrder, SIGNAL(triggered()), this, SLOT(editTagOrder()));
	connect(actionChangeSongDirectory, SIGNAL(triggered()), this, SLOT(changeSongDir()));

	connect(actionLangEnglish, SIGNAL(triggered()), this, SLOT(enableEnglish()));
	connect(actionLangGerman, SIGNAL(triggered()), this, SLOT(enableGerman()));

	// help menu
	connect(actionShowMonty, SIGNAL(triggered()), helpFrame, SLOT(show()));
	connect(actionQt, SIGNAL(triggered()), this, SLOT(aboutQt()));
	connect(actionUman, SIGNAL(triggered()), this, SLOT(aboutUman()));
}

/*!
 * Set up the song tree the first time.
 * \sa createSongFiles()
 * \sa createSongTree()
 */
void QUMainWindow::initSongTree() {
	songTree->initHorizontalHeader();

	connect(songTree, SIGNAL(itemSelectionChanged()), this, SLOT(updateDetails()));
	connect(songTree, SIGNAL(itemSelectionChanged()), this, SLOT(updatePreviewTree()));

	connect(songTree, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(editSongSetFileLink(QTreeWidgetItem*, int)));
	connect(songTree, SIGNAL(itemActivated(QTreeWidgetItem*, int)), this, SLOT(showFileContent(QTreeWidgetItem*, int)));

	connect(songTree, SIGNAL(itemExpanded(QTreeWidgetItem*)), songTree, SLOT(resizeToContents()));
	connect(songTree, SIGNAL(itemCollapsed(QTreeWidgetItem*)), songTree, SLOT(resizeToContents()));

	connect(songTree, SIGNAL(finished(const QString&, QU::EventMessageTypes)), this, SLOT(addLogMsg(const QString&, QU::EventMessageTypes)));
	connect(songTree, SIGNAL(songCreated(QUSongFile*)), this, SLOT(appendSong(QUSongFile*)));

	refreshAllSongs();
}

void QUMainWindow::initDetailsTable() {
	connect(detailsTable, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(editSongSetDetail(QTableWidgetItem*)));

	connect(completerChk, SIGNAL(toggled(bool)), this, SLOT(toggleCompleterChk(bool)));
}

/*!
 * Initializes all available tasks.
 * \sa editSongApplyTasks()
 * \sa editSongSetDetail()
 */
void QUMainWindow::initTaskList() {
	// connect task buttons
	connect(taskBtn, SIGNAL(clicked()), this, SLOT(editSongApplyTasks()));
	connect(allTasksBtn, SIGNAL(clicked()), taskList, SLOT(checkAllTasks()));
	connect(noTasksBtn, SIGNAL(clicked()), taskList, SLOT(uncheckAllTasks()));

	connect(taskList, SIGNAL(finished(const QString&, QU::EventMessageTypes)), this, SLOT(addLogMsg(const QString&, QU::EventMessageTypes)));
}

void QUMainWindow::initMonty() {
	montyLbl->setPixmap(monty->pic(QUMonty::seated));
	helpLbl->setText(monty->welcomeMsg(_songs.size()));

	connect(hideMontyBtn, SIGNAL(clicked()), helpFrame, SLOT(hide()));

	if(!actionAllowMonty->isChecked())
		helpFrame->hide();
}

void QUMainWindow::appendSong(QUSongFile *song) {
	_songs.append(song);
}

/*!
 * Re-reads all possible song files and builds a new song tree.
 */
void QUMainWindow::refreshAllSongs(bool force) {
	if(!force && songTree->hasUnsavedChanges()) {
		QUMessageBox::Results result = QUMessageBox::ask(this,
				tr("Rebuild Song Tree"),
				tr("Songs have been modified."),
				":/control/save_all.png", tr("Save all changes."),
				":/control/bin.png", tr("Discard all changes."),
				":/marks/cancel.png", tr("Cancel this action."));
		if(result == QUMessageBox::third)
			return;
		else if(result == QUMessageBox::first)
			songTree->saveUnsavedChanges();
	}

	// -------------------------------------

	songTree->clear();
	updateDetails();

	foreach(QUSongFile *song, _songs) {
		disconnect(song, 0, 0, 0);
	}

	qDeleteAll(_songs);
	_songs.clear();

	createSongFiles();
	songTree->fill(_songs);

	updatePreviewTree();
}

/*!
 * Creates all instances of QUSongFile to fill the song tree.
 * \sa CreateSongTree();
 */
void QUMainWindow::createSongFiles() {
	QList<QDir> dirList;
	dirList.append(QDir(BaseDir));

	this->readSongDir(dirList);

	QUProgressDialog dlg(tr("Reading song files..."), dirList.size(), this);
	dlg.setPixmap(":/types/folder.png");
	dlg.show();

	foreach(QDir dir, dirList) {
		QStringList files = dir.entryList(QUSongFile::allowedSongFiles(), QDir::Files);

		dlg.update(dir.dirName());
		if(dlg.cancelled()) break;

		if(!files.isEmpty()) {
			// TODO: What about more song files in a folder? Really choose the first one? Hmmm...
			_songs.append(new QUSongFile(QFileInfo(dir, files.first()).filePath()));
			// enable event log
			connect(_songs.last(), SIGNAL(finished(const QString&, QU::EventMessageTypes)), this, SLOT(addLogMsg(const QString&, QU::EventMessageTypes)));
		}
	}
}

/*!
 * Reads recursively all available directories and puts them in the dirList.
 */
void QUMainWindow::readSongDir(QList<QDir> &dirList) {
	if(dirList.last().entryList(QDir::NoDotAndDotDot | QDir::Dirs, QDir::Name).isEmpty()) {
		return;
	} else {
		QDir thisDir(dirList.last());
		QStringList subDirs = dirList.last().entryList(QDir::NoDotAndDotDot | QDir::Dirs, QDir::Name);

		foreach(QString dir, subDirs) {
			QDir newDir(thisDir);
			newDir.cd(dir);

			dirList.append(newDir);
			this->readSongDir(dirList);
		}
	}
}

void QUMainWindow::updateDetails() {
	disconnect(detailsTable, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(editSongSetDetail(QTableWidgetItem*)));

	detailsTable->updateValueColumn(songTree->selectedSongs());

	connect(detailsTable, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(editSongSetDetail(QTableWidgetItem*)));
}

/*!
 * Updates all preview information according to the current selection or dimension of the
 * song tree.
 */
void QUMainWindow::updatePreviewTree() {
	previewTree->setSongCount(songTree->topLevelItemCount() + songTree->hiddenItemsCount());
	previewTree->setSelectedSongCount(songTree->currentItem() ? qMax(songTree->selectedItems().size(), 1) : songTree->selectedItems().size());
	previewTree->setVisibleSongCount(songTree->topLevelItemCount());
	previewTree->setHiddenSongCount(songTree->hiddenItemsCount());

	QUSongItem *item = dynamic_cast<QUSongItem*>(songTree->currentItem());

	if(item) {
		QFileInfo fi(item->song()->songFileInfo().dir(), item->text(FOLDER_COLUMN));
		previewTree->showFileInformation(fi);
	} else
		previewTree->showFileInformation(QFileInfo());
}

/*!
 * Save all changes of the details into the song file.
 * \sa updateDetails()
 */
void QUMainWindow::editSongSetFileLink(QTreeWidgetItem *item, int column) {
	if(column < 3 or column > 6)
		return;

	QUSongItem *songItem = dynamic_cast<QUSongItem*>(item);

	if(!songItem || songItem->isToplevel())
		return;

	QUSongFile *song = songItem->song();

	QString fileScheme("*." + QFileInfo(songItem->text(FOLDER_COLUMN)).suffix());

	if( songItem->icon(MP3_COLUMN).isNull()
			and QUSongFile::allowedAudioFiles().contains(fileScheme, Qt::CaseInsensitive)
			and column == MP3_COLUMN ) {
		addLogMsg(QString(tr("Audio file changed from \"%1\" to: \"%2\".")).arg(song->mp3()).arg(songItem->text(FOLDER_COLUMN)));
		song->setInfo(MP3_TAG, songItem->text(FOLDER_COLUMN));
		song->save();
	} else if( songItem->icon(COVER_COLUMN).isNull()
			and QUSongFile::allowedPictureFiles().contains(fileScheme, Qt::CaseInsensitive)
			and column == COVER_COLUMN ) {
		addLogMsg(QString(tr("Cover changed from \"%1\" to: \"%2\".")).arg(song->cover()).arg(songItem->text(FOLDER_COLUMN)));
		song->setInfo(COVER_TAG, songItem->text(FOLDER_COLUMN));
		song->save();
	} else if( songItem->icon(BACKGROUND_COLUMN).isNull()
			and QUSongFile::allowedPictureFiles().contains(fileScheme, Qt::CaseInsensitive)
			and column == BACKGROUND_COLUMN ) {
		addLogMsg(QString(tr("Background changed from \"%1\" to: \"%2\".")).arg(song->background()).arg(songItem->text(FOLDER_COLUMN)));
		song->setInfo(BACKGROUND_TAG, songItem->text(FOLDER_COLUMN));
		song->save();
	} else if( songItem->icon(VIDEO_COLUMN).isNull()
			and QUSongFile::allowedVideoFiles().contains(fileScheme, Qt::CaseInsensitive)
			and column == VIDEO_COLUMN ) {
		addLogMsg(QString(tr("Video file changed from \"%1\" to: \"%2\".")).arg(song->video()).arg(songItem->text(FOLDER_COLUMN)));
		song->setInfo(VIDEO_TAG, songItem->text(FOLDER_COLUMN));
		song->save();
	}

	updateDetails();
	(dynamic_cast<QUSongItem*>(songItem->parent()))->update();

	montyTalk();
}


void QUMainWindow::editSongSetDetail(QTableWidgetItem *item) {
	QUDetailItem *detailItem = dynamic_cast<QUDetailItem*>(detailsTable->currentItem());

	if(!detailItem)
		return;

	QUProgressDialog dlg(QString(tr("Applying new value for %1 to all selected songs...")).arg(detailItem->tag()),
			detailItem->songs().size(), this);
	dlg.show();

	// save changes for each song
	foreach(QUSongFile *song, detailItem->songs()) {
		dlg.update(QString("%1 - %2").arg(song->artist()).arg(song->title()));
		if(dlg.cancelled()) break;

		song->setInfo(detailItem->tag(), detailItem->text());
		song->save();
	}
	dlg.hide();

	// update all selected items with these new details in the song tree
	songTree->refreshSelectedItems();
}

/*!
 * Does all checked tasks for all selected songs. You can only select
 * toplevel items (folders) which represent songs.
 * \sa initTaskList();
 */
void QUMainWindow::editSongApplyTasks() {
	QList<QTreeWidgetItem*> itemList = songTree->selectedItems();

	if(itemList.isEmpty()) // if no songs are selected use the current item (which has also a song)
		itemList.append(songTree->currentItem());

	QUProgressDialog dlg(tr("Applying all checked tasks to all selected songs..."), itemList.size(), this);
	dlg.show();

	foreach(QTreeWidgetItem *item, itemList) {
		QUSongItem *songItem = dynamic_cast<QUSongItem*>(item);

		if(songItem) {
			QUSongFile *song = songItem->song();

			dlg.update(QString("%1 - %2").arg(song->artist()).arg(song->title()));
			if(dlg.cancelled()) break;

			taskList->doTasksOn(song);

			song->save();
			songItem->update();
		}
	}

	updateDetails();
	montyTalk();
}

void QUMainWindow::addLogMsg(const QString &msg, QU::EventMessageTypes type) {
	if(type == QU::information and disableInfoChk->isChecked())
		return;
	if(type == QU::warning and disableWarningChk->isChecked())
		return;
	if(type == QU::saving and disableSaveChk->isChecked())
		return;

	log->insertItem(0, QDateTime::currentDateTime().toString("[hh:mm:ss] ") + msg);

	switch(type) {
	case 0: log->item(0)->setIcon(QIcon(":/marks/information.png")); break;
	case 1: log->item(0)->setIcon(QIcon(":/marks/error.png")); break;
	case 2: log->item(0)->setIcon(QIcon(":/marks/help.png")); break;
	case 3: log->item(0)->setIcon(QIcon(":/marks/disk.png")); break;
	}

}

void QUMainWindow::aboutQt() {
	QMessageBox::aboutQt(this, "About Qt");
}

void QUMainWindow::aboutUman() {
	QString aboutStr(tr("<b>UltraStar Manager</b><br>"
			"Version %1.%2.%3 #%4<br>"
			"<br>"
			"�2008 by Marcel Taeumel<br>"
			"<br>"
			"<i>Tested By</i><br>"
			"Michael Gr�newald"));

	QMessageBox::about(this, "About", aboutStr
			.arg(MAJOR_VERSION)
			.arg(MINOR_VERSION)
			.arg(PATCH_VERSION)
			.arg(QString(revision).remove(QRegExp("(.*:)|\\D"))));
}

void QUMainWindow::editTagOrder() {
	QUTagOrderDialog *dlg = new QUTagOrderDialog(this);
	dlg->exec();
	delete dlg;

	montyTalk();
}

/*!
 * Enables the user to set up a new location where all
 * UltraStar songs can be found.
 */
void QUMainWindow::changeSongDir() {
	if(songTree->hasUnsavedChanges()) {
		QUMessageBox::Results result = QUMessageBox::ask(this,
				tr("Change Song Directory"),
				tr("Songs have been modified."),
				":/control/save_all.png", tr("Save all changes."),
				":/control/bin.png", tr("Discard all changes."),
				":/marks/cancel.png", tr("Cancel this action."));
		if(result == QUMessageBox::third)
			return;
		else if(result == QUMessageBox::first)
			songTree->saveUnsavedChanges();
	}

	// ---------------------------------------------------------

	QSettings settings;
	QString path = settings.value("songPath").toString();

	path = QFileDialog::getExistingDirectory(this, tr("Choose your UltraStar song directory:"), path);

	if(!path.isEmpty()) {
		settings.setValue("songPath", QVariant(path));
		BaseDir.setPath(path);
		refreshAllSongs(true);

		monty->setSongCount(_songs.size());
		montyTalk();

		addLogMsg(QString(tr("UltraStar song directory changed to: \"%1\".")).arg(BaseDir.path()));

		songTree->headerItem()->setText(FOLDER_COLUMN, QString(tr("Folder (%1)")).arg(BaseDir.path()));
	}
}

void QUMainWindow::montyTalk() {
	if(!actionAllowMonty->isChecked())
		return;

	helpFrame->show();
	monty->talk(montyLbl, helpLbl);
}

/*!
 * Updates all toplevel items to display the relative song path
 * or only the song directory - like selected.
 */
void QUMainWindow::toggleRelativeSongPath(bool checked) {
	QSettings settings;
	settings.setValue("showRelativeSongPath", QVariant(checked));

	QList<QUSongItem*> tmpList; // tmp list needed so avoid sorting problems

	QUProgressDialog dlg(checked ? tr("Show relative song path for each song...") : tr("Show song folder for each song..."), songTree->topLevelItemCount(), this, false);
	dlg.setPixmap(":/types/folder.png");
	dlg.show();

	for(int i = 0; i < songTree->topLevelItemCount(); i++)
		tmpList.append(dynamic_cast<QUSongItem*>(songTree->topLevelItem(i)));

	foreach(QUSongItem *item, tmpList) {
		dlg.update(item->song()->songFileInfo().dir().dirName());

		item->updateAsDirectory(checked);
	}

	if(checked)
		addLogMsg(tr("Relative song paths are displayed in the song tree now."));
	else
		addLogMsg(tr("Only song directories are displayed in the song tree now."));
}

/*!
 * Toggle whether the completion for detail edition should be case-sensitive.
 */
void QUMainWindow::toggleCompleterChk(bool checked) {
	QSettings settings;
	settings.setValue("caseSensitiveAutoCompletion", QVariant(checked));
}

void QUMainWindow::toggleAutoSaveChk(bool checked) {
	QSettings settings;
	settings.setValue("autoSave", QVariant(checked));

	actionSaveAll->setEnabled(!checked);
}

/*!
 * Shows the content of the current file. Another text-file will be set as new song text
 * file.
 */
void QUMainWindow::showFileContent(QTreeWidgetItem *item, int column) {
	if(column != FOLDER_COLUMN)
		return;

	QUSongItem *songItem = dynamic_cast<QUSongItem*>(item);

	if(!songItem || songItem->isToplevel())
		return;

	QString fileScheme("*." + QFileInfo(songItem->text(FOLDER_COLUMN)).suffix());

	if(QString::compare(item->text(FOLDER_COLUMN), songItem->song()->songFileInfo().fileName(), Qt::CaseInsensitive) == 0) {
		// show the (raw) content of the current song text file
		QUTextDialog *dlg = new QUTextDialog(songItem->song(), this);
		dlg->exec();
		delete dlg;
	} else if(QUSongFile::allowedSongFiles().contains(fileScheme, Qt::CaseInsensitive)) {
		// use this song text file for the folder now
		// --> useful for more than one valid song text file per folder
		songItem->song()->setFile(QFileInfo(songItem->song()->songFileInfo().dir(), item->text(FOLDER_COLUMN)).filePath());

		songTree->setCurrentItem(songItem->parent());
		songItem->update();
	} else if(QUSongFile::allowedPictureFiles().contains(fileScheme, Qt::CaseInsensitive)) {
		QUPictureDialog dlg(QFileInfo(songItem->song()->songFileInfo().dir(), songItem->text(FOLDER_COLUMN)).filePath(), this);
		dlg.exec();
	}
}

void QUMainWindow::toggleFilterFrame(bool checked) {
	filterFrame->setVisible(checked);

	if(checked) {
		filterEdit->setFocus();
		filterEdit->selectAll();
	}
}

void QUMainWindow::toggleFilterNegateBtn() {
	if(filterNegateBtn->text() == "negate") {
		filterNegateBtn->setText("");
		filterNegateBtn->setIcon(QIcon(":/marks/plus.png"));
	} else {
		filterNegateBtn->setText("negate");
		filterNegateBtn->setIcon(QIcon(":/marks/minus.png"));
	}
}

void QUMainWindow::applyFilter() {
	int modes = 0;

	if(filterNegateBtn->text() == "negate")
		modes |= QU::negateFilter;

	if(filterTypeCombo->currentIndex() == 0)
		modes |= QU::informationTags | QU::fileTags | QU::controlTags;

	if(filterTypeCombo->currentIndex() == 1)
		modes |= QU::informationTags;

	if(filterTypeCombo->currentIndex() == 2)
		modes |= QU::fileTags;

	if(filterTypeCombo->currentIndex() == 3)
		modes |= QU::controlTags;

	songTree->filterItems(filterEdit->text(), (QU::FilterModes) modes);
}

/*!
 * Shows all items in the song tree again.
 */
void QUMainWindow::removeFilter() {
	filterEdit->setText("");
	songTree->filterItems("");
	actionFilter->setChecked(false);
}

void QUMainWindow::reportCreate() {
	QUReportDialog *dlg = new QUReportDialog(songTree, this);

	connect(dlg, SIGNAL(finished(const QString&, QU::EventMessageTypes)), this, SLOT(addLogMsg(const QString&, QU::EventMessageTypes)));
	dlg->exec();
	disconnect(dlg, SIGNAL(finished(const QString&, QU::EventMessageTypes)), this, SLOT(addLogMsg(const QString&, QU::EventMessageTypes)));

	delete dlg;
}

/*!
 * Changes the application language to english.
 */
void QUMainWindow::enableEnglish() {
	actionLangGerman->setChecked(false);
	actionLangEnglish->setChecked(true);

	QSettings settings;
	settings.setValue("language", QVariant("en_EN"));

	// ---------------

	QUMessageBox::Results result = QUMessageBox::ask(this,
			tr("Change Language"),
			tr("Application language changed to <b>English</b>. You need to restart UltraStar Manager to take effect."),
			":/control/quit.png", tr("Quit UltraStar Manager."),
			":/marks/accept.png", tr("Continue."));
	if(result == QUMessageBox::first)
		this->close();
}

/*!
 * Changes the application language to german.
 */
void QUMainWindow::enableGerman() {
	actionLangGerman->setChecked(true);
	actionLangEnglish->setChecked(false);

	QSettings settings;
	settings.setValue("language", QVariant("de_DE"));

	// ---------------

	QUMessageBox::Results result = QUMessageBox::ask(this,
			tr("Change Language"),
			tr("Application language changed to <b>German</b>. You need to restart UltraStar Manager to take effect."),
			":/control/quit.png", tr("Quit UltraStar Manager."),
			":/marks/accept.png", tr("Continue."));
	if(result == QUMessageBox::first)
		this->close();
}
